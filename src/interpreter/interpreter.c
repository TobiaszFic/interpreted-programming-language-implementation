#include "../inc/interpreter.h"
#include "../error/error.h"
#include "number.h"
#include "context.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static struct number* visit__node(struct node* node, struct context* context);
static struct number* interpreter__subInterpret(struct node* node, struct context* parentContext); 
static struct number* visit__nodeList(struct nodeList* nodes, struct context* context);

static struct number* visit__numNode(struct node* node, struct context* context) {
	return number__initFromToken(node->exp.intExp, node, context);
}

static struct number* visit__binNode(struct node* node, struct context* context) {
	
	struct number* left = visit__node(node->exp.binExp.left, context);
	struct number* right = visit__node(node->exp.binExp.right, context);
	
	switch(node->exp.binExp.op->type) {
		case T_PLUS:
			return left->added_to(left, right, context);
		case T_MINUS:
			return left->subbed_by(left, right, context);
		case T_MUL:
			return left->multiplied_by(left, right, context);

		case T_DIV:
			return left->divided_by(left, right, context);
		case T_MOD:
			return number__mod(left, right, context);
		case T_EQEQ:
			return number__eqeq(left, right, context);
		case T_NEQ:
			return number__neq(left, right, context);
		case T_GT:
			return number__gt(left, right, context);
		case T_GTE:
			return number__gte(left, right, context);
		case T_LT:
			return number__lt(left, right, context);
		case T_LTE:
			return number__lte(left, right, context);
		case T_KEYWORD:
			if(!strcmp(node->exp.binExp.op->value.id_str, "and"))
				return number__and(left, right, context);
			if(!strcmp(node->exp.binExp.op->value.id_str, "or"))
				return number__or(left, right, context);
			if(!strcmp(node->exp.binExp.op->value.id_str, "xor"))
				return number__xor(left, right, context);

		default:
			error_runtime("Invalid binary operator", context, node);
			return NULL;
	}
}

static struct number* visit__unaNode(struct node* node, struct context* context) {
	struct number* result = visit__node(node->exp.unaExp.operand, context);
	switch(node->exp.unaExp.op->type) {
		case T_MINUS:
			if(result->type == T_INT)
				result->value.i *= -1;
			if(result->type == T_FLOAT)
				result->value.f *= -1;
			break;
		case T_PLUS:
			break;
		default:
			error_runtime("Invalid unary operator", context, node);
	}
	return result;
}

static struct number* visit__varAsn(struct node* node, struct context* context) {
	struct number* val = visit__node(node->exp.varAsn.binExp, context); 
	context->symbolTable =context->symbolTable->add(context, node->exp.varAsn.varName, val);	
	return val;
}

static struct number* visit__varAcc(struct node* node, struct context* context) {
	return context->symbolTable->get(context, node->exp.varAcc.varName, node);
}

static bool isTrue(struct number* number) {
	if((number->type == T_FLOAT && number->value.f) || (number->type == T_INT && number->value.i))
			return true;
	return false;
}

static struct number* visit__ifNode(struct node* node, struct context* context) {
	struct number* condition_res = NULL;
	for(int i = 0; i<node->exp.ifNode.len; i++) {
		condition_res = interpreter__subInterpret(node->exp.ifNode.conditions[i], context);
		if(isTrue(condition_res))
			return visit__nodeList(node->exp.ifNode.bodies[i], context);
	}
	if(node->exp.ifNode.elseBody != NULL)
		return visit__nodeList(node->exp.ifNode.elseBody, context);

	return NULL;
}

struct number* visit__whileNode(struct node* node, struct context* context) {
	while(1) {
		if(!isTrue(interpreter__subInterpret(node->exp.nWhile.condition, context))) {
			break;
		}
		visit__nodeList(node->exp.nWhile.body, context);
	}
	return NULL;
}
struct number* visit__funDefNode(struct node* node, struct context* context) {
	context->symbolTable = context->symbolTable->addFunc(context, node);
	return NULL;
}

struct number* visit__funCallNode(struct node* node, struct context* context) {
	struct node* definition = context->symbolTable->getFunc(context, node);
	if(definition->exp.funcDef.argNum != node->exp.funcCall.argNum) 
		error_runtime("Number of arguments in function call does not match the number of arguments in the function definition", context, node);
	struct context* funcContext = context__init(definition->exp.funcDef.name->value.id_str, context, definition->pos_start);
	for(int i = 0; i<definition->exp.funcDef.argNum; i++)
		funcContext->symbolTable->add(funcContext, definition->exp.funcDef.args[i], visit__node(node->exp.funcCall.args[i], context));
	return visit__nodeList(definition->exp.funcDef.body, funcContext);
}

static struct number* visit__node(struct node* node, struct context* context) {
	struct number* result;
	switch (node->type) {
	case N_INTEXP:
		result = visit__numNode(node, context);
		break;
	case N_BINEXP:
		result = visit__binNode(node, context);
		break;
	case N_UNAEXP:
		result = visit__unaNode(node, context);
		break;
	case N_VARASN:
		result = visit__varAsn(node, context);
		break;
	case N_VARACC:
		result = visit__varAcc(node, context);
		break;
	case N_IFNODE:
		result = visit__ifNode(node, context);
		break;
	case N_WHILE:
		result = visit__whileNode(node, context);
		break;
	case N_FUNDEF:
		result = visit__funDefNode(node, context);
		break;
	case N_FUNCAL:
		result = visit__funCallNode(node, context);
		break;
	default:
		result = NULL;
		error_runtime("No visit method for encountered node type declared.", context, node);
	}
	return result;
}
static struct number* visit__nodeList(struct nodeList* nodes, struct context* context) {
	struct number* returnVal = NULL;
	for (int i = 0; i< nodes->len; i++)
		returnVal = visit__node(nodes->items[i], context);
	return returnVal;
}

static struct number* interpreter__interpret(struct nodeList* nodes) {
	static struct context* context = NULL;
	if(context == NULL){
		context = context__init("program", NULL, 0); 
	}
	return visit__nodeList(nodes, context);
}

static struct number* interpreter__subInterpret(struct node* node, struct context* parentContext) {
	struct context* context = context__init("sub-context", parentContext, 0); 
	return visit__node(node, context);
}

struct interpreter* interpreter__init() {
	struct interpreter* interpreter = (struct interpreter*)calloc(1, sizeof(struct interpreter));
	interpreter->interpret = &interpreter__interpret;
	return interpreter;
}

