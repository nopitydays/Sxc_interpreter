#include "sxc_interface.h"
#include "sxc_substm.h"

void for_stm(FILE *fp){
	int f1, f2, f3, forstack_num;
	long int for_begin, for_end, con_pos, con_pos2, plus_pos, to_pos, step_pos, e_con_pos, pos, sim_pos;
	char str[B_SIZE+1], s[B_SIZE+1], st[B_SIZE+1], con[B_SIZE + 1], stri[B_SIZE + 1];
	
	simple_stm(fp);
	to_pos = ftell(fp);
	 
	fgets(str, B_SIZE, fp);
    if(strcmp(str, "to\n")== 0){
	 	con_pos = ftell(fp);
 		f1 = value_stm(fp);
		con_pos2 = ftell(fp);
		f3 = value_stm(fp);
 	} else {
	 	fseek(fp, to_pos, SEEK_SET);
 	    error_sxc(FOR_MISSING_1_ERROR, fp);
    }
    
    step_pos = ftell(fp);
    
    fgets(s, B_SIZE, fp);
    if(strcmp(s, "step\n")== 0){
    	plus_pos = ftell(fp);
    	next_line(fp);
    } else {
    	fseek(fp, step_pos, SEEK_SET);
    	error_sxc(FOR_MISSING_2_ERROR, fp);
    }
    
    for_begin = ftell(fp);
    
   /* fseek(fp,-1L,SEEK_CUR);
    e_con_pos = ftell(fp);
    fgets(con, B_SIZE, fp);
    if(!strcmp(con, "\n") == 0) {
    	fseek(fp, e_con_pos, SEEK_SET);
    	error_sxc(FOR_CONDITION_ERROR, fp);
    }
    else
    {
    	fseek(fp, for_begin, SEEK_SET);
    }*/
    
    fgets(st, B_SIZE, fp);
    if(strcmp(st, "{\n")== 0 ){
    	for_begin = ftell(fp);
    } else {
    	fseek(fp, for_begin, SEEK_SET);
    	error_sxc(FOR_MISSING_3_ERROR, fp);
    }
    
    /*for_end = find_stm(fp);
    
    while(1){
    	exec_stm(for_begin, for_end, fp);
    	//n_pos = for_begin;
    	//fseek(fp, n_pos, SEEK_SET);
    	//simple_stm(fp);
    	fseek(fp, plus_pos, SEEK_SET);
    	simple_stm(fp);
    	fseek(fp, con_pos, SEEK_SET);
    	f2 = value_stm(fp);
		if (f1 == 1 && f2 == 0){
			break;
		}
		if (f1 == 0 && f2 == 1){
			fseek(fp, con_pos2, SEEK_SET);
			f3 = value_stm(fp);
			if (f3 == 1){
				fseek(fp, for_begin, SEEK_SET);
				exec_stm(for_begin, for_end, fp);
				break;
			}
			else break;
		}
    }
	fseek(fp, for_end, SEEK_SET);*/

	while (1){
		fseek(fp, for_begin, SEEK_SET);
		forstack_num = 1;
		while (forstack_num){
			sim_pos = ftell(fp);
			fgets(stri, B_SIZE, fp);
			if (strcmp(stri, "}\n") == 0){
				forstack_num -= 1;
			}
			else if (strcmp(stri, "{\n") == 0){
				forstack_num += 1;
			}
			else if (strcmp(stri, "end\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				error_sxc(MISSING_1_ERROR, fp);
			}
			else if (strcmp(st, "else\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				error_sxc(MISSING_1_ERROR, fp);
			}
			else if (strcmp(stri, "if\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				if_stm(fp);
			}
			else if (strcmp(stri, "while\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				while_stm(fp);
			}
			else if (strcmp(stri, "in\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				in_stm(fp);
			}
			else if (strcmp(stri, "out\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				out_stm(fp);
			}
			else if (strcmp(stri, "for\n") == 0){
				fseek(fp, sim_pos, SEEK_SET);
				next_line(fp);
				for_stm(fp);
			}
			else
			{
				fseek(fp, sim_pos, SEEK_SET);
				simple_stm(fp);
			}
			for_end = ftell(fp);
		}
		fseek(fp, plus_pos, SEEK_SET);
		simple_stm(fp);
		fseek(fp, con_pos, SEEK_SET);
		f2 = value_stm(fp);
		if (f1 == 1 && f2 == 0){
			break;
		}
		if (f1 == 0 && f2 == 1){
			fseek(fp, con_pos2, SEEK_SET);
			f3 = value_stm(fp);
			if (f3 == 1){
				fseek(fp, for_begin, SEEK_SET);
				/*exec_stm(for_begin, for_end, fp);*/
				pos = for_begin;
				fseek(fp, pos, SEEK_SET);
				while (pos < for_end)
				{
					sxc(fp);
					pos = ftell(fp);
				}
				break;
			}
			else break;
		}
	}
	fseek(fp, for_end, SEEK_SET);
} 