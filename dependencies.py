import os
import subprocess

S3E = os.environ['S3E_DIR']
if __name__ == "__main__": 
    if(not os.path.exists("./Dependencies/librocket")):
        os.system("git clone https://github.com/BlazingSun/libRocket.git -b marmalade-libs --single-branch ./Dependencies/librocket")
    #    os.system(S3E + "/python/python "+S3E+"/makefile_builder/mkb.py --make --no-ide --compiler=gcc ./Dependencies/librocket/Build/marmalade/librocket.mkb")
    #else:
    #    if('up-to-date' not in subprocess.check_output("git -C ./Dependencies/librocket pull")):
    #        os.system(S3E + "/python/python "+S3E+"/makefile_builder/mkb.py --make --no-ide --compiler=gcc ./Dependencies/librocket/Build/marmalade/librocket.mkb")