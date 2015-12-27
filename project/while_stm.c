#include "sxc_interface.h"
#include "sxc_substm.h"

extern int while_num;

void while_stm(FILE *fp) {
    int f, whilestack_num;
    long int while_begin, while_end, con_pos, sim_pos, e_con_pos;
    char str[B_SIZE + 1], st[B_SIZE + 1], con[B_SIZE + 1];

    if (while_num >= WHILE_LIMITED_NUM) {
        error_sxc(WHILE_LIMITED_ERROR, fp);
    }
    while_num += 1;

    con_pos = ftell(fp);
    f = value_stm(fp);
    while_begin = ftell(fp);

	fseek(fp, -1L, SEEK_CUR);
	e_con_pos = ftell(fp);
	fgets(con, B_SIZE, fp);
	if (!strcmp(con, "\n") == 0) {
		fseek(fp, e_con_pos, SEEK_SET);
		error_sxc(WHILE_CONDITION_ERROR, fp);
	}
	else
	{
		fseek(fp, while_begin, SEEK_SET);
	}

    fgets(str, B_SIZE, fp);
    if(strcmp(str, "{\n") == 0) {
        while_begin = ftell(fp);
    } else {
        fseek(fp, while_begin, SEEK_SET);
		error_sxc(WHILE_MISSING_1_ERROR, fp);
    }

    /*while_end = find_stm(fp);

    while(f) {
        exec_stm(while_begin, while_end, fp);
	fseek(fp, con_pos, SEEK_SET);
	f = value_stm(fp);
    }
	fseek(fp, while_end, SEEK_SET);
    while_num -= 1;*/
	
	if (f == 0){
		while_end = find_stm(fp);
	}
	while (f){
		fseek(fp, while_begin, SEEK_SET);
		whilestack_num = 1;
		while (whilestack_num){
			sim_pos = ftell(fp);
			fgets(st, B_SIZE, fp);
			if (strcmp(st, "}\n") == 0){
				whilestack_num -= 1;
			}
			else if (strcmp(st, "{\n") == 0){
				whilestack_num += 1;
			}
			else if (strcmp(st, "end\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				error_sxc(MISSING_1_ERROR, fp);
			}
			else if (strcmp(st, "else\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				error_sxc(MISSING_1_ERROR, fp);
			}
			else if (strcmp(st, "if\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				if_stm(fp);
			}
			else if (strcmp(st, "while\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				while_stm(fp);
			}
			else if (strcmp(st, "in\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				in_stm(fp);
			}
			else if (strcmp(st, "out\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				out_stm(fp);
			}
			else if (strcmp(st, "for\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				in_stm(fp);
			}
			else
			{
				fseek(fp, sim_pos, SEEK_SET);
				simple_stm(fp);
			}
			while_end = ftell(fp);
		}
		fseek(fp, con_pos, SEEK_SET);
		f = value_stm(fp);
	}

	fseek(fp, while_end, SEEK_SET);
	while_num -= 1;
}
