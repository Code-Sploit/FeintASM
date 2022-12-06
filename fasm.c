#include "fasm.h"

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

    return 0;
}
