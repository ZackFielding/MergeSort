#include "windows.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>

//[] add block to nameDirectory() that creates the vector of char ptrs
//		... that's passed into multiStrCat()

char* multiStrCat(std::vector<char *> charVector){
	int startIndex {0};	
	size_t z {1}, y {0};

	while(z < charVector.size()){
	startIndex = std::strlen(charVector.at(0)); // starting point	
		for(y ; y <= std::strlen(charVector.at(z)); ++y){
			//appends to char[] at first index
			// '<=' should copy null terminator (strlen stops at delim)
			(charVector.at(0))[startIndex + y] = (charVector.at(z))[y];	
		}
	++z;
	}

	return charVector.at(0); // return first index char [] which was appended to with ea iteration
};



void nameDirectory(char *dir_ptr, const int &maxDirSize, bool &dirCheck, LPSECURITY_ATTRIBUTES access,
		void(*func)(bool &dirCheck, LPSECURITY_ATTRIBUTES access, char *dir_ptr)){
   		// enter name of new directory folder .. append to dir
	char folderName [25] {"NNF"}; //no name folder -> default
	std::cout << "Enter name of new directory folder: " << std::endl;
	std::cin >> folderName;
		
		// check to see if string sum of both string lengths < dir size
	if(std::strlen(dir_ptr) + std::strlen(folderName) < maxDirSize){
		dir_ptr = std::strcat(dir_ptr, folderName);
		func(dirCheck, access, dir_ptr);
	}else{
		char *updatedDir{nullptr};
		updatedDir = new char [std::strlen(dir_ptr) + std::strlen(folderName) + 5];
		func(dirCheck, access, updatedDir); //same but passing dynamic alloc dir string
		delete [] updatedDir; // free heap
	}	

};

void createNewDirectory(bool &dirCheck, LPSECURITY_ATTRIBUTES access, char *dir_ptr){
	DWORD threadError {16000}; // local thread error reciever
	dirCheck = CreateDirectory(dir_ptr, access);	
			threadError = GetLastError(); //returns previous threads error code
			if(threadError != 0){
				switch (threadError){
					case 183: std::cerr << "Error: directory already exists." << std::endl;
						break;
					case 3: std::cerr << "ERROR: path not found." << std::endl;
						break;
					default: std::cerr << "ERROR " << threadError 
								<< " code not found." << std::endl;				 
				};
			}else{
				// if error == 0 -> create dir func was successfull
				std::cerr << "Directory successfully created." << std::endl;
			}
};

int main(){
	bool dirCheck {false};
	const int maxDirSize {50};
	char dir[maxDirSize] {"no dir"}; // default
	char *const dir_ptr {dir}; // const pointing -> directory string
	LPSECURITY_ATTRIBUTES access {nullptr}; // security acess -> default
	DWORD threadError {16000}; //error code 16000 does not exist

	// create new directory?
	// display current directory -> ask if you want it here
	DWORD curDirBuffer {50}, curDirLength {}; 

	do{
		curDirLength = GetCurrentDirectory(curDirBuffer, dir);

		if(curDirLength == 0){ // returns -> 0 if failed to get current directory
			std::cerr << "Return current directory failed.  Error "
				<< (threadError =  GetLastError()) << std::endl; 
				//... write more later on to help address this issue within program
			return 1; // stop program
		}else{
			std::cout << "Current directory is: " << dir
				<< "\n... Would you like create new directory here (Y/N)?" << std::endl;
			// if yes, ask new dir name
			char userAns {'D'}; //d --> default
			std::cin >> userAns;
			if (userAns == 'Y'){
				nameDirectory(dir_ptr, maxDirSize, dirCheck, access, createNewDirectory);
			}else if (userAns == 'N'){
				//changeCurrentDirectory();
				nameDirectory(dir_ptr, maxDirSize, dirCheck, access, createNewDirectory);
			}else{
				std::cerr << "Error in determining user direction." << std::endl;
			}
		}

	}while(false); // will need to change
		
	return 0;
}
