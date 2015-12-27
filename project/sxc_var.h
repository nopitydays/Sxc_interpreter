#include "sxc_struct.h"

Position Find(char *name, List L); // 返回name在L中的位置，不存在返回NULL
void Insert(char *name, int value, List L); // 在L中插入变量名为name，值为value的变量
void DeleteList(List L); // 销毁变量列表
// begin V3.0
Position FindAddr(int addr, List L); // 返回name在L中的位置，不存在返回NULL
// void InsertAddr(int addr, int value, List L); // 在L中插入变量名为name，值为value的变量
// end V3.0
