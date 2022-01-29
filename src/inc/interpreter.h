#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../inc/parser.h"
#include "../inc/node.h"
#include "../interpreter/number.h"
#include "../interpreter/context.h"




struct interpreter {
	//methods
	struct number* (*interpret)(struct nodeList*);
};
struct interpreter* interpreter__init();

#endif

