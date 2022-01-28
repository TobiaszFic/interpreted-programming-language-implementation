
#include "lexer_add.h"
#include "../error/error.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
struct token** makeIdentifier(struct lexer* lexer) {
	//making the string to be validated
	char* str = (char*)calloc(2, sizeof(char));
	if (str == NULL) {
		error_memory("lexer_add.c", "addNumber", "str");
	}
	str[0] = '\0';
	int addr = 0;
	while((lexer->c>=48 && lexer->c<=57) || (lexer->c >= 65 && lexer->c <= 90) || (lexer->c >= 97 && lexer->c <= 122) || lexer->c == '_') {//letters or numbers
		str[addr] = lexer->c;
		str[addr+1] = '\0';
		str = (char*)realloc(str, (addr+3)*sizeof(char));
		if (str == NULL) {
			error_memory("lexer_add.c", "addNumber", "str");
		}
		addr++;
		lexer->advance(lexer);
	}
	lexer->tokenList.items = reallocTokenList(lexer);

	if(!strcmp(str, "var")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "and")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "or")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "xor")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "mod")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_MOD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "if")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "then")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "elif")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "else")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "while")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "do")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	} else if(!strcmp(str, "func")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_KEYWORD, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;

	} else if(!strcmp(str, "true")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_INT, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.i = 1;
	} else if(!strcmp(str, "false")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_INT, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.i = 0;
	} else if(!strcmp(str, "pi")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_FLOAT, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.f = 3.14159;
	} else if(!strcmp(str, "e")) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_FLOAT, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.f = 2.71828;

	} else {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_IDENTIFIER, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.id_str = str;
	}
	return lexer->tokenList.items;
}
