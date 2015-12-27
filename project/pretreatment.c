//date: 2015/9/21 0:22

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sxc_interface.h"

/*
struct offset
{
	long off; //新文件的行首文件偏移量
	int lin; //旧文件的所在行数，
};
*/

//struct pre_check;
//typedef struct pre_check check;

struct check
{
	long off;     //当前符号在中间文件的偏移	
	char chr;     //当前符号
};

struct forstruct;
typedef struct forstruct forstru;
struct forstruct
{
	char var[100];
	char init[100];
	char fina[100];
	char step[100];
	int ste;
}fornode;

extern offset off_set[1000];  //新文件所在行数作为下标
struct check douquote_stack[100];
struct check sinquote_stack[100];
struct check big_stack[100];
struct check small_stack[100];
int dou_quote = 0; //双引号数
int sin_quote = 0; //单引号数
int douquote = 0, sinquote = 0, big = 0, small = 0; //栈顶指针
int col_num = 0, flag = 0, be, en;
extern int line_num;
extern int new_line_num;
char word_list[7][10] = { "if", "else", "in", "out","{", "}", "while" };//关键字表
//char old_pro[201][1000], word[1000]; //old_pro保存原来程序的每行，word保存当前关键字
char word[1000];
extern char old_pro[201][1000];

FILE *fin, *fout;

char *pre_sxc(char *filename)
{
	char ch, prech, temp[4], forch, step[100];
	char *new_filename;
	int i;
	int search, len, is_for[4];
	new_filename="new_pro.sxc";
	//fin = fopen(argv[1], "r");
	fin = fopen(filename, "r");
	fout = fopen(new_filename, "w");
	memset(temp, 0, sizeof(temp));
	temp[0] = fgetc(fin);
	if (temp[0] == -17)
	{
		fgetc(fin);
		fgetc(fin);
	}
	else
		fseek(fin, 0, SEEK_SET);
	//rand 1 ************************************************************ //格式标准化
#pragma region rand1
	memset(old_pro, 0, sizeof(old_pro));
	memset(word, 0, sizeof(word));
	memset(off_set, 0, sizeof(off_set));
	off_set[0].lin = 0;
	off_set[0].off = 0;
	prech = 'a';
	while ((ch = fgetc(fin)) || 1)
	{
		if (ch == EOF)
		{
			en++;
			fputs(word, fout);
			fputc('\n', fout);
			new_line_num++;
			off_set[new_line_num].lin = lin_num;
			off_set[new_line_num].off = ftell(fout);
			memset(word, 0, sizeof(word));
			flag = 0;
			break;
		}
		if (col_num<=900)
			old_pro[lin_num][col_num] = ch;
		if (ch == '\n')                             //把源程序按按单个字符读取并记录到二维数组
		{
			lin_num++;
			col_num = 0;
		}
		else
		{
			col_num++;
		}
		/****************************************************************************************************/
		if (ch == '/'&&prech == '/')
		{
			if (strlen(word) > 0 && strcmp(word, "/") != 0)
			{
				if (word[strlen(word)-1]=='/')
				{
					word[strlen(word) - 1] = 0;
				}
				fputs(word, fout);
				fputc('\n', fout);
				new_line_num++;
				off_set[new_line_num].lin = lin_num;
				off_set[new_line_num].off = ftell(fout);
			}
			memset(word, 0, sizeof(word));
			flag = 0;
			while ((ch = fgetc(fin)) != '\n' && (ch) != EOF)
			{
				if (col_num <= 900)
					old_pro[lin_num][col_num] = ch;
				col_num++;
			}
			if (col_num <= 900)
				old_pro[lin_num][col_num] = '\n';
			lin_num++;
			col_num = 0;
		}
		if ((ch != ' '&&ch != '\n'&&ch != '	')||dou_quote%2!=0||sin_quote%2!=0)       //各行的缩进、换行标准化
		{
			if (ch == '"'&&prech!='\\')
			{
				dou_quote++;
			}
			else if (ch == '\''&&prech!='\\')
			{
				sin_quote++;
			}
			if ((ch == '{' || ch == '}') && dou_quote % 2 == 0&&sin_quote % 2 == 0)
			{
				fputs(word, fout);
				if (flag != 0)
				{
					fputc('\n', fout);
					new_line_num++;
					off_set[new_line_num].lin = lin_num;
					off_set[new_line_num].off = ftell(fout);
				}
				fputc(ch, fout);
				fputc('\n', fout);
				new_line_num++;
				off_set[new_line_num].lin = lin_num;
				off_set[new_line_num].off = ftell(fout);
				memset(word, 0, sizeof(word));
				flag = 0;
			}
			else if ((ch == ';') && dou_quote % 2 == 0 && sin_quote % 2 == 0)
			{
				fputs(word, fout);
				fputc(';', fout);
				fputc('\n', fout);
				new_line_num++;
				off_set[new_line_num].lin = lin_num;
				off_set[new_line_num].off = ftell(fout);
				memset(word, 0, sizeof(word));
				flag = 0;
			}
			else
			{
				word[flag++] = ch;
				if (flag == 2 && strcmp(word, "//") == 0)
				{
					memset(word, 0, sizeof(word));
					flag = 0;
					while ((ch = fgetc(fin)) != '\n' && (ch) != EOF)
					{
						if (col_num <= 900)
							old_pro[lin_num][col_num] = ch;
						col_num++;
					}
					if (col_num <= 900)
					old_pro[lin_num][col_num] = '\n';
					lin_num++;
					col_num = 0;
				}
			}
		}
		else if (flag!=0)
		{
			if (strcmp(word, "begin") == 0)
			{
				be++;
				fputs(word, fout);
				fputc('\n', fout);
				new_line_num++;
				off_set[new_line_num].lin = lin_num;
				off_set[new_line_num].off = ftell(fout);
				memset(word, 0, sizeof(word));
				flag = 0;
			}
			else if (strcmp(word, "end") == 0)
			{
				en++;
				fputs(word, fout);
				fputc('\n', fout);
				new_line_num++;
				off_set[new_line_num].lin = lin_num;
				off_set[new_line_num].off = ftell(fout);
				memset(word, 0, sizeof(word));
				flag = 0;
			}
			/**************************************for********************************************/
#pragma region forloop
			else if (strcmp(word, "for") == 0)
			{
				fputs(word, fout);
				fputc('\n', fout);
				new_line_num++;
				off_set[new_line_num].lin = lin_num;
				off_set[new_line_num].off = ftell(fout);
				memset(word, 0, sizeof(word));
				flag = 0;
				memset(step, 0, sizeof(step));
				memset(fornode.var, 0, sizeof(fornode.var));
				memset(fornode.init, 0, sizeof(fornode.init));
				memset(fornode.fina, 0, sizeof(fornode.fina));
				memset(fornode.step, 0, sizeof(fornode.step));
				fornode.ste = 1;
				while (1)                     // for "var"=
				{
					forch = getc(fin);
					if (forch == '{')
					{
						fseek(fin, -1, SEEK_CUR);
						break;
					}
					if (col_num <= 900)
					old_pro[lin_num][col_num] = forch;
					if (forch == '\n')                             //把源程序按按单个字符读取并记录到二维数组
					{
						lin_num++;
						col_num = 0;
					}
					else
					{
						col_num++;
					}
					if (forch == '\n')
					{
						fputs(word, fout);
						fputc('\n', fout);
						new_line_num++;
						off_set[new_line_num].lin = lin_num;
						off_set[new_line_num].off = ftell(fout);
						memset(word, 0, sizeof(word));
						flag = 0;
						break;
					}
					else if (forch == '=')
					{
						break;
					}
					else if (forch == ' ' || forch=='	')
					{
						continue;
					}
					else 
					{
						fornode.var[flag++] = forch;
					}
				}
				flag = 0;
				fseek(fin, -1, SEEK_CUR);
				memset(step, 0, sizeof(step));
				while (1)                              //for var"=init"
				{
					forch = getc(fin);
					if (forch == '{')
					{
						fseek(fin, -1, SEEK_CUR);
						break;
					}
					if (col_num <= 900)
					old_pro[lin_num][col_num] = forch;
					if (forch == '\n')                             //把源程序按按单个字符读取并记录到二维数组
					{
						lin_num++;
						col_num = 0;
					}
					else
					{
						col_num++;
					}
					if (forch == '\n')
					{
						if (flag != 0)
						{
							if (strlen(word) > 0)
								strcat(step, word);
							memset(word, 0, sizeof(word));
							flag = 0;
						}
						fputs(step, fout);
						fputc('\n', fout);
						new_line_num++;
						off_set[new_line_num].lin = lin_num;
						off_set[new_line_num].off = ftell(fout);
						memset(word, 0, sizeof(word));
						flag = 0;
						break;
					}
					if (forch != ' '&& forch != '	')
					{
						word[flag++] = forch;
					}
					else if (flag != 0)
					{
						if (strcmp(word, "to") == 0)
						{
							strcpy(fornode.init, step);
							memset(step, 0, sizeof(step));
							strcat(step, fornode.var); strcat(step, fornode.init); strcat(step, ";");
							fputs(step, fout);
							fputc('\n', fout);
							new_line_num++;
							off_set[new_line_num].lin = lin_num;
							off_set[new_line_num].off = ftell(fout);
							fputs(word, fout);
							fputc('\n', fout);
							new_line_num++;
							off_set[new_line_num].lin = lin_num;
							off_set[new_line_num].off = ftell(fout);
							memset(word, 0, sizeof(word));
							flag = 0;
							break;
						}
						else
						{
							strcat(step, word);
							memset(word, 0, sizeof(word));
							flag = 0;
						}
					}
				}
				memset(step, 0, sizeof(step));
				flag = 0;
				strcat(step, fornode.var);
				strcat(step, "<=");
				while (1)                  // for var=init to "fina"
				{
					forch = getc(fin);
					if (forch == '{')
					{
						fseek(fin, -1, SEEK_CUR);
						break;
					}
					if (col_num <= 900)
					old_pro[lin_num][col_num] = forch;
					if (forch == '\n')                             //把源程序按按单个字符读取并记录到二维数组
					{
						lin_num++;
						col_num = 0;
					}
					else
					{
						col_num++;
					}
					if (forch == '\n')
					{
						if (flag != 0)
						{
							if (strlen(word) > 0)
								strcat(step, word);
							memset(word, 0, sizeof(word));
							flag = 0;
						}
						strcpy(fornode.fina, step);
						fputs(step, fout);
						fputc('\n', fout);
						new_line_num++;
						off_set[new_line_num].lin = lin_num;
						off_set[new_line_num].off = ftell(fout);
						memset(step, 0, sizeof(step));
						strcpy(step, fornode.fina);
						step[strlen(fornode.var)] = '=';
						fputs(step, fout);
						fputc('\n', fout);
						new_line_num++;
						off_set[new_line_num].lin = lin_num;
						off_set[new_line_num].off = ftell(fout);
						memset(word, 0, sizeof(word));
						flag = 0;
						break;
					}
					if (forch != ' '&& forch != '	')
					{
						word[flag++] = forch;
					}
					else if (flag != 0)
					{
						if (strcmp(word, "step") == 0)
						{
							strcpy(fornode.fina, step);
							fputs(step, fout);
							fputc('\n', fout);
							new_line_num++;
							off_set[new_line_num].lin = lin_num;
							off_set[new_line_num].off = ftell(fout);
							memset(step, 0, sizeof(step));
							strcpy(step, fornode.fina);
							step[strlen(fornode.var)] = '=';
							fputs(step, fout);
							fputc('\n', fout);
							new_line_num++;
							off_set[new_line_num].lin = lin_num;
							off_set[new_line_num].off = ftell(fout);
							fputs(word, fout);
							fputc('\n', fout);
							new_line_num++;
							off_set[new_line_num].lin = lin_num;
							off_set[new_line_num].off = ftell(fout);
							memset(word, 0, sizeof(word));
							flag = 0;
							fornode.ste = 3;
							break;
						}
						else
						{
							strcat(step, word);
							memset(word, 0, sizeof(word));
							flag = 0;
						}
					}
				}
				memset(step, 0, sizeof(step));
				flag = 0;
				if (fornode.ste == 3)			//for var=init to final step "step"
				{
					strcat(step, fornode.var);
					strcat(step, "="); strcat(step, fornode.var); strcat(step, "+");
					while (1)                    
					{
						forch = getc(fin);
						if (forch == '{')
						{
							fseek(fin, -1, SEEK_CUR);
							break;
						}
						if (col_num <= 900)
						old_pro[lin_num][col_num] = forch;
						if (forch == '\n')                             //把源程序按按单个字符读取并记录到二维数组
						{
							lin_num++;
							col_num = 0;
						}
						else
						{
							col_num++;
						}
						if (forch == '\n')
						{
							if (strlen(word) > 0)
								strcat(step, word);
							fputs(step, fout);
							fputc(';', fout);
							fputc('\n', fout);
							new_line_num++;
							off_set[new_line_num].lin = lin_num;
							off_set[new_line_num].off = ftell(fout);
							memset(word, 0, sizeof(word));
							flag = 0;
							break;
						}
						if (forch != ' '&& forch != '	')
						{
							word[flag++] = forch;
						}
						else if (flag != 0)
						{
							strcat(step, word);
							memset(word, 0, sizeof(word));
							flag = 0;
						}
					}
				}
				else 
				{
					fputs("step", fout);
					fputc('\n', fout);
					new_line_num++;
					off_set[new_line_num].lin = lin_num;
					off_set[new_line_num].off = ftell(fout);
					memset(word, 0, sizeof(word));
					flag = 0;
					strcat(step, fornode.var);
					strcat(step, "="); strcat(step, fornode.var); strcat(step, "+1;");
					fputs(step, fout);
					fputc('\n', fout);
					new_line_num++;
					off_set[new_line_num].lin = lin_num;
					off_set[new_line_num].off = ftell(fout);
				}
			}
#pragma endregion forloop
			/**************************************for********************************************/
			else
			{
				for (i = 0; i <= 6; i++)
				{
					if (strcmp(word, word_list[i]) == 0)
					{
						fputs(word, fout);
						fputc('\n', fout);
						new_line_num++;
						off_set[new_line_num].lin = lin_num;
						off_set[new_line_num].off = ftell(fout);
						memset(word, 0, sizeof(word));
						flag = 0;
						break;
					}
				}
				if (i == 7 && ch=='\n')
				{
					fputs(word, fout);
					fputc('\n', fout);
					new_line_num++;
					off_set[new_line_num].lin = lin_num;
					off_set[new_line_num].off = ftell(fout);
					memset(word, 0, sizeof(word));
					flag=0;
				}
			}
		}
		prech = ch;
	}
	fputs(word,fout);
	fclose(fin);
	fclose(fout);

#pragma endregion rand1

	//rand 2 ************************************************************  //初步语法检查
#pragma region rand2
//	fin = fopen("new_pro.sxc", "r");
//	memset(douquote_stack,0,sizeof(douquote_stack));
//	memset(sinquote_stack, 0, sizeof(sinquote_stack));
//	memset(big_stack, 0, sizeof(big_stack));
//	memset(small_stack, 0, sizeof(small_stack));
//	prech = 'a';
//	while ((ch = fgetc(fin)) != EOF)
//	{
//		if (prech != '\\'&&ch == '\"')
//		{
//			if (douquote == 0)
//			{
//				douquote_stack[douquote].chr = '\"';
//				douquote_stack[douquote].off = ftell(fin);
//				douquote++;
//			}
//			else
//				douquote--;
//		}
//		if (prech != '\\'&&ch == '\'')
//		{
//			if (sinquote == 0)
//			{
//				sinquote_stack[sinquote].chr = '\'';
//				sinquote_stack[sinquote].off = ftell(fin);
//				sinquote++;
//			}
//			else
//				sinquote--;
//		}
//		if (douquote % 2 == 0 && sinquote % 2 == 0)
//		{
//			if (ch == '{')
//			{
//				big_stack[big].chr = '{';
//				big_stack[big].off = ftell(fin);
//				big++;
//			}
//			else if (ch == '}')
//			{
//				if (big>=0&&big_stack[big - 1].chr == '{')
//					big--;
//				else
//				{
//					//fseek(fin, big_stack[big - 1].off, SEEK_SET);
//					fseek(fin, big_stack[big - 1].off, SEEK_SET);
//					error_sxc(BRACKET_ERROR, fin);
//				}
//			}
//			else if (ch == '(')
//			{
//				small_stack[small].chr = '(';
//				small_stack[small].off = ftell(fin);
//				small++;
//			}
//			else if (ch == ')')
//			{
//				if (small >= 1 && small_stack[small - 1].chr == '(')
//					small--;
//				else
//				{
//					fseek(fin, small_stack[small-1].off, SEEK_SET);
//					error_sxc(BRACKET_ERROR, fin);
//				}
//			}
//		}
//		prech = ch;
//	}
//	if (be != 1)
//	{
//		fseek(fin, 0, SEEK_SET);
//		error_sxc(BEGIN_NUM_ERROR, fin);
//	}
//	if (en != 1)
//	{
//		fseek(fin, 0, SEEK_END);
//		error_sxc(END_NUM_ERROR, fin);
//	}
//	if (douquote >= 1)
//	{
//		fseek(fin, douquote_stack[douquote-1].off, SEEK_SET);
//		error_sxc(DOUBLEQUOTE_ERROR, fin);
//	}
//	if (sinquote >= 1)
//	{
//		fseek(fin, sinquote_stack[sinquote-1].off, SEEK_SET);
//		error_sxc(SINGLEQUOTE_ERROR, fin);
//	}
//	if (big >= 1)
//	{
//		fseek(fin, big_stack[big - 1].off, SEEK_SET);
//		error_sxc(BRACES_ERROR, fin);
//	}
//	if (small >= 1)
//	{
//		fseek(fin, small_stack[small - 1].off, SEEK_SET);
//		error_sxc(BRACKET_ERROR, fin);
//	}
#pragma endregion rand2
	return "new_pro.sxc";
}

void error_line(long int n, ErrorLine e)
{
	char errorline[1000];
	int i, mid=0;
	for (i = 0; i <= new_line_num - 1; i++)
	{
		if (off_set[i].off <= n&&off_set[i + 1].off > n)
		{
			mid = i;
			break;
		}
	}
	memset(e->line_info, 0, sizeof(e->line_info));
	e->line_no = off_set[mid].lin+1;
	if (e->line_no > 1)
	{
		strcat(e->line_info, old_pro[e->line_no - 2]);
	}
	strcat(e->line_info, old_pro[e->line_no-1]);
	strcat(e->line_info, old_pro[e->line_no]);

	return;
}

void next_line(FILE *fp)
{
	int n, i;
	n = ftell(fp);
	for (i = 0; i <= new_line_num; i++)
	{
		if (off_set[i].off <= n&&off_set[i + 1].off > n)
		{
			break;
		}
	}
	fseek(fp, off_set[i+1].off, SEEK_SET);
	return;
}

int current_line(FILE *fp) //返回当前指针指向的字符在源文件的行数
{
	int n;
	n = ftell(fp);
	int start = 0, mid, end = new_line_num - 1;  //-2也有可能
	while (end - start > 1)
	{
		mid = (start + end) / 2;
		if (off_set[mid].off > n)
		{
			end = mid;
		}
		else
		{
			start = mid;
		}
	}
	//fseek(fp, off_set[end].off, SEEK_SET);
	return off_set[start].lin + 1;
}
