#ifndef NUMBER_H
#define NUMBER_H
#include "../inc/node.h"
struct context;
struct number {

	enum tokenType type;

	union {
		int i;
		float f;
	}value;

	struct number* (*added_to)(struct number*, struct number*, struct context* context);
	struct number* (*subbed_by)(struct number*, struct number*, struct context* context);
	struct number* (*multiplied_by)(struct number*, struct number*, struct context* context);
	struct number* (*divided_by)(struct number*, struct number*, struct context* context);

	void (*show)(struct number*);
	
	struct context* context;
	struct node* parentNode;
	int pos_start;
	int pos_end;
};
struct number* number__init__int(int intVal, struct node* node, struct context* context);
struct number* number__init__float(float floatVal, struct node* node, struct context* context);
struct number* number__initFromToken(struct token* token, struct node* node, struct context* context); 



struct number* number__mod(struct number* number, struct number* other, struct context* context) ;
struct number* number__eqeq(struct number* number, struct number* other, struct context* context) ;
struct number* number__neq(struct number* number, struct number* other, struct context* context) ;
struct number* number__gt(struct number* number, struct number* other, struct context* context) ;
struct number* number__gte(struct number* number, struct number* other, struct context* context) ;
struct number* number__lt(struct number* number, struct number* other, struct context* context) ;
struct number* number__lte(struct number* number, struct number* other, struct context* context) ;
struct number* number__and(struct number* number, struct number* other, struct context* context) ;
struct number* number__or(struct number* number, struct number* other, struct context* context) ;
struct number* number__xor(struct number* number, struct number* other, struct context* context) ;
#endif
