// merge sort algo
// recrusive (depends on return from same func)
#import <iostream>
#import <vector>
#import <cmath>

std::vector <int> mergeSort(std::vector <int>& array){
	if(array.size() == 1) //base case -> return array and pop
			return array;
	else{
		int arraySizeA {0}, arraySizeB {0};
		if(array.size() % 2 != 0){
				// if odd size -> splits into non equal sub array sizes
			arraySizeA = (int)(floor((double)array.size() / 2));
			arraySizeB = arraySizeA + 1; // equivalant to array.size()/2 
		}else{
			arraySizeA = (int)(array.size() / 2);
			arraySizeB = (int)(array.size() / 2);
		}

		// declare subarrays with computed sizes
		std::vector<int> subArrayA(arraySizeA, 0);
		std::vector<int> subArrayB(arraySizeB, 0);
		
		{
		size_t a{0}; 
		for(a; a < arraySizeA ; ++a){
			subArrayA.at(a) = array.at(a);
		}

		size_t b {a}, sub_b_index {0};
		while(b < array.size()){
			subArrayB.at(sub_b_index) = array.at(b);
			++b;
			++sub_b_index;
		}
		}
		
		// ctor null subarrays (will be passed sorted subarrays) 
		std::vector<int> sortedSubArrayA(arraySizeA, 0);
		std::vector<int> sortedSubArrayB(arraySizeB, 0);
		// pass unsroted subarrays back into merge func (recrusive)
		sortedSubArrayA = mergeSort(subArrayA);
		sortedSubArrayB = mergeSort(subArrayB);

		std::cout << "sorted sub a: ";
		for(int &x : sortedSubArrayA){
			std::cout << x << " ";
		}
		std::cout << "\nsorted sub b: ";
		for(int &y : sortedSubArrayB){
			std::cout << y << " ";
		}
		std::cout << std::endl;

		// point to start of sorted subarrays
		int a{0}, b{0};
		int ind{0}; //used to index into to-be-returned sorted array
		std::vector <int> sortedArray((int)array.size(), 0);
		while(a < arraySizeA && b < arraySizeB){ 
			if(sortedSubArrayA.at(a) < sortedSubArrayB.at(b)){
				sortedArray.at(ind) = sortedSubArrayA.at(a);
				++a;
			}else if (sortedSubArrayB.at(b) < sortedSubArrayA.at(a)){
				sortedArray.at(ind) = sortedSubArrayB.at(b);
				++b;
			}else{ // if equal -> index both & increment both
				sortedArray.at(ind) = sortedSubArrayA.at(a);
				++a;
				sortedArray.at(ind) = sortedSubArrayB.at(b);
				++b;
			}
			++ind;
		}

		// index in rest of elements of subarray that still has 'unread' elements
	while(b < arraySizeB){
		sortedArray.at(ind) = sortedSubArrayB.at(b);
		++b;
		++ind;
	}
	while(a < arraySizeA){
		sortedArray.at(ind) = sortedSubArrayA.at(a);
		++a;
		++ind;
	}

	return sortedArray;
	}
}


int main(){
	std::vector <int> numArray {5,6,9,7,8};
	numArray = mergeSort(numArray);
	for(auto &x : numArray)
		std::cout << x << " ";
	std::cout << std::endl;

	return 0;
}
