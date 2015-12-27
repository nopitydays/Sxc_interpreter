#include "sxc_interface.h"
#include <string.h>
#include <stdlib.h>


int de_flag = 0;
extern BP *PointHead;
extern List var_l;
extern int runcode;
extern int nextcode;
extern int othercode;
void func_d(int line_num, int single);
void func_b(int line_num, int single);

void show_help()
{
	//FILE *fp;
	//char a[200];
	de_flag = 1;
	printf("常用命令：\nh --帮助\nb --下断点，参数为行号\nd --删除断点，参数为行号\ns --显示所有断点\nn --单步解释\nr --解释到下一个断点\np --打印变量的值，没有提供变量名则打印全部变量\nl --打印即将解释的代码,没有提供行号则打印下一行\nq --重新调试\nk --退出程序\n");
	othercode = 0;
	return;
}
int IS_ARRIVE(FILE *fp)
{
	int line;
	BP *start;
	start = PointHead->next;
	line = current_line(fp)+1;
	while (start != NULL)
	{
		if (start->line_num == line)
		{
			return 1;
		}
		start = start->next;
	}
	return 0;
}

void func_b(int line_num,int single)  //添加断点
{
	BP *new_node;
	BP *start;
	int flag = 0;
	if (line_num > lin_num || line_num < 0)
	{
		printf("Cannot find this line!\n");
		othercode = 0;
		return;
	}
	new_node = malloc(sizeof(BP));
	if (new_node == NULL)
		printf("malloc fault!");
	start = PointHead;
	while (1)
	{
		if ((start->next == NULL&&start->line_num<line_num) || (start->line_num<line_num&&start->next->line_num>line_num))
		{
			new_node->next = start->next;
			new_node->line_num = line_num;
			new_node->flag = 0;
			new_node->single = single;
			start->next = new_node;
			flag = 1;
			break;
		}
		start = start->next;
		if (start == NULL)
			break;
	}
	if (flag == 0 && single==0)
		printf("This BreakPoint is already added!\n");
	othercode = 0;
	return;
}
void func_d(int line_num,int single)  //删除指定断点
{
	BP *start,*temp;
	int res = 0;
	start = PointHead;
	while (start->next->line_num != line_num)
	{
		start = start->next;
		if (start->next == NULL)
		{
			printf("Cannot find this breakpoint!\n");
			othercode = 0;
			return;
		}
	}
	if (start->next->single == single)
	{
		temp = start->next;
		start->next = start->next->next;
		free(temp);
	}
	othercode = 0;
	return;
}

void func_q(FILE *fp)  //重新调试
{
	BP *start;
	char str[B_SIZE+1];
	memset(str, 0, sizeof(str));
	DeleteList(var_l);
	fseek(fp, 0, SEEK_SET);
	start = PointHead->next;
	while (start != NULL)
	{
		start->flag = 0;
		start = start->next;
	}
	printf("程序运行完毕，重新开始调试或输入k结束调试.\n");
	runcode = 0;
	nextcode = 1;
	othercode = 0;
	fgets(str, B_SIZE, fp);
	return;
}
void func_p(char *var_name)   //显示变量
{
	List next;
	next = var_l->Next;
	if (strcmp(var_name, "-a")==0)
	{
		if (next == NULL)
		{
			printf("No variable\n");
		}
		while (next != NULL)
		{
			printf("%s=%d\n", next->name, next->value);
			next = next->Next;
		}
	}
	else
	{
		while (next != NULL)
		{
			if (strcmp(next->name, var_name) == 0)
			{
				printf("%s=%d\n", next->name, next->value);
				othercode = 0;
				return;
			}
			next = next->Next;
		}
		printf("Cannot find this variable!\n");
	}
	othercode = 0;
}

void func_l(FILE *fp,int para)
{
	int line = current_line(fp);
	int pos = ftell(fp);
	if (para==-1)
		printf("line:%d %s", line+1, old_pro[line]);
	else
	{
		if (para >= 0 && para <= lin_num)
		{
			printf("line:%d %s", para, old_pro[para-1]);
		}
	}
	othercode = 0;
	return;
}
void func_s()     //显示所有断点
{
	BP *start;
	start = PointHead->next;
	if (start == NULL)
		printf("no breakpoint!\n");
	while (start!=NULL)
	{
		printf("line:%d\n", start->line_num);
		printf("%s", old_pro[start->line_num-1]);
		start = start->next;
	}
	othercode = 0;
	return;
}

void func_n(FILE *fp)
{
	nextcode = 0;
	//int line;
	//line = current_line(fp);
	//func_b(line+1, 1);
	sxc(fp);
	//func_d(line+1, 1);
	return;
}
void func_k()  //退出断点
{
	BP *start;
	start = PointHead->next;
	while (start != NULL)
	{
		start->flag = 0;
		start = start->next;
	}
	exit(1);
}
void sxc_debug(FILE *fp)  //调试主监听程序
{
	char input[100],var[100];
	char cmd;
	int para = 0;
	int i = 0;
	
	de_flag = 0;
	para = 0;
	while (1)
	{
		gets(input);
		if (strcmp(input, "\0") != 0)
			break;
	}
	cmd = input[0];
	switch (cmd)
	{
	case 'h':	runcode = 0; show_help(); break;
	case 'b':	runcode = 0; if (input[1] != ' ')
				show_help();
		     else
			 {
				 i = 2;
				 while (input[i] != '\n' && input[i++] != '\0')
				 {
					 if (input[i - 1] >= 48 && input[i - 1] <= 57)
					 {
						 para = para*10+(input[i - 1] - '0');
					 }
					 else
					 {
						 show_help();
						 break;
					 }
				 }
				 if (de_flag == 0)
				 {
					 func_b(para,0);
				 }
			 }
			 break;
	case 'n':	runcode = 1; if (input[1] != '\0') show_help(); else func_n(fp); break;
	case 'r':	runcode = 1; if (input[1] != '\0') show_help(); else sxc(fp); break;			
	case 'p':	runcode = 0; if (input[1] != ' ' && input[1] != '\n' && input[1] != '\0')
			     show_help();
	    	 else if (input[1] == '\0')
				 func_p("-a");
			 else
				 func_p(input + 2);
			 break;
	case 'l':	runcode = 0; if (input[1] != ' ' && input[1] != '\n' && input[1] != '\0')
		show_help();
				else if (input[1] == '\0')
					func_l(fp, -1);
				else
				{
					i = 2;
					while (input[i] != '\n' && input[i++] != '\0')
					{
						if (input[i - 1] >= 48 && input[i - 1] <= 57)
						{
							para = para * 10 + (input[i - 1] - '0');
						}
						else
						{
							show_help();
							break;
						}
					}
					if (de_flag == 0)
					{
						func_l(fp, para);
					}
				}
				break;
	case 'd':	runcode = 0; if (input[1] != ' ')
				 show_help();
			 else
			 {
				 i = 2;
				 while (input[i] != '\n' && input[i++] != '\0')
				 {
					 if (input[i - 1] >= 48 && input[i - 1] <= 57)
					 {
						 para = para * 10 + (input[i - 1] - '0');
					 }
					 else
					 {
						 show_help();
						 break;
					 }
				 }
				 if (de_flag == 0)
				 {
					 func_d(para,0);
				 }
			 }
			 break;
	case 's':	runcode = 0; if (input[1] != '\0') show_help(); else func_s(); break;
	case 'k':if (input[1] != '\0') show_help(); else func_k(); break;
	case 'q':if (input[1] != '\0') show_help(); else func_q(fp); break;
	default:show_help(); break;
	}
}
