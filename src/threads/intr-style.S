#include "threads/loader.h"

        .text
	
.globl intr_exit_style
.func intr_exit_style
intr_exit_style:
	//subl $4, %esp
	jmp intr_exit
.endfunc

	
