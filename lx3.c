#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static char* readfile(const char* filename) {
	char *res = 0;
	FILE *f = fopen(filename, "r");
	if (f) {
		fseek(f, 0, SEEK_END);
		size_t size = ftell(f);
		fseek(f, 0, SEEK_SET);

		res = (char*)malloc(size+1);
		fread(res, size, 1, f);
		res[size] = 0;

		fclose(f);
	}
	return res;
}
// djb2
unsigned long hash(unsigned char *str) {
	unsigned long hash = 5381;
	int c;
	while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}
typedef enum {
	TK_UNKNOWN,
	TK_EOS,
	TK_IDENTIFIER,
	TK_QUOTE
} TK_type;
const char* TK_name(TK_type t) {
	switch(t){
	case TK_UNKNOWN: return "unknown";
	case TK_EOS: return "eos";
	// case TK_QUOTE: return "quote";
	case TK_IDENTIFIER: return "ident";
	// case TK_PAREN_OPEN: return "paren_open";
	// case TK_PAREN_CLOSE: return "paren_close";
	// case TK_BRACKET_OPEN: return "bracket_open";
	// case TK_BRACKET_CLOSE: return "bracket_close";
	// case TK_BRACE_OPEN: return "brace_close";
	// case TK_BRACE_CLOSE: return "brace_close";
	// case TK_DOT: return "dot";
	// case TK_COMMA: return "comma";
	// case TK_STRING: return "string";
	case TK_QUOTE: return "quote";
	}
	return "invalid";
}
bool is_endofline(char c) {
	return c == '\n' || c == '\r';
}
bool is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\v' || c == '\f' || is_endofline(c);
}
bool is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool is_num(char c) {
	return c >= '0' && c <= '9';
}
bool is_alphanum(char c) {
	return is_alpha(c) || is_num(c);
}
typedef enum {
	OP_CALL,
} OP_type;
typedef struct {
	void **ops;
} LxState;
typedef struct {
	char* c;
	int n;
	TK_type t;
} Token;
typedef struct {
	char* c; // Current
	int l; // Line
	int o; // Offset
	Token curr;
	Token peek;
} LexState;
void LexState_init(LexState *ls) {
	ls->peek.t = TK_EOS;
	ls->l = 1;
}
Token TK_lex(LexState *ls) {
	for(;;) {
		if (*ls->c == '\n') ls->l++;
		if (is_whitespace(*ls->c)) ++ls->c;
		else break;
	}
	Token t;
	t.t = TK_UNKNOWN;
	t.n = 1;
	t.c = ls->c;
	char c = *ls->c;
	++ls->c;
	switch(c) {
	case '\0':	{t.t = TK_EOS;} break;
	// case '(':	{t.t = TK_PAREN_OPEN;} break;
	// case ')':	{t.t = TK_PAREN_CLOSE;} break;
	// case '[':	{t.t = TK_BRACKET_OPEN;} break;
	// case ']':	{t.t = TK_BRACKET_CLOSE;} break;
	// case '{':	{t.t = TK_BRACE_OPEN;} break;
	// case '}':	{t.t = TK_BRACE_CLOSE;} break;
	// case '.':	{t.t = TK_DOT;} break;
	// case ',':	{t.t = TK_COMMA;} break;
	case '"':
	case '\'':	{t.t = TK_QUOTE;} break;
	default: {
		if (is_alpha(c)) {
			t.t = TK_IDENTIFIER;
			while (is_alphanum(*ls->c) || *ls->c == '_') ++ls->c;
		}
		t.n = ls->c - t.c;
	}
	}
	return t;
}
void TK_next(LexState *ls) {
	if (ls->peek.t != TK_EOS) {
		ls->curr = ls->peek;
		ls->peek.t = TK_EOS;
		return;
	}
	ls->curr = TK_lex(ls);
}
void TK_peek(LexState *ls) {
	assert(ls->peek.t == TK_EOS);
	ls->peek = TK_lex(ls);
}
bool check_next1(LexState *ls, char c) {
	return *ls->peek.c == c;
}
static const char* V_print = "print";
bool lx_strcmp(char* a, int n, const char* b) {
	int i;
	for (i=0;i<n;i++) if (*(a+i)!=*(b+i)) return false;
	return true;
}
void func(LexState *ls) {
	bool eq = lx_strcmp(ls->curr.c, ls->curr.n, V_print);
	if (eq) {

	} else {
		printf("line %d: undefined function: %.*s\n", ls->l, ls->curr.n, ls->curr.c);
	}
}
void assign(LexState *ls) {

}
void statement(LexState *ls) {
	switch(ls->curr.t){
	case ';': break;
	case TK_IDENTIFIER: {
		TK_peek(ls);
		if (check_next1(ls, '(')) func(ls);
		else if (check_next1(ls, '=')) assign(ls);
	} break;
	default: {
		printf("line %d: unexpected identifier: %.*s\n", ls->l, ls->curr.n, ls->curr.c);
	}
	}
}
int main(int argc, char* argv[]) {
	// printf("Hello, World!\n");
	char* s = readfile("test/test.lx");
	LexState ls;
	LexState_init(&ls);
	ls.c = s;
	for (;;) {
		TK_next(&ls);
		if (ls.curr.t == TK_EOS) break;
		statement(&ls);
		// printf("[%d] %.*s %s\n", ls.l, t.n, t.c, TK_name(t.t));
	}
	return 0;
}

/* TODO(alex):
- Use musl 
*/