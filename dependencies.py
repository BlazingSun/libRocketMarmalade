import os

S3E = os.environ['S3E_DIR']
if __name__ == "__main__": 
	if(not os.path.exists("./Dependencies/librocket")):
		os.system("git clone https://github.com/BlazingSun/libRocket.git ./Dependencies/librocket")
	else:
		os.system("git -C ./Dependencies/librocket pull")
	os.system("python "+S3E+"/makefile_builder/mkb.py --make --no-ide ./Dependencies/librocket/Build/marmalade/librocket.mkb")