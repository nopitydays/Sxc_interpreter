#include "sxc_interface.h"

long int find_stm(FILE *fp) {
    int stack_num;
    char str[B_SIZE + 1];
    long int block_end;

    stack_num = 1;
    while(stack_num) {
        block_end = ftell(fp);
	    fgets(str, B_SIZE, fp);
		if (block_end == 0)
		{
			error_sxc(OTHER_BRACES_ERROR,fp);
		}

	/*if (strcmp(str, "end\n") == 0) {
	    fseek(fp, block_end, SEEK_SET);
		error_sxc(MISSING_1_ERROR, fp);
	}*/

	if (strcmp(str, "{\n") == 0) {
	    stack_num += 1;
	} else if (strcmp(str, "}\n") == 0) {
	    stack_num -= 1;
	} else {
	    ;
	}
	

	
	fseek(fp, block_end, SEEK_SET);
	    next_line(fp);
    }

    return ftell(fp);
}

void exec_stm(long int s_begin, long int s_end, FILE *fp) {
    long int pos;
    
    pos = s_begin;
    fseek(fp, pos, SEEK_SET);
    while(pos < s_end) {
        sxc(fp);
		pos = ftell(fp);
    }
}

