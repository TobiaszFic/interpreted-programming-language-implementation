#include "lexer_add.h"
#include "../error/error.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct token** reallocTokenList(struct lexer* lexer) {
	lexer->tokenList.length += 1;
	lexer->tokenList.items = (struct token**)realloc(lexer->tokenList.items, (lexer->tokenList.length+1)*sizeof(struct token*));
	if (lexer->tokenList.items == NULL) {
		error_memory("lexer_add.c", "addSimpleToken", "lexer->tokenList.items");
	}
	return lexer->tokenList.items;
}

//helper functions
struct token** addSimpleToken(struct lexer* lexer, enum tokenType initType) {
	lexer->tokenList.items = reallocTokenList(lexer);
	lexer->tokenList.items[lexer->tokenList.length-1] = token__init(initType, lexer->lineNumber, lexer->position, lexer->position + 1);
	lexer->advance(lexer);
	return lexer->tokenList.items;
}

struct token** addNumber(struct lexer* lexer) {
	//making the string to be validated
	char* str = (char*)calloc(2, sizeof(char));
	if (str == NULL) {
		error_memory("lexer_add.c", "addNumber", "str");
	}

	str[0] = '\0';
	int addr = 0;
	while((lexer->c>=48 && lexer->c<=57) || lexer->c == '.') {
		str[addr] = lexer->c;
		str[addr+1] = '\0';
		str = realloc(str, (addr+3)*sizeof(char));
		if (str == NULL) {
			error_memory("lexer_add.c", "addNumber", "str");
		}

		addr++;
		lexer->advance(lexer);
	}
	//checking validity
	bool commaFlag = false;
	bool invalid = false;
    for(size_t i = 0; i < strlen(str) ;i++){
		if((str[i]<48 || str[i]>57) && str[i] != '.') //can be compounded into one if but then messy 
            invalid = true;
        if(str[i] == '.' && i == 0)         
            invalid = true;
        if(str[i] == '.' && commaFlag == true)   
            invalid = true;                      
        if(str[i] == '.' && commaFlag == false)  
            commaFlag = true;                 
	}
	if(invalid == true) {
		error_lexer("invalid number", lexer);
	}
	//adding the int/float token
	lexer->tokenList.items = reallocTokenList(lexer);
	//storing converted value into proper union spaces
	if(commaFlag == true) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_FLOAT, lexer->lineNumber, lexer->position - strlen(str), lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.f = atof(str);
	}
	if(commaFlag == false) {
		lexer->tokenList.items[lexer->tokenList.length - 1] = token__init(T_INT, lexer->lineNumber, lexer->position - strlen(str) , lexer->position);
		lexer->tokenList.items[lexer->tokenList.length-1]->value.i = atoi(str);
	}

	free(str);
    return lexer->tokenList.items;
}

