#include "asm.h"

#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int MatchExpr(char *__e1, char *__e2)
{
    int __res = strcmp(__e1, __e2);

    return !__res;
}

int AssembleELF64(char *__file)
{
    const char *__mode = "rb";

    FILE *__ptr = fopen(__file, __mode);

    if (!__ptr)
    {
        printf("\nError while opening: (%s) in mode: (%s)\n", __file, __mode);

        exit(-1);
    }

    /*
    1) Lex each line
    2) Write machine code into file
    3) Done
    */

    char *__contents = calloc(1, sizeof(char));
    char *__sTemp    = calloc(1, sizeof(char));

    size_t Length;
    ssize_t Read;

    while ((Read = getline(&__sTemp, &Length, __ptr)) != -1)
    {
        __contents = realloc(__contents, (strlen(__contents) + strlen(__sTemp) + 1));

        strcat(__contents, __sTemp);
    }

    printf("\n--------------\n");
    printf("Read file contents: ");

    printf("\n--------------\n%s\n--------------\n", __contents);

    printf("TOKEN LIST:\n--------------\n");

    /* Initialize a lexer */

    ASMLexer_T *lexer  = InitializeASMLexer(__contents);
    ASMToken_Node *tok = ASMLexerGetNextToken(lexer);

    while (tok->Type != ASMToken_EOF)
    {
        if (tok->Type == ASMToken_ID)
        {
            if (MatchExpr(tok->_value, ASM_SYNTAX_MOV))
            {
                printf("\nMOV Instruction!\n");

                /* Next tokens should be: (register, TOKEN_COMMA, value) ! */

                ASMToken_Node *__register = ASMLexerGetNextToken(lexer);
                ASMToken_Node *__comma    = ASMLexerGetNextToken(lexer);
                ASMToken_Node *__value    = ASMLexerGetNextToken(lexer);

                printf("REG: (%s)\n", __register->_value);
                printf("VAL: (%s)\n", __value->_value);
            }

            if (MatchExpr(tok->_value, ASM_SYNTAX_POP))
            {
                printf("\nPOP Instruction!\n");

                /* Next tokens should be: (register) ! */

                ASMToken_Node *__register = ASMLexerGetNextToken(lexer);

                printf("REG: (%s)\n", __register->_value);
            }

            if (MatchExpr(tok->_value, ASM_SYNTAX_PUSH))
            {
                printf("\nPUSH Instruction!\n");

                /* Next tokens should be: (register) ! */

                ASMToken_Node *__register = ASMLexerGetNextToken(lexer);

                printf("REG: (%s)\n", __register->_value);
            }

            if (MatchExpr(tok->_value, ASM_SYNTAX_SYSCALL))
            {
                printf("\nSYSCALL Instruction!\n");

                /* Next tokens should be: (null) ! */

                printf("(null)\n");
            }
        }

        tok = ASMLexerGetNextToken(lexer);
    }

    fclose(__ptr);

    FILE *__ptr_w = fopen("output", "wb");

    if (!__ptr_w)
    {
        printf("\nError opening (output)!\n");

        exit(1);
    }

    //write(__ptr_w, &elf_hdr_default, ELF_HDR_SIZE);
    /*
    fwrite(&elf_hdr_default, ELF_HDR_SIZE, 1, __ptr_w);
    fwrite(&elf_prog_hdr_default, ELF_HDR_SIZE, 1, __ptr_w);
    fwrite(&elf_hdr_prog, sizeof(char) * 12, 1, __ptr_w);*/

    fwrite(&hdr_magic, sizeof(char) * 4, 1, __ptr_w);
    fwrite(&hdr_start, sizeof(uint16_t) * 38, 1, __ptr_w);
    fwrite(&hdr_minstr, sizeof(uint16_t) * 8, 1, __ptr_w);

    fclose(__ptr_w);

    return 0;
}
