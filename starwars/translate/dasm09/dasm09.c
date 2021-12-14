#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define codes codes09 /* avoid clash for now - static later */

// byte *memory = NULL;

#define OPCODE(address)  memory[address&0xffff]
#define ARGBYTE(address) memory[address&0xffff]
#define ARGWORD(address) (word)((memory[address&0xffff]<<8)|memory[(address+1)&0xffff])

//#include "dasm09.h"

#ifndef TYPES
#define TYPES
typedef unsigned char  byte;
typedef unsigned short word;
#endif

#ifndef FALSE
#define FALSE 0L
#define TRUE  1L
#endif

char *disasm(unsigned char *memory, unsigned int *pc, int showhex, int showaddr);

/***************************************************************************
 * dasm09 -- Portable M6809 Disassembler                                   *
 * Copyright (C) 2000  Arto Salmi                                          *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program; if not, write to the Free Software             *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               *
 ***************************************************************************/

enum addr_mode {
_nom,     /* no mode                    */
_imp,     /* inherent/implied           */
_imb,     /* immediate byte             */
_imw,     /* immediate word             */
_dir,     /* direct                     */
_ext,     /* extended                   */
_ind,     /* indexed                    */
_reb,     /* relative byte              */
_rew,     /* relative word              */
_r1 ,     /* tfr/exg mode               */
_r2 ,     /* pul/psh system             */
_r3 ,     /* pul/psh user               */
_bd ,     /* Bit Manipulation direct    */
_bi ,     /* Bit Manipulation index     */
_be ,     /* Bit Manipulation extended  */
_bt ,     /* Bit Transfers direct       */
_t1 ,     /* Block Transfer r0+,r1+     */
_t2 ,     /* Block Transfer r0-,r1-     */
_t3 ,     /* Block Transfer r0+,r1      */
_t4 ,     /* Block Transfer r0,r1+      */
_iml      /* immediate 32-bit           */
};

enum opcodes {
_ill=0,_abx,  _adca, _adcb, _adda, _addb, _addd, _anda, _andb,
_andcc,_asla, _aslb, _asl,  _asra, _asrb, _asr,  _bcc,  _lbcc,
_bcs,  _lbcs, _beq,  _lbeq, _bge,  _lbge, _bgt,  _lbgt, _bhi,
_lbhi, _bita, _bitb, _ble,  _lble, _bls,  _lbls, _blt,  _lblt,
_bmi,  _lbmi, _bne,  _lbne, _bpl,  _lbpl, _bra,  _lbra, _brn,
_lbrn, _bsr,  _lbsr, _bvc,  _lbvc, _bvs,  _lbvs, _clra, _clrb,
_clr,  _cmpa, _cmpb, _cmpd, _cmps, _cmpu, _cmpx, _cmpy, _coma,
_comb, _com,  _cwai, _daa,  _deca, _decb, _dec,  _eora, _eorb,
_exg,  _inca, _incb, _inc,  _jmp,  _jsr,  _lda,  _ldb,  _ldd,
_lds,  _ldu,  _ldx,  _ldy,  _leas, _leau, _leax, _leay, _lsra,
_lsrb, _lsr,  _mul,  _nega, _negb, _neg,  _nop,  _ora,  _orb,
_orcc, _pshs, _pshu, _puls, _pulu, _rola, _rolb, _rol,  _rora,
_rorb, _ror,  _rti,  _rts,  _sbca, _sbcb, _sex,  _sta,  _stb,
_std,  _sts,  _stu,  _stx,  _sty,  _suba, _subb, _subd, _swi,
_swi2, _swi3, _sync, _tfr,  _tsta, _tstb, _tst,  _reset,
};

char *mne[] = {
"???",   "ABX  ", "ADCA ", "ADCB ", "ADDA ", "ADDB ", "ADDD ", "ANDA ", "ANDB ",
"ANDCC", "ASLA ", "ASLB ", "ASL  ", "ASRA ", "ASRB ", "ASR  ", "BCC  ", "LBCC ",
"BCS  ", "LBCS ", "BEQ  ", "LBEQ ", "BGE  ", "LBGE ", "BGT  ", "LBGT ", "BHI  ",
"LBHI ", "BITA ", "BITB ", "BLE  ", "LBLE ", "BLS  ", "LBLS ", "BLT  ", "LBLT ",
"BMI  ", "LBMI ", "BNE  ", "LBNE ", "BPL  ", "LBPL ", "BRA  ", "LBRA ", "BRN  ",
"LBRN ", "BSR  ", "LBSR ", "BVC  ", "LBVC ", "BVS  ", "LBVS ", "CLRA ", "CLRB ",
"CLR  ", "CMPA ", "CMPB ", "CMPD ", "CMPS ", "CMPU ", "CMPX ", "CMPY ", "COMA ",
"COMB ", "COM  ", "CWAI ", "DAA  ", "DECA ", "DECB ", "DEC  ", "EORA ", "EORB ",
"EXG  ", "INCA ", "INCB ", "INC  ", "JMP  ", "JSR  ", "LDA  ", "LDB  ", "LDD  ",
"LDS  ", "LDU  ", "LDX  ", "LDY  ", "LEAS ", "LEAU ", "LEAX ", "LEAY ", "LSRA ",
"LSRB ", "LSR  ", "MUL  ", "NEGA ", "NEGB ", "NEG  ", "NOP  ", "ORA  ", "ORB  ",
"ORCC ", "PSHS ", "PSHU ", "PULS ", "PULU ", "ROLA ", "ROLB ", "ROL  ", "RORA ",
"RORB ", "ROR  ", "RTI  ", "RTS  ", "SBCA ", "SBCB ", "SEX  ", "STA  ", "STB  ",
"STD  ", "STS  ", "STU  ", "STX  ", "STY  ", "SUBA ", "SUBB ", "SUBD ", "SWI  ",
"SWI2 ", "SWI3 ", "SYNC ", "TFR  ", "TSTA ", "TSTB ", "TST  ", "RESET",
};

byte codes[512] = {
_neg  ,_dir,   _ill  ,_nom,   _ill  ,_nom,   _com  ,_dir,
_lsr  ,_dir,   _ill  ,_nom,   _ror  ,_dir,   _asr  ,_dir,
_asl  ,_dir,   _rol  ,_dir,   _dec  ,_dir,   _ill  ,_nom,
_inc  ,_dir,   _tst  ,_dir,   _jmp  ,_dir,   _clr  ,_dir,
_ill  ,_nom,   _ill  ,_nom,   _nop  ,_imp,   _sync ,_imp,
_ill  ,_nom,   _ill  ,_nom,   _lbra ,_rew,   _lbsr ,_rew,
_ill  ,_nom,   _daa  ,_imp,   _orcc ,_imb,   _ill  ,_nom,
_andcc,_imb,   _sex  ,_imp,   _exg  ,_r1 ,   _tfr  ,_r1 ,
_bra  ,_reb,   _brn  ,_reb,   _bhi  ,_reb,   _bls  ,_reb,
_bcc  ,_reb,   _bcs  ,_reb,   _bne  ,_reb,   _beq  ,_reb,
_bvc  ,_reb,   _bvs  ,_reb,   _bpl  ,_reb,   _bmi  ,_reb,
_bge  ,_reb,   _blt  ,_reb,   _bgt  ,_reb,   _ble  ,_reb,
_leax ,_ind,   _leay ,_ind,   _leas ,_ind,   _leau ,_ind,
_pshs ,_r2 ,   _puls ,_r2 ,   _pshu ,_r3 ,   _pulu ,_r3 ,
_ill  ,_nom,   _rts  ,_imp,   _abx  ,_imp,   _rti  ,_imp,
_cwai ,_imb,   _mul  ,_imp,   _reset,_imp,   _swi  ,_imp,
_nega ,_imp,   _ill  ,_nom,   _ill  ,_nom,   _coma ,_imp,
_lsra ,_imp,   _ill  ,_nom,   _rora ,_imp,   _asra ,_imp,
_asla ,_imp,   _rola ,_imp,   _deca ,_imp,   _ill  ,_nom,
_inca ,_imp,   _tsta ,_imp,   _ill  ,_nom,   _clra ,_imp,
_negb ,_imp,   _ill  ,_nom,   _ill  ,_nom,   _comb ,_imp,
_lsrb ,_imp,   _ill  ,_nom,   _rorb ,_imp,   _asrb ,_imp,
_aslb ,_imp,   _rolb ,_imp,   _decb ,_imp,   _ill  ,_nom,
_incb ,_imp,   _tstb ,_imp,   _ill  ,_nom,   _clrb ,_imp,
_neg  ,_ind,   _ill  ,_nom,   _ill  ,_nom,   _com  ,_ind,
_lsr  ,_ind,   _ill  ,_nom,   _ror  ,_ind,   _asr  ,_ind,
_asl  ,_ind,   _rol  ,_ind,   _dec  ,_ind,   _ill  ,_nom,
_inc  ,_ind,   _tst  ,_ind,   _jmp  ,_ind,   _clr  ,_ind,
_neg  ,_ext,   _ill  ,_nom,   _ill  ,_nom,   _com  ,_ext,
_lsr  ,_ext,   _ill  ,_nom,   _ror  ,_ext,   _asr  ,_ext,
_asl  ,_ext,   _rol  ,_ext,   _dec  ,_ext,   _ill  ,_nom,
_inc  ,_ext,   _tst  ,_ext,   _jmp  ,_ext,   _clr  ,_ext,
_suba ,_imb,   _cmpa ,_imb,   _sbca ,_imb,   _subd ,_imw,
_anda ,_imb,   _bita ,_imb,   _lda  ,_imb,   _ill  ,_nom,
_eora ,_imb,   _adca ,_imb,   _ora  ,_imb,   _adda ,_imb,
_cmpx ,_imw,   _bsr  ,_reb,   _ldx  ,_imw,   _ill  ,_nom,
_suba ,_dir,   _cmpa ,_dir,   _sbca ,_dir,   _subd ,_dir,
_anda ,_dir,   _bita ,_dir,   _lda  ,_dir,   _sta  ,_dir,
_eora ,_dir,   _adca ,_dir,   _ora  ,_dir,   _adda ,_dir,
_cmpx ,_dir,   _jsr  ,_dir,   _ldx  ,_dir,   _stx  ,_dir,
_suba ,_ind,   _cmpa ,_ind,   _sbca ,_ind,   _subd ,_ind,
_anda ,_ind,   _bita ,_ind,   _lda  ,_ind,   _sta  ,_ind,
_eora ,_ind,   _adca ,_ind,   _ora  ,_ind,   _adda ,_ind,
_cmpx ,_ind,   _jsr  ,_ind,   _ldx  ,_ind,   _stx  ,_ind,
_suba ,_ext,   _cmpa ,_ext,   _sbca ,_ext,   _subd ,_ext,
_anda ,_ext,   _bita ,_ext,   _lda  ,_ext,   _sta  ,_ext,
_eora ,_ext,   _adca ,_ext,   _ora  ,_ext,   _adda ,_ext,
_cmpx ,_ext,   _jsr  ,_ext,   _ldx  ,_ext,   _stx  ,_ext,
_subb ,_imb,   _cmpb ,_imb,   _sbcb ,_imb,   _addd ,_imw,
_andb ,_imb,   _bitb ,_imb,   _ldb  ,_imb,   _ill  ,_nom,
_eorb ,_imb,   _adcb ,_imb,   _orb  ,_imb,   _addb ,_imb,
_ldd  ,_imw,   _ill  ,_nom,   _ldu  ,_imw,   _ill  ,_nom,
_subb ,_dir,   _cmpb ,_dir,   _sbcb ,_dir,   _addd ,_dir,
_andb ,_dir,   _bitb ,_dir,   _ldb  ,_dir,   _stb  ,_dir,
_eorb ,_dir,   _adcb ,_dir,   _orb  ,_dir,   _addb ,_dir,
_ldd  ,_dir,   _std  ,_dir,   _ldu  ,_dir,   _stu  ,_dir,
_subb ,_ind,   _cmpb ,_ind,   _sbcb ,_ind,   _addd ,_ind,
_andb ,_ind,   _bitb ,_ind,   _ldb  ,_ind,   _stb  ,_ind,
_eorb ,_ind,   _adcb ,_ind,   _orb  ,_ind,   _addb ,_ind,
_ldd  ,_ind,   _std  ,_ind,   _ldu  ,_ind,   _stu  ,_ind,
_subb ,_ext,   _cmpb ,_ext,   _sbcb ,_ext,   _addd ,_ext,
_andb ,_ext,   _bitb ,_ext,   _ldb  ,_ext,   _stb  ,_ext,
_eorb ,_ext,   _adcb ,_ext,   _orb  ,_ext,   _addb ,_ext,
_ldd  ,_ext,   _std  ,_ext,   _ldu  ,_ext,   _stu  ,_ext,
};

byte codes10[512] = {
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _lbrn ,_rew,   _lbhi ,_rew,   _lbls ,_rew,
_lbcc ,_rew,   _lbcs ,_rew,   _lbne ,_rew,   _lbeq ,_rew,
_lbvc ,_rew,   _lbvs ,_rew,   _lbpl ,_rew,   _lbmi ,_rew,
_lbge ,_rew,   _lblt ,_rew,   _lbgt ,_rew,   _lble ,_rew,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _swi2 ,_imp,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpd ,_imw,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmpy ,_imw,   _ill  ,_nom,   _ldy  ,_imw,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpd ,_dir,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmpy ,_dir,   _ill  ,_nom,   _ldy  ,_dir,   _sty  ,_dir,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpd ,_ind,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmpy ,_ind,   _ill  ,_nom,   _ldy  ,_ind,   _sty  ,_ind,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpd ,_ext,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmpy ,_ext,   _ill  ,_nom,   _ldy  ,_ext,   _sty  ,_ext,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _lds  ,_imw,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _lds  ,_dir,   _sts  ,_dir,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _lds  ,_ind,   _sts  ,_ind,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _lds  ,_ext,   _sts  ,_ext,
};

byte codes11[512] = {
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _swi3 ,_imp,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpu ,_imw,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmps ,_imw,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpu ,_dir,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmps ,_dir,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpu ,_ind,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmps ,_ind,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _cmpu ,_ext,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_cmps ,_ext,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
_ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,   _ill  ,_nom,
};

char *exg_tfr[] =
{
 "D", "X", "Y", "U", "S", "PC","??","??",
 "A", "B", "CC","DP","??","??","??","??"
};

char *bit_r[] = {"CC","A","B","??"};

char *block_r[] =
{
 "D","X","Y","U","S","?","?","?","?","?","?","?","?","?","?","?"
};

char *off4[] =
{
  "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",
  "8",  "9", "10", "11", "12", "13", "14", "15",
"-16","-15","-14","-13","-12","-11","-10", "-9",
 "-8", "-7", "-6", "-5", "-4", "-3", "-2", "-1"
};

char reg[] = { 'X','Y','U','S' };

unsigned index_string(unsigned char *memory, char *buffer, unsigned pc)
{
  byte T;
  word W;
  char R;
  char buf[30];
  unsigned PC = pc;

  T = ARGBYTE(PC); PC++;
  R = reg[(T>>5)&0x03];

  if(T&0x80)
  {
    switch(T&0x1F)
    {
      case 0x00: sprintf(buf,",%c+",R);break;

      case 0x01: sprintf(buf,",%c++",R);break;

      case 0x02: sprintf(buf,",-%c",R);break;

      case 0x03: sprintf(buf,",--%c",R);break;

      case 0x04: sprintf(buf,",%c",R);break;

      case 0x05: sprintf(buf,"B,%c",R);break;

      case 0x06: sprintf(buf,"A,%c",R);break;

      case 0x08: T = ARGBYTE(PC); PC++;
                 sprintf(buf,"$%02X,%c",T,R);
                 break;

      case 0x09: W = ARGWORD(PC); PC+=2;
                 sprintf(buf,"$%04X,%c",W,R);
                 break;

      case 0x0B: sprintf(buf,"D,%c",R);break;

      case 0x0C: T = ARGBYTE(PC); PC++;
                 sprintf(buf,"$%02X,PC",T);
                 break;

      case 0x0D: W = ARGWORD(PC); PC+=2;
                 sprintf(buf,"$%04X,PC",W);
                 break;

      case 0x11: sprintf(buf,"[,%c++]",R);break;

      case 0x13: sprintf(buf,"[,--%c]",R);break;

      case 0x14: sprintf(buf,"[,%c]",R);break;

      case 0x15: sprintf(buf,"[B,%c]",R);break;

      case 0x16: sprintf(buf,"[A,%c]",R);break;

      case 0x18: T = ARGBYTE(PC); PC++;
                 sprintf(buf,"[$%02X,%c]",T,R);
                 break;

      case 0x19: W = ARGWORD(PC); PC+=2;
                 sprintf(buf,"[$%04X,%c]",W,R);
                 break;

      case 0x1B: sprintf(buf,"[D,%c]",R);break;

      case 0x1C: T = ARGBYTE(PC); PC++;
                 sprintf(buf,"[$%02X,PC]",T);
                 break;

      case 0x1D: W = ARGWORD(PC); PC+=2;
                 sprintf(buf,"[$%04X,PC]",W);
                 break;

      case 0x07: goto index_error;

      case 0x17: goto index_error;

      case 0x0A: goto index_error;

      case 0x1A: goto index_error;

      case 0x0E: goto index_error;

      case 0x1E: goto index_error;

      index_error: sprintf(buf,"???"); break;

      default:   if(T==0x9F)
                 {
                   W = ARGWORD(PC); PC+=2;
                   sprintf(buf,"[$%04X]",W);
                 }
                 else sprintf(buf,"???");
                 break;
    }

  } else sprintf(buf,"%s,%c",off4[T&0x1F],R);

  strcat(buffer,buf);
  return(PC);
}

unsigned Dasm(unsigned char *memory, char *buffer, unsigned pc)
{
  byte T,M;
  word W;
  char *I;
  char buf[30];
  unsigned PC = pc;

  T = OPCODE(PC); PC++;

  if(T==0x10)
  {
    T = OPCODE(PC); PC++;
    W = (word)(T*2);
    T = codes10[W++];
    I = (char *)mne[T];
    M = codes10[W];
  }
  else if(T==0x11)
  {
    T = OPCODE(PC); PC++;
    W = (word)(T*2);
    T = codes11[W++];
    I = (char *)mne[T];
    M = codes11[W];
  }
  else
  {
    W = (word)(T*2);
    T = codes[W++];
    I = (char *)mne[T];
    M = codes[W];
  }

  switch(M)
  {
    case _nom: sprintf(buffer,"Invalid");break;

    case _imp: sprintf(buffer,"%s", I);break;

    case _imb: T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s #$%02X", I, T);
               break;

    case _imw: W = ARGWORD(PC); PC+=2;
               sprintf(buffer,"%s #$%04X",I,W);
               break;

    case _dir: T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s <$%02X",I,T);
               break;

    case _ext: W = ARGWORD(PC); PC+=2;
               sprintf(buffer,"%s $%04X",I,W);
               break;

    case _ind: sprintf(buffer,"%s ",I);
               PC = index_string(memory, buffer,PC);
               break;

    case _reb: T = ARGBYTE(PC); PC++;
               W = (word)(PC + (signed char)T);
               sprintf(buffer,"%s $%04X",I,W);
               break;

    case _rew: W = ARGWORD(PC); PC+=2;
               W += (word)PC;
               sprintf(buffer,"%s $%04X",I,W);
               break;

    case _r1:  T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s %s,%s",I,exg_tfr[T>>4],exg_tfr[T&0xF]);
               break;

    case _r2:
    case _r3:  buf[0] = '\0';
               T = ARGBYTE(PC); PC++;
               if(T&0x80) strcat(buf,"PC,");
               if(T&0x40)
               {
                 if(M==_r2) strcat(buf,"U,");
                 if(M==_r3) strcat(buf,"S,");
               }
               if(T&0x20) strcat(buf,"Y,");
               if(T&0x10) strcat(buf,"X,");
               if(T&0x08) strcat(buf,"DP,");
               if(T&0x04) strcat(buf,"B,");
               if(T&0x02) strcat(buf,"A,");
               if(T&0x01) strcat(buf,"CC,");
               if(buf[0]!='\0') buf[strlen(buf)-1]='\0';
               sprintf(buffer,"%s %s",I,buf);
               break;

    case _bd:  M = ARGBYTE(PC); PC++;
               T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s #$%02X,<$%02X",I,M,T);
               break;

    case _be:  T = ARGBYTE(PC); PC++;
               W = ARGWORD(PC); PC+=2;
               sprintf(buffer,"%s #$%02X,$%04X",I,T,W);
               break;

    case _bt:  M = ARGBYTE(PC); PC++;
               T = ARGBYTE(PC); PC++;
#if 1
               sprintf(buffer,"%s %s.%01d,<$%02X.%01d",
               I,bit_r[M>>6],M&7,T,(M>>3)&7);
#else
               sprintf(buffer,"%s %s,%01d,%01d,$%02X",
               I,bit_r[M>>6],(M>>3)&7,M&7,T);
#endif
               break;

    case _t1:  T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s %s+,%s+",I,block_r[T>>4],block_r[T&0xF]);
               break;

    case _t2:  T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s %s-,%s-",I,block_r[T>>4],block_r[T&0xF]);
               break;

    case _t3:  T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s %s+,%s",I,block_r[T>>4],block_r[T&0xF]);
               break;

    case _t4:  T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s %s,%s+",I,block_r[T>>4],block_r[T&0xF]);
               break;

    case _iml: W = ARGWORD(PC); PC+=2;
               T = ARGBYTE(PC); PC++;
               M = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s #$%04X%02X%02X",I,W,T,M);
               break;

    case _bi:  T = ARGBYTE(PC); PC++;
               sprintf(buffer,"%s #$%02X,",I,T);
               PC = index_string(memory, buffer,PC);
               break;

    default:   sprintf(buffer,"%s ERROR",I);

  }

  return(PC-pc);
}

char *disasm(unsigned char *memory, unsigned int *pc, int showhex, int showaddr) {
  static char outs[128];
  char buf[30];
  char *outp = outs;
  unsigned int add;
  int i;
  
  outs[0] = '\0';

  if (showaddr) outp += sprintf(outp, "%04X: ",*pc);

  add = Dasm(memory,buf,*pc);

  if (showhex)
  {
    for (i = 0; i<5; i++)
    {
      if (add) {
        add--;
        outp += sprintf(outp, "%02X ",memory[((*pc)++)&0xFFFF]);
      } else outp += sprintf(outp, "   ");
    }
  } else (*pc) += add;

  if ((!showaddr) && (!showhex)) {
    do {
      outp += sprintf(outp, " "); // tab
    } while ((strlen(outp) & 7) != 0);
  }
  outp += sprintf(outp, "%s",buf);
  return outs;
}
