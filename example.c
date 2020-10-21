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
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "CLR_Stack.h" //CLR stack header file must be included

#define MEM_SIZE 32	// Size in buffer of the memory block to be managed as a stack
#define IO_DATA_MAX 16	// How many bytes can be input or output at one go

void DEBUG_print_stack_status(CLR_STACK *S)
{

	int i = 0;

	//Get Adress of the memory block
	unsigned char *p = S->memory_chunk;

	printf("STACK DEBUG DATA:\n");
	printf("----------------------\n");
	printf("The Stack manages a memory block at %p\n\n", p);
	printf("The Stack memory block has a size of %d bytes\n\n", S->size_maximum);

	if (CLR_STACK_is_empty(S))
	{
		printf("The Stack is empty\n\n");
	}
	else if (CLR_STACK_is_full(S))
	{
		printf("The Stack is full\n\n");
	}
	else{
		printf("The Stack has %d bytes used\n\n", S->size_current);
		printf("The Stack has %d bytes free\n\n", CLR_STACK_get_free_space(S));
	}

	if (S->mode == CLR_STACK_MODE_FIFO)
	{
		printf("The Stack is configured as a FIFO queue\n\n");
	}
	if (S->mode == CLR_STACK_MODE_RING)
	{
		printf("The Stack is configured as a Ring Buffer\n\n");
	}

	printf("Stack Memory content is:\n");
	printf("----------------------\n");
	for (i = 0; i < S->size_maximum; i++)
	{
		printf("\t");
		if (S->pointer_write == p)
		{
			printf("wp>>");
		}
		else
		{
			printf("    ");
		}
		printf("\t");
		printf("%p:0x%02X", p, *p);
		printf("\t");
		if (S->pointer_read == p)
		{
			printf("rp>>");
		}
		printf("\n");
		p++;
	}
	printf("----------------------\n");

}

void print_menu_INIT(void)
{
	printf("This is the CLR_STACK example program, please choose the type of stack to test:\n"
			"1 - FIFO queue stack\n"
			"2 - Ring Buffer stack\n"
			"q - QUIT\n\n"
			"Please enter command: ");
}

void print_menu_STACK(void)
{
	printf("MENU STACK:\n"
			"1 - PUSH\n"
			"2 - POP\n"
			"3 - DEBUG STACK\n"
			"q - QUIT\n\n"
			"Please enter command: ");
}

int char_to_int_hex(char c)
{

	int ret = 0;

	switch (c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		ret = c - 48;	//ASCII chars [0,9]
		break;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		ret = c - 55;	//ASCII chars[A,F]
		break;
	default:
		break;
	}

	return ret;
}

uint8_t Get_input_data(uint8_t *bytes_to_push)
{

	int i = 0;

	char str[64] =
	{ 0 };
	uint8_t chars_read = 0;
	uint8_t chars_remaining = 0;

	uint8_t bytes_processed = 0;

	bool valid_input_data = false;
	bool valid_char = false;

	if (bytes_to_push != NULL)
	{

		printf("Please input the data you want to PUSH into the stack and press ENTER...\n");
		printf("Use Hex format (numbers [0,9] and letters [A,F])\n");
		printf("Input a maximum of %d bytes in one go, first bytes writen will be first bytes pushed\n",
		IO_DATA_MAX);
		printf("----------------------\n");

		do
		{
			printf("Data: ");
			fflush(stdin);
			//Get the string with input data
			gets(str);
			chars_read = strlen(str);

			//Only accept as many chars as twitce the ammount of bytes to input in one go
			if (chars_read <= (IO_DATA_MAX << 1))
			{

				//Check the data for any bad character
				for (i = 0; i < chars_read; i++)
				{
					valid_char = false;
					str[i] = toupper(str[i]);
					switch (str[i])
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
						valid_char = true;
						break;
					default:
						break;
					}
					//If a bad character is found
					if (valid_char == false)
					{
						printf("wrong character %c detected, please try again\n", str[i]);
						break;
					}
					//If we are at the last character and no bad character has been found
					else if (i == (chars_read - 1))
					{
						valid_input_data = true;
					}
				}
			}
			else
			{
				printf("Too many input data (%d chars), please input a maximum of %d chars\n", chars_read, (IO_DATA_MAX << 1));
			}

		} while (valid_input_data == false);	//Continue to ask as long as the data contains invalid chars

		//Store the number of chars remaining to process
		chars_remaining = chars_read;
		//chars_read will now store the number of chars processed
		chars_read = 0;
		while (chars_remaining > 0)
		{
			if (chars_remaining >= 2)
			{
				//First char to value
				bytes_to_push[bytes_processed] = (char_to_int_hex(str[chars_read]) << 4);
				//Second char to value
				bytes_to_push[bytes_processed] = bytes_to_push[bytes_processed] + char_to_int_hex(str[chars_read + 1]);

				//printf("Chars %c%c Value %02X\n",str[chars_read],str[chars_read+1],input_bytes[bytes_processed]);

				//update counters
				bytes_processed++;
				chars_read = chars_read + 2;
				chars_remaining = chars_remaining - 2;
			}
			else
			{
				//First char to value
				bytes_to_push[bytes_processed] = (char_to_int_hex(str[chars_read]));

				//printf("Char %c Value %02X\n",'0',str[chars_read],input_bytes[bytes_processed]);

				//update counters
				bytes_processed++;
				chars_read++;
				chars_remaining--;
			}
		}

		printf("The following %d bytes will be pushed: ", bytes_processed);
		for (i = 0; i < bytes_processed; i++)
		{
			printf("%02X|", bytes_to_push[i]);
		}
		printf("\n");
	}

	//return the number of bytes written into the array
	return bytes_processed;
}

int Get_output_data_count(void)
{

	char str[64] = { 0 };
	int ret = 0;

	char *ptr;

	printf("Please input the ammount of bytes you want to POP from the stack and press ENTER...\n");
	printf("In this example, a maximum of %d bytes can be popped in one go\n",
	IO_DATA_MAX);
	printf("----------------------\n");

	printf("Data: ");
	fflush(stdin);
	//Get the string with input data
	gets(str);

	ret = strtol(str, &ptr, 10);

	if (ret != 0)
	{
		printf("%d bytes will be popped\n", ret);
	}

	return ret;
}

int main(void)
{

	time_t t;

	int i = 0;

	//Stack mode chosen by the user for the test
	CLR_STACK_OPERATION_MODES Chosen_mode;

	//Error code for interpreting the returns of the stack functions
	CLR_STACK_ERROR_CODES Error_Code;

	//char for returning user menu option
	char c = 'c';

	//memory block that will be managed by the CLR_STACK. The block can have any size. Multiple blocks can be managed by a single CLR_STACK each.
	uint8_t memory[MEM_SIZE] = { 0 };

	//Array for pushing data
	unsigned char input_data[IO_DATA_MAX] = { 0 };
	//Number of bytes to push
	uint8_t input_data_count = 0;

	//Array for popping data
	unsigned char output_data[IO_DATA_MAX] = { 0 };
	int output_data_count = 0;

	//CLR_STACK that will manage the memory block "memory".
	CLR_STACK CLR_STACK_STR;

	srand((unsigned) time(&t));

	print_menu_INIT();

	fflush(stdin);
	//get the user command
	c = getchar();
	printf("\n");

	//Initialize the stack with the correct type
	switch (c)
	{
	case '1':
		Chosen_mode = CLR_STACK_MODE_FIFO;
		break;
	case '2':
		Chosen_mode = CLR_STACK_MODE_RING;
		break;
	default:
		printf("Please enter a valid command\n"); // @suppress("No break at end of case")
	case 'q':
		printf("----------------------\n");
		printf("BYE\n");
		system("pause");
		return 0;
		break;
	}

	/*
	 * Initialise the memory block "memory" of size "MEM_SIZE" in bytes,
	 * to be managed by the CLR_STACK "CLR_STACK_STR",
	 * as a Chosen_mode stack (1= FIFO, 2 = RING BUFFER)
	 * */
	Error_Code = CLR_STACK_init(&CLR_STACK_STR, memory, MEM_SIZE, Chosen_mode);
	//Print Init Success
	if (Error_Code == 0)
	{
		printf("Memory block at adress %p Initialized succesfully as a stack with mode %d and size %d bytes\n", &memory, Chosen_mode, MEM_SIZE);
	}
	//Print Init Fail
	else
	{
		printf("Error %d while initializing the memory block, check the declaration of CLR_STACK_ERROR_CODES for more info!\n", Error_Code);
		printf("----------------------\n");
		printf("BYE\n");
		system("pause");
		return Error_Code;
	}

	do
	{
		printf("----------------------\n");

		//print the menu for the user
		print_menu_STACK();

		fflush(stdin);
		//get the user command
		c = getchar();
		printf("\n");

		switch (c)
		{
		case '1':
			if ((Chosen_mode == CLR_STACK_MODE_FIFO) && (CLR_STACK_is_full(&CLR_STACK_STR)))
			{
				printf("Stack is full! it wont accept any data!\n");
			}
			else
			{
				//Get input data from the user
				input_data_count = Get_input_data(input_data);

				if (input_data_count > 0)
				{
					//PUSH data into the stack
					Error_Code = CLR_STACK_push(&CLR_STACK_STR, input_data, input_data_count);

					//Handle result of the popping
					switch (Error_Code)
					{
					case CLR_STACK_SUCCESS:
						//Handle Success
						printf("%d bytes pushed succesfully\n", input_data_count);
						break;
					case CLR_STACK_ERROR_PUT_NOT_ENOUGH_SPACE:
						printf("ERROR: Not enough space to push, only %d bytes available!\n", CLR_STACK_get_free_space(&CLR_STACK_STR));
						break;
					case CLR_STACK_ERROR_NULL_POINTER:
						printf("ERROR: Passed a NULL pointer!\n");
						break;
					case CLR_STACK_ERROR_UNKNOWN:
						printf("ERROR: Unknown error! this should have never happened, please report!\n");
						break;
					default:
						printf("ERROR: Unknown return code! this should have never happened, please report!\n");
						break;
					}

				}
			}

			break;
		case '2':
			if (!CLR_STACK_is_empty(&CLR_STACK_STR))
			{
				//Get the number of bytes of data that the user wants to pop
				output_data_count = Get_output_data_count();

				if (output_data_count > 0)
				{

					//POP data from the stack
					Error_Code = CLR_STACK_pop(&CLR_STACK_STR, output_data, output_data_count);
					//Handle result of the popping
					switch (Error_Code)
					{
					case CLR_STACK_SUCCESS:
						//Handle Success
						printf("Succesfully popped %d bytes\n", output_data_count);
						printf("Data: ");
						for (i = 0; i < output_data_count; i++)
						{
							printf("%02X|", output_data[i]);
						}
						printf("\n");
						break;
					case CLR_STACK_ERROR_POP_NOT_ENOUGH_BYTES:
						printf("ERROR: Not enough Bytes to POP, only %d bytes in Stack\n", CLR_STACK_get_used_space(&CLR_STACK_STR));
						break;
					case CLR_STACK_ERROR_NULL_POINTER:
						printf("ERROR: Passed a NULL pointer!\n");
						break;
					case CLR_STACK_ERROR_UNKNOWN:
						printf("ERROR: Unknown error! this should have never happened, please report!\n");
						break;
					default:
						printf("ERROR: Unknown return code! this should have never happened, please report!\n");
						break;
					}
				}
			}
			else
			{
				printf("Stack is empty! no point in popping data\n");
			}

			break;
		case '3':
			DEBUG_print_stack_status(&CLR_STACK_STR);
			break;
		case 'q':
			break;
		default:
			printf("Please input a valid command...\n");
			break;
		}
		system("pause");

	} while (c != 'q');

	return 0;
}

