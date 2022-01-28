#include "grammar.h"
#include "../../error/error.h"

#include <stdlib.h>
#include <string.h>
struct node* parser__funcDef(struct parser* parser) {
	parser->advance(parser); 
	if(parser->currentToken->type != T_IDENTIFIER)
		error_parser("invalid function name, expected identifier.", parser);
	struct token* name = parser->currentToken;
	parser->advance(parser);

	if(parser->currentToken->type != T_LPAREN)
		error_parser("expecting \'(\' in function declaration.", parser);
	parser->advance(parser);

	struct token** args = (struct token**)calloc(1, sizeof(struct token*));
	int argNum = 0;
	while(1) {
		args = (struct token**)realloc(args, (argNum + 2)*sizeof(struct token*));
		if(parser->currentToken->type != T_IDENTIFIER)
			error_parser("invalid argument in function declaration, expected identifier.", parser);
		args[argNum] = parser->currentToken;
		argNum++;
		parser->advance(parser);
		if(parser->currentToken->type == T_RPAREN)
			break;
		if(parser->currentToken->type != T_COMMA)
			error_parser("expecting \',\'", parser);
		parser->advance(parser);
	}
	parser->advance(parser);
	if(parser->currentToken->type != T_ARROW)
		error_parser("expecting \"->\" in function declaration.", parser);
	parser->advance(parser);
	struct node* body = parser__expression(parser);
	return node__init__funcDef(name, args, argNum, body, parser);
}

struct node* parser__whileExp (struct parser* parser) { 
	struct node* condition;
	struct node* body;
	parser->advance(parser);
	condition = parser__expression(parser);
	if(parser->currentToken->type != T_KEYWORD || strcmp(parser->currentToken->value.id_str,"do"))
		error_parser("expecting \"do\" keyword after \"while\" condition.", parser);
	parser->advance(parser);
	body = parser__expression(parser);
	return node__init__nWhile(condition, body, parser);
}	

struct node* parser__ifExp (struct parser* parser){
	struct node** conds = (struct node**)calloc(2, sizeof(struct node*));
	struct node** exprs = (struct node**)calloc(2, sizeof(struct node*));
	int len = 1;
	struct node* elseCond = NULL;
	struct node* elseExp = NULL;
	parser->advance(parser);

	conds[0] = parser__expression(parser);
	if(parser->currentToken->type != T_KEYWORD || strcmp(parser->currentToken->value.id_str,"then"))
		error_parser("expecting \"then\" keyword after \"if\" condition", parser);
	parser->advance(parser);
	exprs[0] = parser__expression(parser);

	while(parser->currentToken->type == T_KEYWORD && !strcmp(parser->currentToken->value.id_str,"elif")) {
		conds = (struct node**)realloc(conds, (len+2)*sizeof(struct node*));
		exprs = (struct node**)realloc(exprs, (len+2)*sizeof(struct node*));
		parser->advance(parser);
		conds[len] = parser__expression(parser);
		if(parser->currentToken->type != T_KEYWORD || strcmp(parser->currentToken->value.id_str,"then"))
			error_parser("expecting \"then\" keyword after \"if\" condition", parser);
		parser->advance(parser);
		exprs[len] = parser__expression(parser);
		len++;
	}
	if(parser->currentToken->type == T_KEYWORD && !strcmp(parser->currentToken->value.id_str,"else")) {
		parser->advance(parser);
		elseExp = parser__expression(parser);
	}
	return node__init__ifNode(conds, exprs, len, elseCond, elseExp, parser);
}


struct node* parser__atom (struct parser* parser) {
	if(parser->currentToken->type == T_LPAREN) {
		parser->advance(parser);
		struct node* expression = parser->exp(parser);
		if(parser->currentToken->type != T_RPAREN) {
			error_parser("missing\")\"", parser);
		}
		parser->advance(parser);
		return expression;
	} else if (parser->currentToken->type == T_IDENTIFIER){
		struct node* var = node__init__varAcc(parser->currentToken, parser);
		parser->advance(parser);
		return var;
	} else if (parser->currentToken->type == T_KEYWORD && !strcmp(parser->currentToken->value.id_str, "if")){
		return parser__ifExp(parser); 
	} else if (parser->currentToken->type == T_KEYWORD && !strcmp(parser->currentToken->value.id_str, "while")){
		return parser__whileExp(parser); 
	} else if (parser->currentToken->type == T_KEYWORD && !strcmp(parser->currentToken->value.id_str, "func")){
		return parser__funcDef(parser); 
	} else if(parser->currentToken->type != T_INT && parser->currentToken->type != T_FLOAT ) {
		error_parser("Expecting numerical value", parser);
	}

	struct node* intNode = node__init__intExp(parser->currentToken, parser);	
	parser->advance(parser);
	return intNode;
}

struct node* parser__call(struct parser* parser) {
	struct node* node = parser__atom(parser);//if paren is next, this becomes the node called.	
	if(parser->currentToken->type == T_LPAREN && node->type == N_VARACC) {
		struct node** args = (struct node**)calloc(2, sizeof(struct node*));
		int argNum = 0;
			parser->advance(parser);
		while(1) {
			if(parser->currentToken->type == T_RPAREN)
				break;
			if(parser->currentToken->type == T_COMMA)
				parser->advance(parser);
			args = (struct node**)realloc(args, (argNum + 3)*sizeof(struct node*));
			args[argNum] = parser__expression(parser);
			argNum++;
			if(parser->currentToken->type == T_RPAREN)
				break;
			if(parser->currentToken->type != T_COMMA)
				error_parser("expecting \',\' or \')\' in function call",  parser );
		}
		parser->advance(parser);
		fflush(stdout);
		return node__init__funcCall(node->exp.varAcc.varName, args, argNum, parser);
	}
	return node;
}


struct node* parser__factor (struct parser* parser) {
	//unary op
	if(parser->currentToken->type == T_PLUS || parser->currentToken->type == T_MINUS) {
		struct token* opToken = parser->currentToken;
		parser->advance(parser);
		struct node* operand = parser->factor(parser);
		return node__init__unaExp(opToken, operand, parser);
	}
	return parser__call(parser);
}
//term
struct node* parser__term (struct parser* parser) {
	struct node* leftNode = parser->factor(parser);
	struct node* leftNode_Temp;
	struct node* rightNode;	
	struct token* opToken;
		
	while(parser->currentToken->type == T_DIV || parser->currentToken->type == T_MUL || parser->currentToken->type == T_MOD) {
		opToken = parser->currentToken;
		parser->advance(parser);
		rightNode = parser->factor(parser);
		leftNode_Temp = leftNode;
		leftNode = node__init__binExp(opToken, leftNode_Temp, rightNode, parser);
	}

	return leftNode;
}

struct node* parser__arithmeticExp (struct parser* parser) {
	struct node* leftNode = parser->term(parser);
	struct node* leftNode_Temp;
	struct node* rightNode;	
	struct token* opToken;
	while(parser->currentToken->type == T_PLUS || parser->currentToken->type == T_MINUS) {
		opToken = parser->currentToken;
		parser->advance(parser);
		rightNode = parser->term(parser);
		leftNode_Temp = leftNode;
		leftNode = node__init__binExp(opToken, leftNode_Temp, rightNode, parser);
	}
	return leftNode;
}

struct node* parser__comparison (struct parser* parser) {
	if(parser->currentToken->type == T_NOT){
		parser->advance(parser);
		parser->comp(parser);
	}
	struct node* leftNode = parser->arit(parser);
	struct node* leftNode_Temp;
	struct node* rightNode;	
	struct token* opToken;
	while(parser->currentToken->type == T_EQEQ || parser->currentToken->type == T_GT || parser->currentToken->type == T_GTE|| parser->currentToken->type == T_LT || parser->currentToken->type == T_LTE|| parser->currentToken->type == T_NEQ) {
		opToken = parser->currentToken;
		parser->advance(parser);
		rightNode = parser->arit(parser);
		leftNode_Temp = leftNode;
		leftNode = node__init__binExp(opToken, leftNode_Temp, rightNode, parser);
	}
	return leftNode;
}

//expression
struct node* parser__expression(struct parser* parser) {
	if(parser->currentToken->type == T_KEYWORD && !strcmp(parser->currentToken->value.id_str, "var")) {
		parser->advance(parser);
		if(parser->currentToken->type != T_IDENTIFIER)
			error_parser("expected identifier", parser);
		struct token* var_name = parser->currentToken;
		parser->advance(parser);
		if(parser->currentToken->type != T_EQ)
			error_parser("expected \'=\'", parser);
		parser->advance(parser);
		struct node* expr = parser__expression(parser);
		struct node* varAssignNode = node__init__varAsn(var_name, expr, parser);
		return varAssignNode;	
	}

	struct node* leftNode = parser->comp(parser);
	struct node* leftNode_Temp;
	struct node* rightNode;	
	struct token* opToken;
	while(parser->currentToken->type == T_KEYWORD && (!strcmp(parser->currentToken->value.id_str, "and") || !strcmp(parser->currentToken->value.id_str, "or") || !strcmp(parser->currentToken->value.id_str, "xor"))) {
		opToken = parser->currentToken;
		parser->advance(parser);
		rightNode = parser->comp(parser);
		leftNode_Temp = leftNode;
		leftNode = node__init__binExp(opToken, leftNode_Temp, rightNode, parser);
	}
	return leftNode;
}
