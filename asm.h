#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

/* Keyword definitions */

static uint16_t EAX = 0x01b8;
static uint16_t EBX = 0xbb00;
static uint16_t ECX = 0x01b9;
static uint16_t EDX = 0x01bb;

static uint16_t PRE_SYSCALL = 0x0000;
static uint16_t SYSCALL = 0x80cd;

/* Error type definitions */

#define ERROR_INVALID_REGISTER 10
#define ERROR_INVALID_FILE_POINTER 20

/* Hdr size definitions */

#define ELF_HDR_SIZE (0x40)

#define HDR_MAX_SIZE 20
#define HDR_SRT_SIZE 40
#define HDR_MGC_SIZE 4

static char ELF_HDR_MAGIC[] = {
    0x7f, 0x45, 0x4c, 0x46
};

static uint16_t ELF_HDR_SRT[] = {
    0x0102,
    0x01,
    0x00,
    0x00,
    0x00, 0x00,
    0x02,
    0x3e,
    0x01, 0x00,
    0x8054, 0x0804,
    0x34, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x34, 0x20, 0x01, 0x28, 0x00, 0x00,
    0x01, 0x00,
    0x54, 0x00, 0x8054, 0x0804,
    0x00, 0x00,
    0xCC, 0x00,
    0xCC, 0x00,
    0x05, 0x00,
    0x1000, 0x00,
};

typedef struct ELF_FILE_STRUCT {
    uint16_t PHDR[HDR_SRT_SIZE];
    uint16_t SHDR[HDR_MAX_SIZE];
    uint16_t EHDR[HDR_SRT_SIZE];

    unsigned int __minc;

    char MHDR[HDR_MGC_SIZE];

    char *__fsrc;
} ELF_File;

ELF_File *NewELFFile(char *__fsrc);

int AssembleELF64(char *__file);
