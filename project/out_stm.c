#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sxc_interface.h"
//#include "sub_stm.h"
//#include "error_no.h"
//#include "sxc_golbal.h"

//out语句8种情况： out n,"XXXXX",a;  2（有无n）*2（有无提示串）*2（有无a）=8
void out_stm(FILE *fp) {
	int i = 0, n = 0, j = 1, k = 0, flag = 0, flag1 = 0, flag2 = 0, flag3 = 0;//flag对应" n,a; ",flag1对应第一个"," ,flag2对应';'
	int f1 = 0, f2 = 0, f3 = 0;//f1有无n,f2有无提示串,f3有无a
	int error1 = 0, error2 = 0, error3 = 0, error4 = 0;//报错标记
	int ef1 = 0, ef2 = 0, ef3 = 0, ef4 = 0, ef5 = 0, ef6 = 0;
	int str1 = 1, str2 = 0;//str1循环次数变量值，str2 输出变量值
	long pointoffset1 = 0, pointoffset2;//记录文件指针位置，字符串求值调用
	char x, string1[1024], string2[1024], chinese[2048];//string1存循环次数变量名，str2 输出变量名，chinese提示串字符
	memset(string1, 0, sizeof(string1));
	memset(string2, 0, sizeof(string2));
	memset(chinese, 0, sizeof(chinese));
	while (1){
		pointoffset1 = ftell(fp);
		x = fgetc(fp);
		while (1){
			if (x == '\n')
			{
				x = fgetc(fp);
			}
			if (x == EOF){
				error_sxc(OUT_Nothing, fp);
			}
			if (x >= 42 && x <= 57){
				while (1){
					if (x == '\n'&&flag2 == 0){
						error_sxc(501, fp);
					}
					if (x == '\"'&& flag1 == 0){
						error_sxc(503, fp);
					}
					if (x == ','){
						flag1 = 1;
						x = fgetc(fp);
						if (x != '\"'){
							flag = 1;
							break;
						}
						else {
							break;
						}
					}
					if(x == ';'){
						flag2 = 1;
						x = fgetc(fp);
						if (x == '\n'){
							fseek(fp, -3, SEEK_CUR);
							x = fgetc(fp);
							break;
						}
						else{
							fseek(fp, -1, SEEK_CUR);
							error_sxc(504, fp);
						}
						break;
					}
					string1[i] = x;
					i++;
					f1 = 1;
					x = fgetc(fp);
				}
				pointoffset2 = ftell(fp);
				fseek(fp, pointoffset1, SEEK_SET);
				str1 = value_stm(fp);
				fseek(fp, pointoffset2, SEEK_SET);
			}
			if (x >= 65 && x <= 127){
				while (1){
					if (x == '\n'&&flag2 == 0){
						error_sxc(501, fp);
					}
					if (x == '\"'&& flag1 == 0){
						error_sxc(503, fp);
					}
					if (x == ','){
						flag1 = 1;
						x = fgetc(fp);
						if (x != '\"'){
							flag = 1;
							break;
						}
						else {
							break;
						}
					}
					if (x == ';'){
						flag2 = 1;
						x = fgetc(fp);
						if (x == '\n'){
							fseek(fp, -3, SEEK_CUR);
							x = fgetc(fp);
							break;
						}
						else{
							fseek(fp, -1, SEEK_CUR);
							error_sxc(504, fp);
						}
						break;
					}
					string1[i] = x;
					i++;
					f1 = 1;
					x = fgetc(fp);
				}
				pointoffset2 = ftell(fp);
				fseek(fp, pointoffset1, SEEK_SET);
				str1 = value_stm(fp);
				fseek(fp, pointoffset2, SEEK_SET);
			}

			if (x == '"'){
				while (1){
					x = fgetc(fp);
					if (x == '\\'){
						x = fgetc(fp);
						while (1){
							if (x == '\\'){
								chinese[k] = '\\';
								k++;
								x = fgetc(fp);
								if (x == '\\'){
									x = fgetc(fp);
									continue;
								}
								else break;
							}
							else if (x == 'n'){
								chinese[k] = '\n';
								k++;
								x = fgetc(fp);
								if (x == '\\'){
									x = fgetc(fp);
									continue;
								}
								else break;
							}
							else if (x == '\''){
								chinese[k] = '\'';
								k++;
								x = fgetc(fp);
								if (x == '\\'){
									x = fgetc(fp);
									continue;
								}
								else break;
							}
							else if (x == '\"'){
								chinese[k] = '\"';
								k++;
 								x = fgetc(fp);
								if (x == '\\'){
									x = fgetc(fp);
									continue;
								}
								else break;
							}
							else if (x == 't'){
								chinese[k] = '\t';
								k++;
								x = fgetc(fp);
								if (x == '\\'){
									x = fgetc(fp);
									continue;
								}
								else break;
							}
							else{
								break;
							}
						}
					}
					if (x == '"'){
						x = fgetc(fp);
						if (x != ','&&x != ';'){
							error_sxc(503, fp);
						}
						fseek(fp, -1, SEEK_CUR);
						fseek(fp, -2, SEEK_CUR);
						x = fgetc(fp);
						if (x == '\\'){
							j = 1;
							break;
						}
						else{
							fseek(fp, 1, SEEK_CUR);
							x = fgetc(fp);
							break;
						}
						fseek(fp, 1, SEEK_CUR);
					}
					
						chinese[k] = x;
						k++;
						f2 = 1;
						j = 1;
					
					if (x == '\n'){
						error_sxc(502, fp);
					}
				}
			}
			if (x == ','&& flag1 == 1){
				i = 0, j = 1;
				pointoffset1 = ftell(fp);
				while (1){
					x = fgetc(fp);
					string2[i] = x;
					i++;
					pointoffset2 = ftell(fp);
					fseek(fp, pointoffset1, SEEK_SET);
					str2 = value_stm(fp);
					fseek(fp, pointoffset2, SEEK_SET);
					//x = fgetc(fp);
					if (x == ';'){
						flag2 = 1;
						x = fgetc(fp);
						if (x == '\n'){
							fseek(fp, -3, SEEK_CUR);
							x = fgetc(fp);
							break;
						}
						else{
							fseek(fp, -1, SEEK_CUR);
							error_sxc(504, fp);
						}
						break;
					}
					if (x == '\n'){
						error_sxc(501, fp);
					}
				}
			}
			if (x == ','&&f2 == 1){
				i = 0, j = 1;
				pointoffset1 = ftell(fp);
				while (1){
					x = fgetc(fp);
					string2[i] = x;
					f3 = 1;
					i++;
					pointoffset2 = ftell(fp);
					fseek(fp, pointoffset1, SEEK_SET);
					str2 = value_stm(fp);
					fseek(fp, pointoffset2, SEEK_SET);
					//x = fgetc(fp);
					if (x == ';'){
						flag2 = 1;
						x = fgetc(fp);
						if (x == '\n'){
							fseek(fp, -3, SEEK_CUR);
							x = fgetc(fp);
							break;
						}
						else{
							fseek(fp, -1, SEEK_CUR);
							error_sxc(504, fp);
						}
						break;
					}
					if (x == '\n'){
						error_sxc(501, fp);
					}
				}
			}
			if (flag == 1){
				i = 0, j = 1;
				fseek(fp, -1, SEEK_CUR);
				pointoffset1 = ftell(fp);
				while (1){
 					x = fgetc(fp);
					string2[i] = x;
					f3 = 1;
					i++;
					//x = fgetc(fp);
					if (x == '\"'){
						error_sxc(502, fp);
					}
					if (x == ';'){
						pointoffset2 = ftell(fp);
						fseek(fp, pointoffset1, SEEK_SET);
						str2 = value_stm(fp);
						fseek(fp, pointoffset2, SEEK_SET);
						flag2 = 1;
						flag = 0;
						x = fgetc(fp);
						if (x == '\n'){
							fseek(fp, -3, SEEK_CUR);
							x = fgetc(fp);
							break;
						}
						else{
							fseek(fp, -1, SEEK_CUR);
							error_sxc(504, fp);
						}
						break;
					}
					if (x == '\n'){
						error_sxc(501, fp);
					}
				}
			}

			if (x == ';'){
				flag2 = 1;
				x = fgetc(fp);
				if (x == '\n'){
					fseek(fp, -1, SEEK_CUR);
					x = fgetc(fp);
					break;
				}
				else{
					fseek(fp, -1, SEEK_CUR);						
					error_sxc(504,fp);
				}
				break;
			}
		}

		if (flag2 == 1){
			break;
		}
	}

	if (string1[0] != '\0'){
		f1 = 1;
	}
	if (chinese[0] != '\0'){
		f2 = 1;
	}
	if (string2[0] != '\0'){
		f3 = 1;
	}

	if (str1 < 0 && f2 == 0 && f3 == 1){
		error_sxc(506, fp);
	}
	if (str1 < 0 && f2 == 1){
		error_sxc(506, fp);
	}

	if (f1 == 1){
		for (i = 0; i < str1; i++){
			printf("%s", chinese);
		}
	}
	if (f1 != 1 && f2 == 1){
		printf("%s", chinese);
	}
	if (f3 == 1){
		printf("%d", str2);
	}
	if (f1 == 1 && f2 == 0 && f3 == 0){
		printf("%d", str1);
	}

}
