/* Name: Sarah Huang
 * Program: lab1.cpp (the review lab)
 * Date: 1/25/21
 * Purpose: To read in a file of room descriptions, dynamically allocate all memory associated with the rooms file, 
 *			and let the player look and navigate through the rooms to find the exit in this text-based game. 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


struct Room{	//BASED ON TAs 8 AM SESSION (sorry I forgot your names!)
	string rmName;
	string rmDescription;
	string rmExits[4]; //Storing max 4 possible exits for a room (n room#, e room#, s room#, w room#)
	int numExits;	  //Storing the actual number of possible exits
};


//Functions
void printDirection(char);	//Prints the direction user is going in based on choice


int main(int argc, char *argv[]){
	//Variables
	ifstream ifs;
	stringstream ss;
	string holder;		//Temporary string holder to store strings from room file
	int numTildas = 0;
	int numRooms = 0;	
	char choice;
	int currentRoom = 0;	//Starting room index is 0	
	string roomNum;
	string direction;


	//This error checking chunk is BASED ON TAs 8 AM SESSION
	if(argc != 2){	//If user doesn't put file name
		cerr << "Usage: " << argv[0] << " [inputfile]" << endl;
		return 1;
	}
	ifs.open(argv[1]); 
	if(!ifs.is_open()){ //If file can't open (ex. file doesn't exist)
		perror(argv[1]); //Provides error message saying why
		return 1;
	}


	//Counting rooms - BASED ON TAs 8 AM SESSION
	while(getline(ifs, holder)){
		if(holder == "~")
			numTildas++;
	}
	numRooms = numTildas/3;


	//Reset the file to read in room info - BASED ON TAs 8 AM SESSION
	ifs.clear();
	ifs.seekg(0);


	//Create pointer
	Room *spaces;
	spaces = new Room[numRooms];

	
	//Combing through the room file and storing all info in memory
	for(int i = 0; i < numRooms; i++){
		//Read room name
		getline(ifs, holder);
		spaces[i].rmName = holder;
		
		//Skip ~
		getline(ifs, holder);
		
		//Read room description -BASED ON TAs 8 AM SESSION
		while(getline(ifs, holder)){
			if(holder != "~"){
				spaces[i].rmDescription += holder;
				spaces[i].rmDescription += "\n"; //Starts the next line if description is multiple lines long
			}
			else
				break;
		}
		cout << spaces[i].rmName << " - " << spaces[i].rmDescription << endl;
		
		//Read possible exits (n #, e #, s #, w #)
		int posExit = 0;	//Has to be initialized here so the number of exits resets for every room
		while(getline(ifs, holder)){
			if(holder != "~"){
				spaces[i].rmExits[posExit] = holder;
				posExit++;				//Increments the index of a Room's exits array
				spaces[i].numExits++;	//Increments the total number of exits in a Room
			}
			else
				break;
		}
	}
	ifs.close();	//Close the room file
	



	//The game actually starts HERE!
	cout << "> ";
	cin >> choice;

	//Error check - if user inputs invalid choice, will be prompted again
	while(choice != 'q' && choice != 'l' && choice != 'n' && choice != 'e' && choice != 's' && choice != 'w'){
		cout << "> ";
		cin >> choice;
	}

	//The game will loop until the user enters 'q' to quit
	while(choice != 'q'){
		if(choice == 'l'){	//Look option - print room name, description, and direction exits
			cout << spaces[currentRoom].rmName << endl;
			cout << spaces[currentRoom].rmDescription << endl;
			cout << "Exits: ";
			//Prints out the first char of the direction string (n,e,s,w) while incrementing through Room's exits array
			for(int index = 0; index < spaces[currentRoom].numExits; index++){
				cout << spaces[currentRoom].rmExits[index][0];
				if(index + 1 != spaces[currentRoom].numExits) //If the next exit is not the last one, it will print a comma
					cout << ", ";
			}
		}
		else if(choice == spaces[currentRoom].rmExits[0][0]){ //User enters exit #1 letter - either n, e, s, or w
			//We don't know exactly what's first exit's direction, so we check here
			printDirection(choice);

			//Extracts the room number from the Room's exit array  (ex. s 12)
			//											     	index#: 0123
			roomNum = spaces[currentRoom].rmExits[0][2];
			if(spaces[currentRoom].rmExits[0][3] != -1)	//If the next char is a number and exists = double digits
				roomNum += spaces[currentRoom].rmExits[0][3];
			//Changes string roomNum to integer to change advance to the next room
			stringstream ss(roomNum);
			ss >> currentRoom;
		}
		
		//!!!THE NEXT 3 ELSE IFs ARE THE EXACT SAME BUT FOR THE OTHER 3 POSSIBLE EXIT DIRECTIONS!!!

		else if(choice == spaces[currentRoom].rmExits[1][0]){ //Advances through exit #2 (if it exists)
			//Actually figure out which direction
			printDirection(choice);

			//Extract room number and change into int to change rooms
			roomNum = spaces[currentRoom].rmExits[1][2];
			if(spaces[currentRoom].rmExits[1][3] != -1)
				roomNum += spaces[currentRoom].rmExits[1][3];
			stringstream ss(roomNum);
			ss >> currentRoom;
		}
		else if(choice == spaces[currentRoom].rmExits[2][0]){ //Advances through exit #3 (if it exists)
			//Extract which direction it is
			printDirection(choice); 

			//Extract room number and change into int to advance rooms
			roomNum = spaces[currentRoom].rmExits[2][2];
			if(spaces[currentRoom].rmExits[2][3] != -1)
				roomNum += spaces[currentRoom].rmExits[2][3];
			stringstream ss(roomNum);
			ss >> currentRoom;
		}
		else if(choice == spaces[currentRoom].rmExits[3][0]){ //Advances through exit #4 (if it exists)
			//Extract which direction it is
			printDirection(choice);

			//Extract room number and change into int to advance rooms
			roomNum = spaces[currentRoom].rmExits[3][2];
			if(spaces[currentRoom].rmExits[3][3] != -1){
				roomNum += spaces[currentRoom].rmExits[3][3];
			}
			stringstream ss(roomNum);
			ss >> currentRoom;
		}

		//If the direction choice is not a possible exit
		else{
			if(choice == 'n')
				direction = "NORTH";
			else if(choice == 'e')
				direction = "EAST";
			else if(choice == 's')
				direction = "SOUTH";
			else if(choice == 'w')
				direction = "WEST";
			cout << "You can't go " << direction << "!";
		}
		
	
		//Asks for another choice and will quit program if it's 'q'
		cout << endl;
		cout << "> ";
		cin >> choice;
		while(choice != 'q' && choice != 'l' && choice != 'n' && choice != 'e' && choice != 's' && choice != 'w'){
			cout << "> ";
			cin >> choice;
		}
	}	
	
	delete[] spaces;	//Delete the memory
	return 0;
}

//Based off the user input, it prints which direction the user is going in
void printDirection(char c){
	string d;
	if(c == 'n')
		d = "NORTH";
	else if(c == 'e')
		d = "EAST";
	else if(c == 's')
		d = "SOUTH";
	else
		d = "WEST";
	cout << "You moved " << d << ".";
}
