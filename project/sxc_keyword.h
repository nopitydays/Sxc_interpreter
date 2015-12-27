#include "sxc_struct.h"
#include "sxc_interface.h"

#define KEYWORD_NUM 6

Keyword key[KEYWORD_NUM] = {
	"simple\n", simple_stm,
	"if\n", if_stm,
	"while\n", while_stm,
	"in\n", in_stm,
	"out\n", out_stm,
	"for\n", for_stm,
};
