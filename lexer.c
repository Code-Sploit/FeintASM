#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

ASMLexer_T *InitializeASMLexer(char *src)
{
    ASMLexer_T *NewASMLexer = calloc(1, sizeof(ASMLexer_T));

    NewASMLexer->src = src;
    NewASMLexer->i   = 0;
    NewASMLexer->c   = NewASMLexer->src[NewASMLexer->i];

    return NewASMLexer;
}

char ASMLexerPeekOffset(ASMLexer_T *ASMLexer, int _offset)
{
    return ASMLexer->src[ASMLexer->i + _offset];
}

char *ASMLexerGetValue(ASMLexer_T *ASMLexer)
{
    char *value = calloc(1, sizeof(char));

    while (ASMLexerPeekOffset(ASMLexer, 0) != '"')
    {
        value = realloc(value, (strlen(value) + 2));

        strcat(value, (char[]) {ASMLexerPeekOffset(ASMLexer, 0), 0});

        ASMLexerAdvanceCharacter(ASMLexer);
    }

    ASMLexerAdvanceCharacter(ASMLexer);

    return value;
}

void ASMLexerAdvanceCharacter(ASMLexer_T *ASMLexer)
{
    if (strlen(ASMLexer->src) >= (ASMLexer->i + 1))
    {
        ASMLexer->i++;

        ASMLexer->c = ASMLexer->src[ASMLexer->i];
    }
}

void ASMLexerDevance(ASMLexer_T *ASMLexer)
{
    if ((ASMLexer->i - 1) >= 0)
    {
        ASMLexer->i = ASMLexer->i - 1;

        ASMLexer->c = ASMLexer->src[ASMLexer->i];
    }
}

void ASMLexerAdvanceOffset(ASMLexer_T *ASMLexer, int _offset)
{
    for (int i = 0; i < _offset; i++)
    {
        ASMLexerAdvanceCharacter(ASMLexer);
    }
}

void ASMLexerSkipUntil(ASMLexer_T *ASMLexer, char ch)
{
    while (ASMLexerPeekOffset(ASMLexer, 0) != ch)
    {
        ASMLexerAdvanceCharacter(ASMLexer);
    }

    ASMLexerAdvanceOffset(ASMLexer, 2);
}

char *ASMLexerAdvanceWithId(ASMLexer_T *ASMLexer)
{
    char *id = calloc(1, sizeof(id));

    while (isalpha(ASMLexerPeekOffset(ASMLexer, 0)) != 0 || ASMLexerPeekOffset(ASMLexer, 0) == '_')
    {
        id = realloc(id, (strlen(id) + 2));

        strcat(id, (char[]) {ASMLexerPeekOffset(ASMLexer, 0), 0});

        ASMLexerAdvanceCharacter(ASMLexer);
    }

    return id;
}

char *ASMLexerAdvanceWithDigit(ASMLexer_T *ASMLexer)
{
    char *digit = calloc(1, sizeof(char));

    while (isdigit(ASMLexerPeekOffset(ASMLexer, 0)) != 0)
    {
        digit = realloc(digit, (strlen(digit) + 2));

        strcat(digit, (char[]) {ASMLexer->c, 0});

        ASMLexerAdvanceCharacter(ASMLexer);
    }

    return digit;
}

void ASMLexerSkipWhitespace(ASMLexer_T *ASMLexer)
{
    while (isspace(ASMLexerPeekOffset(ASMLexer, 1)) == 1)
    {
        ASMLexerAdvanceCharacter(ASMLexer);
    }
}

ASMToken_Node *ASMLexerSkipToken(ASMLexer_T *ASMLexer, int _count)
{
    ASMToken_Node *tok = calloc(1, sizeof(ASMToken_Node));

    for (int i = 0; i < _count; i++)
    {
        tok = ASMLexerGetNextToken(ASMLexer);
    }

    return tok;
}

ASMToken_Node *ASMLexerGetNextToken(ASMLexer_T *ASMLexer)
{
    ASMLexerSkipWhitespace(ASMLexer);

    while (ASMLexerPeekOffset(ASMLexer, 1) != '\0')
    {

        if (ASMLexer->c == 13 || ASMLexer->c == 10 || ASMLexer->c == ' ' || ASMLexer->c == '\t') {ASMLexerAdvanceCharacter(ASMLexer); continue;}

        if (isalpha(ASMLexer->c) != 0) {return InitializeASMToken(ASMLexerAdvanceWithId(ASMLexer), ASMToken_ID);}
        if (isdigit(ASMLexer->c) != 0) {return InitializeASMToken(ASMLexerAdvanceWithDigit(ASMLexer), ASMToken_DIGIT);}

        if (isspace(ASMLexer->c) == 0 && isalpha(ASMLexer->c) == 0 && isdigit(ASMLexer->c) == 0)
        {
            switch (ASMLexer->c)
            {
                case '!': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("!", ASMToken_EXCL);}
                case '@': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("@", ASMToken_AT);}
                case '#': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("#", ASMToken_HASHTAG);}
                case '$': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("$", ASMToken_DOLLAR);}
                case '%': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("%", ASMToken_PERCENT);}
                case '^': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("^", ASMToken_UP);}
                case '&': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("&", ASMToken_ZS);}
                case '*': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("*", ASMToken_STAR);}
                case '(': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("(", ASMToken_LPAREN);}
                case ')': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken(")", ASMToken_RPAREN);}
                case '+': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("+", ASMToken_PLUS);}
                case '=': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("=", ASMToken_IS);}
                case '{': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("{", ASMToken_RBRACE);}
                case '}': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken("}", ASMToken_LBRACE);}
                case ';': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken(";", ASMToken_SEMICOLON);}
                case ',': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken(",", ASMToken_COMMA);}
                case '"': {ASMLexerAdvanceCharacter(ASMLexer); return InitializeASMToken(ASMLexerGetValue(ASMLexer), ASMToken_ID);}
            }
        }

        ASMLexerAdvanceCharacter(ASMLexer);
    }

    return InitializeASMToken("\0", ASMToken_EOF);
}
