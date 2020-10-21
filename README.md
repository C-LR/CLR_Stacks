# CLR_Stacks
C implementation of FIFO stacks and ring buffers, specially for embedded applications. 
Depedant only of the standard C library. 
Focus on managing multiple stacks of different size with a single code implementation.

Developed by CLR on march 2020. Any criticism welcome.

Released under the GNU Lesser General Public License v3 (LGPL-3.0)

-----------------------------------------------------------------------

The aim of this "library" is to implement the basic functionality of memory stacks in a way that a single code can manage as many stacks as necessary, by using a manager structure for each memory block to be managed.


Real quick use guide:

  1- Import CLR_stack.c and CLR_stack.h to your project.
  
  2- #include "CLR_Stack.h" in the files of your code expected to handle CLR_STACK structures.
  
  3- Define a memory block (an array, for example).
  
  4- Define the CLR_STACK structure that will manage the memory block.
  
  5- Use CLR_STACK_init to init the structure, by passing it the structure, the memory block, the size of the memory block in bytes, and the operation mode     (CLR_STACK_MODE_FIFO or CLR_STACK_MODE_RING).
  
  6- now you can use CLR_STACK_put to PUSH data and CLR_STACK_pop to POP data. additional functions exist with extra functionality (check if empty/full, get free space...)
  
  
An example file is provided with a CLI application using the basic functionality. If the provided documentation and comments is not enough, contact CLR for further explanations.

-----------------------------------------------------------------------

Changelog

v1.0 Initial Release.

v1.1 Various Improvements
  Remade example program with multiple improvements, both QoL and Robustness.
  Added a function to get the number of bytes in a stack.
  Renamed CLR_STACK_put to CLR_STACK_push. In case somebody was using v1.0 and is updating to v1.1 it's function calls must be renamed too.
  Improved Documentation.
