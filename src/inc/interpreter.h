#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../inc/node.h"
#include "../interpreter/number.h"
#include "../interpreter/context.h"




struct interpreter {
	//methods
	struct number* (*interpret)(struct node*);
};
struct interpreter* interpreter__init();

#endif

