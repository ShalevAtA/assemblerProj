assembler: macro_table.c preassembler.c first_pass.c  second_pass.c output_files.c code_image.c symbol_table.c opcode_table.c utils.c main.c
	gcc -ansi -pedantic -g -Wall macro_table.c preassembler.c first_pass.c second_pass.c output_files.c code_image.c symbol_table.c opcode_table.c utils.c main.c -o assembler

