/* Name:
 * Program:
 * Date:
 * Purpose:
 */


#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

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


const unsigned int NUM_REGS = 32;


struct Machine{
	unsigned int regs[NUM_REGS];
	unsigned int pc;
	unsigned int num_instructions;
	unsigned int *instructions;


	bool reg(const string &name, unsigned int &val){
		for(unsigned int i = 0; i < NUM_REGS; i++){
			if(name == reg_names[i]){
				val = regs[i];
				return true;
			}
		}
		
		/*
		 * istringstream sin(name);
		 * int r;
		 *
		 * if(sin >> r && r >= 0 && r <= 31){
		 *		val = regs[r];
		 *		return true;
		 * }*/
		return false;
	}


	//if you can get the other functions to work in the main, put them here!
	
	
	
};




string leftInput(string);
string rightInput(unsigned int, string);



int main(){
	//ifstream ifs;
	//stringstream ss;
	FILE *fl;


	//String input
	string input;
	char quitCheck = 'n';
	string command;
	string file;
	unsigned int charCount = 0;

	//Machine stuff
	Machine *mac = nullptr;




	//Separate command and file/second input from whole input
	printf("> ");
	getline(cin, input);
	if(input == "quit")
		quitCheck = 'y';

	command = leftInput(input);
	charCount = command.length();
	file = rightInput(charCount, input);




	//LOOP STARTS HERE!!!!
	while(command != "quit" && quitCheck != 'y'){			
		if(command == "regs"){
			if(mac != nullptr){
				int rowCounter = 0;
				unsigned int v = 0;


				for(unsigned int r = 0; r < NUM_REGS; r++){
					mac->reg(reg_names[r], v);
					short value = v & 0xffff;
					if((value >> 15) == 1)
						value = ~value + 1;

					if(reg_names[r] == "$zero")
						printf("%s : 0x%08x (%5d) ", reg_names[r].c_str(), v, value);
					else
						printf("%s   : 0x%08x (%5d) ", reg_names[r].c_str(), v, value);

					rowCounter++;
					if(rowCounter >= 4){
						printf("\n");
						rowCounter = 0;
					}
				}
				printf("PC: %u\n", mac->pc);
			}
			else{
				printf("No machine loaded\n");
			}
		}


		else if(command == "reg"){
			unsigned int v = 0;
			if(mac != nullptr){
				mac->reg(file, v);
				short value = v & 0xffff;
				if((value >> 15) == 1)
					value = ~value + 1; 
				
				printf("%s   : 0x%08x ( %d)", file.c_str(), v, value);
			}
			else{
				printf("No machine loaded\n");
			}
			printf("\n");
		}



		else if(command == "run"){	
			//next but as a for loop
			//why the hell is it an unknown command, good question
			if(mac != nullptr){
				printf("run\n");
			}
			else{
				printf("No machine loaded\n");
			}
		}



		else if(command == "next"){
			if(mac == nullptr){
				printf("No machine loaded\n");
			}
			else if(mac->pc >= (mac->num_instructions * 4)){
				printf("No more instructions to run.\n");
			}
			else{
				unsigned int currInstruction = mac->instructions[mac->pc/4];	
				//printf("%d\n", currInstruction);
				

				//if it's not special
				unsigned int opcode = (currInstruction >> 26) & 0x3f;
				//printf("opcode: %u\n", opcode);
		
				int rs = 0;
				int rt = 0;
				int rd = 0;
				int sa = 0;
				unsigned int opend = 0;
				short immediate = 0;
				short offset = 0;
				

				switch(opcode){
					case 8:
						rs = (currInstruction >> 21) & 0x1f;
						rt = (currInstruction >> 16) & 0x1f;
						immediate = currInstruction & 0xffff;
						if((immediate >> 15) == 1)
							immediate = ~immediate + 1;
						
						mac->regs[rt] = mac->regs[rs] + immediate;
						//printf("rs: %d, rt: %d, immediate: %d\n", rs, rt, immediate);
						break;
					

					case 4:
						rs = (currInstruction >> 21) & 0x1f;
						rt = (currInstruction >> 16) & 0x1f;
						offset = currInstruction & 0xffff;
						if((offset >> 15) == 1)
							offset = ~offset + 1;

						//printf("rs: %d, rt: %d, offset: %d\n", rs, rt, offset);
						if(mac->regs[rs] == mac->regs[rt]){
							mac->pc = mac->pc + (offset * 4);
						}
						break;


					case 5:
						rs = (currInstruction >> 21) & 0x1f;
						rt = (currInstruction >> 16) & 0x1f;
						offset = currInstruction & 0xffff;
						if((offset >> 15) == 1)
							offset = ~offset + 1;

						//printf("rs: %d, rt: %d, offset: %d\n", rs, rt, offset);
						if(mac->regs[rs] != mac->regs[rt]){
							mac->pc = mac->pc + (offset * 4);
						}
						break;


					case 0:
						opend = currInstruction & 0x3f;
						

						if(opend == 32){
							rs = (currInstruction >> 21) & 0x1f;
							rt = (currInstruction >> 16) & 0x1f;
							rd = (currInstruction >> 11) & 0x1f;
							//printf("opend: %u, rs: %d, rt: %d, rd: %d\n", opend, rs, rt, rd);
							mac->regs[rd] = mac->regs[rs] + mac->regs[rt];
							break;
						}

						else if(opend == 36){
							rs = (currInstruction >> 21) & 0x1f;
							rt = (currInstruction >> 16) & 0x1f;
							rd = (currInstruction >> 11) & 0x1f;
							//printf("opend: %u, rs: %d, rt: %d, rd: %d\n", opend, rs, rt, rd);
							mac->regs[rd] = mac->regs[rs] & mac->regs[rt];
							break;
						}

						else if(opend == 37){
							rs = (currInstruction >> 21) & 0x1f;
							rt = (currInstruction >> 16) & 0x1f;
							rd = (currInstruction >> 11) & 0x1f;
							//printf("opend: %u, rs: %d, rt: %d, rd: %d\n", opend, rs, rt, rd);
							mac->regs[rd] = mac->regs[rs] | mac->regs[rt];
							break;
						}

						else if(opend == 0){
							rt = (currInstruction >> 16) & 0x1f;
							rd = (currInstruction >> 11) & 0x1f;
							sa = (currInstruction >> 6) & 0x1f;
							//printf("opend: %u, rt: %d, rd: %d, sa:%d\n", opend, rt, rd, sa);
							mac->regs[rd] = mac->regs[rt] << mac->regs[sa];
							break;
						}

						else if(opend == 42){ //set on less
							rs = (currInstruction >> 21) & 0x1f;
							rt = (currInstruction >> 16) & 0x1f;
							rd = (currInstruction >> 11) & 0x1f;
							//printf("opend: %u, rs: %d, rt: %d, rd: %d\n", opend, rs, rt, rd);
							if(mac->regs[rs] < mac->regs[rt])
								mac->regs[rd] = 1;
							else
								mac->regs[rd] = 0;
							break;
						}
						
						//AHHHH KILLLL MEEEEE
						else if(opend == 3){
							rt = (currInstruction >> 16) & 0x1f;
							rd = (currInstruction >> 11) & 0x1f;
							sa = (currInstruction >> 6) & 0x1f;
							//printf("opend: %u, rt: %d, rd: %d, sa: %d\n", opend, rt, rd, sa);
							
							mac->regs[rd] = (signed)mac->regs[rt] >> sa;
							break;
						}

						else if(opend == 2){
							rt = (currInstruction >> 16) & 0x1f;
							rd = (currInstruction >> 11) & 0x1f;
							sa = (currInstruction >> 6) & 0x1f;
							//printf("opend: %u, rt: %d, rd: %d, sa: %d\n", opend, rt, rd, sa);
							mac->regs[rd] = mac->regs[rt] >> sa;
							break;
						}

					default:
						printf("Unknown instruction...continuing.\n");
						break;
				}

				mac->pc += 4;
			}
		}




		else if(command == "load"){
			//hopefully i can make a function of this later
			//if(mac != nullptr){
			//	delete mac;
			//	Machine *mac = nullptr;
			//}
			
			fl = fopen(file.c_str(), "rb");
			if(!fl){
				printf("Can't open file!\n");
			}
			else{
				mac = new Machine;	//not equal to nullptr
				fseek(fl, 0, SEEK_END);	
				mac->num_instructions = ftell(fl) / 4;	//calc total # of instructions
				mac->instructions = new unsigned int[mac->num_instructions];
				
				//Read in instruction from file and store in respective instruction array index
				fseek(fl, 0, SEEK_SET);
				
				
				for(unsigned int i = 0; i < mac->num_instructions; i++){
					fread(mac->instructions + i, 1, 4, fl); //every instruction is 4 bytes	
					//cout << "Instruction decimal #: " << mac->instructions[i] << endl;
				}
				fclose(fl);
				mac->pc = 0;
			}
		}




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
			quitCheck = 'n';
			printf("> ");
			getline(cin, input);

			command = leftInput(input);
			charCount = command.length();
			file = rightInput(charCount, input);	
		}
	}
	return 0;
}





string leftInput(string choice){
	string left;
	for(int ch = 0; ch <= 3; ch++){
		if(choice[ch] != ' ')
			left += choice[ch];
	}
	return left;
}


string rightInput(unsigned int index, string choice){
	string right;
	for(unsigned int ch = index; ch < choice.length(); ch++){
		if(choice[ch] != ' '){
			right += choice[ch];
		}
	}
	return right;
}

