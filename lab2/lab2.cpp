/*Name: Sarah Huang
 *Date: 
 *Program: lab2.cpp (Bitset Lab)
 *Purpose: 
 */

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <ios>
#include <limits>
#include <cmath>

using namespace std;

const unsigned int N_BITS = sizeof(int) * 8; //Gregory

class BITSET {	//Gregory
	private:
		vector <int> bits;	//32 bits per set (0-31, 32-63, 64-95, 96-127, etc.)
	public:
		BITSET();
		bool Test(unsigned int idx) const;
		void Set(unsigned int idx);
		void Clear(unsigned int idx);
		int GetNumSets() const;
		int GetSet(int idx) const;
};

string ToBinary(unsigned int value, int spacing);


int main(){

	string answer; //User's full choice (ex. t 21)
	char choice;   //Just the command
	string intParam;  //Whatever the int parameter user puts (but it's a string for now)
	int realInt;    //User input as an actual int
	BITSET bitset;
	string testVal;


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
	
	for(int index = 2; index < 5; index++){
		if(answer[index] != -1)
			intParam += answer[index];
	}
	stringstream ss(intParam);
	ss >> realInt;
	


	while(choice != 'q'){
		if(choice == 't'){
			//Prints 1 or 0 if bit number is 1 or 0
			if(bitset.Test(realInt))
				testVal = "true";
			else
				testVal = "false";
			cout << "Does bit " << realInt << " equal 1? " << testVal << endl << endl;
		}

		else if(choice == 's'){
			//Sets given bit to 1
			bitset.Set(realInt);
		}

		else if(choice == 'c'){
			//Clears the given bit to 0
			bitset.Clear(realInt);
		}	

		else if(choice == 'g'){
			//Prints binary representation of set
			cout << ToBinary(bitset.GetSet(realInt), 4) << endl << endl;
		}
		
		else if(choice == 'n'){
			//Prints number of sets in vector
			cout << "Number of sets: " << bitset.GetNumSets() << endl << endl;
		}



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

		for(int index = 2; index < 5; index++){
			if(answer[-1] != -1)
				intParam += answer[index];
		}
		stringstream ss(intParam);
		ss >> realInt;
	}

	return 0;
}




BITSET::BITSET(){ 
	//Initialize vector to size 1 with value 0
	bits.resize(1, 0); //Size = 1, puts in 0 as default
	
}
	
bool BITSET::Test(unsigned int idx) const {
	bool is1 = false;
	
	if(idx >= (bits.size() * N_BITS))
		is1 = false;
	
	else{
		for(unsigned int set = 0; set <= bits.size() - 1; set++){
			if((set*N_BITS) <= idx && idx < ((set+1)*N_BITS)){
				for(unsigned int bit = 0; bit < N_BITS; bit++){
					if(1 == ((bits.at(idx/N_BITS) >> idx) & 1)){
						is1 = true;
					}
				}
			}
		}
	}
	return is1; 
}


void BITSET::Set(unsigned int idx){
	//Growing the vector if needed
	int oldSize = bits.size();
	if(idx >= (bits.size() * N_BITS)){
		while(idx >= (bits.size() * N_BITS)){
			bits.resize(bits.size() + 1);
		}
		for(unsigned int i = oldSize; i < bits.size(); i++){
			bits.at(i) = 0;
		}
	}

	//Setting the bit at the given index equal to 1
	bits.at(idx/N_BITS) |= (1 << idx);
}


void BITSET::Clear(unsigned int idx){
	//Shrinking vector
	if(bits.at(idx/N_BITS) != 0){
		if(idx < (bits.size() * N_BITS)){
			bits.at(idx/N_BITS) = bits.at(idx/N_BITS) & ~(1 << idx);
			
			
			
			for(unsigned int set = bits.size() - 1; set > 0; set--){
				if(bits.at(set) == 0)
					bits.resize(bits.size() - 1);
			}
		}
	}
}


int BITSET::GetNumSets() const{
	return bits.size(); 
}


int BITSET::GetSet(int idx) const{	
	int intInSet = 0;
	if(bits.at(idx) != -1)
		intInSet = bits.at(idx);
	return intInSet;
}


string ToBinary(unsigned int value, int spacing){
	string binary;
	int counter = 0; 
	unsigned int currNum;

	if(N_BITS % spacing == 0){
		for(int b = N_BITS - 1; b >= 0; b--){
			if(counter >= spacing){
				binary += " ";
				counter = 0;
			}
			currNum = pow(2, b);
		
			if(currNum > value)
				binary += "0";
			else{
				value -= currNum;	//value = value - currNum	
				binary += "1";
			}
			counter++;
			currNum = 0;
		}
	}
	else{
		for(int b = N_BITS - (N_BITS % spacing); b > 0; b--){
			if(counter >= spacing){
				binary += " ";
				counter = 0;
			}
			currNum = pow(2, b);
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
	return binary;
}
