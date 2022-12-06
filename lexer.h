#include "token.h"

typedef struct ASMLexer_STRUCT {
    char *src;
    char c;

    unsigned int i;
} ASMLexer_T;

int MatchExpr(char *_e1, char *_e2);

ASMLexer_T *InitializeASMLexer(char *src);

char ASMLexerPeekOffset(ASMLexer_T *ASMLexer, int _offset);

char *ASMLexerAdvanceWithId(ASMLexer_T *ASMLexer);
char *ASMLexerAdvanceWithDigit(ASMLexer_T *ASMLexer);

void ASMLexerSkipWhitespace(ASMLexer_T *ASMLexer);
void ASMLexerAdvanceCharacter(ASMLexer_T *ASMLexer);

ASMToken_Node *ASMLexerGetNextToken(ASMLexer_T *ASMLexer);
ASMToken_Node *ASMLexerSkipToken(ASMLexer_T *ASMLexer, int _count);
