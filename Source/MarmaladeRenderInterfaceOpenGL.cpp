#include "MarmaladeRenderInterfaceOpenGL.h"
#include <Rocket/Core.h>
#include <IwGx.h>

MarmaladeRenderInterfaceOpenGL::MarmaladeRenderInterfaceOpenGL()
  : _enableScissors(false)
{
    verticesCount = 0;
    indicesCount = 0;
    newMaterial(NULL);
}

// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void MarmaladeRenderInterfaceOpenGL::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
    CIwTexture* tex = NULL;
    if (texture)
        tex = _textures.element_at(texture - 1);
    flushIfNeeded(tex, num_vertices, num_indices);

    //indices
    for (int i = 0; i < num_indices; ++i){
        indices_stream[indicesCount] = (uint16)indices[i] + verticesCount;
        indicesCount++;
    }

    //vertices
	for (int i = 0; i < num_vertices; ++i) {
		if (texture) {
            uvs[verticesCount].x = vertices[i].tex_coord.x;
            uvs[verticesCount].y = vertices[i].tex_coord.y;
		}

        verts[verticesCount].x = (int)((vertices[i].position.x + translation.x)*8.f);
        verts[verticesCount].y = (int)((vertices[i].position.y + translation.y)*8.f);

        colors[verticesCount].r = vertices[i].colour.red;
        colors[verticesCount].g = vertices[i].colour.green;
        colors[verticesCount].b = vertices[i].colour.blue;
        colors[verticesCount].a = vertices[i].colour.alpha;
        verticesCount ++;
	}
}

void MarmaladeRenderInterfaceOpenGL::Flush()
{
    IwGxSetUVStream(uvs);
    IwGxSetVertStreamScreenSpaceSubPixel(verts, verticesCount);
    IwGxSetColStream(colors, verticesCount);
    IwGxSetNormStream(NULL);
    IwGxDrawPrims(IW_GX_TRI_LIST, indices_stream, indicesCount);
    verticesCount = 0;
    indicesCount = 0;
}

bool MarmaladeRenderInterfaceOpenGL::flushIfNeeded(CIwTexture* texture, uint32 verticesCount, uint32 indicesCount)
{
    if ((currentTexture != texture) || verticesCount + this->verticesCount >= VERTICES_BATCH_SIZE || indicesCount + this->indicesCount >= INDICES_BATCH_SIZE){
        Flush();
        newMaterial(texture);
        return true;
    }
    return false;
}

void MarmaladeRenderInterfaceOpenGL::newMaterial(CIwTexture* texture)
{
    CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
    mat->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
    mat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
    if (texture) mat->SetTexture(texture);
    IwGxSetMaterial(mat);
    currentTexture = texture;
}

Rocket::Core::CompiledGeometryHandle MarmaladeRenderInterfaceOpenGL::CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture)
{
    return (Rocket::Core::CompiledGeometryHandle) NULL;
}

void MarmaladeRenderInterfaceOpenGL::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
}

void MarmaladeRenderInterfaceOpenGL::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.		
void MarmaladeRenderInterfaceOpenGL::EnableScissorRegion(bool enable)
{
  _enableScissors = enable;
  if (!_enableScissors)
    IwGxClearScissorScreenSpace();
}

// Called by Rocket when it wants to change the scissor region.		
void MarmaladeRenderInterfaceOpenGL::SetScissorRegion(int x, int y, int width, int height)
{
  _scissors.Make(x, y, width, height);
  if (_enableScissors)
    IwGxSetScissorScreenSpace(_scissors.x, _scissors.y, _scissors.w, _scissors.h);
}

// Called by Rocket when a texture is required by the library.		
bool MarmaladeRenderInterfaceOpenGL::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	CIwTexture *s_Texture = new CIwTexture;
    s_Texture->SetFormatHW(TEXTURE_FORMAT);
    s_Texture->SetFormatSW(TEXTURE_FORMAT);
	s_Texture->LoadFromFile(source.CString());
	s_Texture->Upload();

	texture_dimensions.x = s_Texture->GetWidth();
	texture_dimensions.y = s_Texture->GetHeight();

	_textures.append(s_Texture);
	texture_handle = (Rocket::Core::TextureHandle)_textures.size();   // indexing from 1, 0 means no texture

	return true;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool MarmaladeRenderInterfaceOpenGL::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	CIwTexture *s_Texture = new CIwTexture;
    s_Texture->SetFormatHW(TEXTURE_FORMAT);
    s_Texture->SetFormatSW(TEXTURE_FORMAT);
	s_Texture->CopyFromBuffer(source_dimensions.x, source_dimensions.y, CIwImage::ARGB_8888, source_dimensions.x << 2, (uint8*)source, NULL);
	s_Texture->Upload();

	_textures.append(s_Texture);
	texture_handle = (Rocket::Core::TextureHandle)_textures.size();		// indexing from 1, 0 means no texture

	return true;
}

// Called by Rocket when a loaded texture is no longer required.		
void MarmaladeRenderInterfaceOpenGL::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	CIwTexture *tex = _textures.element_at(texture_handle-1);
	delete tex;
	tex = NULL;
}
