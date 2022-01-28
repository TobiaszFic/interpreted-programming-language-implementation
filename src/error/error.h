#ifndef ERROR_H
#define ERROR_H

#include "../shell/run.h"
#include "../inc/token.h"
#include "../inc/parser.h"
#include "../interpreter/context.h"


void error__(FILE* fp);
void error_lexer(char* eMessage, struct lexer* eLexer);
void error_parser(char* eMessage, struct parser* eParser);
void error_runtime(char* eMessage, struct context* context, struct node* node);
void error_memory(char* fileName, char* functionName, char* varName);

void warning_lexer(char* eMessage, struct lexer* eLexer);
void warning_parser(char* eMessage, struct parser* eParser);

#endif
