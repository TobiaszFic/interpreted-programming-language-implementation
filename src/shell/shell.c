#include "shell.h"
#include "../inc/lexer.h"
#include "../inc/parser.h"
#include "../inc/interpreter.h"
#include "../interpreter/context.h"

#include <stdio.h>

void launchShell() {
	static struct lexer* lexer = NULL;
	static struct parser* parser = NULL;
	static struct interpreter* interpreter = NULL;
	if(lexer != NULL) {
		lexer__cleanup(lexer);
	}
	if(parser != NULL) {
		parser__cleanup(parser);
	}

	while(1) {
		printf("\n\t>");
		fflush(stdin);
		lexer = lexer__init(stdin);
		lexer->lex(lexer);

		parser = parser__init(lexer);
		parser->parse(parser);

		interpreter = interpreter__init();
		struct number* result = interpreter->interpret(parser->AST);
		
		//printf("\n\nINTERPRETER: ");
		putchar('\n');
		if(result != NULL)
			result->show(result);	
		else 
			printf(" Result is (null)");
	}
}
