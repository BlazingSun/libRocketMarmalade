import os

if __name__ == "__main__": 
	if(not os.path.exists("./Dependencies/librocket")):
		os.system("git clone https://github.com/BlazingSun/libRocket.git ./Dependencies/librocket")