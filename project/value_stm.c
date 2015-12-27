#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "sxc_interface.h"

#define VAL_MAX ((1 << 15) - 1)
#define VAL_MIN (-(1 << 15))
#define NAME_LENGTH 8

#define TYPE_HEAD 0
#define TYPE_NUM 1
#define TYPE_OP 2
#define TYPE_NAME 3 
#define TYPE_P 4
#define TYPE_ADDR 5
#define TYPE_OP_S 12

struct myNode;
typedef struct myNode opNode;
typedef struct myNode *valPtrToNode;
typedef valPtrToNode Expression;
typedef valPtrToNode ExpStack;

union myUnion;
typedef union myUnion data;

valPtrToNode createNode();
void deleteNode(valPtrToNode L);
void printNode(valPtrToNode L);

void splitExp(FILE *fp);
void insertNode(int d_type, data d);
void checkName();
void checkNumS(valPtrToNode p);
void checkNum(valPtrToNode p);

void reversePolish();

int cacl();

void pushRev(valPtrToNode p);
valPtrToNode getTop(ExpStack S);
void popNode(ExpStack S);
void pushNode(valPtrToNode p, ExpStack S);
int getL(valPtrToNode p);
valPtrToNode getTopNum(ExpStack S);
void changeType(valPtrToNode p);

void pointergetVal(valPtrToNode p);
void pointergetAddr(valPtrToNode p);

void error_value(int e);

union myUnion {
    char op;
    int value;
    char name[NAME_LENGTH + 1];
};

struct myNode {
    int d_type;
    data d;
    valPtrToNode Next;
};

Expression Exp;
Expression LastExp;
Expression reverseExp;
long int pos;
FILE *valfp;

int value_stm(FILE *fp) {
    int res;
    
    pos = ftell(fp);
	valfp = fp;

    Exp = createNode();
    LastExp = Exp;
    splitExp(fp);
//    printNode(Exp);
    checkName();
    
    reverseExp = createNode();
    LastExp = reverseExp;
    reversePolish();
//    printNode(reverseExp);

//    printf("%d\n", cacl());
    return cacl();
}

valPtrToNode createNode() {
    valPtrToNode p;

    p = malloc(sizeof(struct myNode));
    p -> d_type = TYPE_HEAD;
    p -> Next = NULL;

    return p;
}

void deleteNode(valPtrToNode L) {
    valPtrToNode tmp, p;
    
    p = L -> Next;
    L -> Next = NULL;

    while (p != NULL) {
        tmp = p -> Next;
	free(p);
	p = tmp;
    }
}

void printNode(valPtrToNode L) {
    valPtrToNode tmp;

    tmp = L -> Next;

    while (tmp != NULL) {
        if (tmp -> d_type == TYPE_NUM) {
	    printf("%d", tmp -> d.value);
	} else if (tmp -> d_type == TYPE_OP || tmp -> d_type == TYPE_OP_S) {
	    printf("%c", tmp -> d.op);
	} else {
	    printf("%s", tmp -> d.name);
	}
	tmp = tmp -> Next;
    }
    printf("\n");

    return ;
}

void insertNode(int d_type, data d) {
    LastExp -> Next = malloc(sizeof(struct myNode));

    LastExp -> Next -> d_type = d_type;
    LastExp -> Next -> d = d;
    LastExp -> Next -> Next = NULL;
    LastExp = LastExp -> Next;
}

void splitExp(FILE *fp) {
    char c;
    data d;
    int i;

    c = fgetc(fp);
    while(1) {
        if (c == ',' || c == ';' || c == '\n') {
	    break;
	}
		if (c < 0 || c > 127) {
			error_value(VALUE_CharCannotBeIdentified);
		}
	if (isspace(c)) {
	    c = fgetc(fp);
	    continue;
	}
	
	if (isdigit(c)) {
	    d.value = 0;
	    while(isdigit(c)) {
	        d.value = d.value * 10 + c - '0';
		c = fgetc(fp);
	    }
	    insertNode(TYPE_NUM, d);
	} else if (isalpha(c)) {
	    i = 0;
	    while(isalnum(c) && i < NAME_LENGTH) {
	        d.name[i] = c;
		i += 1;
		c = fgetc(fp);
	    }
	    if (i == NAME_LENGTH && isalnum(c)) {
			error_value(SIMPLE_NameTooLong);
	    }
	    d.name[i] = '\0';
	    insertNode(TYPE_NAME, d);
	} else if (c == '&') {
	    d.op = c;
	    c = fgetc(fp);
	    insertNode(TYPE_OP_S, d);
	} else if (c == '+' || c == '-' || c == '*') {
	    if (LastExp -> d_type == TYPE_HEAD || LastExp -> d_type == TYPE_OP_S
	     || LastExp -> d_type == TYPE_P
	     || (LastExp -> d_type == TYPE_OP && LastExp -> d.op != ')')) {
	        d.op = c;
		c = fgetc(fp);
		insertNode(TYPE_OP_S, d);
	    } else {
	        d.op = c;
		c = fgetc(fp);
		insertNode(TYPE_OP, d);
	    }
	} else if (c == '/' || c == '%' || c == '(' || c == ')') {
	    d.op = c;
	    c = fgetc(fp);
	    insertNode(TYPE_OP, d);
	} else if (c == '>' || c == '<' || c == '=') {
	    i = 0;
	    d.name[i] = c;
	    i += 1;
	    c = fgetc(fp);
	    if (c == '>' || c == '<' || c == '=') {
	        d.name[i] = c;
		i += 1;
		c = fgetc(fp);
	    }
	    d.name[i] = '\0';
	    insertNode(TYPE_P, d);
	} else {
		error_value(VALUE_CharCannotBeIdentified);
	}
    }
}

void checkName() {
	valPtrToNode l;
	l = Exp;
    LastExp = Exp -> Next;

    while(LastExp != NULL) {
        if (LastExp -> d_type == TYPE_NAME) {
	    if ((strcmp(LastExp -> d.name, "begin") == 0)
	     || (strcmp(LastExp -> d.name, "end") == 0)
	     || (strcmp(LastExp -> d.name, "in") == 0)
	     || (strcmp(LastExp -> d.name, "out") == 0)
	     || (strcmp(LastExp -> d.name, "while") == 0)
	     || (strcmp(LastExp -> d.name, "if") == 0)
	     || (strcmp(LastExp -> d.name, "else") == 0)
	     || (strcmp(LastExp -> d.name, "for") == 0)
	     || (strcmp(LastExp -> d.name, "to") == 0)
	     || (strcmp(LastExp -> d.name, "step") == 0)) {
			error_value(SIMPLE_VariableIsKeyword);
	     }
	} else if (LastExp -> d_type == TYPE_NUM) {
	    checkNumS(LastExp, l);
	}
		l = LastExp;
	LastExp = LastExp -> Next;
    }
}

void checkNumS(valPtrToNode p, valPtrToNode l) {
	if (l->d_type == TYPE_OP_S && l->d.op == '-') {
		if (p->d.value > VAL_MAX + 1)
			error_value(VALUE_BeyondBound);
	}
	else{
		if (p->d.value > VAL_MAX) {
			error_value(VALUE_BeyondBound);
		}
	}
}

void checkNum(valPtrToNode p) {
    if (p -> d.value > VAL_MAX || p -> d.value < VAL_MIN) {
		error_value(VALUE_BeyondBound);
    }
}

void reversePolish() {
    Expression tmp, tmpNew, tmpOp;
    ExpStack StackOp;
    int outL, inL;

    StackOp = createNode();
    tmp = Exp -> Next;
    while (tmp != NULL) {
        tmpNew = tmp -> Next;
        if (tmp -> d_type == TYPE_NUM || tmp -> d_type == TYPE_NAME) {
	    pushRev(tmp);
	} else if (tmp -> d_type == TYPE_OP || tmp -> d_type == TYPE_P){
	    if (tmp -> d.op == '(') {
	        pushNode(tmp, StackOp);
	    } else if (tmp -> d.op == ')') {
	        tmpOp = getTop(StackOp);
	        while (tmpOp -> d.op != '(') {
		    if (tmpOp == NULL) {
				error_value(VALUE_ParenthesesNoMatching);
		    }
		    popNode(StackOp);
		    pushRev(tmpOp);
	            tmpOp = getTop(StackOp);
		}
		popNode(StackOp);
		tmpOp = getTop(StackOp);
	        while (tmpOp != NULL && tmpOp -> d_type == TYPE_OP_S) {
	            popNode(StackOp);
	            pushRev(tmpOp);
	            tmpOp = getTop(StackOp);
		}
	    } else {
	        tmpOp = getTop(StackOp);
	        if (tmpOp == NULL) {
	            pushNode(tmp, StackOp);
		}else if (tmp -> d_type == TYPE_P 
		      && strcmp(tmp -> d.name, "=") == 0){
		      pushNode(tmp, StackOp);
	        } else {
	            outL = getL(tmp);
	            inL = getL(tmpOp);
	            if (outL <= inL) {
	                while (inL >= outL) {
	            	    popNode(StackOp);
	                    pushRev(tmpOp);
	            	    tmpOp = getTop(StackOp);
	            	    if (tmpOp == NULL) {
	            	        break;
	            	    }
	            	    inL = getL(tmpOp);
	                }
	            }
	            pushNode(tmp, StackOp);
	        }
	    }
	} else if (tmp -> d_type == TYPE_OP_S) {
	    pushNode(tmp, StackOp);
	    while (tmpNew != NULL) {
	        tmp = tmpNew;
	        tmpNew = tmp -> Next;
	        if (tmp -> d_type == TYPE_NUM || tmp -> d_type == TYPE_NAME) {
	            pushRev(tmp);
	            tmpOp = getTop(StackOp);
	            while (tmpOp != NULL && tmpOp -> d_type == TYPE_OP_S) {
	                popNode(StackOp);
	                pushRev(tmpOp);
	                tmpOp = getTop(StackOp);
	            }
		    break;
	        } else if (tmp -> d_type == TYPE_OP_S) {
	            pushNode(tmp, StackOp);
	        } else if (tmp -> d_type == TYPE_OP && tmp -> d.op == '(') {
		    pushNode(tmp, StackOp);
		    break;
	        } else {
				error_value(VALUE_EquationNotIntact);
	        }
	    }
	}
        tmp = tmpNew;
    }
    tmpOp = getTop(StackOp);
    while (tmpOp != NULL) {
		if (tmpOp -> d_type == TYPE_OP && tmpOp -> d.op == '(') {
			error_value(VALUE_ParenthesesNoMatching);

		}
	popNode(StackOp);
        pushRev(tmpOp);
	tmpOp = getTop(StackOp);
    }
}

int cacl() {
    ExpStack StackNum;
    Expression tmp, tmpNew, a, b;
    int res;

    StackNum = createNode();
    tmp = reverseExp -> Next;

    while (tmp != NULL) {
        tmpNew = tmp -> Next;
		if (tmp->d_type == TYPE_NUM || tmp->d_type == TYPE_NAME || tmp->d_type == TYPE_ADDR) {
	    pushNode(tmp, StackNum);
	} else if (tmp -> d_type == TYPE_OP) {
	    a = getTopNum(StackNum);
	    b = getTopNum(StackNum);
	    changeType(a);
	    changeType(b);
	    checkNum(a);
	    checkNum(b);
	    switch (tmp -> d.op) {
	    case '+': b -> d.value = b -> d.value + a -> d.value;break;
	    case '-': b -> d.value = b -> d.value - a -> d.value;break;
	    case '*': b -> d.value = b -> d.value * a -> d.value;break;
		case '/': if (a->d.value == 0){ error_value(VALUE_DivideOrModZero); }
	              b -> d.value = b -> d.value / a -> d.value;break;
		case '%': if (a->d.value == 0){ error_value(VALUE_DivideOrModZero); }
	              b -> d.value = b -> d.value % a -> d.value;break;
		default: error_value(SIMPLE_CharCannotBeIdentified);
	    }
	    checkNum(b);
	    pushNode(b, StackNum);
	    free(a);
	} else if (tmp -> d_type == TYPE_OP_S) {
	    a = getTopNum(StackNum);
	    switch (tmp -> d.op) {
	    case '+' : changeType(a);checkNum(a); break;
		case '-': changeType(a); a->d.value = -(a->d.value); checkNum(a);break;
	    case '*' : pointergetVal(a); break;
	    case '&' : pointergetAddr(a); break;
		default: error_value(SIMPLE_CharCannotBeIdentified);
	    }
	    pushNode(a, StackNum);
	} else if (tmp -> d_type == TYPE_P) {
	    if (strcmp(tmp -> d.name, "=") == 0) {
	        a = getTopNum(StackNum);
		b = getTopNum(StackNum);
		if (a->d_type == TYPE_ADDR){
			;
		}
		else
		{
			changeType(a);
		}
		
		if (b -> d_type == TYPE_NAME) {
		    var_update(b -> d.name, 0, a -> d.value, 0);
		    b -> d_type = a -> d_type;
		    b -> d.value = a -> d.value;
		} else {
		    var_update(NULL, b -> d.value, a -> d.value, 1);
		    b -> d_type = a -> d_type;
		    b -> d.value = a -> d.value;
		}
	    } else {
	        a = getTopNum(StackNum);
	        b = getTopNum(StackNum);
	        changeType(a);
	        changeType(b);
		checkNum(a);
		checkNum(b);
	        if (strcmp(tmp -> d.name, "==") == 0){
		    b -> d.value = (b -> d.value == a -> d.value);
	        } else if (strcmp(tmp -> d.name, ">=") == 0){
		    b -> d.value = (b -> d.value >= a -> d.value);
	        } else if (strcmp(tmp -> d.name, "<=") == 0){
		    b -> d.value = (b -> d.value <= a -> d.value);
	        } else if (strcmp(tmp -> d.name, ">") == 0){
		    b -> d.value = (b -> d.value > a -> d.value);
	        } else if (strcmp(tmp -> d.name, "<") == 0){
		    b -> d.value = (b -> d.value < a -> d.value);
	        } else if (strcmp(tmp -> d.name, "<>") == 0){
		    b -> d.value = (b -> d.value != a -> d.value);
	        } else {
				error_value(SIMPLE_CharCannotBeIdentified);
	        }
	    }
	    pushNode(b, StackNum);
	    free(a);
	} else {
		error_value(SIMPLE_CharCannotBeIdentified);
	}
	tmp = tmpNew;
    }
    if (StackNum -> Next -> Next != NULL) {
        deleteNode(StackNum);
		error_value(VALUE_EquationNotIntact);
    }
	if (StackNum->Next->d_type == TYPE_ADDR){
		;
	}
	else{
		changeType(StackNum->Next);
	}
    res = StackNum -> Next -> d.value;
    deleteNode(StackNum);
    return res;
}

valPtrToNode getTopNum(ExpStack S) {
    valPtrToNode tmp;

    tmp = getTop(S);
    if (tmp == NULL) {
		error_value(SIMPLE_MissingVariableOrNumber);
    }
    popNode(S);
    return tmp;
}

void changeType(valPtrToNode p){
	if (p->d_type == TYPE_NUM) {
		if (p->d.value > BASE_ADDR){
			p->d.value = var_value(NULL, p->d.value, 1, 0);
		}
	}
	else if (p->d_type == TYPE_ADDR)
	{
		error_value(VALUE_CANNOT_CACULATING);
    } else {
        p -> d_type = TYPE_NUM;
	p -> d.value = var_value(p -> d.name, 0, 0, 0);
	if (p->d.value == VAR_NOT_FOUND_ERROR){
		error_value(VAR_NOT_FOUND_ERROR);
	}
    }
}

void pushRev(valPtrToNode p) {
    LastExp -> Next = p;
    LastExp = p;
    LastExp -> Next = NULL;
}

valPtrToNode getTop(ExpStack S) {
    return S -> Next;
}

void popNode(ExpStack S) {
    S -> Next = S -> Next -> Next;
}

void pushNode(valPtrToNode p, ExpStack S) {
    p -> Next = S -> Next;
    S -> Next = p;
}

int getL(valPtrToNode p) {
    if (p -> d_type == TYPE_P) {
        if (strcmp(p -> d.name, "=") == 0)
            return 1;
	return 2;
    }
    if (p -> d_type == TYPE_OP_S) {
        return 8;
    }
    switch (p -> d.op) {
    case '+': case '-' : return 3;
    case '*': case '/' : case '%' : return 4;
    case '(': return 0;
    }
}

void error_value(int e) {
    fseek(valfp, pos, SEEK_SET);
    error_sxc(e, valfp);
    // printf("SXC ERROR %d!\n", e);
    // exit(1);
}

void pointergetVal(valPtrToNode p) {
    if (p -> d_type == TYPE_NAME) {
		if (p->d.name[0] != 'p') {
			error_value(SIMPLE_After_1_MustBe_p);
		}
        p -> d.value = var_value(p -> d.name, 0, 0, 0);
    }
    //p -> d.value = var_value(NULL, p -> d.value, 1, 0);
    p -> d_type = TYPE_NUM;
}
void pointergetAddr(valPtrToNode p) {
    if (p -> d_type == TYPE_NAME) {
        p -> d.value = var_value(p -> d.name, 0, 0, 1);
	p -> d_type = TYPE_ADDR;
    } else {
		error_value(VALUE_After_2_MustBeLetter);
    }
}
