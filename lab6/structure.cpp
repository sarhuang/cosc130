#include <cstdio>


struct Values{
	char operation;
	short left;
	short right;
	int result;
};

struct Matrix3{
	int mat[9];
};

struct Vector3{
	int vec[3];
};


//Name				Offset		Size
//char operation		0			1
//short left			2			2
//short right			4			2
//int result			8			4
//								
//									12?m			



//Name			Offset			Size
//	int 0			0			4
//	1				4			4
//	2				8			4
//	3				12			4	
//	4				16			4
//	5				20			4
//	6				24			4
//	7				32			4
//	8				36			4
//		
//						Total size: 36



//Name			Offset		Size
//	int 0		0			4
//	1			4			4
//	2			8			4
					
//						Total size: 12



//Takes array of Values and perform operation givein in the field "operation" to every Value's left <op> right and store result in result field
//Do it for num_values number of elements
//Return number of valid operatiosn that were successfully performed
//& --> result = left & right 
//| --> result = left | right
//^ --> result = left ^ right
//~ --> result = ~left 
int gofunc(Values values[], int num_values);



//Perform matrix multiplication on mat x vec and store resulting vector into result
//USE LOOPS
void matmul(Vector3 &result, const Matrix3 &mat, const Vector3 &vec);




int main(){
//Don't prompt for data
//DO NOT TRANSLATE THE MAIN FUNCTION (JUST THE 2 FUNCTIONS!!!)
	
	Values test[] = {
		{'|', 0b1100, 0b0101, 0},	//left = 12
		{'&', 0b1100, 0b0101, 0},	//right = 5
		{'^', 0b1100, 0b0101, 0},	//result = 0
		{'~', 0b1100, 0b0101, 0},
		{'!', 0b1100, 0b0101, 0},
	};
	printf("Number of successful operations: %d\n", gofunc(test, 5));	
/*

	Matrix3 mat {
		1, 0, 1,
		0, 0, 0,
		1, 2, 3
	};
	
	Vector3 in_vec = {
		1, 2, 3	
	};

	Vector3 result;

	matmul(result, mat, in_vec);
*/		
	return 0;
}



int gofunc(Values a0[], int a1){
	int badOps = 0;
	Values* tmp = a0;
	//printf("%p\n", a0);

	for(int t0 = 0; t0 < a1; t0++){
		short t1 = ((a0)->left);				//a0 + offset 2
		short t2 = ((a0)->right);				//a0 + offset 4
		char t3 = ((a0)->operation);			//a0 + offset 0
		int t4 = -1;
	
		switch(t3){
			case '&':
				//*(values + i)->result = values[i].left & values[i].right;
				t4 = t1 & t2;
				
				break;
			case '|':
				//values[i].result = values[i].left | values[i].right;
				t4 = t1 | t2;
				break;
			case '^':
				//values[i].result = values[i].left ^ values[i].right;
				t4 = t1 ^ t2;
				break;
			case '~':
				//values[i].result = ~values[i].left;
				t4 = ~t1;
				break;
			default:
				badOps++;
				break;
		}
		//sw t4, 8($a0)
		((a0)->result) = t4;

		//a0 += 12 (size of a Value struct)
		a0 += 1;
	}

	//DO NOT TRANSLATE THIS INTO ASSEMBLY
	for(int i = 0; i < a1; i++){
		printf("%d: %d %c %d = %d\n", i, tmp[i].left, tmp[i].operation, tmp[i].right, tmp[i].result);
	}


	//Return number of valid operations (should not be -1)
	//Ex. one operation is something random like '!', it should not count
	return (a1 - badOps); //array_size - invalid_operations
}






void matmul(Vector3 &result, const Matrix3 &mat, const Vector3 &vec){
	result = {0, 0, 0};

	//t0 is matrix row iterator
	for(int t0 = 0; t0 < 3; t0++){
		//t1 vector row iterator / matrix column iterator
		for(int t1 = 0; t1 < 3; t1++){
			result.vec[t0] += mat.mat[t0 * 3 + t1] * vec.vec[t1];
		}
	}

	for(int t0 = 0; t0 < 3; t0++){
		//t1 vector row iterator / matrix column iterator
		printf("%d\n", result.vec[t0]);
	}
	printf("\n");
}




