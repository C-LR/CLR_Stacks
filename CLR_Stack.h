/////////////////////////////////////////////////
//	V1.1	October 2020
//	v1.0 made by CLR in his spare time during the March 2020 COVID-19 lockdown
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

#ifndef __CLR_STACK_H_
#define __CLR_STACK_H_

#include <stdbool.h>

/**
 * Returns given by the CLR_STACK functions
 */
typedef enum{
	CLR_STACK_SUCCESS 					= 0,	///< Execution Succesfull
	CLR_STACK_ERROR_UNKNOWN	 			= -1,	///< An Error non accounted for during design phase happened
	CLR_STACK_ERROR_NULL_POINTER 		= -2,	///< Function was passed a NULL pointer
	CLR_STACK_ERROR_WRONG_SIZE 			= -3,	///< Size passed was either 0 or too big
	CLR_STACK_ERROR_WRONG_MODE			= -4,	///< Mode passed is not defined in CLR_STACK_OPERATION_MODES
	CLR_STACK_ERROR_PUT_NOT_ENOUGH_SPACE = -5,	///< Struct is not working as a RING buffer, and you are trying to put more bytes than space remains
	CLR_STACK_ERROR_POP_NOT_ENOUGH_BYTES = -6,	///< You are trying to pop more bytes than bytes there are in memory. Wait or try a lesser number
}CLR_STACK_ERROR_CODES;

/**
 * Modes of operation of the CLR_STACK structure
 * */
typedef enum{
	CLR_STACK_MODE_FIFO = 1,	///< Will work as a FIFO Stack, rejecting new data if there is not enough free space.
	CLR_STACK_MODE_RING = 2,	///< Will work as a RING BUFFER, erasing old data in order to make space for new data.
}CLR_STACK_OPERATION_MODES;

/**
 * CLR_STACK Structure, manages a memory block passed with init, used to interact with all the CLR_STACK functions.
 * YOU SHALL NOT interact with its elements, the functions given below will manage it safely.
 * */
typedef struct st_CLR_STACK{
	unsigned char * memory_chunk;	///< Pointer to the memory block that will act as a stack
	unsigned char * pointer_read;	///< Read pointer of the stack
	unsigned char * pointer_write;	///< Write pointer of the stack
	int size_maximum;				///< Total size of the stack in bytes, configured in the init function
	int size_current;				///< Number of bytes with data on them
	int mode;						///< Operation data of the stack
}CLR_STACK;

/**
 * Returns if the passed CLR_STACK structure is empty, meaning it has 0 bytes on it
 * */
bool CLR_STACK_is_empty(CLR_STACK* S);

/**
 * Returns if the passed CLR_STACK structure is full, meaning it has 0 space on it for new data.
 * */
bool CLR_STACK_is_full(CLR_STACK* S);

/**
 * Returns the space occupied by date in the passed CLR_STACK stack
 * */
int CLR_STACK_get_used_space(CLR_STACK* S);

/**
 * Returns the space available for new data in the passed CLR_STACK stack.
 * Putting more than this number will result in either an error or older data being erased, depending of the mode
 * */
int CLR_STACK_get_free_space(CLR_STACK* S);

/**
 * Function for set-up and start managing the memory passed in mem_chunk (of size size) in the CLR_STACK structure S as a stack of mode mode.
 * This function MUST be called before any other for succesfull oepration.
 *
 * \param S Pointer to the CLR_STACK structure that will manage te memory block
 * \param mem_chunk pointer to the memory block that will be managed by the CLR_STACK structure
 * \param size the size in BYTES of the memory block to set-up as a stack
 * \param mode the operation mode for the stack
 *
 * \returns A CLR_STACK_ERROR_CODES value.
 * \li CLR_STACK_SUCCESS if init succesful.
 * \li CLR_STACK_ERROR_UNKNOWN if an error not accounted for in my design happened.
 * \li CLR_STACK_ERROR_WRONG_SIZE if size < 1.
 * \li CLR_STACK_ERROR_WRONG_MODE if the mode passed is not included in CLR_STACK_OPERATION_MODES.
 * \li CLR_STACK_ERROR_NULL_POINTER if bytes is a NULL pointer.
 *
 * */
CLR_STACK_ERROR_CODES CLR_STACK_init(CLR_STACK* S, unsigned char * mem_chunk, int size, CLR_STACK_OPERATION_MODES mode);

/**
 * Function for putting data in a PREVIOUSLY INITIALIZED CLR_STACK Structure.
 *
 * \param S Pointer to the CLR_STACK structure to put data into.
 * \param bytes pointer to the memory block to put in the stack
 * \param size the size in BYTES of the memory block to put.
 *
  * \returns A CLR_STACK_ERROR_CODES value:
 * \li CLR_STACK_SUCCESS if push succesful.
 * \li CLR_STACK_ERROR_UNKNOWN if an error not accounted for in my design happened.
 * \li CLR_STACK_ERROR_PUT_NOT_ENOUGH_SPACE if size > (size_maximum - size_current).
 * \li CLR_STACK_ERROR_NULL_POINTER if bytes is a NULL pointer.
 *
 * */
CLR_STACK_ERROR_CODES CLR_STACK_push(CLR_STACK* S, unsigned char * bytes, int size);

/**
 * Function for popping data from a PREVIOUSLY INITIALIZED CLR_STACK Structure.
 *
 * \param S Pointer to the CLR_STACK structure to pop data from.
 * \param bytes pointer to the memory block in which the popped data will be written.
 * \param size the amount of bytes to pop from the stack, it must be equal or smaller than the "bytes" memory block
 *
 * \returns A CLR_STACK_ERROR_CODES value:
 * \li CLR_STACK_SUCCESS if pop succesful.
 * \li CLR_STACK_ERROR_UNKNOWN if an error not accounted for in my design happened.
 * \li CLR_STACK_ERROR_POP_NOT_ENOUGH_BYTES if size > size_current.
 * \li CLR_STACK_ERROR_NULL_POINTER if bytes is a NULL pointer.
 *
 * */
CLR_STACK_ERROR_CODES CLR_STACK_pop(CLR_STACK* S, unsigned char * bytes, int size);

/**
 * Function for peeking (reading without taking out) data from a PREVIOUSLY INITIALIZED CLR_STACK Structure.
 *
 * \param S Pointer to the CLR_STACK structure to peek data from.
 * \param bytes pointer to the memory block in which the peeked data will be written.
 * \param size the amount of bytes to peek from the stack, it must be equal or smaller than the "bytes" memory block
 *
 * \returns A CLR_STACK_ERROR_CODES value:
 * \li CLR_STACK_SUCCESS if pop succesful.
 * \li CLR_STACK_ERROR_UNKNOWN if an error not accounted for in my design happened.
 * \li CLR_STACK_ERROR_POP_NOT_ENOUGH_BYTES if size > size_current.
 * \li CLR_STACK_ERROR_NULL_POINTER if bytes is a NULL pointer.
 *
 * */
CLR_STACK_ERROR_CODES CLR_STACK_peek(CLR_STACK* S, unsigned char * bytes, int size);

#endif //__CLR_STACK_H_
