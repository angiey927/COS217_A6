/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Bob Dondero, Donna Gabai                                   */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
#include <assert.h>
#include <stddef.h>

/*--------------------------------------------------------------------*/
/* Modify *puiDest in place,
   setting uiNumBits starting at uiDestStartBit (where 0 indicates
   the least significant bit) with bits taken from uiSrc,
   starting at uiSrcStartBit.
   uiSrcStartBit indicates the rightmost bit in the field.
   setField sets the appropriate bits in *puiDest to 1.
   setField never unsets any bits in *puiDest.                        */
static void setField(unsigned int uiSrc, unsigned int uiSrcStartBit,
                     unsigned int *puiDest, unsigned int uiDestStartBit,
                     unsigned int uiNumBits)
{
   unsigned int srcMask = 1;
   unsigned int srcSet;
   /* create mask to extract target bits of uiSrc */
   /* make mask cover the uiNumBits */
   srcMask = srcMask << uiNumBits;
   /* subtract 1 so the 0s shifted in become 1s*/
   srcMask--;
   /* from uiSrc, remove bits before the start bit */
   uiSrc = uiSrc >> uiSrcStartBit;
   /* apply mask to extract bits from uiSrc */
   srcSet = uiSrc & srcMask;
   /* shift srcSet to start from uiDestStartBit*/
   srcSet = srcSet << uiDestStartBit;
   /* set puiDest's bits */
   *puiDest = *puiDest | srcSet;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_mov(unsigned int uiReg, int iImmed)
{
   /* fixed bits for instr */
   unsigned int uiInstr = 0b01010010100000000000000000000000;
   /* shift bits for imm into position */
   iImmed = iImmed << 5;
   /* set imm bits by ORing */
   uiInstr = uiInstr | iImmed;
   /* set reg bits */
   uiInstr = uiInstr | uiReg;
   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x10000000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* displacement to be split into immlo and immhi and inserted */
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   setField(uiDisp, 0, &uiInstr, 29, 2);
   setField(uiDisp, 2, &uiInstr, 5, 19);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_strb(unsigned int uiFromReg,
   unsigned int uiToReg)
{
   unsigned int uiInstr;

   /* base instruction code */
   uiInstr = 0b00111000000000000000010000000000;
   /* imm bits stay as 0 since header does not specify offset option */
   /* inserting source reg */
   setField(uiFromReg, 0, uiInstr, 5, 5);
   /* inserting dest reg */
   setField(uiToReg, 0, uiInstr, 0, 5);
   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_b(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   /* Your code here */

}
