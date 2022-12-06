#include "token.h"

#include <stdlib.h>

ASMToken_Node *InitializeASMToken(char *_value, int _type)
{
    ASMToken_Node *NewASMToken = calloc(1, sizeof(ASMToken_Node));

    NewASMToken->_value = _value;
    NewASMToken->Type   = _type;

    return NewASMToken;
}
