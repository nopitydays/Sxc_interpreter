#ifndef __ERROR_NUM__
#define __ERROR_NUM__
/**************NUM**********************************/
#define PRETREATMENT_ERROR 100
#define IF_ERROR 200
#define WHILE_ERROR 300
#define IN_ERROR 400
#define OUT_ERROR 500
#define VALUE_ERROR 600
#define SIMPLE_ERROR 700
#define READ_CODE_ERROR 800
#define VAR_ERROR 900
/**************NUM**********************************/

/****************PRETREATMENT***********************/
#define BRACKET_ERROR 101
#define DOUBLEQUOTE_ERROR 102
#define SINGLEQUOTE_ERROR 103
#define BRACES_ERROR 104
#define BEGIN_NUM_ERROR 105			//预处理begin数量错误
#define END_NUM_ERROR 106			//预处理end数量错误
/****************PRETREATMENT***********************/
#define NO_BEGIN 191
#define END_ERROR 192


#define MISSING_1_ERROR 232
/****************IF*********************************/
#define IF_CONDITION_ERROR 201
#define IF_MISSING_1_ERROR 202
#define IF_MISSING_2_ERROR 203
#define IF_ELSE_MISSING_1_ERROR 212
#define IF_ELSE_MISSING_2_ERROR 213
#define IF_LIMITED_ERROR 225
#define OTHER_BRACES_ERROR 231
/****************IF*********************************/

/****************WHILE******************************/
#define WHILE_CONDITION_ERROR 301
#define WHILE_MISSING_1_ERROR 302
#define WHILE_MISSING_2_ERROR 303
#define WHILE_LIMITED_ERROR 325
/****************WHILE******************************/

/*****************IN********************************/
#define IN_SemicolonMissing 401// 缺‘;’
#define IN_QuoteMissing  402//‘"’未闭合
#define IN_CommaMissing	 403// 缺','
#define IN_ExtraCharAS	404//; 后有多余字符
#define IN_NameTooLong 405//变量名过长
#define IN_BeyondBound 406 // 超过变量数值范围
#define IN_VariableNameNotLetter 407 //变量必须以字母开头
#define IN_VariableIsKeyword 408 //变量名是保留字
/*****************IN********************************/

/******************VAR******************************/
#define VAR_NOT_FOUND_ERROR (2 << 20)
/******************VAR******************************/

/******************LIMITED**************************/
#define IF_LIMITED_NUM 5
#define WHILE_LIMITED_NUM 5
/******************LIMITED**************************/

/******************OUT**************************/
#define OUT_SemicolonMissing 501//缺‘;’
#define OUT_QuoteMissing 	 502//‘"’未闭合
#define OUT_CommaMissing	 503//缺','
#define OUT_ExtraCharAS		 504//;后有多余字符
#define OUT_Nothing          505//out 后无语句
#define OUT_NLessThanZero	 506//n小于0即n为负
/******************OUT**************************/

/******************VALUE**************************/
#define VALUE_EqualsignMissing 601// "=="中少一个"="
#define VALUE_EquationNotIntact 602// 表达式不完整
#define VALUE_ParenthesesNoMatching 603// 括号不匹配
#define VALUE_DivideOrModZero 604 //表达式有除零项
#define VALUE_VariableNotExist 605 //变量没有初始化
#define VALUE_BeyondBound 606 // 超过变量数值范围
#define VALUE_VariableNameNotLetter 607 //变量必须以字母开头
#define VALUE_ExtraCharCollectionExp 608 //关系式中出现多余字符
#define VALUE_CharCannotBeIdentified 609 //出现未知字符
#define VALUE_After_1_MustBe_p 610 //'*'后面必须p开头的字母
#define VALUE_After_2_MustBeLetter 611//'&'后面必须是字母
#define VALUE_CANNOT_CACULATING 622
/******************VALUE**************************/

/******************SIMPLE**************************/
#define SIMPLE_EqualsignMissing 701// '='缺失
#define SIMPLE_SemicolonMissing 702// 缺';'
#define SIMPLE_ExtraCharAS	703// ';'后有多余字符
#define SIMPLE_NameTooLong 704//变量名过长
#define SIMPLE_CharCannotBeIdentified 705 //出现未知字符
#define SIMPLE_VariableNameNotLetter 706 //变量必须以字母开头
#define SIMPLE_MissingVariableOrNumber 707 //缺少变量或数值
#define SIMPLE_VariableIsKeyword 708 //变量名不能是保留字
#define SIMPLE_After_1_MustBe_p 709 //'*'后面必须p开头的字母
#define SIMPLE_After_2_MustBeLetter 710//'&'后面必须是字母
/******************SIMPLE**************************/

/******************FOR*****************************/
#define FOR_MISSING_1_ERROR 801     //缺to
#define FOR_MISSING_2_ERROR 802     //缺step
#define FOR_MISSING_3_ERROR 803     //缺{
#define FOR_CONDITION_ERROR 804     //for最后带分号
/******************FOR*****************************/

/********************OK*****************************/
/********************OK*****************************/







#endif
