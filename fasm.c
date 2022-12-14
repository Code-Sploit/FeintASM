#include "fasm.h"

#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Invalid syntax!\n");

        return -1;
    }

    printf("Assembling: (%s)\n", argv[1]);

    int __ret = AssembleELF64(argv[1]);

    if (__ret != 0)
    {
        printf("\nRan into errors while assembling: (%s) for: (x86_64-ELF64)\n", argv[1]);

        return -1;
    }

    printf("\nSaved executable to: (%s)\n", "output.bin");
    printf("Setting permissions for: (%s)\n", "output.bin");

    struct stat fs;

    stat(argv[1], &fs);

    int __cret = chmod("output.bin", fs.st_mode | S_IRWXU + S_IXGRP + S_IXOTH);

    if (__cret != 0)
    {
        printf("Ran into errors while changing file permissions: (%s)!\n", argv[1]);

        return -1;
    }

    return 0;
}
