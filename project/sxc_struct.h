#ifndef __SXC_STRUCT__
#define __SXC_STRUCT__

struct keyword;
typedef struct keyword Keyword;

struct keyword {
	char *key;
	void(*fun)(FILE *fp);
};


struct errorop;
typedef struct errorop ErrorOp;

struct errorop {
	int errorcode;
	char *str;
};


struct listnode;
typedef struct listnode *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

struct listnode {
	char *name;
	int addr;
	int value;
	Position Next;
};


struct errorLineInfo;
typedef struct errorLineInfo *ErrorLine;

struct errorLineInfo {
	int line_no;
	char line_info[3000];
};


struct preOffset;
typedef struct perOffset offset;

struct perOffset {
	long int off;
	int lin;
};	

struct breakpoint;
typedef struct breakpoint BP;  //存储断点的链表结构

struct breakpoint
{
	int line_num;  //断点行号(源文件的行号)
	int flag;      //是否运行过
	int single;    //是否是单步调试时所加断点(如果是则为1)
	struct breakpoint *next;
};

#endif
