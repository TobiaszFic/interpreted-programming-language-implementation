#ifndef TOKEN_H 
#define TOKEN_H
 

enum tokenType {
	T_INT,
	T_FLOAT,
	T_IDENTIFIER,
	T_KEYWORD,
	T_EQ,
	T_PLUS,
	T_MINUS,
	T_MUL,
	T_DIV,
	T_MOD,
	T_EQEQ,
	T_NEQ,
	T_GT,
	T_GTE,
	T_LT,
	T_LTE,
	T_NOT,
	T_LPAREN,
	T_RPAREN,
	T_LBRACE,
	T_RBRACE,
	T_COMMA,
	T_ARROW,
	T_SEMI,
	T_EOF
};

struct token {
	//parameters
	enum tokenType type;
	union value {
		int i;
		float f;
		char* id_str;
	} value;
	unsigned int lineNumber;
	int pos_start;
	int pos_end;
	
	//methods
	void (*show)(struct token*);
};

struct token* token__init(enum tokenType initType, int initLineNumber, int initPos_start, int initPos_end);
struct token* token__init__copy(const struct token* initToken);

void token__cleanup(struct token* token);
#endif
