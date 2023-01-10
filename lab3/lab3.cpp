/* Name: Sarah Huang
 * Program: lab3.cpp (Binary Arithmetic lab) 
 * Date: 2/11/21
 * Purpose: Replicate addition, subtraction, multiplication, division, and modulus by manipulating bits
 */

#include <cstdio>

int Multiply(int leftop, int rightop);	//Multiply two numbers
int Add(int leftop, int rightop);		//Add two numbers
int Sub(int leftop, int rightop);		//Subtract two numbers
int Twos(int op);						//Two's compliment
int Div(int leftop, int rightop);		//Divide two numbers
int Mod(int leftop, int rightop);		//Divide two numbers but return remainder


int main(int argc, char *argv[]){
	int left, right, result;
	char op;

	if(4 != argc){	//Check if there are 4 command line arguments (file, leftop, op, rightop)
		printf("Usage : %s <left> <op> <right>\n", argv[0]);
		return -1;
	}
	sscanf(argv[1], "%d", &left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%d", &right);

	switch(op){
		case 'm':
		case 'x':
		case '*':
			result = Multiply(left, right);
			break;
		case 'a':
		case '+':
			result = Add(left, right);
			break;
		case 's':
		case '-':
			result = Sub(left, right);
			break;
		case 'd':
		case '/':
			result = Div(left, right);
			break;
		case '%':
			result = Mod(left, right);
			break;
		default:
			result = -11111111;
			break;
	}
	printf("%d\n", result);
	return 0;
}



int Add(int leftop, int rightop){	
	int outcome = 0; //Each loop iteration's sum

	int carry = 0;	
	int topBit, bottomBit;
	int answer = 0;	//Final sum

	for(int bit = 0; bit < 32; ++bit){
		topBit = ((leftop >> bit) & 1);		//Isolate bit in top number
		bottomBit = ((rightop >> bit) & 1);	//Isolate bit in bottom number
		
		outcome = (topBit ^ bottomBit) ^ carry;	//1 or 0
		carry = ((topBit & bottomBit) ^ (carry & topBit) ^ (carry & bottomBit));	//1 or 0
		answer |= (outcome << bit);
	}
	return answer;
}


int Twos(int op){
	//Reverse the bits and add 1
	op = Add(~op, 1);
	return op;
}


int Sub(int leftop, int rightop){
	//Twos compliment the bottom number and add it to the top number
	rightop = Twos(rightop);
	return Add(leftop, rightop);
}



int Multiply(int leftop, int rightop){	//Thanks to http://web.eecs.utk.edu/~cmuncey/cs130/binary_arithmetic.html for the help!
	int outcome = 0;
	int top31 = ((leftop >> 31) & 1);	//31st bit of the top number
	int bot31 = ((rightop >> 31) & 1);	//31st bit of the bottom number
	int negative = top31 ^ bot31;		//1 if negative and 0 if positive

	//Does Twos Compliment if either or both are negative
	if(top31)	
		leftop = Twos(leftop);
	if(bot31)
		rightop = Twos(rightop);
	
	//For every bit, add the top operand if the bottom bit is 1 and then shift top number left 1 and bottom number right 1
	for(int bit = 0; bit < 32; ++bit){
		while(rightop != 0){
			if(((rightop >> bit) & 1))
				outcome = Add(outcome, leftop);
			leftop = (leftop << 1);
			rightop = (rightop >> 1);
		}
	}
	//This piece of code is from http://web.eecs.utk.edu/~cmuncey/cs130/binary_arithmetic.html
	if(negative)
		return (-outcome);
	return outcome;
}


int Div(int leftop, int rightop){	//Thanks to http://web.eecs.utk.edu/~cmuncey/cs130/binary_arithmetic.html for the help!
	int outcome = 0;
	int top31 = ((leftop >> 31) & 1);
	int bot31 = ((rightop >> 31) & 1);
	int negative = top31 ^ bot31;

	if(top31)
		leftop = Twos(leftop);
	if(bot31)
		rightop = Twos(rightop);

	long remainder = leftop;
	long divisor = rightop;
	
	for(int bit = 31; bit >= 0; --bit){		
		//The equation R(i) = R(i+1) - (D << i) from http://web.eecs.utk.edu/~cmuncey/cs130/binary_arithmetic.html
		remainder = remainder - (divisor << bit);
		
		//If there is a remainder, set 1 to the current bit
		if(remainder >= 0)
			outcome |= (1 << bit); 
		//Else add back what we just subtracted
		else
			remainder += (divisor << bit);
	}
	//This code bit is from http://web.eecs.utk.edu/~cmuncey/cs130/binary_arithmetic.html
	if(negative)
		return (-outcome);
	return outcome;
}



int Mod(int leftop, int rightop){	//Basically same as Div but returning the remainder
	int outcome = 0;
	int top31 = ((leftop >> 31) & 1);
	int bot31 = ((rightop >> 31) & 1);
	int negative = top31 ^ bot31;

	if(top31)
		leftop = Twos(leftop);
	if(bot31)
		rightop = Twos(rightop);

	long remainder = leftop;
	long divisor = rightop;

	for(int bit = 31; bit >= 0; --bit){
		remainder = remainder - (divisor << bit);
		if(remainder >= 0)
			outcome |= (1 << bit);
		else
			remainder += (divisor << bit);
	}

	if(negative)
		return (-remainder);
	return remainder;
}
