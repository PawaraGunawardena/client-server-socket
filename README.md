# client-server-socket
This project is about socket programming in cpp. It uses Windows environment and winsock for the demonstration.

You can clone this project into a local windows environment, compile and run for the better understanding. If need some guidance for the compilation follow the below steps. 

Because the need of winsock and Windows environment, this uses cl tool for the compilation. For more information: https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options?view=vs-2019

* To setup the cl tool, run the vcvarsall.bat file in your MS Visual Studio installation using the CMD. Below is the example used for this project:

	call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
	
* Next move to the directory, which include your project. 

* Compile server.cpp file. 

	cl /EHsc server.cpp
	
* Compile the client.cpp file.

	cl /EHsc client.cpp
	
* Run the server.exe manually or by CMD as in below. 
	
	start server.exe
	
* Run the client.exe manually or by CMD as in below. 

	start client.exe
	
	