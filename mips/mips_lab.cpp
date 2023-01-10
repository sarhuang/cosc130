/*
 * Name: Sarah Huang
 * Program: mips_lab.cpp (MIPS Machine Lab)
 * Date: 3/25/21
 * Purpose: Simulate a MIPS processor using a given binary file with assembly instructions 
 *			and translating/running those instructions in C++.
 */

//Also shoutout to TA Dylan Lee! Where would I be without the pseudocode and bits of code crumbs?

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

using namespace std;




const unsigned int NUM_REGS = 32;

//Instruction opcode in decimal
const unsigned int ADDI = 8;
const unsigned int BEQ = 4;
const unsigned int BNE = 5;
const unsigned int ADD = 32;
const unsigned int AND = 36;
const unsigned int OR = 37;
const unsigned int SLL = 0;
const unsigned int SLT = 42;
const unsigned int SRA = 3;
const unsigned int SRL = 2;

//Number of bits to shift for each part
const unsigned int OP_SHIFT = 26;
const unsigned int RS_SHIFT = 21;
const unsigned int RT_SHIFT = 16;
const unsigned int RD_SHIFT = 11;
const unsigned int SA_SHIFT = 6;

const string reg_names[] = {
	"$zero",
	"$at",
	"$v0",			
	"$v1",
	"$a0",
	"$a1",
	"$a2",
	"$a3",
	"$t0",
	"$t1",
	"$t2",
	"$t3",
	"$t4",
	"$t5",
	"$t6",															   
	"$t7",
	"$s0",
	"$s1",																		   
	"$s2",	
	"$s3",
	"$s4",
	"$s5",
	"$s6",
	"$s7",
	"$t8",
	"$t9",
	"$k0",
	"$k1",
	"$gp",
	"$sp",
	"$fp",																					
	"$ra"
};



struct Machine{
	unsigned int regss[NUM_REGS];	//array holding every register value
	unsigned int pc;				//program counter used to keep track of which instruction to run
	unsigned int num_instructions;	//total number of instructions
	unsigned int *instructions;		//pointer array to store every instruction
	

	//Searches the registers for given register stores its value in a variable (second parameter)
	bool search(const string &name, unsigned int &val){
		for(unsigned int i = 0; i < NUM_REGS; i++){
			if(name == reg_names[i]){
				val = regss[i];
				return true;
			}
		}
		return false;
	}
	


	//COMMAND: reg <register>
	//Prints out given register's stored hexidecimal and decimal values
	void reg(const string &regInput){
		unsigned int v = 0;
		search(regInput, v);
				
		//https://stackoverflow.com/questions/14733761/printf-formatting-for-hex
		//Help with extra zeroes
		for(unsigned int r = 0; r < NUM_REGS; r++){
			if(regInput == reg_names[r])
				printf("%s   : 0x%08x ( %d)\n", regInput.c_str(), v, v); 
		}
	}



	//COMMAND: regs
	//Prints out all the registers and their stored hexidecimal and decimal values
	void regs(){
		int rowCounter = 0;
		unsigned int v = 0;	//Value of register
		for(unsigned int r = 0; r < NUM_REGS; r++){
			search(reg_names[r], v);	//Grab register value and store in v	


			//Prints 4 registers in a row until all registers are printed out
			//https://stackoverflow.com/questions/1809399/how-to-format-strings-using-printf-to-get-equal-length-in-the-output - help with spacing
			if(reg_names[r] == "$zero") //Also accounting for the extra char $zero takes up
				printf("%s : 0x%08x (%5d) ", reg_names[r].c_str(), v, v);
			else
				printf("%s   : 0x%08x (%5d) ", reg_names[r].c_str(), v, v);
			rowCounter++;
			if(rowCounter >= 4){
				printf("\n");
				rowCounter = 0;
			}
		}
		printf("PC    : %u\n", pc);
	}
	

	//COMMAND: next
	//Executes the next instruction and stop
	void next(bool doingRun){	//bool doingRun = true if the command is "run" and false if it's "next" ("run" does not print if the command is unknown)
		//Bitshifting and masking all the necessary components
		unsigned int currInstruction = instructions[pc/4];
		unsigned int opcode = (currInstruction >> OP_SHIFT) & 0x3f;
		int rs = (currInstruction >> RS_SHIFT) & 0x1f;
		int rt = (currInstruction >> RT_SHIFT) & 0x1f;
		int rd = (currInstruction >> RD_SHIFT) & 0x1f;
		int sa = (currInstruction >> SA_SHIFT) & 0x1f;
		unsigned int opend = currInstruction & 0x3f;	//Last 6 bits
		short offsetImmediate = currInstruction & 0xffff;

		pc += 4;

		switch(opcode){
			case ADDI:
				regss[rt] = regss[rs] + offsetImmediate;
				break;


			case BEQ:
				if(regss[rs] == regss[rt])
					pc = pc + (offsetImmediate * 4);	//Like jumping to different instructions
				break;


			case BNE:
				if(regss[rs] != regss[rt])
					pc = pc + (offsetImmediate * 4);
				break;


			case 0:
				if(opend == ADD){
					regss[rd] = regss[rs] + regss[rt];
					break;
				}
				else if(opend == AND){
					regss[rd] = regss[rs] & regss[rt];
					break;
				}
				else if(opend == OR){
					regss[rd] = regss[rs] | regss[rt];
					break;
				}
				else if(opend == SLL){
					regss[rd] = regss[rt] << regss[sa];
					break;
				}
				else if(opend == SLT){
					if(regss[rs] < regss[rt])	//Sets 1 (true) if the comparison is true
						regss[rd] = 1;
					else
						regss[rd] = 0;
					break;
				}
				else if(opend == SRA){
					regss[rd] = (signed) regss[rt] >> sa; //SRA requires the value to be signed
					break;
				}
				else if(opend == SRL){
					regss[rd] = regss[rt] >> sa;
					break;
				}

			default:	//Since nonexisting and special both have opcode of 0, will run if it hasn't break yet
				if(!doingRun)
					printf("Unknown instruction...continuing.\n");
				break;
		}
		//pc += 4;	//Every instruction is 4 bytes
	}
};




string leftInput(string);					
string rightInput(unsigned int, string);


int main(){
	FILE *fl;
	Machine *mac = nullptr;
	
	//String input variables
	string input;
	char quitCheck = 'n';			//Another test to verify that the user typed 'quit'
	string command;					//First argument  (ex. load, regs, next, etc.)	
	string secondArg;				//Second argument (ex. load _____ )
	unsigned int charCount = 0;		//Total number of characters in input string



	//Asking for input the first time
	printf("> ");
	getline(cin, input);
	if(input == "quit")
		quitCheck = 'y';	


	//Separates the first and second arguments (if applicable) into separate variables
	command = leftInput(input);	
	charCount = command.length();
	secondArg = rightInput(charCount, input);
	


	while(command != "quit" && quitCheck != 'y'){
	//Dump the given register in both hexadecimal and decimal. 
	//<register> must be a register name including the $, such as $ra or $t2.
		if(command == "reg"){
			if(mac != nullptr)
				mac->reg(secondArg);	
			else
				printf("No machine loaded\n");
		}


		//Dump all registers
		else if(command == "regs"){
			if(mac != nullptr)
				mac->regs();
			else
				printf("No machine loaded\n");
		}
		

		//Run the next instruction and stop.
		else if(command == "next"){
			if(mac == nullptr)
				printf("No machine loaded\n");
			else if(mac->pc >= (mac->num_instructions * 4)) //num_instructions * 4 bytes
				printf("No more instructions to run.\n");
			else{	
				mac->next(false);	//False = do not print "unknown instruction"
				mac->regss[0] = 0;	//Set $zero to 0
			}
		}
		

		//Run the file until there are no more instructions left to run (PC > size).
		else if(command == "run"){
			if(mac == nullptr)
				printf("No machine loaded\n");
			else{
				while(mac->pc < mac->num_instructions * 4){
					mac->next(true);	//True = print "unknown instruction"
					mac->regss[0] = 0;
				}
			}
		}


		//Load a new binary program given by <filename>.
		else if(command == "load"){
			fl = fopen(secondArg.c_str(), "rb");
			if(!fl)
				printf("Can't open file.\n");
			else{
				mac = new Machine; //no longer nullptr
			
				//Finding number of instructions
				fseek(fl, 0, SEEK_END);
				mac->num_instructions = ftell(fl) / 4;
				mac->instructions = new unsigned int[mac->num_instructions];

				//Storing an instruction (as a decimal #) in each index
				fseek(fl, 0, SEEK_SET);
				for(unsigned int i = 0; i < mac->num_instructions; i++){
					fread(mac->instructions + i, 1, 4, fl);
					//cout << mac->instructions[i] << endl;
				}
				fclose(fl);
				mac->pc = 0;
			}
		}
		

		//Quit the program.
		else if(command == "quit"){
			quitCheck = 'y';
		}
		else{
			printf("Unknown command\n");
		}



		if(quitCheck == 'y'){
			command = "quit";
			delete mac;
		}
		else{
			//Ask for new input
			quitCheck = 'n';
			printf("> ");
			getline(cin, input);

			command = leftInput(input);
			charCount = command.length();
			secondArg = rightInput(charCount, input);
		}
	}
	return 0;
}





//Takes the entire user input and stores the first 4 characters (not space) into a new string and returns that.
string leftInput(string choice){
	string left;
	for(unsigned int ch = 0; ch <= 3; ch++){
		if(choice[ch] != ' ' && ch != choice.length())		
			left += choice[ch];
	}
	return left;
}


//Takes the entire user input and stores the remaining characters after the space (if there is a second argument) into a new string and returns that.
string rightInput(unsigned int index, string choice){
	string right;
	for(unsigned int ch = index; ch < choice.length(); ch++){
		if(choice[ch] != ' ')
			right += choice[ch];
	}
	return right;
}
