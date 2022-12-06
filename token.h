#ifndef SYNTAXES
#define SYNTAXES

#define ASM_SYNTAX_MOV "mov"
#define ASM_SYNTAX_PUSH "push"
#define ASM_SYNTAX_POP "pop"
#define ASM_SYNTAX_SYSCALL "syscall"

#define PRIMARY_FUNCTION "start"

#endif

typedef struct ASMToken_STRUCT {
    char *_value;
    char *_dest;
    char *_arg;

    enum {
        ASMToken_INIT,
        ASMToken_ID,
        ASMToken_VAL,
        ASMToken_DIGIT,
        ASMToken_INT,
        ASMToken_STR,
        ASMToken_EXCL,
        ASMToken_AT,
        ASMToken_HASHTAG,
        ASMToken_DOLLAR,
        ASMToken_PERCENT,
        ASMToken_UP,
        ASMToken_ZS,
        ASMToken_STAR,
        ASMToken_LBRACE,
        ASMToken_RBRACE,
        ASMToken_LPAREN,
        ASMToken_RPAREN,
        ASMToken_PLUS,
        ASMToken_MINUS,
        ASMToken_IS,
        ASMToken_SEMICOLON,
        ASMToken_BFUNC,
        ASMToken_COMMA,
        ASMToken_NULL,
        ASMToken_EOF
    } Type;
} ASMToken_Node;

ASMToken_Node *InitializeASMToken(char *_value, int _type);
