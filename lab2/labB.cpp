/* Name: Sarah Huang
 * Date: 2/4/21
 * Program: labB.cpp (Bitset Lab)
 * Purpose: To simulate bitwise commands (test, clear, set) with bitwise operators (&, |, ~, <<, >>) and a vector.
 */

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath> //Needed to use the pow function

using namespace std;


class BITSET {	//Thanks to TA Gregory for getting me started! You da best!	
	private:
		vector <int> vecbits;	//Every index/set stores an integer base 10
	public:
		BITSET();

		//Function prototypes
		bool Test(unsigned int index) const;	//Checks if given bit number is 1 or 0	
		void Set(unsigned int index);		//Sets given bit number to 1
		void Clear(unsigned int index);		//Sets given bit number to 0
		int GetNumSets() const;			//Returns total number of indeces/sets in vector
		int GetSet(int index) const;		//Returns integer base 10 from given vector index/set
};

string ToBinary(unsigned int value, int spacing);	//Prints integer base 10 from requested set as a binary number with given spaces


int main(){
	//Variables
	string answer;			//User's full choice (ex. t 21)
	char choice;		//Just the char command
	string intParam;	//Whatever the int parameter user put (but it's a string for now)
	int realInt;		//User input as an actual int
	BITSET bitset;	
	string testVal;		//Test function's true or false instead of 1 and 0

	//Command format - char int (ex. s 31 = set bit 31)
	//Error checking to make sure the command char is a legitimate choice
	//If the user puts in a dash/negative, the loop will repeat 
	choice = 'z';
	while(choice != 't' && choice != 's' && choice != 'c' && choice != 'g' && choice != 'n' && choice != 'q'){
		cout << "> ";
		getline(cin, answer);
		choice = answer[0];
		if(answer[1] != -1)
			answer += " ";

		if(answer[2] == '-')
			choice = 'z';
	}

	//Collects numbers from user's input and converts them to actual, usable integers
	for(int index = 2; index < 12; index++){
		if(answer[index] != -1)
			intParam += answer[index];
	}
	stringstream ss(intParam);
	ss >> realInt;


	//Menu loop that will continue until the user enters 'q'
	while(choice != 'q'){
		if(choice == 't'){	//Test
			if(bitset.Test(realInt))
				testVal = "true";
			else
				testVal = "false";
			cout << "Does bit " << realInt << " equal 1? " << testVal << endl << endl;
		}
		else if(choice == 's')	//Set
			bitset.Set(realInt);

		else if(choice == 'c')	//Clear
			bitset.Clear(realInt);

		else if(choice == 'g')	//ToBinary
			cout << ToBinary(bitset.GetSet(realInt), 4) << endl << endl;

		else if(choice == 'n')	//NumberOfSets
			cout << "Number of sets: " << bitset.GetNumSets() << endl << endl;


		//Asks the user again for a command and error checks
		choice = 'z';
		while(choice != 't' && choice != 's' && choice != 'c' && choice != 'g' && choice != 'n' && choice != 'q'){
			ss.clear();
			intParam = "";
			realInt = 0;	
			cout << "> ";
			getline(cin, answer);
			choice = answer[0];

			if(answer[1] != -1)
				answer += " ";		
			if(answer[2] == '-')
				choice = 'z';
		}

		//Converts string user input integer to real integer
		for(int index = 2; index < 12; index++){
			if(answer[-1] != -1)
				intParam += answer[index];
		}
		stringstream ss(intParam);
		ss >> realInt;
	}
	return 0;
}



//Initialize vector to size 1 with value 0 as default
BITSET::BITSET(){ 
	vecbits.resize(1, 0);
}


//Takes in given bit number and checks if it is 0 or 1
bool BITSET::Test(unsigned int index) const {
	bool is1 = false;
	if(index >= (vecbits.size() * 32))	//If the index is greater than the total number of bits in the vector, it is definetly a 0 (false)
		is1 = false;

	else{
		for(unsigned int set = 0; set <= vecbits.size() - 1; set++){		//Running through each set in vector
			if((set*32) <= index && index < ((set+1)*32)){			//Checking if the bit number is between the set ranges 0-31, 32-63, etc.
				for(unsigned int bit = 0; bit < 32; bit++){		//Running through each individual bit in the set
					if(1 == ((vecbits.at(index/32) >> index) & 1)){	
						is1 = true;
					}
				}
			}
		}
	}
	return is1; 
}


//Sets given bit number to 1
void BITSET::Set(unsigned int index){
	//Growing the vector if needed
	int oldSize = vecbits.size();
	if(index >= (vecbits.size() * 32)){	//If the bit number is greater than the total number of bits in vector, increase the size by 1 until it is not
		while(index >= (vecbits.size() * 32)){
			vecbits.resize(vecbits.size() + 1);
		}
		for(unsigned int i = oldSize; i < vecbits.size(); i++){	//Set newly created sets to 0
			vecbits.at(i) = 0;
		}
	}

	//Setting the bit at the given index equal to 1
	vecbits.at(index/32) |= (1 << index);
}


//Sets/clears given bit number to 0
void BITSET::Clear(unsigned int index){
	if(vecbits.at(index/32) != 0){	//No need to clear anything if a set already equals zero
		if(index < (vecbits.size() * 32)){
			vecbits.at(index/32) = vecbits.at(index/32) & ~(1 << index);


			//Shrinks the vector - cycle through every set and remove those equal to 0
			for(unsigned int set = vecbits.size() - 1; set > 0; set--){
				if(vecbits.at(set) == 0)
					vecbits.resize(vecbits.size() - 1);
			}
		}
	}
}


//Returns the number of sets/indeces in the vector
int BITSET::GetNumSets() const{
	return vecbits.size(); 
}


//Returns the base 10 integer from the given set
int BITSET::GetSet(int index) const{	
	int intInSet = 0;
	if(vecbits.at(index) != -1)
		intInSet = vecbits.at(index);
	return intInSet;
}


//Returns the binary number form of a set's base 10 integer in string form
string ToBinary(unsigned int value, int spacing){
	string binary;
	int counter = 0; 
	unsigned int currNum;	

	if(32 % spacing == 0){	//If the given spaces is a factor of 32
		for(int b = 32 - 1; b >= 0; b--){
			if(counter >= spacing){	//If there are 4 digits in a row, add a space, and reset
				binary += " ";
				counter = 0;
			}
			currNum = pow(2, b);	//2 to the power of b

			if(currNum > value)	
				binary += "0";
			else{
				value -= currNum;	
				binary += "1";
			}
			counter++;
			currNum = 0;
		}
	}
	else{	//If the given spaces is NOT a factor of 32
		for(int b = 32 - (32 % spacing); b > 0; b--){	//Should prevent left over digits
			if(counter >= spacing){
				binary += " ";
				counter = 0;
			}
			currNum = pow(2, b);
			if(currNum > value)
				binary += "0";
			else{																																										value -= currNum;
				binary += "1";
			}
			counter++;
			currNum = 0;
		}
	}
	return binary;
}
