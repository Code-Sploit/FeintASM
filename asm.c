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

ELF_File *NewELFFile(char *__fsrc)
{
    ELF_File *__eptr = calloc(1, sizeof(ELF_File));

    for (int i = 0; i < HDR_MGC_SIZE; i++)
    {
        __eptr->MHDR[i] = ELF_HDR_MAGIC[i];
    }

    for (int i = 0; i < HDR_SRT_SIZE; i++)
    {
        __eptr->EHDR[i] = ELF_HDR_SRT[i];
    }

    __eptr->__fsrc = __fsrc;

    return __eptr;
}

void SetELFInst(ELF_File *__eptr, uint16_t __minstr[HDR_MAX_SIZE])
{
    for (int i = 0; i < HDR_MAX_SIZE; i++)
    {
        __eptr->SHDR[i] = __minstr[i];
    }
}

void AppendELFInst(ELF_File *__eptr, uint16_t __inst)
{
    __eptr->SHDR[__eptr->__minc] = __inst;

    __eptr->__minc += 1;
}

char *TranslateIdToError(int __code)
{
    switch (__code)
    {
        case ERROR_INVALID_FILE_POINTER:
        {
            return "Error invalid FILE pointer!";
        }

        case ERROR_INVALID_REGISTER:
        {
            return "Error invalid register!";
        }
    }

    return "Unknown error!";
}

uint16_t ConvertCharToBase16(char *__cstr)
{
    uint16_t __result = strtol(__cstr, NULL, 16);

    return __result;
}

int WriteMovInstruction(FILE *__fptr, char *__reg, char *__val)
{
    if (!__fptr) {return ERROR_INVALID_FILE_POINTER;}

    unsigned int _reg_matched = 0;

    if (MatchExpr(__reg, "eax"))
    {
        _reg_matched = 1;

        uint16_t __utval = ConvertCharToBase16(__val);

        fwrite(&EAX, sizeof(uint16_t), 1, __fptr);
        fwrite(&__utval, sizeof(uint16_t), 1, __fptr);
    }

    if (MatchExpr(__reg, "ebx"))
    {
        _reg_matched = 1;

        uint16_t __utval = ConvertCharToBase16(__val);

        fwrite(&EBX, sizeof(uint16_t), 1, __fptr);
        fwrite(&__utval, sizeof(uint16_t), 1, __fptr);
    }

    if (MatchExpr(__reg, "ecx"))
    {
        _reg_matched = 1;

        uint16_t __utval = ConvertCharToBase16(__val);

        fwrite(&ECX, sizeof(uint16_t), 1, __fptr);
        fwrite(&__utval, sizeof(uint16_t), 1, __fptr);
    }

    if (MatchExpr(__reg, "edx"))
    {
        _reg_matched = 1;

        uint16_t __utval = ConvertCharToBase16(__val);

        fwrite(&EDX, sizeof(uint16_t), 1, __fptr);
        fwrite(&__utval, sizeof(uint16_t), 1, __fptr);
    }

    if (_reg_matched == 0) {return ERROR_INVALID_REGISTER;}

    return 0;
}

int WriteSyscallInstruction(FILE *__fptr)
{
    if (!__fptr) {return ERROR_INVALID_FILE_POINTER;}

    fwrite(&PRE_SYSCALL, sizeof(uint16_t), 1, __fptr);
    fwrite(&SYSCALL, sizeof(uint16_t), 1, __fptr);

    return 0;
}

int AssembleELF64(char *__file)
{
    /* Define stuff */
    const char *__fmode_w = "wb";
    const char *__fmode_r = "rb";

    const char *__ofile   = "output.bin";
    
    char *__fcons = calloc(1, sizeof(char));
    char *__fline;

    size_t Length;
    ssize_t Read;

    /* Open the file */
    FILE *__fptr_r = fopen(__file, __fmode_r);

    if (!__fptr_r)
    {
        printf("Error while opening file: (%s) in const mode: (%s)\n", __file, __fmode_r);

        exit(-1);
    }

    /* Read the file */
    while ((Read = getline(&__fline, &Length, __fptr_r)) != -1)
    {
        __fcons = realloc(__fcons, (strlen(__fcons) + strlen(__fline) + 1));

        strcat(__fcons, __fline);
    }

    /* Close the read pointer FILE */
    fclose(__fptr_r);

    /* Open the output file for writing */
    FILE *__fptr_w = fopen(__ofile, __fmode_w);

    if (!__fptr_w)
    {
        printf("Error while opening file: (%s) in const mode (%s)\n", __ofile, __fmode_w);

        exit(-1);
    }

    /* Create new ELF object */
    ELF_File *__eptr = NewELFFile(__file);

    /* Write ELF header to file */
    fwrite(__eptr->MHDR, sizeof(char) * HDR_MGC_SIZE, 1, __fptr_w);
    fwrite(__eptr->EHDR, sizeof(uint16_t) * HDR_SRT_SIZE, 1, __fptr_w);

    /* Initialize a lexer */
    ASMLexer_T *lexer  = InitializeASMLexer(__fcons);
    ASMToken_Node *tok = ASMLexerGetNextToken(lexer);

    /* Parse the file */
    while (tok->Type != ASMToken_EOF)
    {
        unsigned int _type = tok->Type;
        char *_value       = tok->_value;

        if (_type == ASMToken_ID)
        {
            if (MatchExpr(_value, ASM_SYNTAX_MOV))
            {
                printf("Instruction MOV\n");

                /* Next token order should be: 
                MOV (reg), (val)

                -> reg = {eax, ebx, ecx, edx}
                -> val = typeof: {int}
                */

                ASMToken_Node *__reg = ASMLexerGetNextToken(lexer);
                ASMToken_Node *__com = ASMLexerGetNextToken(lexer);
                ASMToken_Node *__val = ASMLexerGetNextToken(lexer);

                char *__regv = __reg->_value;
                char *__comv = __com->_value;
                char *__valv = __val->_value;

                /* Write instructions to file */
                int __ret = WriteMovInstruction(__fptr_w, __regv, __valv);

                if (__ret != 0)
                {
                    printf("Error code (%d): [%s]", __ret, TranslateIdToError(__ret));

                    exit(-1);
                }
            }

            if (MatchExpr(_value, ASM_SYNTAX_SYSCALL))
            {
                printf("Instruction SYSCALL\n");

                /* Next token order should be:
                SYSCALL
                */

                int __ret = WriteSyscallInstruction(__fptr_w);

                if (__ret != 0)
                {
                    printf("Error code (%d): [%s]", __ret, TranslateIdToError(__ret));

                    exit(-1);
                }
            }
        }

        tok = ASMLexerGetNextToken(lexer);
    }

    /* Generate an uint16_t array of minstructions */
    /*
    uint16_t hdr_minstr[HDR_MAX_SIZE] = {
        0x01b8, 0x04, // MOV ecx, 10
        0xbb00, 0x01, // MOV edx, 1
        0x00, 0x80cd,  // SYSCALL
        0x01b8, 0x00, // MOV eax, 0
        0xbb00, 0x01, // MOV ebx, 1
        0x00, 0x80cd  // Syscall
    };*/

    /* Close the file */
    fclose(__fptr_w);

    /* Done */
    return 0;
}
