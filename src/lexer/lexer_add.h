#ifndef LEXER_ADD_H
#define LEXER_ADD_H
#include "../inc/lexer.h"

struct token** reallocTokenList(struct lexer* lexer);

struct token** addSimpleToken(struct lexer* lexer, enum tokenType initType);
struct token** addNumber(struct lexer* lexer);
struct token** makeIdentifier(struct lexer* lexer);
struct token** makeEquals(struct lexer* lexer);
struct token** makeNotEquals(struct lexer* lexer);
struct token** makeGreater(struct lexer* lexer);
struct token** makeLesser(struct lexer* lexer);
struct token** makeArrow(struct lexer* lexer);

#endif
