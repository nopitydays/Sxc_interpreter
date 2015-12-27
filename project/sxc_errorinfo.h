#include "sxc_struct.h"
#include "sxc_interface.h"
#include "sxc_error.h"

#define ERROR_NUM 72

ErrorOp eo[ERROR_NUM] = {
	-1, "UKNOWN ERROR!",
	PRETREATMENT_ERROR, "PRETREATMENT ERROR!",
	IF_ERROR, "IF ERROR!",
	WHILE_ERROR, "WHILE ERROR!",
	IN_ERROR, "IN ERROR!",
	OUT_ERROR, "OUT ERROR!",
	VALUE_ERROR, "VALUE ERROR!",
	SIMPLE_ERROR, "SIMPLE ERROR!",
	READ_CODE_ERROR, "READ CODE ERROR!",
	VAR_ERROR, "VAR ERROR!",
	BRACKET_ERROR, "BRACKET ERROR!",
	DOUBLEQUOTE_ERROR, "DOUBLEQUOTE ERROR!",
	SINGLEQUOTE_ERROR, "SINGLEQUOTE ERROR!",
	BRACES_ERROR, "BRACES ERROR!",
	BEGIN_NUM_ERROR, "BEGIN NUM ERROR!",
	END_NUM_ERROR, "END NUM ERROR!",
	MISSING_1_ERROR, "MISSING } ERROR!",
	IF_CONDITION_ERROR, "IF CONDITION ERROR!",
	IF_MISSING_1_ERROR, "IF MISSING { ERROR!",
	IF_MISSING_2_ERROR, "IF MISSING } ERROR!",
	IF_ELSE_MISSING_1_ERROR, "IF ELSE MISSING { ERROR!",
	IF_ELSE_MISSING_2_ERROR, "IF ELSE MISSING } ERROR!",
	IF_LIMITED_ERROR, "IF LIMITED ERROR!",
	OTHER_BRACES_ERROR, "OTHER BRACES ERROR!",
	WHILE_CONDITION_ERROR, "WHILE CONDITION ERROR!",
	WHILE_MISSING_1_ERROR, "WHILE MISSING { ERROR!",
	WHILE_MISSING_2_ERROR, "WHILE MISSING } ERROR!",
	WHILE_LIMITED_ERROR, "WHILE LIMITED ERROR!",
	OUT_SemicolonMissing, "OUT SemicolonMissing ERROR!",
	OUT_QuoteMissing, "OUT QuoteMissing ERROR!",
	OUT_CommaMissing, "OUT CommaMissing ERROR!",
	OUT_ExtraCharAS, "OUT ExtraCharAS ERROR!",
	OUT_NLessThanZero, "The N is negative!",
	VAR_NOT_FOUND_ERROR, "VAR NOT FOUND ERROR!",
	IN_SemicolonMissing, "IN SemicolonMissing!",
	IN_QuoteMissing, "IN QuoteMissing!",
	IN_CommaMissing, "IN CommaMissing!",
	IN_ExtraCharAS, "IN ExtraCharAS!",
	IN_NameTooLong, "IN NameTooLong!",
	IN_BeyondBound, "IN BeyondBound!",
	IN_VariableIsKeyword, "N VariableIsKeyword!",
	IN_VariableNameNotLetter, "IN VariableNameNotLetter!",
	VALUE_EqualsignMissing, "VALUE EqualsignMissing!",
	VALUE_EquationNotIntact, "VALUE EquationNotIntact!",
	VALUE_ParenthesesNoMatching, "VALUE ParenthesesNoMatching!",
	VALUE_DivideOrModZero, "VALUE DivideOrModZero!",
	VALUE_VariableNotExist, "VALUE VariableNotExist!",
	VALUE_BeyondBound, "VALUE BeyondBound!",
	VALUE_VariableNameNotLetter, "VALUE VariableNameNotLetter!",
	VALUE_ExtraCharCollectionExp, "VALUE ExtraCharCollectionExp!",
	VALUE_After_1_MustBe_p, "VALUE After'*'MustBe'p'",
	VALUE_After_2_MustBeLetter, "VALUE After'&'MustBeLetter",
	VALUE_CANNOT_CACULATING, "VALUE CANNOT CACULATING!",
	VALUE_CharCannotBeIdentified, "VALUE CharCannotBeIdentified!",
	SIMPLE_CharCannotBeIdentified, "SIMPLE CharCannotBeIdentified!",
	SIMPLE_EqualsignMissing, "SIMPLE EqualsignMissing!",
	SIMPLE_SemicolonMissing, "SIMPLE SemicolonMissing!",
	SIMPLE_ExtraCharAS, "SIMPLE ExtraCharAS!",
	SIMPLE_NameTooLong, "SIMPLE NameTooLong!",
	SIMPLE_CharCannotBeIdentified, "SIMPLE CharCannotBeIdentified!",
	SIMPLE_VariableNameNotLetter, "SIMPLE VariableNameNotLetter!",
	SIMPLE_MissingVariableOrNumber, "SIMPLE MissingVariableOrNumber!",
	SIMPLE_VariableIsKeyword, "SIMPLE VariableIsKeyword!",
	SIMPLE_After_1_MustBe_p, "SIMPLE After_*_MustBe_p!",
	SIMPLE_After_2_MustBeLetter, "SIMPLE After_&_MustBeLetter!",
	FOR_MISSING_1_ERROR, "FOR MISSING TO ERROR!",     //缺to
	FOR_MISSING_2_ERROR, "FOR MISSING STEP ERROR!",     //缺step
	FOR_MISSING_3_ERROR, "FOR MISSING { ERROR!",     //缺{
	FOR_CONDITION_ERROR, "FOR CONDITION ERROR!",     //for最后带分号
	OUT_Nothing, "Nothing After OUT!",
	NO_BEGIN, "NO BEGIN!",
	END_ERROR, "END ERROR!",






};
