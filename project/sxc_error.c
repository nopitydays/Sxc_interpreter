#include "sxc_interface.h"
#include "sxc_errorinfo.h"

extern List var_l; // 全局变量列表指针

void error_sxc(int e, FILE *fp) { // 出错处理
	long int n; // 错误位置偏移量
	ErrorLine el; // 错误信息
	int i;

	n = ftell(fp); // 得到错误位置偏移量
	el = malloc(sizeof(struct errorLineInfo));
	error_line(n, el); // 得到错误信息

	// 错误分类处理
	/* v1.0
	switch(e) {
	case PRETREATMENT_ERROR :
	printf("Error! PRETREATMENT Error!\nLine: %3d  %s\n", el -> line_no, el -> line_info);
	break;
	case IF_ERROR :
	printf("Error! IF Error!\nLine: %3d  %s\n", el -> line_no, el -> line_info);
	break;
	case WHILE_ERROR :
	printf("Error! WHILE Error!\nLine: %3d  %s\n", el -> line_no, el -> line_info);
	break;
	case IN_ERROR :
	printf("Error! IN Error!\nLine: %3d  %s\n", el -> line_no, el -> line_info);
	break;
	case OUT_ERROR:
	printf("Error! OUT Error!\nLine: %3d  %s\n", el -> line_no, el -> line_info);
	break;
	case VALUE_ERROR:
	printf("Error! VALUE Error!\nLine: %3d  %s\n", el -> line_no, el -> line_info);
	break;
	default :
	printf("Error! UNKNOWN Error!\nLine: %3d  %s\n", el -> line_no, el -> line_info);
	}
	*/

	// v1.1
	for (i = 1; i < ERROR_NUM; i++) {
		if (e == eo[i].errorcode) {
			printf("ERROR! %s\nLine: %3d\n%s\n", eo[i].str, el->line_no, el->line_info);
			break;
		}
	}
	if (i == ERROR_NUM) {
		printf("ERROR! %s\nLine: %3d:\n%s\n", eo[0].str, el->line_no, el->line_info);
	}

	DeleteList(var_l); // 销毁变量列表

	exit(1); // 异常退出

}
