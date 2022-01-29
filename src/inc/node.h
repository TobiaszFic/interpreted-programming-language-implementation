#ifndef NODE_H
#define NODE_H
#include "token.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
//ABSOLUTE TYPE-SAFETY PAIN IN THE ASS
//
//binary operation node


struct node {
	enum {
		N_INTEXP,
		N_BINEXP,
		N_COMEXP,
		N_UNAEXP,
		N_VARASN,
		N_VARACC,
		N_IFNODE,
		N_WHILE,
		N_FUNDEF,
		N_FUNCAL
	} type;

	union {
		//int or float
		struct token* intExp;
		
		//binary operation
		struct {
			struct token* op;
			struct node* left;
			struct node* right;
		} binExp;

		//unary operation
		struct {
			struct token* op;
			struct node* operand;
		} unaExp;
		struct {
			struct token* varName;
			struct node* binExp;
		} varAsn; //variable assignment
		struct {
			struct token* varName;
		} varAcc; //variable access
		struct {
			struct node** conditions;
			struct nodeList** bodies;
			int len;

			struct node* elseCond;
			struct nodeList* elseBody;
		} ifNode;
		struct {
			struct node* condition;
			struct nodeList* body;
		} nWhile;
		struct {
			struct token* name;
			struct token** args;
			int argNum;
			struct nodeList* body;
		} funcDef;
		struct {
			struct token* name;
			struct node** args;
			int argNum; //can access via nodeToCall but gets too bloated
		} funcCall;
	} exp; 
	struct lexer* parentLexer;
	struct token* parentToken;
	int pos_start;
	int pos_end;


	void (*show)(struct node*);
};
struct parser;

struct node* node__init__intExp(struct token*, struct parser* parentParser);
struct node* node__init__binExp(struct token* op, struct node* left, struct node* right, struct parser* parentParser);
struct node* node__init__unaExp(struct token* op, struct node* operand, struct parser* parentParser);
struct node* node__init__varAsn(struct token* varName, struct node* binExp, struct parser* parentParser);
struct node* node__init__varAcc(struct token* varName, struct parser* parentParser);
struct node* node__init__ifNode(struct node** conditions, struct nodeList** bodies, int len, struct node* elseCon, struct nodeList* elseExp, struct parser* parentParser);
struct node* node__init__nWhile(struct node* condition, struct nodeList* body, struct parser* parentParser);
struct node* node__init__funcDef(struct token* name, struct token** args, int argNum, struct nodeList* body, struct parser* parentParser);
struct node* node__init__funcCall(struct token* token, struct node** args, int argNum, struct parser* parentParser);




#endif

