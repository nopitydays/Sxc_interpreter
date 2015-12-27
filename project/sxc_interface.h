#include <stdio.h>
#include <stdlib.h>
#include "sxc_struct.h"
#include "sxc_error.h"
#include "sxc_global.h"

#define B_SIZE 10
#define BASE_ADDR (2 << 25)
// 维护变量列表

// begin V3.0
//int var_value(char *name); // 获得名字为name的变量的值，如果不存在返回2<<10
//void var_update(char *name, int value); // 将name的值赋为value

// addr: Pointor
// VarType: name(0) or addr(1)
// ValueType: return int(0) or addr(1)
int var_value(char *name, int addr, int VarType, int ValueType);
void var_update(char *name, int addr, int value, int VarType);

// end V3.0

// 错误处理，当遇到错误时，设定好fp中的位置偏移量
// 就是将位置定在有错误的地方
// 然后调用error_sxc(int , FILE *)
// 处理完程序会退出

void error_sxc(int e, FILE *fp); // fp当前位置有错，错误码由e提供
void error_line(long int n, ErrorLine e); // n为中间文件的位置偏移
// e为错误信息存储结构指针

void next_line(FILE *fp); // 将fp的位置偏移量设置成下一行行首位置偏移量

// 预处理

char *pre_sxc(char *filename); // 对原文件filename进行预处理，返回中间文件名


// 下面都是对代码的解释
// 进入时fp提供了解释代码的起始位置
// 同样结束时fp要为下一个语句地解释提供起始位置

int value_stm(FILE *fp); // 表达式求值，是不含赋值的表达式
// 为简单语句、if语句和while语句服务

void sxc(FILE *fp); // 从fp当前位置开始解释代码

void simple_stm(FILE *fp); // 简单语句

void in_stm(FILE *fp); // in语句
void out_stm(FILE *fp); // out语句

void if_stm(FILE *fp); // if语句
void while_stm(FILE *fp); // while语句
void for_stm(FILE *fp); // for语句

// debug
void sxc_debug(FILE *fp);
