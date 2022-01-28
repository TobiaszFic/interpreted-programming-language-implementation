#ifndef GRAMMAR_H
#define GRAMMAR_H
#include "../../inc/parser.h"

/*
 *File constains all grammar methods for the parser structure
 */

//Arithmetic
struct node* parser__factor (struct parser* parser); 
struct node* parser__term (struct parser* parser);
struct node* parser__arithmeticExp (struct parser* parser);
struct node* parser__comparison (struct parser* parser);
struct node* parser__expression(struct parser* parser);

#endif

