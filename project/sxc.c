#include "sxc_interface.h"
#include "sxc_keyword.h"
extern int runcode;
extern int modetype;
extern int nextcode;
extern int othercode;
extern int while_num;
extern int if_num;

void sxc(FILE *fp) {
	char stm[B_SIZE + 1]; // 语句前B_SIZE个字符
	long int pos; // 当前文件偏移量
	int i;

	pos = ftell(fp); // 当前文件偏移量
	while (1){
		if (modetype == 1)
		{

			if (othercode == 0)
			{
				othercode = 1;
				sxc_debug(fp);
				continue;
			}
			if (IS_ARRIVE(fp) == 1 && nextcode == 1)
			{
				if (runcode == 0)
				{
					sxc_debug(fp);
					continue;
				}
				else
				{
					runcode = 0;
				}
			}
			else if (IS_ARRIVE(fp) == 0 && runcode == 1)
			{
				runcode = 0;
			}
			if (nextcode == 0)
			{
				nextcode = 2;
				//sxc_debug(fp);
				//continue;
			}
		}

		if (fgets(stm, B_SIZE, fp) == NULL)
		{ // 读取代码失败
			error_sxc(READ_CODE_ERROR, fp);
		}

		if (stm[0] == '}')
		{ // 当前语句块结束
			break;
		}
		if (strcmp(stm, "end\n") == 0)
		{
			// begin V3.0
			if (while_num || if_num) {
				fseek(fp, pos, SEEK_SET);
				error_sxc(END_ERROR, fp);
			}
			// end V3.0
			if (modetype == 1)
			{ // 重置debug
				func_q(fp);
				break;
			}
			else
			{
				break;
			}
		}
		//if (stm[0] == '\n'){
		//	continue;
		//}
		if (strcmp(stm, "\n") == 0)
		{
			continue;
		}
		//	printf("%s", stm);

		/*
		* v1.0
		if (strcmp(stm, "if\n") == 0) { // if语句
		if_stm(fp);
		} else if (strcmp(stm, "while\n") == 0) { // while语句
		while_stm(fp);
		} else if (strcmp(stm, "in\n") == 0) { // in语句
		in_stm(fp);
		} else if (strcmp(stm, "out\n") == 0) { // out语句
		out_stm(fp);
		} else { // 简单语句
		fseek(fp, pos, SEEK_SET); // 重新定位到语句头
		simple_stm(fp);
		}
		*/

		// v1.1
		for (i = 1; i < KEYWORD_NUM; i++) {
			if (strcmp(stm, key[i].key) == 0) {
				(key[i].fun)(fp);
				break;
			}
		}
		if (i == KEYWORD_NUM) {
			fseek(fp, pos, SEEK_SET); // 重新定位到语句头
			(key[0].fun)(fp);
		}

		pos = ftell(fp); // 当前语句头偏移量

		if (modetype == 1 && nextcode == 2)
		{
			nextcode = 1;
			sxc_debug(fp);
		}
	}
}
