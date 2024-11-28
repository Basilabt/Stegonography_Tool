

Hi, Im Basel 
Plese before compiling the .cpp file follow these steps to install the requirements: 


1) Install the library opencv using these commands: 

  a) sudo apt update
  b) sudo apt install libopencv-dev


2) Compile the code using the following: 
	
	 g++ Encoder.cpp -o encoder  `pkg-config --cflags --libs opencv4`
         g++ Decoder.cpp -o decoder  `pkg-config --cflags --libs opencv4`
  

3) Run Encoder:
	
	./encoder [Image To Hide Data In].format [Secret Text File] [New Image Name].format

4) Run Decoer: 

        ./decoder [Image That Contains The Hidden Data].format
