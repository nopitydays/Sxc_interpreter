#include <stdio.h>//"g=",g;
#include <string.h>
#include "sxc_struct.h"
#include "sxc_interface.h"
#include "sxc_error.h"

void in_stm(FILE *fp)
{
	int i = 0, j = 0, flag = 0, fi = 0, n, ffen = 0, fdou = 0;
	char x, string[1000] = {'\0'}, temp[1000] = {'\0'};
	long int start;
	start = ftell(fp);
	while(1){
		x = fgetc(fp);
		if(x == ';'){
			ffen = 1;
			x = fgetc(fp);
			if(x != '\n'){
				fseek(fp, start, SEEK_SET);
				error_sxc(IN_ExtraCharAS, fp); //';�����ж����ַ�'
			}
			if(x == '\n'){
				fseek(fp, -1, SEEK_CUR);
			}
 			continue;
		}
		if(x == ','){
			fdou = 1;
			continue;
		}
		if(x == '\n'){
			if(ffen == 0){
				fseek(fp, start, SEEK_SET);
				error_sxc(IN_SemicolonMissing, fp); //ȱ��':' 
			}
			if(fdou == 0 && fi == 2){
				fseek(fp, start, SEEK_SET);
				error_sxc(IN_CommaMissing, fp); //ȱ��',' 
			}
			if(strlen(temp) > 8){
				fseek(fp, start, SEEK_SET);
				error_sxc(IN_NameTooLong, fp); //���������� 
			}
			break;
		}
		if(x == '"'){
			fi = 1;
			while(1){
				x = fgetc(fp);
				if(x == '"'){
					flag = 1;
					fi = 2;
					break;
				}
				if(x == '\\'){
					x = fgetc(fp);
					if(x == 'n'){
						string[i] = '\n';
					}
					if(x == 't'){
						string[i] = '\t';
					}
					if(x == '\''){
						string[i] = '\'';
					}
					if(x == '"'){
						string[i] = '"';
					}
					if(x == '\\'){
						string[i] = '\\';
					}
					if(x != 'n' && x != '\'' && x != '"' && x != '\\' && x != 't'){
						fseek(fp, -2, SEEK_CUR);
						x = fgetc(fp);
						string[i] = x;
					}
					i++;
					continue;
				}
				string[i] = x;
				i++;
			}
		}
		if(flag == 1){
			flag = 0;
			continue;
		}
		temp[j] = x;
		j++;
	}
	if(fi == 1){
		fseek(fp, start, SEEK_SET);
		error_sxc(IN_QuoteMissing, fp); //���Ų�ƥ����� 
	}
	if(strcmp(temp, "in") == 0){
		error_sxc(IN_VariableIsKeyword, fp);
	}
	if(strcmp(temp, "out") == 0){
		error_sxc(IN_VariableIsKeyword, fp);
	}
	if(strcmp(temp, "if") == 0){
		error_sxc(IN_VariableIsKeyword, fp);
	}
	if (strcmp(temp, "while") == 0){
		error_sxc(IN_VariableIsKeyword, fp);
	}
	printf("%s", string);
	if(temp[0] >= '0' && temp[0] <= '9'){
		error_sxc(IN_VariableNameNotLetter, fp); //���������������ֿ�ͷ 
	}
	scanf("%d", &n);
	if(n > 32767 || n < -32768){
		fseek(fp, start, SEEK_SET);
		error_sxc(IN_BeyondBound, fp); //����������Χ 
	}
	var_update(temp, 0, n, 0);
}
