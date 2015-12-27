#include "sxc_interface.h"
#include "sxc_var.h"

extern List var_l; // 全局变量，变量列表
extern int nowAddr;

void nospace();

// int var_value(char *name) { // 获取就量name的值，不存在返回 VAR_NOT_FOUND_ERROR
//     Position P;
// 
//     if((P = Find(name, var_l)) == NULL){ // 变量不存在
//         return VAR_NOT_FOUND_ERROR;
//     }
//     return P -> value;
// }
// 
// void var_update(char *name, int value) { // 赋值或更新name的值为value
//     Position P;
//     
//     if((P = Find(name, var_l)) != NULL){ // 变量存在
//         P -> value = value; // 赋值
//     }
//     else { // 变量不存在，为新变量
//         Insert(name, value, var_l);
//     }
// }

// V3.0
int var_value(char *name, int addr, int VarType, int ValueType) {
	Position P;

	if (VarType) {
		if ((P = FindAddr(addr, var_l)) == NULL) {
			return VAR_NOT_FOUND_ERROR;
		}
	}
	else {
		if ((P = Find(name, var_l)) == NULL){ // 变量不存在
			return VAR_NOT_FOUND_ERROR;
		}
	}
	if (ValueType) {
		return P->addr;
	}
	return P->value;
}

void var_update(char *name, int addr, int value, int VarType) {
	Position P;

	if (VarType) {
		if ((P = FindAddr(addr, var_l)) != NULL) {
			P->value = value;
		}
		else {
			// InsertAddr(addr, value, val_l);
			printf("SXC ERROR! Segmentation fault!\n");
			exit(1);
		}
	}
	else {
		if ((P = Find(name, var_l)) != NULL){ // 变量存在
			P->value = value; // 赋值
		}
		else { // 变量不存在，为新变量
			Insert(name, value, var_l);
		}
	}

	// if((P = Find(name, var_l)) != NULL){ // 变量存在
	//     P -> value = value; // 赋值
	// }
	// else { // 变量不存在，为新变量
	//     Insert(name, value, var_l);
	// }
}

Position Find(char *name, List L) { // 查找变量name是否存在，存在返回位置，不存在返回NULL
	Position P;

	P = L->Next;
	while (P != NULL && (strcmp(name, P->name) != 0))
		P = P->Next;

	return P;
}

void Insert(char *name, int value, List L) { // 插入新变量
	Position TmpCell;

	TmpCell = malloc(sizeof(struct listnode)); // 申请新变量空间
	if (TmpCell == NULL) { // 失败
		nospace();
	}
	// 赋值
	TmpCell->name = malloc(sizeof(char)* strlen(name) + 1);
	if (TmpCell->name == NULL) {
		nospace();
	}
	strcpy(TmpCell->name, name);
	// TmpCell -> name = name;
	TmpCell->addr = nowAddr;
	nowAddr += 1;
	TmpCell->value = value;
	TmpCell->Next = L->Next;
	L->Next = TmpCell;
}

// begin V3.0
Position FindAddr(int addr, List L) {
	Position P;

	P = L->Next;
	while (P != NULL && P->addr != addr)
		P = P->Next;

	return P;
}

// void InsertAddr(int addr, int value, List L) {
//     Position TmpCell;
// 
//     TmpCell = malloc(sizeof(struct listnode)); // 申请新变量空间
//     if (TmpCell == NULL) { // 失败
//         nospace();
//     }
//     // 赋值
//     TmpCell -> name = malloc(sizeof(char) * strlen(name) + 1);
//     if (TmpCell -> name == NULL) {
//         nospace();
//     }
//     strcpy(TmpCell -> name, name);
//     // TmpCell -> name = name;
//     TmpCell -> value = value;
//     TmpCell -> Next = L -> Next;
//     L -> Next = TmpCell;
// }
// end V3.0

void DeleteList(List L) { // 删除列表
	Position P, Tmp;

	P = L->Next;
	L->Next = NULL;

	while (P != NULL) {
		Tmp = P->Next;
		free(P);
		P = Tmp;
	}
	nowAddr = BASE_ADDR + 1;
}

void nospace() {
	printf("RUNNING ERROR! NO MORE SPACE!\n");
	exit(1);
}
