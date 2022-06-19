#include "windows.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>

// [x] fix multiStrCat
// windows.h create new dir only works one directory at a time
// [x] chain create new dir based on number of folders entered by user
// [] test dynamic mem alloc option if user enters very large directory name

void multiStrCat(std::vector<char *> &charVector){ //passbyref vec of char ptrs

	int startIndex {0};	
	for(size_t z {1}; z < charVector.size(); ++z){
		startIndex = std::strlen(charVector.at(0)); // starting point	
		for(size_t y {0} ; y <= std::strlen(charVector.at(z)); ++y){
			charVector.at(0)[startIndex + y] = charVector.at(z)[y];	
		}
	}

	std::cout << "\nNew directory: " << charVector.at(0) << std::endl;
	return;
};

void nameDirectory(char *dir_ptr, const int &maxDirSize, bool &dirCheck, LPSECURITY_ATTRIBUTES access,
		void(*func)(bool &, LPSECURITY_ATTRIBUTES, char *, const int&, const int&)){
   		// enter name of new directory folder .. append to dir
	char folderName [30] {"NNF"}; //no name folder -> default
	std::cout << "Enter name of new directory folder: " << std::endl;
	std::cin >> folderName;
		
		char backBack[] {"\\"};
	std::vector <char*> charVector {dir_ptr, backBack, folderName};
	const int baseDirSize {(int)std::strlen(dir_ptr)};

		// check to see if string sum of both string lengths < dir size
	if(std::strlen(dir_ptr) + std::strlen(folderName) < maxDirSize){
		multiStrCat(charVector);
		func(dirCheck, access, dir_ptr, maxDirSize, baseDirSize);
	}else{
		char *updatedDir{nullptr};
		updatedDir = new char [std::strlen(dir_ptr) + std::strlen(folderName) + 5];
		//func(dirCheck, access, updatedDir); //UPDATED ONCE NON-DYNAMIC CALL WORKS
		delete [] updatedDir; // free heap
	}	

};

void createNewDirectory(bool &dirCheck, LPSECURITY_ATTRIBUTES access, char *dir_ptr,
	   const int &maxDirSize, const int &baseDirSize){

	char baseDir[maxDirSize] {"nullstr"};

	size_t t {0};
	for(t ; t <= baseDirSize+1 ; ++t) //include '\'
		baseDir[t] = dir_ptr[t];
	
	const int fullDirLength {(int)std::strlen(dir_ptr)};
	DWORD threadError {16000}; // local thread error reciever
	bool firstFolder = true;

	while(t <= fullDirLength){
		if(!firstFolder){
			baseDir[t] = '\\'; //replace '\0' with '\'
			++t;
		}else
			firstFolder = false;

		char backBack{'\\'};
		while(t <= fullDirLength && dir_ptr[t] != backBack){
			baseDir[t] = dir_ptr[t];	
			++t;
		}	
		baseDir[t] = '\0';

		//push into windows.h create dir func
		dirCheck = CreateDirectory(baseDir, access);	
			threadError = GetLastError(); //returns previous threads error code
			if(threadError != 0){
				switch (threadError){
					case 183: std::cerr << "Error: " << baseDir << "... directory already exists." << std::endl;
						break;
					case 3: std::cerr << "ERROR: path not found." << std::endl;
						break;
					default: std::cerr << "ERROR " << threadError 
								<< " code not found." << std::endl;				 
				};
			}else{
				// if error == 0 -> create dir func was successfull
				std::cerr << "Directory ..." << baseDir << " .... successfully created." << std::endl;
			}
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
