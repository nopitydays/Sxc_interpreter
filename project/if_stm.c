#include "sxc_interface.h"
#include "sxc_substm.h"

extern int if_num;

void if_stm(FILE *fp) {
	int f, ifstack_num, elsestack_num;
	long int if_begin, if_end, else_begin, else_end, sim_pos, bla_pos, e_con_pos;
	char str[B_SIZE + 1], st[B_SIZE + 1], s[B_SIZE + 1], stri[B_SIZE + 1], strin[B_SIZE + 1], con[B_SIZE + 1];

	if (if_num >= IF_LIMITED_NUM) {
		error_sxc(IF_LIMITED_ERROR, fp);
	}
	if_num += 1;

	f = value_stm(fp);
	if_begin = ftell(fp);

	fseek(fp, -1L, SEEK_CUR);
	e_con_pos = ftell(fp);
	fgets(con, B_SIZE, fp);
	if (!strcmp(con, "\n") == 0) {
		fseek(fp, e_con_pos, SEEK_SET);
		error_sxc(IF_CONDITION_ERROR, fp);
	}
	else
	{
		fseek(fp, if_begin, SEEK_SET);
	}

	fgets(str, B_SIZE, fp);
	if (strcmp(str, "{\n") == 0) {
		if_begin = ftell(fp);
	}
	else {
		fseek(fp, if_begin, SEEK_SET);
		error_sxc(IF_MISSING_1_ERROR, fp);
	}

	/*if_end = find_stm(fp);

	else_begin = if_end;
	fseek(fp, else_begin, SEEK_SET);
	fgets(str, B_SIZE, fp);
	if (strcmp(str, "else\n") == 0) {
	fgets(str, B_SIZE, fp);
	if (strcmp(str, "{\n") == 0) {
	else_begin = ftell(fp);
	}
	else {
	fseek(fp, else_begin, SEEK_SET);
	error_sxc(IF_ELSE_MISSING_1_ERROR, fp);
	}

	else_end = find_stm(fp);


	}
	else {
	else_end = else_begin;
	fseek(fp, else_begin, SEEK_SET);
	}

	if (f) { // IF
	exec_stm(if_begin, if_end, fp);
	} else { // ELSE
	exec_stm(else_begin, else_end, fp);
	}
	fseek(fp, else_end, SEEK_SET);
	if_num -= 1;*/

	if (f){
		fseek(fp, if_begin, SEEK_SET);
		ifstack_num = 1;
		while (ifstack_num){
			sim_pos = ftell(fp);
			fgets(st, B_SIZE, fp);
			if (strcmp(st, "}\n") == 0){
				ifstack_num -= 1;
			}
			else if (strcmp(st, "{\n") == 0){
				ifstack_num += 1;
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
			if_end = ftell(fp);
			else_begin = if_end;
			fseek(fp, else_begin, SEEK_SET);
			fgets(st, B_SIZE, fp); 
			if (strcmp(st, "else\n") == 0) {
				next_line(fp);
			//	next_line(fp);
				else_end = find_stm(fp);
			}
			else {
				else_end = else_begin;
				fseek(fp, else_begin, SEEK_SET);
			}
		}
	}
	else{
		if_end = find_stm(fp);
		else_begin = if_end;
		fseek(fp, else_begin, SEEK_SET);
		fgets(s, B_SIZE, fp);
		if (strcmp(s, "else\n") == 0) {
			fgets(stri, B_SIZE, fp);
			bla_pos = ftell(fp);
			if (strcmp(stri, "{\n") == 0){
				
				elsestack_num = 1;
				while (elsestack_num)
				{
                sim_pos = ftell(fp);
				else_begin = sim_pos;
				fgets(strin, B_SIZE, fp);
				if (strcmp(strin, "}\n") == 0){
					elsestack_num -= 1;
				}
				else if (strcmp(strin, "{\n") == 0){
					elsestack_num += 1;
				}
				else if (strcmp(strin, "end\n") == 0){
					fseek(fp, sim_pos, SEEK_SET);
					error_sxc(MISSING_1_ERROR, fp);
				}
				else if (strcmp(st, "else\n") == 0){
					fseek(fp, sim_pos, SEEK_SET);
					error_sxc(MISSING_1_ERROR, fp);
				}
				else if (strcmp(strin, "if\n") == 0){
					fseek(fp, sim_pos, SEEK_SET);
					next_line(fp);
					if_stm(fp);
				}
				else if (strcmp(strin, "while\n") == 0){
					fseek(fp, sim_pos, SEEK_SET);
					next_line(fp);
					while_stm(fp);
				}
				else if (strcmp(strin, "in\n") == 0){
					fseek(fp, sim_pos, SEEK_SET);
					next_line(fp);
					in_stm(fp);
				}
				else if (strcmp(strin, "out\n") == 0){
					fseek(fp, sim_pos, SEEK_SET);
					next_line(fp);
					out_stm(fp);
				}
				else if (strcmp(strin, "for\n") == 0){
					fseek(fp, sim_pos, SEEK_SET);
					next_line(fp);
					for_stm(fp);
				}
				else
				{
					fseek(fp, sim_pos, SEEK_SET);
					simple_stm(fp);
				}
				else_end = ftell(fp);
				}
			}
			else {
				fseek(fp, bla_pos, SEEK_SET);
				error_sxc(IF_ELSE_MISSING_1_ERROR, fp);
			}
		}
		else {
			else_end = else_begin;
			fseek(fp, else_begin, SEEK_SET);
		}
	}
	fseek(fp, else_end, SEEK_SET);
	if_num -= 1;
}
