#ifndef ROCKETMARMALADERENDERINTERFACE_H
#define ROCKETMARMALADERENDERINTERFACE_H

#include "Rocket/Core/RenderInterface.h"
#include <IwTexture.h>
#include <IwList.h>

#define VERTICES_BATCH_SIZE 4096
#define INDICES_BATCH_SIZE 8192
#define TEXTURE_FORMAT CIwImage::ABGR_8888

/**
	Low level OpenGL render interface for Rocket
 */

class MarmaladeRenderInterfaceOpenGL : public Rocket::Core::RenderInterface
{
public:
	MarmaladeRenderInterfaceOpenGL();

	/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

	/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
	virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);

	/// Called by Rocket when it wants to render application-compiled geometry.
	virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
	/// Called by Rocket when it wants to release application-compiled geometry.
	virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
	/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);
	/// Called by Rocket when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

    ///Flush all outstanding geometry
    void Flush();

protected:
    bool flushIfNeeded(CIwTexture* texture, uint32 verticesCount, uint32 indicesCount);
    void newMaterial(CIwTexture* texture);

    CIwList<CIwTexture*> _textures;
	CIwRect32 _scissors;
    bool _enableScissors;
    CIwFVec2 uvs[VERTICES_BATCH_SIZE];
    CIwSVec2 verts[VERTICES_BATCH_SIZE];
    CIwColour colors[VERTICES_BATCH_SIZE];
    uint16 indices_stream[INDICES_BATCH_SIZE];
    int32 verticesCount;
    uint16 indicesCount;
    CIwTexture* currentTexture;
    CIwMaterial* mat;
};

#endif
