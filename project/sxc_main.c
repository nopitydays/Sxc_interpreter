#include "sxc_interface.h"

FILE *sxc_test(char *argv);

int main(int argc, char *argv[]) {
	FILE *fp;
	char *filename;

	var_l = malloc(sizeof(struct listnode)); // 建立变量列表
	if (var_l == NULL) { // 申请内存失败
		printf("RUNNING ERROR! CREATINT VAR_LIST ERROR!\n");
		exit(1);
	}

	// 初始化
	while_num = 0;
	if_num = 0;
	var_l->Next = NULL;
	lin_num = 0;
	nowAddr = BASE_ADDR + 1;


	if (argc == 2) { // ./sxc_main filename
		fp = sxc_test(argv[1]); // 文件预处理

		sxc(fp);
		fclose(fp);
	}
	else if (argc == 3) { // ./sxc_main filename -db
		if (strcmp(argv[2], "-db") == 0) {
			filename = argv[1];
		}
		else if (strcmp(argv[1], "-db") == 0) {
			filename = argv[2];
		}
		else { // 格式问题
			printf("Usage: ./sxc filename.sxc [-db]\n");
			exit(1);
		}

		// 开始进入debug模式
		printf("Begin Debug\n");
		fp = sxc_test(argv[1]);

		PointHead = malloc(sizeof(BP));
		if (PointHead == NULL)
			printf("malloc fault!");
		PointHead->next = NULL;
		PointHead->line_num = -1;
		PointHead->flag = 0;
		PointHead->single = 0;
		modetype = 1;
		runcode = 0;
		nextcode = 1;
		othercode = 0;
		sxc(fp);
		fclose(fp);
	}
	else { // 格式问题
		printf("Usage: ./sxc filename.sxc [-db]\n");
		exit(1);
	}
	/*fp = sxc_test("benoo0.sxc","r");
	sxc(fp);
	fclose(fp);*/
	DeleteList(var_l); // 删除变量列表

	return 0;
}


FILE *sxc_test(char *argv) {
	FILE *fp;
	char *filename;
	char str[B_SIZE + 1];
	long int pos;

	if ((fp = fopen(argv, "r")) == NULL) { // 文件打开失败
		printf("Error! The file <%s> can not be opened.\n", argv);
		DeleteList(var_l);
		exit(1);
	}
	fclose(fp);

	filename = pre_sxc(argv); // 源代码预处理
	// filename = "new_pro.sxc";
	if ((fp = fopen(filename, "r")) == NULL) {
		printf("Error@ The file <%s> can not be opened.\n", filename);
		DeleteList(var_l);
		exit(1);
	}
	// fgets(str, B_SIZE, fp); // 读取begin
	while (1) {
		pos = ftell(fp);
		fgets(str, B_SIZE, fp);
		if (strcmp(str, "begin\n") == 0) {
			break;
		}
		else if (strcmp(str, "end\n") == 0 || str == NULL){
			fseek(fp, pos, SEEK_SET);
			error_sxc(NO_BEGIN, fp);
		}
		fseek(fp, pos, SEEK_SET);
		next_line(fp);
	}

	return fp;
}
