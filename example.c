/////////////////////////////////////////////////
//
//	This file is part of CLR_Stack.
//
//	CLR_Stack is free software: you can redistribute it and/or modify
//	it under the terms of the GNU Lesser General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	CLR_Stack is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License
//	along with CLR_Stack.  If not, see <https://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "CLR_Stack.h" //CLR stack header file must be included


#define TEST_MODE 2	// 1 = test a FIFO STACK, 2 = test a RING BUFFER
#define MEM_SIZE 10	// Size in buffer of the memory block to be managed as a stack

void print_menu_STACK(void){
	printf("MENU STACK:\n"
			"1 - PUSH\n"
			"2 - POP\n"
			"q - QUIT\n\n"
			"Please enter command: "
			);
}

int init_wrapper(int test_mode, CLR_STACK* S,uint8_t * bytes, uint32_t size)
{
	int ret = -1;

	switch (test_mode)
	{
	case 1:
		/* init a FIFO stack in memory block pointed by "bytes" of size "size"
		 * managed by the CLR_STACK pointed by S
		 * */
		ret = CLR_STACK_init(S, bytes, size, CLR_STACK_MODE_FIFO);
		break;
	case 2:
		/* init a RING BUFFER stack in memory block pointed by "bytes" of size "size"
		 * managed by the CLR_STACK pointed by S
		 * */
		ret = CLR_STACK_init(S, bytes, size, CLR_STACK_MODE_RING);
		break;
	case 3:
		break;
	default:
		break;
	}

	return ret;
}

int put_wrapper(int test_mode, CLR_STACK* S,uint8_t * bytes, uint32_t size){

	int ret = -1;

	switch (test_mode)
	{
	case 1:
		// PUSH "size" bytes pointed by "bytes" into the stack managed by the CLR_STACK S
		ret = CLR_STACK_put(S, bytes, size);
		break;
	case 2:
		// PUSH "size" bytes pointed by "bytes" into the stack managed by the CLR_STACK S
		ret = CLR_STACK_put(S, bytes, size);
		break;
	case 3:
		break;
	default:
		break;
	}

	return ret;
}

int pop_wrapper(int test_mode, CLR_STACK* S,uint8_t * bytes, uint32_t size){

	int ret = -1;

	switch (test_mode)
	{
	case 1:
		// POP "size" bytes from the stack managed by the CLR_STACK S into the memory block pointed by "bytes"
		ret = CLR_STACK_pop(S, bytes, size);
		break;
	case 2:
		// POP "size" bytes from the stack managed by the CLR_STACK S into the memory block pointed by "bytes"
		ret = CLR_STACK_pop(S, bytes, size);
		break;
	case 3:
		break;
	default:
		break;
	}

	return ret;
}

int main(void){

	time_t t;

	//return variable for functions
	int check = 0;

	//char for returning user menu option
	char c = 'c';

	//16bit integer to generate random numbers for input / reading the output of the stack
	uint16_t num = 0;

	//memory block that will be managed by the CLR_STACK. The block can have any size. Multiple blocks can be managed by a single CLR_STACK each.
	uint8_t memory[MEM_SIZE] = {0};

	//CLR_STACK that will manage the memory block "memory".
	CLR_STACK CLR_STACK_STR;

	srand((unsigned) time(&t));

	/*
	 * Initialise the memory block "memory" of size "MEM_SIZE" in bytes,
	 * to be managed by the CLR_STACK "CLR_STACK_STR",
	 * as a TEST_MODE stack (1= FIFO, 2 = RING BUFFER)
	 * */
	init_wrapper(TEST_MODE, &CLR_STACK_STR, memory, MEM_SIZE);


	do{
		printf("----------------------\n");

		//print the menu for the user
		print_menu_STACK();

		fflush(stdin);
		//get the user command
		c = getchar();
		printf("\n");

		//manage user commands.
		switch (c)
		{
		case '1':	//PUSH data into the stack

			//Generate a 16 bit number using all the range
			num = rand() % 65535;

			/*
			 * PUSH sizeof(num) bytes at the adress pointed by "num",
			 * into the memory block managed by the stack CLR_STACK_STR
			 * */
			check = put_wrapper(TEST_MODE, &CLR_STACK_STR,(uint8_t *)&num, sizeof(num));

			//print results returned
			if(check == 0){
				printf("succesfully put %04X \n", num);
			}
			else{
				printf("Error %04X while trying to put\n", check);
			}
			break;
		case '2':	//POP data from the stack

			num = 0;	//Set the return variable to 0

			/*
			 * POP sizeof(num) bytes from the adress pointed by "num",
			 * from the memory block managed by the stack CLR_STACK_STR
			 * */
			check = pop_wrapper(TEST_MODE, &CLR_STACK_STR,(uint8_t *)&num, sizeof(num));

			//print results returned
			if (check == 0)
			{
				printf("succesfully pop %04X \n", num);
			}
			else
			{
				printf("Error %d while trying to pop\n", check);
			}
			break;

		case 'q':
			break;
		}

		printf("----------------------\n");

		//Pause to wait for new user input
		system("pause");

	}while(c != 'q');



	return 0;
}
