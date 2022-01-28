#include "../error/error.h"
#include "number.h"
#include <stdlib.h>
#include <stdio.h>

//ADDITION-------------------------------------------------------------------------------
static struct number* float_added_to_float(struct number* number, struct number* other, struct context* context) {
	return number__init__float(number->value.f + other->value.f, number->parentNode, context);
}
	
static struct number* float_added_to_int(struct number* number, struct number* other, struct context* context){
	return number__init__float(number->value.f + other->value.i, number->parentNode, context);
}

static struct number* int_added_to_int(struct number* number, struct number* other, struct context* context){
	return number__init__int(number->value.i + other->value.i, number->parentNode, context);
}

static struct number* added_to (struct number* number, struct number* other, struct context* context) {

	struct number* result;
	if(number->type == T_FLOAT && other->type == T_FLOAT)
		result = float_added_to_float(number, other, context);
	if(number->type == T_FLOAT && other->type == T_INT)
		result = float_added_to_int(number, other, context);
	if(number->type == T_INT && other->type == T_FLOAT)
		result = float_added_to_int(other, number, context);
	if(number->type == T_INT && other->type == T_INT)
		result = int_added_to_int(number, other, context);

	return result;
}

//SUBTRACTION-----------------------------------------------------------------------
static struct number* float_subbed_by_float(struct number* number, struct number* other, struct context* context) {
	return number__init__float(number->value.f - other->value.f, number->parentNode, context);
}

static struct number* float_subbed_by_int(struct number* number, struct number* other, struct context* context){
	return number__init__float(number->value.f - other->value.i, number->parentNode, context);
}

static struct number* int_subbed_by_float(struct number* number, struct number* other, struct context* context){
	return number__init__float(number->value.i - other->value.f, number->parentNode, context);
}

static struct number* int_subbed_by_int(struct number* number, struct number* other, struct context* context){
	return number__init__int(number->value.i - other->value.i, number->parentNode, context);
}

static struct number* subbed_by (struct number* number, struct number* other, struct context* context) { 
	struct number* result;
	if(number->type == T_FLOAT && other->type == T_FLOAT)
		result = float_subbed_by_float(number, other, context);
	if(number->type == T_FLOAT && other->type == T_INT)
		result = float_subbed_by_int(number, other, context);
	if(number->type == T_INT && other->type == T_FLOAT)
		result = int_subbed_by_float(number, other, context);
	if(number->type == T_INT && other->type == T_INT)
		result = int_subbed_by_int(number, other, context);
	return result;
}

//MULTIPLICATION--------------------------------------------------------------------------
static struct number* float_multiplied_by_float(struct number* number, struct number* other, struct context* context) {
	return number__init__float(number->value.f * other->value.f, number->parentNode, context);
}

static struct number* float_multiplied_by_int(struct number* number, struct number* other, struct context* context){
	return number__init__float(number->value.f * other->value.i, number->parentNode, context);
}

static struct number* int_multiplied_by_int(struct number* number, struct number* other, struct context* context){
	return number__init__int(number->value.i * other->value.i, number->parentNode, context);
}

static struct number* multiplied_by (struct number* number, struct number* other, struct context* context) {
	struct number* result;
	if(number->type == T_FLOAT && other->type == T_FLOAT)
		result = float_multiplied_by_float(number, other, context);
	if(number->type == T_FLOAT && other->type == T_INT)
		result = float_multiplied_by_int(number, other, context);
	if(number->type == T_INT && other->type == T_FLOAT)
		result = float_multiplied_by_int(other, number, context);
	if(number->type == T_INT && other->type == T_INT)
		result = int_multiplied_by_int(number, other, context);
	return result;
}

//MULTIPLICATION--------------------------------------------------------------------------
static struct number* float_divided_by_float(struct number* number, struct number* other, struct context* context) {
	if(other->value.f == 0) {
		error_runtime("division by zero", context, other->parentNode);
		return NULL;
	}
	return number__init__float(number->value.f / other->value.f, number->parentNode, context);
}
static struct number* float_divided_by_int(struct number* number, struct number* other, struct context* context){
	if(other->value.f == 0) {
		error_runtime("division by zero", context, other->parentNode);
		return NULL;
	}
	return number__init__float(number->value.f / other->value.i, number->parentNode, context);
}
static struct number* int_divided_by_float(struct number* number, struct number* other, struct context* context){
	if(other->value.f == 0) {
		error_runtime("division by zero", context, other->parentNode);
		return NULL;
	}
	return number__init__float(number->value.i / other->value.f, number->parentNode, context);
}
static struct number* int_divided_by_int(struct number* number, struct number* other, struct context* context){
	if(other->value.f == 0) {
		error_runtime("division by zero", context, other->parentNode);
		return NULL;
	}
	return number__init__int(number->value.i / other->value.i, number->parentNode, context);
}
static struct number* divided_by (struct number* number, struct number* other, struct context* context) {
	struct number* result;
	if(number->type == T_FLOAT && other->type == T_FLOAT)
		result = float_divided_by_float(number, other, context);
	if(number->type == T_FLOAT && other->type == T_INT)
		result = float_divided_by_int(number, other, context);
	if(number->type == T_INT && other->type == T_FLOAT)
		result = int_divided_by_float(number, other, context);
	if(number->type == T_INT && other->type == T_INT)
		result = int_divided_by_int(number, other, context);
	return result;
}

struct number* number__mod(struct number* number, struct number* other, struct context* context) {
	if(number->type != T_INT || other->type != T_INT)
		error_runtime("mod function expects two integers and input", context, number->parentNode);
	return number__init__int((number->value.i % other->value.i), number->parentNode, context);
}

struct number* number__eqeq(struct number* number, struct number* other, struct context* context) {
	
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a==b), number->parentNode, context);

}
struct number* number__neq(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a!=b), number->parentNode, context);

}
struct number* number__gt(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a>b), number->parentNode, context);

}
struct number* number__gte(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a>=b), number->parentNode, context);
}
struct number* number__lt(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a<b), number->parentNode, context);
}
struct number* number__lte(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a<=b), number->parentNode, context);
}
struct number* number__and(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a&&b), number->parentNode, context);
}
struct number* number__or(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int((a||b), number->parentNode, context);
}
struct number* number__xor(struct number* number, struct number* other, struct context* context) {
	float a, b;
	if(number->type == T_INT)
		a = number->value.i;
	if(number->type == T_FLOAT)
		a = number->value.f;
	if(other->type == T_INT)
		b = other->value.i;
	if(other->type == T_FLOAT)
		b = other->value.f;
	return number__init__int(((a||b) && !(a&&b)), number->parentNode, context);
}


void number__show(struct number* number) {
	if(number == NULL)
		printf("\nprocess returned (null)\n");
	if(number->type == T_FLOAT)
		printf("%f", number->value.f);
	if(number->type == T_INT)
		printf("%d", number->value.i);
}


//constructors
struct number* number__init__int(int intVal, struct node* node, struct context* context) {
	struct number* number = (struct number*)calloc(1,sizeof(struct number));
		number->type = T_INT;
		number->value.i = intVal; 
	number->context = context;
	number->parentNode = node;

	number->added_to = &added_to;
	number->subbed_by = &subbed_by;
	number->multiplied_by = &multiplied_by;
	number->divided_by = &divided_by;
	number->show = &number__show;
	
	return number;
}

struct number* number__init__float(float floatVal, struct node* node, struct context* context) {
	struct number* number = (struct number*)calloc(1,sizeof(struct number));
		number->type = T_FLOAT;
		number->value.f = floatVal; 
	number->context = context;
	number->parentNode = node;

	number->added_to = &added_to;
	number->subbed_by = &subbed_by;
	number->multiplied_by = &multiplied_by;
	number->divided_by = &divided_by;
	number->show = &number__show;
	
	return number;
}

struct number* number__initFromToken(struct token* token, struct node* node, struct context* context) {
	struct number* number = (struct number*)calloc(1,sizeof(struct number));

	if(token->type == T_INT) {
		number->type = T_INT;
		number->value.i = token->value.i; 
	}
	if(token->type == T_FLOAT) {
		number->type = T_FLOAT;
		number->value.f = token->value.f; 
	}
	number->context = context;
	number->parentNode = node;

	number->added_to = &added_to;
	number->subbed_by = &subbed_by;
	number->multiplied_by = &multiplied_by;
	number->divided_by = &divided_by;


	number->show = &number__show;

	return number;
}
