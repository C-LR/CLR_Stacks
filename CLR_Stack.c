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

#include <stdbool.h>
#include <string.h>

#include "CLR_Stack.h"

bool CLR_STACK_is_empty(CLR_STACK* S){
	return (S->size_current == 0);
}

bool CLR_STACK_is_full(CLR_STACK* S){
	return (S->size_current == S->size_maximum);
}

int CLR_STACK_get_used_space(CLR_STACK* S){
	return (S->size_current);
}

int CLR_STACK_get_free_space(CLR_STACK* S){
	return (S->size_maximum - S->size_current);
}

unsigned char * CLR_STACK_get_last_position(CLR_STACK* S){
	return &S->memory_chunk[S->size_maximum-1];
}

CLR_STACK_ERROR_CODES CLR_STACK_init(CLR_STACK* S, unsigned char * mem_chunk, int size, CLR_STACK_OPERATION_MODES mode){
	CLR_STACK_ERROR_CODES ret = CLR_STACK_ERROR_UNKNOWN;

	if(mem_chunk != 0){
		if(size > 0){
			if (mode > 0 && mode < 3)
			{
				S->memory_chunk = mem_chunk;
				S->pointer_read = S->memory_chunk;
				S->pointer_write = S->memory_chunk;

				S->size_current = 0;
				S->size_maximum = size;

				memset(S->memory_chunk, 0, size);

				S->mode = mode;
			}
			else
				ret = CLR_STACK_ERROR_WRONG_MODE;
		}
		else
			ret = CLR_STACK_ERROR_WRONG_SIZE;
	}
	else
		ret = CLR_STACK_ERROR_NULL_POINTER;

	ret = CLR_STACK_SUCCESS;

	return ret;
}

CLR_STACK_ERROR_CODES CLR_STACK_push(CLR_STACK* S, unsigned char * bytes, int size){
	CLR_STACK_ERROR_CODES ret = CLR_STACK_ERROR_UNKNOWN;

	if (bytes != 0)
	{
		if ((size > 0)
				&& (((size <= S->size_maximum)&& (S->mode == CLR_STACK_MODE_RING))
						|| ((size <= CLR_STACK_get_free_space(S)) && (S->mode == CLR_STACK_MODE_FIFO))))
		{

			if (((S->pointer_write - 1) + size)
					<= CLR_STACK_get_last_position(S))
			{
				memcpy(S->pointer_write, bytes, size);
				S->pointer_write = S->pointer_write + size;
				if (S->pointer_write >= CLR_STACK_get_last_position(S))
					S->pointer_write = S->memory_chunk;
			}
			else
			{
				int remaining_size_before_end = CLR_STACK_get_last_position(S) - (S->pointer_write - 1);
				memcpy(S->pointer_write, bytes, remaining_size_before_end);
				memcpy(S->memory_chunk, &bytes[remaining_size_before_end], size - remaining_size_before_end);
				S->pointer_write = S->memory_chunk + size - remaining_size_before_end;
			}

			if ((S->mode == CLR_STACK_MODE_RING) && ((S->size_current + size) > S->size_maximum))
			{
				S->pointer_read = S->pointer_write;
				S->size_current = S->size_maximum;
			}
			else
				S->size_current = S->size_current + size;

			ret = CLR_STACK_SUCCESS;

		}
		else
			ret = CLR_STACK_ERROR_PUT_NOT_ENOUGH_SPACE;
	}
	else
		ret = CLR_STACK_ERROR_NULL_POINTER;

	return ret;
}

CLR_STACK_ERROR_CODES CLR_STACK_get(CLR_STACK* S, unsigned char * bytes, int size, bool peek){
	CLR_STACK_ERROR_CODES ret = CLR_STACK_ERROR_UNKNOWN;

	if (bytes != 0)
	{
		if (size > 0 && S->size_current >= size)
		{
			if(((S->pointer_read-1) + size) <= CLR_STACK_get_last_position(S)){
				memcpy(bytes, S->pointer_read, size);
				if(peek == false){
					S->pointer_read = S->pointer_read + size;
					if(S->pointer_read >= CLR_STACK_get_last_position(S))
						S->pointer_read = S->memory_chunk;
				}

			}
			else{
				int remaining_size_before_end = CLR_STACK_get_last_position(S)-(S->pointer_read-1);
				memcpy(bytes, S->pointer_read, size);
				memcpy(&bytes[remaining_size_before_end], S->memory_chunk, size-remaining_size_before_end);
				if(peek == false)
					S->pointer_read = S->memory_chunk + size-remaining_size_before_end;
			}

			if(peek == false)
				S->size_current = S->size_current - size;
			ret = CLR_STACK_SUCCESS;
		}
		else
			ret = CLR_STACK_ERROR_POP_NOT_ENOUGH_BYTES;
	}
	else
		ret = CLR_STACK_ERROR_NULL_POINTER;

	return ret;
}

CLR_STACK_ERROR_CODES CLR_STACK_pop(CLR_STACK* S, unsigned char * bytes, int size){
	CLR_STACK_ERROR_CODES ret = CLR_STACK_ERROR_UNKNOWN;

	ret = CLR_STACK_get(S, bytes, size, false);

	return ret;
}

CLR_STACK_ERROR_CODES CLR_STACK_peek(CLR_STACK* S, unsigned char * bytes, int size){
	CLR_STACK_ERROR_CODES ret = CLR_STACK_ERROR_UNKNOWN;

	ret = CLR_STACK_get(S, bytes, size, true);

	return ret;
}



