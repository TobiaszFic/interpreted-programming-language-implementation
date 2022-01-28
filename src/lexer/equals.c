#include "lexer_add.h"
 

struct token** makeEquals(struct lexer* lexer) {
	enum tokenType t_type = T_EQ;
	lexer->advance(lexer);
	if(lexer->c == '=') {
		t_type = T_EQEQ;
		lexer->advance(lexer);
	}
	lexer->tokenList.items = reallocTokenList(lexer);
	lexer->tokenList.items[lexer->tokenList.length-1] = token__init(t_type, lexer->lineNumber, lexer->position-1, lexer->position + 1);
	return lexer->tokenList.items;
}
struct token** makeNotEquals(struct lexer* lexer) {
	enum tokenType t_type = T_NOT;
	lexer->advance(lexer);
	if(lexer->c == '=') {
		t_type = T_NEQ;
		lexer->advance(lexer);
	}
	lexer->tokenList.items = reallocTokenList(lexer);
	lexer->tokenList.items[lexer->tokenList.length-1] = token__init(t_type, lexer->lineNumber, lexer->position-1, lexer->position + 1);
	return lexer->tokenList.items;

}
struct token** makeGreater(struct lexer* lexer) {
	enum tokenType t_type = T_GT;
	lexer->advance(lexer);
	if(lexer->c == '='){
		t_type = T_GTE;
		lexer->advance(lexer);
	}
	lexer->tokenList.items = reallocTokenList(lexer);
	lexer->tokenList.items[lexer->tokenList.length-1] = token__init(t_type, lexer->lineNumber, lexer->position-1, lexer->position + 1);
	return lexer->tokenList.items;

}

struct token** makeLesser(struct lexer* lexer) {
	enum tokenType t_type = T_LT;
	lexer->advance(lexer);
	if(lexer->c == '=') {
		t_type = T_LTE;
		lexer->advance(lexer);
	}
	lexer->tokenList.items = reallocTokenList(lexer);
	lexer->tokenList.items[lexer->tokenList.length-1] = token__init(t_type, lexer->lineNumber, lexer->position-1, lexer->position + 1);
	return lexer->tokenList.items;
}
struct token** makeArrow(struct lexer* lexer) {
	enum tokenType t_type = T_MINUS;
	lexer->advance(lexer);
	if(lexer->c == '>') {
		t_type = T_ARROW;
		lexer->advance(lexer);
	}
	lexer->tokenList.items = reallocTokenList(lexer);
	lexer->tokenList.items[lexer->tokenList.length-1] = token__init(t_type, lexer->lineNumber, lexer->position-1, lexer->position + 1);
	return lexer->tokenList.items;
}
