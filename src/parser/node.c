#include "../inc/node.h"
#include "../inc/token.h"
#include "../inc/parser.h"

#include <stdlib.h>
#include <stdio.h>



void node__show(struct node* node) {
	if(node->type == N_INTEXP) {
		putchar(' ');
		node->exp.intExp->show(node->exp.intExp);
		putchar(' ');
	}
	if(node->type == N_BINEXP) {
		putchar('(');
		node->exp.binExp.left->show(node->exp.binExp.left);
		node->exp.binExp.op->show(node->exp.binExp.op);
		node->exp.binExp.right->show(node->exp.binExp.right);
		putchar(')');
	}
	if(node->type == N_UNAEXP) {
		putchar('(');
		node->exp.unaExp.op->show(node->exp.unaExp.op);
		node->exp.unaExp.operand->show(node->exp.unaExp.operand);
		putchar(')');
	}
	if(node->type == N_VARASN) {
		putchar('(');
		node->exp.varAsn.varName->show(node->exp.varAsn.varName);
		printf(" \'%s\' : ", node->exp.varAsn.varName->value.id_str);
		node->exp.varAsn.binExp->show(node->exp.varAsn.binExp);
		putchar(')');
		
	}
	if(node->type == N_VARACC) {
		putchar('(');
		node->exp.varAcc.varName->show(node->exp.varAcc.varName);
		printf(" \'%s\' : ", node->exp.varAcc.varName->value.id_str);
		putchar(')');
	}
	if(node->type == N_IFNODE) {
		printf("\nIF: ");
		node->exp.ifNode.conditions[0]->show(node->exp.ifNode.conditions[0]);
		printf("\n THEN: ");
		node->exp.ifNode.bodies[0]->show(node->exp.ifNode.bodies[0]);
		
		for(int i = 1; i<node->exp.ifNode.len; i++) {
			printf("\nELIF: ");
			node->exp.ifNode.conditions[i]->show(node->exp.ifNode.conditions[i]);
			printf("\n THEN: ");
			node->exp.ifNode.bodies[i]->show(node->exp.ifNode.bodies[i]);
		}

		if(node->exp.ifNode.elseBody != NULL){
			printf("\nELSE: ");
			node->exp.ifNode.elseBody->show(node->exp.ifNode.elseBody);
		}

	}
	if(node->type == N_WHILE) {
		printf("\nWHILE: ");
		node->exp.nWhile.condition->show(node->exp.nWhile.condition);
		printf("\nDO: ");
		node->exp.nWhile.body->show(node->exp.nWhile.body);
	}
	if(node->type == N_FUNCAL) {
		printf("\nfuncCall");
		fflush(stdout);
		printf("\nfunction call, %d args: ", node->exp.funcCall.argNum);
		node->exp.funcCall.name->show(node->exp.funcCall.name);
		for(int i = 0 ; i<node->exp.funcCall.argNum; i++)
			node->exp.funcCall.args[i]->show(node->exp.funcCall.args[i]);
	}
	if(node->type == N_FUNDEF) {
		printf("\nfuncDef");
		fflush(stdout);
		printf("\nfunction definition, %d args: ", node->exp.funcDef.argNum);
		node->exp.funcDef.name->show(node->exp.funcDef.name);
		for(int i = 0 ; i<node->exp.funcDef.argNum; i++)
			printf(" %s ", node->exp.funcDef.args[i]->value.id_str);
		node->exp.funcDef.body->show(node->exp.funcDef.body);
	}
}

//constructors
struct node* node__init__intExp(struct token* token, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	
	node->type = N_INTEXP;
	node->exp.intExp = token;

	node->parentLexer = parentParser->parentLexer;
	node->parentToken = token;
	node->pos_start = token->pos_start;
	node->pos_end = token->pos_end;

	node->show = &node__show;
	return node;
}

struct node* node__init__binExp(struct token* op, struct node* left, struct node* right, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));

	node->type = N_BINEXP;

	node->exp.binExp.op = op;
	
	node->exp.binExp.left = left;
	node->exp.binExp.right = right;
	
	node->parentLexer = parentParser->parentLexer;
	node->parentToken = op;
	node->pos_start = left->pos_start;
	node->pos_end = right->pos_end;

	node->show = &node__show;
	return node;
}

struct node* node__init__unaExp(struct token* op, struct node* operand, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	
	node->type = N_UNAEXP;

	node->exp.unaExp.op = op;
	node->exp.unaExp.operand = operand;

	node->parentLexer = parentParser->parentLexer;
	node->parentToken = op;
	node->pos_start = op->pos_start;
	node->pos_end = operand->pos_end;

	node->show = &node__show;
	return node;
}

struct node* node__init__varAsn(struct token* varName, struct node* binExp, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	node->type = N_VARASN;
	
	node->exp.varAsn.varName = varName;
	node->exp.varAsn.binExp = binExp;
	
	node->parentLexer = parentParser->parentLexer;
	node->parentToken = varName;
	node->pos_start = varName->pos_start;
	node->pos_end = binExp->pos_end;

	node->show = &node__show;
	return node;
}

struct node* node__init__varAcc(struct token* varName, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	node->type = N_VARACC;

	node->exp.varAsn.varName = varName;
	
	node->parentLexer = parentParser->parentLexer;
	node->parentToken = varName;
	node->pos_start = varName->pos_start;
	node->pos_end = varName->pos_end;

	node->show = &node__show;

	return node;
}

struct node* node__init__ifNode(struct node** conditions, struct nodeList** bodies, int len, struct node* elseCon, struct nodeList* elseBody, struct parser* parentParser ) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	node->type = N_IFNODE;

	node->exp.ifNode.conditions = conditions;
	node->exp.ifNode.bodies = bodies;
	node->exp.ifNode.len = len;
	node->exp.ifNode.elseCond = elseCon;
	node->exp.ifNode.elseBody = elseBody;

	node->parentLexer = parentParser->parentLexer;
	node->parentToken = parentParser->currentToken;
	node->pos_start = node->exp.ifNode.conditions[0]->pos_start;
	node->pos_end = node->exp.ifNode.bodies[len - 1]->items[node->exp.ifNode.bodies[len-1]->len - 1]->pos_end;
	if(node->exp.ifNode.elseBody != NULL && node->exp.ifNode.elseCond != NULL)
		node->pos_end = node->exp.ifNode.elseCond->pos_end;

	node->show = &node__show;
	return node;
}

struct node* node__init__nWhile(struct node* condition, struct nodeList* body, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	node->type = N_WHILE;

	node->exp.nWhile.condition = condition;
	node->exp.nWhile.body = body;

	node->parentLexer = parentParser->parentLexer;
	node->parentToken = parentParser->currentToken;
	node->pos_start = node->exp.nWhile.condition->pos_start;
	node->pos_end = node->exp.nWhile.body->items[node->exp.nWhile.body->len - 1]->pos_end;

	node->show = &node__show;
	return node;
}
struct node* node__init__funcDef(struct token* name, struct token** args, int argNum, struct nodeList* body, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	node->type = N_FUNDEF;

	node->exp.funcDef.name = name;
	node->exp.funcDef.args = args; 
	node->exp.funcDef.argNum = argNum; 
	node->exp.funcDef.body = body;

	node->parentLexer = parentParser->parentLexer;
	node->parentToken = parentParser->currentToken;
	node->pos_start = node->exp.funcDef.name->pos_start;
	node->pos_end = node->exp.funcDef.body->items[node->exp.funcDef.body->len - 1]->pos_end;
	
	node->show = &node__show;
	return node;
}
struct node* node__init__funcCall(struct token* token, struct node** args, int argNum, struct parser* parentParser) {
	struct node* node = (struct node*)calloc(1, sizeof(struct node));
	node->type = N_FUNCAL;

	node->exp.funcCall.name = token;
	node->exp.funcCall.args = args;
	node->exp.funcCall.argNum = argNum;

	node->parentLexer = parentParser->parentLexer;
	node->parentToken = parentParser->currentToken;
	node->pos_start = node->exp.funcCall.name->pos_start;
	node->pos_end = parentParser->currentToken->pos_end;
	
	node->show = &node__show;
	return node;
}
//notice, the tokens in the parse tree are not copied from the tokenList, 
//meaning we can't free the tokenList;
