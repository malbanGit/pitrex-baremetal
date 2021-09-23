/* ---------------------------------------------- Main jump table cineops.c */


/* cineops.c is meant to be included directly into cinecpu.c, or turned
 * into a master dispatcher macro, or some other horrible thing. cineops.c
 * is the state dispatcher, which also causes tate changes. This design
 * keeps things running very fast, since no costly flag calculations
 * need to be performed. Thank Zonn for this twisted but effective
 * idea.
 */

/* extern CINEBYTE opcode */

/* table for state "A" -- Use this table if the last opcode was not
 * an ACC related opcode, and was not a B flip/flop operation.
 * Translation:
 *   Any ACC related routine will use A-reg and go on to opCodeTblAA
 *   Any B flip/flop instructions will jump to opCodeTblB
 *   All other instructions remain in opCodeTblA
 *   JMI will use the current sign of the A-reg
 */
opCodeTblA: 

switch ( rom [ register_PC ] ) {
case 0: goto opLDAimm_A_AA;
case 1: goto opLDAimm_A_AA;
case 2: goto opLDAimm_A_AA;
case 3: goto opLDAimm_A_AA;
case 4: goto opLDAimm_A_AA;
case 5: goto opLDAimm_A_AA;
case 6: goto opLDAimm_A_AA;
case 7: goto opLDAimm_A_AA;
case 8: goto opLDAimm_A_AA;
case 9: goto opLDAimm_A_AA;
case 10: goto opLDAimm_A_AA;
case 11: goto opLDAimm_A_AA;
case 12: goto opLDAimm_A_AA;
case 13: goto opLDAimm_A_AA;
case 14: goto opLDAimm_A_AA;
case 15: goto opLDAimm_A_AA;
case 16: goto opINP_A_AA;
case 17: goto opINP_A_AA;
case 18: goto opINP_A_AA;
case 19: goto opINP_A_AA;
case 20: goto opINP_A_AA;
case 21: goto opINP_A_AA;
case 22: goto opINP_A_AA;
case 23: goto opINP_A_AA;
case 24: goto opINP_A_AA;
case 25: goto opINP_A_AA;
case 26: goto opINP_A_AA;
case 27: goto opINP_A_AA;
case 28: goto opINP_A_AA;
case 29: goto opINP_A_AA;
case 30: goto opINP_A_AA;
case 31: goto opINP_A_AA;
case 32: goto opADDimmX_A_AA;
case 33: goto opADDimm_A_AA;
case 34: goto opADDimm_A_AA;
case 35: goto opADDimm_A_AA;
case 36: goto opADDimm_A_AA;
case 37: goto opADDimm_A_AA;
case 38: goto opADDimm_A_AA;
case 39: goto opADDimm_A_AA;
case 40: goto opADDimm_A_AA;
case 41: goto opADDimm_A_AA;
case 42: goto opADDimm_A_AA;
case 43: goto opADDimm_A_AA;
case 44: goto opADDimm_A_AA;
case 45: goto opADDimm_A_AA;
case 46: goto opADDimm_A_AA;
case 47: goto opADDimm_A_AA;
case 48: goto opSUBimmX_A_AA;
case 49: goto opSUBimm_A_AA;
case 50: goto opSUBimm_A_AA;
case 51: goto opSUBimm_A_AA;
case 52: goto opSUBimm_A_AA;
case 53: goto opSUBimm_A_AA;
case 54: goto opSUBimm_A_AA;
case 55: goto opSUBimm_A_AA;
case 56: goto opSUBimm_A_AA;
case 57: goto opSUBimm_A_AA;
case 58: goto opSUBimm_A_AA;
case 59: goto opSUBimm_A_AA;
case 60: goto opSUBimm_A_AA;
case 61: goto opSUBimm_A_AA;
case 62: goto opSUBimm_A_AA;
case 63: goto opSUBimm_A_AA;
case 64: goto opLDJimm_A_A;
case 65: goto opLDJimm_A_A;
case 66: goto opLDJimm_A_A;
case 67: goto opLDJimm_A_A;
case 68: goto opLDJimm_A_A;
case 69: goto opLDJimm_A_A;
case 70: goto opLDJimm_A_A;
case 71: goto opLDJimm_A_A;
case 72: goto opLDJimm_A_A;
case 73: goto opLDJimm_A_A;
case 74: goto opLDJimm_A_A;
case 75: goto opLDJimm_A_A;
case 76: goto opLDJimm_A_A;
case 77: goto opLDJimm_A_A;
case 78: goto opLDJimm_A_A;
case 79: goto opLDJimm_A_A;
case 80: goto tJPP_A_B;      /* redirector */
case 81: goto tJMI_A_B;      /* redirector */
case 82: goto opJDR_A_B;
case 83: goto opJLT_A_B;
case 84: goto opJEQ_A_B;
case 85: goto opJNC_A_B;
case 86: goto opJA0_A_B;
case 87: goto opNOP_A_B;
case 88: goto opJMP_A_A;
case 89: goto tJMI_A_A;      /* redirector */
case 90: goto opJDR_A_A;
case 91: goto opJLT_A_A;
case 92: goto opJEQ_A_A;
case 93: goto opJNC_A_A;
case 94: goto opJA0_A_A;
case 95: goto opNOP_A_A;
case 96: goto opADDdir_A_AA;
case 97: goto opADDdir_A_AA;
case 98: goto opADDdir_A_AA;
case 99: goto opADDdir_A_AA;
case 100: goto opADDdir_A_AA;
case 101: goto opADDdir_A_AA;
case 102: goto opADDdir_A_AA;
case 103: goto opADDdir_A_AA;
case 104: goto opADDdir_A_AA;
case 105: goto opADDdir_A_AA;
case 106: goto opADDdir_A_AA;
case 107: goto opADDdir_A_AA;
case 108: goto opADDdir_A_AA;
case 109: goto opADDdir_A_AA;
case 110: goto opADDdir_A_AA;
case 111: goto opADDdir_A_AA;
case 112: goto opSUBdir_A_AA;
case 113: goto opSUBdir_A_AA;
case 114: goto opSUBdir_A_AA;
case 115: goto opSUBdir_A_AA;
case 116: goto opSUBdir_A_AA;
case 117: goto opSUBdir_A_AA;
case 118: goto opSUBdir_A_AA;
case 119: goto opSUBdir_A_AA;
case 120: goto opSUBdir_A_AA;
case 121: goto opSUBdir_A_AA;
case 122: goto opSUBdir_A_AA;
case 123: goto opSUBdir_A_AA;
case 124: goto opSUBdir_A_AA;
case 125: goto opSUBdir_A_AA;
case 126: goto opSUBdir_A_AA;
case 127: goto opSUBdir_A_AA;
case 128: goto opLDPimm_A_A;
case 129: goto opLDPimm_A_A;
case 130: goto opLDPimm_A_A;
case 131: goto opLDPimm_A_A;
case 132: goto opLDPimm_A_A;
case 133: goto opLDPimm_A_A;
case 134: goto opLDPimm_A_A;
case 135: goto opLDPimm_A_A;
case 136: goto opLDPimm_A_A;
case 137: goto opLDPimm_A_A;
case 138: goto opLDPimm_A_A;
case 139: goto opLDPimm_A_A;
case 140: goto opLDPimm_A_A;
case 141: goto opLDPimm_A_A;
case 142: goto opLDPimm_A_A;
case 143: goto opLDPimm_A_A;
case 144: goto tOUT_A_A;         /* redirector */
case 145: goto tOUT_A_A;         /* redirector */
case 146: goto tOUT_A_A;         /* redirector */
case 147: goto tOUT_A_A;         /* redirector */
case 148: goto tOUT_A_A;         /* redirector */
case 149: goto tOUT_A_A;         /* redirector */
case 150: goto tOUT_A_A;         /* redirector */
case 151: goto tOUT_A_A;         /* redirector */
case 152: goto tOUT_A_A;         /* redirector */
case 153: goto tOUT_A_A;         /* redirector */
case 154: goto tOUT_A_A;         /* redirector */
case 155: goto tOUT_A_A;         /* redirector */
case 156: goto tOUT_A_A;         /* redirector */
case 157: goto tOUT_A_A;         /* redirector */
case 158: goto tOUT_A_A;         /* redirector */
case 159: goto tOUT_A_A;         /* redirector */
case 160: goto opLDAdir_A_AA;
case 161: goto opLDAdir_A_AA;
case 162: goto opLDAdir_A_AA;
case 163: goto opLDAdir_A_AA;
case 164: goto opLDAdir_A_AA;
case 165: goto opLDAdir_A_AA;
case 166: goto opLDAdir_A_AA;
case 167: goto opLDAdir_A_AA;
case 168: goto opLDAdir_A_AA;
case 169: goto opLDAdir_A_AA;
case 170: goto opLDAdir_A_AA;
case 171: goto opLDAdir_A_AA;
case 172: goto opLDAdir_A_AA;
case 173: goto opLDAdir_A_AA;
case 174: goto opLDAdir_A_AA;
case 175: goto opLDAdir_A_AA;
case 176: goto opCMPdir_A_AA;
case 177: goto opCMPdir_A_AA;
case 178: goto opCMPdir_A_AA;
case 179: goto opCMPdir_A_AA;
case 180: goto opCMPdir_A_AA;
case 181: goto opCMPdir_A_AA;
case 182: goto opCMPdir_A_AA;
case 183: goto opCMPdir_A_AA;
case 184: goto opCMPdir_A_AA;
case 185: goto opCMPdir_A_AA;
case 186: goto opCMPdir_A_AA;
case 187: goto opCMPdir_A_AA;
case 188: goto opCMPdir_A_AA;
case 189: goto opCMPdir_A_AA;
case 190: goto opCMPdir_A_AA;
case 191: goto opCMPdir_A_AA;
case 192: goto opLDIdir_A_A;
case 193: goto opLDIdir_A_A;
case 194: goto opLDIdir_A_A;
case 195: goto opLDIdir_A_A;
case 196: goto opLDIdir_A_A;
case 197: goto opLDIdir_A_A;
case 198: goto opLDIdir_A_A;
case 199: goto opLDIdir_A_A;
case 200: goto opLDIdir_A_A;
case 201: goto opLDIdir_A_A;
case 202: goto opLDIdir_A_A;
case 203: goto opLDIdir_A_A;
case 204: goto opLDIdir_A_A;
case 205: goto opLDIdir_A_A;
case 206: goto opLDIdir_A_A;
case 207: goto opLDIdir_A_A;
case 208: goto opSTAdir_A_A;
case 209: goto opSTAdir_A_A;
case 210: goto opSTAdir_A_A;
case 211: goto opSTAdir_A_A;
case 212: goto opSTAdir_A_A;
case 213: goto opSTAdir_A_A;
case 214: goto opSTAdir_A_A;
case 215: goto opSTAdir_A_A;
case 216: goto opSTAdir_A_A;
case 217: goto opSTAdir_A_A;
case 218: goto opSTAdir_A_A;
case 219: goto opSTAdir_A_A;
case 220: goto opSTAdir_A_A;
case 221: goto opSTAdir_A_A;
case 222: goto opSTAdir_A_A;
case 223: goto opSTAdir_A_A;
case 224: goto opVDR_A_A;
case 225: goto opLDJirg_A_A;
case 226: goto opXLT_A_AA;
case 227: goto opMULirg_A_AA;
case 228: goto opLLT_A_AA;
case 229: goto opWAI_A_A;
case 230: goto opSTAirg_A_A;
case 231: goto opADDirg_A_AA;
case 232: goto opSUBirg_A_AA;
case 233: goto opANDirg_A_AA;
case 234: goto opLDAirg_A_AA;
case 235: goto opLSRe_A_AA;
case 236: goto opLSLe_A_AA;
case 237: goto opASRe_A_AA;
case 238: goto opASRDe_A_AA;
case 239: goto opLSLDe_A_AA;
case 240: goto opVIN_A_A;
case 241: goto opLDJirg_A_A;
case 242: goto opXLT_A_AA;
case 243: goto opMULirg_A_AA;
case 244: goto opLLT_A_AA;
case 245: goto opWAI_A_A;
case 246: goto opSTAirg_A_A;
case 247: goto opAWDirg_A_AA;
case 248: goto opSUBirg_A_AA;
case 249: goto opANDirg_A_AA;
case 250: goto opLDAirg_A_AA;
case 251: goto opLSRf_A_AA;
case 252: goto opLSLf_A_AA;
case 253: goto opASRf_A_AA;
case 254: goto opASRDf_A_AA;
case 255: goto opLSLDf_A_AA;
} /* switch on opcode */

/* opcode table AA -- Use this table if the last opcode was an ACC
 * related opcode. Translation:
 *   Any ACC related routine will use A-reg and remain in OpCodeTblAA
 *   Any B flip/flop instructions will jump to opCodeTblB
 *   All other instructions will jump to opCodeTblA
 *   JMI will use the sign of acc_old
 */

opCodeTblAA:

switch ( rom [ register_PC ] ) {
case 0: goto opLDAimm_AA_AA;
case 1: goto opLDAimm_AA_AA;
case 2: goto opLDAimm_AA_AA;
case 3: goto opLDAimm_AA_AA;
case 4: goto opLDAimm_AA_AA;
case 5: goto opLDAimm_AA_AA;
case 6: goto opLDAimm_AA_AA;
case 7: goto opLDAimm_AA_AA;
case 8: goto opLDAimm_AA_AA;
case 9: goto opLDAimm_AA_AA;
case 10: goto opLDAimm_AA_AA;
case 11: goto opLDAimm_AA_AA;
case 12: goto opLDAimm_AA_AA;
case 13: goto opLDAimm_AA_AA;
case 14: goto opLDAimm_AA_AA;
case 15: goto opLDAimm_AA_AA;
case 16: goto opINP_AA_AA;
case 17: goto opINP_AA_AA;
case 18: goto opINP_AA_AA;
case 19: goto opINP_AA_AA;
case 20: goto opINP_AA_AA;
case 21: goto opINP_AA_AA;
case 22: goto opINP_AA_AA;
case 23: goto opINP_AA_AA;
case 24: goto opINP_AA_AA;
case 25: goto opINP_AA_AA;
case 26: goto opINP_AA_AA;
case 27: goto opINP_AA_AA;
case 28: goto opINP_AA_AA;
case 29: goto opINP_AA_AA;
case 30: goto opINP_AA_AA;
case 31: goto opINP_AA_AA;
case 32: goto opADDimmX_AA_AA;
case 33: goto opADDimm_AA_AA;
case 34: goto opADDimm_AA_AA;
case 35: goto opADDimm_AA_AA;
case 36: goto opADDimm_AA_AA;
case 37: goto opADDimm_AA_AA;
case 38: goto opADDimm_AA_AA;
case 39: goto opADDimm_AA_AA;
case 40: goto opADDimm_AA_AA;
case 41: goto opADDimm_AA_AA;
case 42: goto opADDimm_AA_AA;
case 43: goto opADDimm_AA_AA;
case 44: goto opADDimm_AA_AA;
case 45: goto opADDimm_AA_AA;
case 46: goto opADDimm_AA_AA;
case 47: goto opADDimm_AA_AA;
case 48: goto opSUBimmX_AA_AA;
case 49: goto opSUBimm_AA_AA;
case 50: goto opSUBimm_AA_AA;
case 51: goto opSUBimm_AA_AA;
case 52: goto opSUBimm_AA_AA;
case 53: goto opSUBimm_AA_AA;
case 54: goto opSUBimm_AA_AA;
case 55: goto opSUBimm_AA_AA;
case 56: goto opSUBimm_AA_AA;
case 57: goto opSUBimm_AA_AA;
case 58: goto opSUBimm_AA_AA;
case 59: goto opSUBimm_AA_AA;
case 60: goto opSUBimm_AA_AA;
case 61: goto opSUBimm_AA_AA;
case 62: goto opSUBimm_AA_AA;
case 63: goto opSUBimm_AA_AA;
case 64: goto opLDJimm_AA_A;
case 65: goto opLDJimm_AA_A;
case 66: goto opLDJimm_AA_A;
case 67: goto opLDJimm_AA_A;
case 68: goto opLDJimm_AA_A;
case 69: goto opLDJimm_AA_A;
case 70: goto opLDJimm_AA_A;
case 71: goto opLDJimm_AA_A;
case 72: goto opLDJimm_AA_A;
case 73: goto opLDJimm_AA_A;
case 74: goto opLDJimm_AA_A;
case 75: goto opLDJimm_AA_A;
case 76: goto opLDJimm_AA_A;
case 77: goto opLDJimm_AA_A;
case 78: goto opLDJimm_AA_A;
case 79: goto opLDJimm_AA_A;
case 80: goto tJPP_AA_B;         /* redirector */
case 81: goto tJMI_AA_B;         /* redirector */
case 82: goto opJDR_AA_B;
case 83: goto opJLT_AA_B;
case 84: goto opJEQ_AA_B;
case 85: goto opJNC_AA_B;
case 86: goto opJA0_AA_B;
case 87: goto opNOP_AA_B;
case 88: goto opJMP_AA_A;
case 89: goto tJMI_AA_A;         /* redirector */
case 90: goto opJDR_AA_A;
case 91: goto opJLT_AA_A;
case 92: goto opJEQ_AA_A;
case 93: goto opJNC_AA_A;
case 94: goto opJA0_AA_A;
case 95: goto opNOP_AA_A;
case 96: goto opADDdir_AA_AA;
case 97: goto opADDdir_AA_AA;
case 98: goto opADDdir_AA_AA;
case 99: goto opADDdir_AA_AA;
case 100: goto opADDdir_AA_AA;
case 101: goto opADDdir_AA_AA;
case 102: goto opADDdir_AA_AA;
case 103: goto opADDdir_AA_AA;
case 104: goto opADDdir_AA_AA;
case 105: goto opADDdir_AA_AA;
case 106: goto opADDdir_AA_AA;
case 107: goto opADDdir_AA_AA;
case 108: goto opADDdir_AA_AA;
case 109: goto opADDdir_AA_AA;
case 110: goto opADDdir_AA_AA;
case 111: goto opADDdir_AA_AA;
case 112: goto opSUBdir_AA_AA;
case 113: goto opSUBdir_AA_AA;
case 114: goto opSUBdir_AA_AA;
case 115: goto opSUBdir_AA_AA;
case 116: goto opSUBdir_AA_AA;
case 117: goto opSUBdir_AA_AA;
case 118: goto opSUBdir_AA_AA;
case 119: goto opSUBdir_AA_AA;
case 120: goto opSUBdir_AA_AA;
case 121: goto opSUBdir_AA_AA;
case 122: goto opSUBdir_AA_AA;
case 123: goto opSUBdir_AA_AA;
case 124: goto opSUBdir_AA_AA;
case 125: goto opSUBdir_AA_AA;
case 126: goto opSUBdir_AA_AA;
case 127: goto opSUBdir_AA_AA;
case 128: goto opLDPimm_AA_A;
case 129: goto opLDPimm_AA_A;
case 130: goto opLDPimm_AA_A;
case 131: goto opLDPimm_AA_A;
case 132: goto opLDPimm_AA_A;
case 133: goto opLDPimm_AA_A;
case 134: goto opLDPimm_AA_A;
case 135: goto opLDPimm_AA_A;
case 136: goto opLDPimm_AA_A;
case 137: goto opLDPimm_AA_A;
case 138: goto opLDPimm_AA_A;
case 139: goto opLDPimm_AA_A;
case 140: goto opLDPimm_AA_A;
case 141: goto opLDPimm_AA_A;
case 142: goto opLDPimm_AA_A;
case 143: goto opLDPimm_AA_A;
case 144: goto tOUT_AA_A;          /* redirector */
case 145: goto tOUT_AA_A;          /* redirector */
case 146: goto tOUT_AA_A;          /* redirector */
case 147: goto tOUT_AA_A;          /* redirector */
case 148: goto tOUT_AA_A;          /* redirector */
case 149: goto tOUT_AA_A;          /* redirector */
case 150: goto tOUT_AA_A;          /* redirector */
case 151: goto tOUT_AA_A;          /* redirector */
case 152: goto tOUT_AA_A;          /* redirector */
case 153: goto tOUT_AA_A;          /* redirector */
case 154: goto tOUT_AA_A;          /* redirector */
case 155: goto tOUT_AA_A;          /* redirector */
case 156: goto tOUT_AA_A;          /* redirector */
case 157: goto tOUT_AA_A;          /* redirector */
case 158: goto tOUT_AA_A;          /* redirector */
case 159: goto tOUT_AA_A;          /* redirector */
case 160: goto opLDAdir_AA_AA;
case 161: goto opLDAdir_AA_AA;
case 162: goto opLDAdir_AA_AA;
case 163: goto opLDAdir_AA_AA;
case 164: goto opLDAdir_AA_AA;
case 165: goto opLDAdir_AA_AA;
case 166: goto opLDAdir_AA_AA;
case 167: goto opLDAdir_AA_AA;
case 168: goto opLDAdir_AA_AA;
case 169: goto opLDAdir_AA_AA;
case 170: goto opLDAdir_AA_AA;
case 171: goto opLDAdir_AA_AA;
case 172: goto opLDAdir_AA_AA;
case 173: goto opLDAdir_AA_AA;
case 174: goto opLDAdir_AA_AA;
case 175: goto opLDAdir_AA_AA;
case 176: goto opCMPdir_AA_AA;
case 177: goto opCMPdir_AA_AA;
case 178: goto opCMPdir_AA_AA;
case 179: goto opCMPdir_AA_AA;
case 180: goto opCMPdir_AA_AA;
case 181: goto opCMPdir_AA_AA;
case 182: goto opCMPdir_AA_AA;
case 183: goto opCMPdir_AA_AA;
case 184: goto opCMPdir_AA_AA;
case 185: goto opCMPdir_AA_AA;
case 186: goto opCMPdir_AA_AA;
case 187: goto opCMPdir_AA_AA;
case 188: goto opCMPdir_AA_AA;
case 189: goto opCMPdir_AA_AA;
case 190: goto opCMPdir_AA_AA;
case 191: goto opCMPdir_AA_AA;
case 192: goto opLDIdir_AA_A;
case 193: goto opLDIdir_AA_A;
case 194: goto opLDIdir_AA_A;
case 195: goto opLDIdir_AA_A;
case 196: goto opLDIdir_AA_A;
case 197: goto opLDIdir_AA_A;
case 198: goto opLDIdir_AA_A;
case 199: goto opLDIdir_AA_A;
case 200: goto opLDIdir_AA_A;
case 201: goto opLDIdir_AA_A;
case 202: goto opLDIdir_AA_A;
case 203: goto opLDIdir_AA_A;
case 204: goto opLDIdir_AA_A;
case 205: goto opLDIdir_AA_A;
case 206: goto opLDIdir_AA_A;
case 207: goto opLDIdir_AA_A;
case 208: goto opSTAdir_AA_A;
case 209: goto opSTAdir_AA_A;
case 210: goto opSTAdir_AA_A;
case 211: goto opSTAdir_AA_A;
case 212: goto opSTAdir_AA_A;
case 213: goto opSTAdir_AA_A;
case 214: goto opSTAdir_AA_A;
case 215: goto opSTAdir_AA_A;
case 216: goto opSTAdir_AA_A;
case 217: goto opSTAdir_AA_A;
case 218: goto opSTAdir_AA_A;
case 219: goto opSTAdir_AA_A;
case 220: goto opSTAdir_AA_A;
case 221: goto opSTAdir_AA_A;
case 222: goto opSTAdir_AA_A;
case 223: goto opSTAdir_AA_A;
case 224: goto opVDR_AA_A;
case 225: goto opLDJirg_AA_A;
case 226: goto opXLT_AA_AA;
case 227: goto opMULirg_AA_AA;
case 228: goto opLLT_AA_AA;
case 229: goto opWAI_AA_A;
case 230: goto opSTAirg_AA_A;
case 231: goto opADDirg_AA_AA;
case 232: goto opSUBirg_AA_AA;
case 233: goto opANDirg_AA_AA;
case 234: goto opLDAirg_AA_AA;
case 235: goto opLSRe_AA_AA;
case 236: goto opLSLe_AA_AA;
case 237: goto opASRe_AA_AA;
case 238: goto opASRDe_AA_AA;
case 239: goto opLSLDe_AA_AA;
case 240: goto opVIN_AA_A;
case 241: goto opLDJirg_AA_A;
case 242: goto opXLT_AA_AA;
case 243: goto opMULirg_AA_AA;
case 244: goto opLLT_AA_AA;
case 245: goto opWAI_AA_A;
case 246: goto opSTAirg_AA_A;
case 247: goto opAWDirg_AA_AA;
case 248: goto opSUBirg_AA_AA;
case 249: goto opANDirg_AA_AA;
case 250: goto opLDAirg_AA_AA;
case 251: goto opLSRf_AA_AA;
case 252: goto opLSLf_AA_AA;
case 253: goto opASRf_AA_AA;
case 254: goto opASRDf_AA_AA;
case 255: goto opLSLDf_AA_AA;
} /* switch on opcode */

/* opcode table B -- use this table if the last opcode was a B-reg flip/flop
 * Translation:
 *   Any ACC related routine uses B-reg, and goes to opCodeTblAA
 *   All other instructions will jump to table opCodeTblBB (including
 *     B flip/flop related instructions)
 *   JMI will use current sign of the A-reg
 */

opCodeTblB:

switch ( rom [ register_PC ] ) {
case 0: goto opLDAimm_B_AA;
case 1: goto opLDAimm_B_AA;
case 2: goto opLDAimm_B_AA;
case 3: goto opLDAimm_B_AA;
case 4: goto opLDAimm_B_AA;
case 5: goto opLDAimm_B_AA;
case 6: goto opLDAimm_B_AA;
case 7: goto opLDAimm_B_AA;
case 8: goto opLDAimm_B_AA;
case 9: goto opLDAimm_B_AA;
case 10: goto opLDAimm_B_AA;
case 11: goto opLDAimm_B_AA;
case 12: goto opLDAimm_B_AA;
case 13: goto opLDAimm_B_AA;
case 14: goto opLDAimm_B_AA;
case 15: goto opLDAimm_B_AA;
case 16: goto opINP_B_AA;
case 17: goto opINP_B_AA;
case 18: goto opINP_B_AA;
case 19: goto opINP_B_AA;
case 20: goto opINP_B_AA;
case 21: goto opINP_B_AA;
case 22: goto opINP_B_AA;
case 23: goto opINP_B_AA;
case 24: goto opINP_B_AA;
case 25: goto opINP_B_AA;
case 26: goto opINP_B_AA;
case 27: goto opINP_B_AA;
case 28: goto opINP_B_AA;
case 29: goto opINP_B_AA;
case 30: goto opINP_B_AA;
case 31: goto opINP_B_AA;
case 32: goto opADDimmX_B_AA;
case 33: goto opADDimm_B_AA;
case 34: goto opADDimm_B_AA;
case 35: goto opADDimm_B_AA;
case 36: goto opADDimm_B_AA;
case 37: goto opADDimm_B_AA;
case 38: goto opADDimm_B_AA;
case 39: goto opADDimm_B_AA;
case 40: goto opADDimm_B_AA;
case 41: goto opADDimm_B_AA;
case 42: goto opADDimm_B_AA;
case 43: goto opADDimm_B_AA;
case 44: goto opADDimm_B_AA;
case 45: goto opADDimm_B_AA;
case 46: goto opADDimm_B_AA;
case 47: goto opADDimm_B_AA;
case 48: goto opSUBimmX_B_AA;
case 49: goto opSUBimm_B_AA;
case 50: goto opSUBimm_B_AA;
case 51: goto opSUBimm_B_AA;
case 52: goto opSUBimm_B_AA;
case 53: goto opSUBimm_B_AA;
case 54: goto opSUBimm_B_AA;
case 55: goto opSUBimm_B_AA;
case 56: goto opSUBimm_B_AA;
case 57: goto opSUBimm_B_AA;
case 58: goto opSUBimm_B_AA;
case 59: goto opSUBimm_B_AA;
case 60: goto opSUBimm_B_AA;
case 61: goto opSUBimm_B_AA;
case 62: goto opSUBimm_B_AA;
case 63: goto opSUBimm_B_AA;
case 64: goto opLDJimm_B_BB;
case 65: goto opLDJimm_B_BB;
case 66: goto opLDJimm_B_BB;
case 67: goto opLDJimm_B_BB;
case 68: goto opLDJimm_B_BB;
case 69: goto opLDJimm_B_BB;
case 70: goto opLDJimm_B_BB;
case 71: goto opLDJimm_B_BB;
case 72: goto opLDJimm_B_BB;
case 73: goto opLDJimm_B_BB;
case 74: goto opLDJimm_B_BB;
case 75: goto opLDJimm_B_BB;
case 76: goto opLDJimm_B_BB;
case 77: goto opLDJimm_B_BB;
case 78: goto opLDJimm_B_BB;
case 79: goto opLDJimm_B_BB;
case 80: goto tJPP_B_BB;          /* redirector */
case 81: goto tJMI_B_BB1;         /* redirector */
case 82: goto opJDR_B_BB;
case 83: goto opJLT_B_BB;
case 84: goto opJEQ_B_BB;
case 85: goto opJNC_B_BB;
case 86: goto opJA0_B_BB;
case 87: goto opNOP_B_BB;
case 88: goto opJMP_B_BB;
case 89: goto tJMI_B_BB2;         /* redirector */
case 90: goto opJDR_B_BB;
case 91: goto opJLT_B_BB;
case 92: goto opJEQ_B_BB;
case 93: goto opJNC_B_BB;
case 94: goto opJA0_B_BB;
case 95: goto opNOP_B_BB;
case 96: goto opADDdir_B_AA;
case 97: goto opADDdir_B_AA;
case 98: goto opADDdir_B_AA;
case 99: goto opADDdir_B_AA;
case 100: goto opADDdir_B_AA;
case 101: goto opADDdir_B_AA;
case 102: goto opADDdir_B_AA;
case 103: goto opADDdir_B_AA;
case 104: goto opADDdir_B_AA;
case 105: goto opADDdir_B_AA;
case 106: goto opADDdir_B_AA;
case 107: goto opADDdir_B_AA;
case 108: goto opADDdir_B_AA;
case 109: goto opADDdir_B_AA;
case 110: goto opADDdir_B_AA;
case 111: goto opADDdir_B_AA;
case 112: goto opSUBdir_B_AA;
case 113: goto opSUBdir_B_AA;
case 114: goto opSUBdir_B_AA;
case 115: goto opSUBdir_B_AA;
case 116: goto opSUBdir_B_AA;
case 117: goto opSUBdir_B_AA;
case 118: goto opSUBdir_B_AA;
case 119: goto opSUBdir_B_AA;
case 120: goto opSUBdir_B_AA;
case 121: goto opSUBdir_B_AA;
case 122: goto opSUBdir_B_AA;
case 123: goto opSUBdir_B_AA;
case 124: goto opSUBdir_B_AA;
case 125: goto opSUBdir_B_AA;
case 126: goto opSUBdir_B_AA;
case 127: goto opSUBdir_B_AA;
case 128: goto opLDPimm_B_BB;
case 129: goto opLDPimm_B_BB;
case 130: goto opLDPimm_B_BB;
case 131: goto opLDPimm_B_BB;
case 132: goto opLDPimm_B_BB;
case 133: goto opLDPimm_B_BB;
case 134: goto opLDPimm_B_BB;
case 135: goto opLDPimm_B_BB;
case 136: goto opLDPimm_B_BB;
case 137: goto opLDPimm_B_BB;
case 138: goto opLDPimm_B_BB;
case 139: goto opLDPimm_B_BB;
case 140: goto opLDPimm_B_BB;
case 141: goto opLDPimm_B_BB;
case 142: goto opLDPimm_B_BB;
case 143: goto opLDPimm_B_BB;
case 144: goto tOUT_B_BB;         /* redirector */
case 145: goto tOUT_B_BB;         /* redirector */
case 146: goto tOUT_B_BB;         /* redirector */
case 147: goto tOUT_B_BB;         /* redirector */
case 148: goto tOUT_B_BB;         /* redirector */
case 149: goto tOUT_B_BB;         /* redirector */
case 150: goto tOUT_B_BB;         /* redirector */
case 151: goto tOUT_B_BB;         /* redirector */
case 152: goto tOUT_B_BB;         /* redirector */
case 153: goto tOUT_B_BB;         /* redirector */
case 154: goto tOUT_B_BB;         /* redirector */
case 155: goto tOUT_B_BB;         /* redirector */
case 156: goto tOUT_B_BB;         /* redirector */
case 157: goto tOUT_B_BB;         /* redirector */
case 158: goto tOUT_B_BB;         /* redirector */
case 159: goto tOUT_B_BB;         /* redirector */
case 160: goto opLDAdir_B_AA;
case 161: goto opLDAdir_B_AA;
case 162: goto opLDAdir_B_AA;
case 163: goto opLDAdir_B_AA;
case 164: goto opLDAdir_B_AA;
case 165: goto opLDAdir_B_AA;
case 166: goto opLDAdir_B_AA;
case 167: goto opLDAdir_B_AA;
case 168: goto opLDAdir_B_AA;
case 169: goto opLDAdir_B_AA;
case 170: goto opLDAdir_B_AA;
case 171: goto opLDAdir_B_AA;
case 172: goto opLDAdir_B_AA;
case 173: goto opLDAdir_B_AA;
case 174: goto opLDAdir_B_AA;
case 175: goto opLDAdir_B_AA;
case 176: goto opCMPdir_B_AA;
case 177: goto opCMPdir_B_AA;
case 178: goto opCMPdir_B_AA;
case 179: goto opCMPdir_B_AA;
case 180: goto opCMPdir_B_AA;
case 181: goto opCMPdir_B_AA;
case 182: goto opCMPdir_B_AA;
case 183: goto opCMPdir_B_AA;
case 184: goto opCMPdir_B_AA;
case 185: goto opCMPdir_B_AA;
case 186: goto opCMPdir_B_AA;
case 187: goto opCMPdir_B_AA;
case 188: goto opCMPdir_B_AA;
case 189: goto opCMPdir_B_AA;
case 190: goto opCMPdir_B_AA;
case 191: goto opCMPdir_B_AA;
case 192: goto opLDIdir_B_BB;
case 193: goto opLDIdir_B_BB;
case 194: goto opLDIdir_B_BB;
case 195: goto opLDIdir_B_BB;
case 196: goto opLDIdir_B_BB;
case 197: goto opLDIdir_B_BB;
case 198: goto opLDIdir_B_BB;
case 199: goto opLDIdir_B_BB;
case 200: goto opLDIdir_B_BB;
case 201: goto opLDIdir_B_BB;
case 202: goto opLDIdir_B_BB;
case 203: goto opLDIdir_B_BB;
case 204: goto opLDIdir_B_BB;
case 205: goto opLDIdir_B_BB;
case 206: goto opLDIdir_B_BB;
case 207: goto opLDIdir_B_BB;
case 208: goto opSTAdir_B_BB;
case 209: goto opSTAdir_B_BB;
case 210: goto opSTAdir_B_BB;
case 211: goto opSTAdir_B_BB;
case 212: goto opSTAdir_B_BB;
case 213: goto opSTAdir_B_BB;
case 214: goto opSTAdir_B_BB;
case 215: goto opSTAdir_B_BB;
case 216: goto opSTAdir_B_BB;
case 217: goto opSTAdir_B_BB;
case 218: goto opSTAdir_B_BB;
case 219: goto opSTAdir_B_BB;
case 220: goto opSTAdir_B_BB;
case 221: goto opSTAdir_B_BB;
case 222: goto opSTAdir_B_BB;
case 223: goto opSTAdir_B_BB;
case 224: goto opVDR_B_BB;
case 225: goto opLDJirg_B_BB;
case 226: goto opXLT_B_AA;
case 227: goto opMULirg_B_AA;
case 228: goto opLLT_B_AA;
case 229: goto opWAI_B_BB;
case 230: goto opSTAirg_B_BB;
case 231: goto opADDirg_B_AA;
case 232: goto opSUBirg_B_AA;
case 233: goto opANDirg_B_AA;
case 234: goto opLDAirg_B_AA;
case 235: goto opLSRe_B_AA;
case 236: goto opLSLe_B_AA;
case 237: goto opASRe_B_AA;
case 238: goto opASRDe_B_AA;
case 239: goto opLSLDe_B_AA;
case 240: goto opVIN_B_BB;
case 241: goto opLDJirg_B_BB;
case 242: goto opXLT_B_AA;
case 243: goto opMULirg_B_AA;
case 244: goto opLLT_B_AA;
case 245: goto opWAI_B_BB;
case 246: goto opSTAirg_B_BB;
case 247: goto opAWDirg_B_AA;
case 248: goto opSUBirg_B_AA;
case 249: goto opANDirg_B_AA;
case 250: goto opLDAirg_B_AA;
case 251: goto opLSRf_B_AA;
case 252: goto opLSLf_B_AA;
case 253: goto opASRf_B_AA;
case 254: goto opASRDf_B_AA;
case 255: goto opLSLDf_B_AA;
} /* switch on opcode */

/* opcode table BB -- use this table if the last opcode was not an ACC
 * related opcode, but instruction before that was a B-flip/flop instruction.
 * Translation:
 *   Any ACC related routine will use A-reg and go to opCodeTblAA
 *   Any B flip/flop instructions will jump to opCodeTblB
 *   All other instructions will jump to table opCodeTblA
 *   JMI will use the current state of the B-reg
 */

opCodeTblBB:

switch ( rom [ register_PC ] ) {
case 0: goto opLDAimm_BB_AA;
case 1: goto opLDAimm_BB_AA;
case 2: goto opLDAimm_BB_AA;
case 3: goto opLDAimm_BB_AA;
case 4: goto opLDAimm_BB_AA;
case 5: goto opLDAimm_BB_AA;
case 6: goto opLDAimm_BB_AA;
case 7: goto opLDAimm_BB_AA;
case 8: goto opLDAimm_BB_AA;
case 9: goto opLDAimm_BB_AA;
case 10: goto opLDAimm_BB_AA;
case 11: goto opLDAimm_BB_AA;
case 12: goto opLDAimm_BB_AA;
case 13: goto opLDAimm_BB_AA;
case 14: goto opLDAimm_BB_AA;
case 15: goto opLDAimm_BB_AA;
case 16: goto opINP_BB_AA;
case 17: goto opINP_BB_AA;
case 18: goto opINP_BB_AA;
case 19: goto opINP_BB_AA;
case 20: goto opINP_BB_AA;
case 21: goto opINP_BB_AA;
case 22: goto opINP_BB_AA;
case 23: goto opINP_BB_AA;
case 24: goto opINP_BB_AA;
case 25: goto opINP_BB_AA;
case 26: goto opINP_BB_AA;
case 27: goto opINP_BB_AA;
case 28: goto opINP_BB_AA;
case 29: goto opINP_BB_AA;
case 30: goto opINP_BB_AA;
case 31: goto opINP_BB_AA;
case 32: goto opADDimmX_BB_AA;
case 33: goto opADDimm_BB_AA;
case 34: goto opADDimm_BB_AA;
case 35: goto opADDimm_BB_AA;
case 36: goto opADDimm_BB_AA;
case 37: goto opADDimm_BB_AA;
case 38: goto opADDimm_BB_AA;
case 39: goto opADDimm_BB_AA;
case 40: goto opADDimm_BB_AA;
case 41: goto opADDimm_BB_AA;
case 42: goto opADDimm_BB_AA;
case 43: goto opADDimm_BB_AA;
case 44: goto opADDimm_BB_AA;
case 45: goto opADDimm_BB_AA;
case 46: goto opADDimm_BB_AA;
case 47: goto opADDimm_BB_AA;
case 48: goto opSUBimmX_BB_AA;
case 49: goto opSUBimm_BB_AA;
case 50: goto opSUBimm_BB_AA;
case 51: goto opSUBimm_BB_AA;
case 52: goto opSUBimm_BB_AA;
case 53: goto opSUBimm_BB_AA;
case 54: goto opSUBimm_BB_AA;
case 55: goto opSUBimm_BB_AA;
case 56: goto opSUBimm_BB_AA;
case 57: goto opSUBimm_BB_AA;
case 58: goto opSUBimm_BB_AA;
case 59: goto opSUBimm_BB_AA;
case 60: goto opSUBimm_BB_AA;
case 61: goto opSUBimm_BB_AA;
case 62: goto opSUBimm_BB_AA;
case 63: goto opSUBimm_BB_AA;
case 64: goto opLDJimm_BB_A;
case 65: goto opLDJimm_BB_A;
case 66: goto opLDJimm_BB_A;
case 67: goto opLDJimm_BB_A;
case 68: goto opLDJimm_BB_A;
case 69: goto opLDJimm_BB_A;
case 70: goto opLDJimm_BB_A;
case 71: goto opLDJimm_BB_A;
case 72: goto opLDJimm_BB_A;
case 73: goto opLDJimm_BB_A;
case 74: goto opLDJimm_BB_A;
case 75: goto opLDJimm_BB_A;
case 76: goto opLDJimm_BB_A;
case 77: goto opLDJimm_BB_A;
case 78: goto opLDJimm_BB_A;
case 79: goto opLDJimm_BB_A;
case 80: goto tJPP_BB_B;         /* redirector */
case 81: goto tJMI_BB_B;         /* redirector */
case 82: goto opJDR_BB_B;
case 83: goto opJLT_BB_B;
case 84: goto opJEQ_BB_B;
case 85: goto opJNC_BB_B;
case 86: goto opJA0_BB_B;
case 87: goto opNOP_BB_B;
case 88: goto opJMP_BB_A;
case 89: goto tJMI_BB_A;         /* redirector */
case 90: goto opJDR_BB_A;
case 91: goto opJLT_BB_A;
case 92: goto opJEQ_BB_A;
case 93: goto opJNC_BB_A;
case 94: goto opJA0_BB_A;
case 95: goto opNOP_BB_A;
case 96: goto opADDdir_BB_AA;
case 97: goto opADDdir_BB_AA;
case 98: goto opADDdir_BB_AA;
case 99: goto opADDdir_BB_AA;
case 100: goto opADDdir_BB_AA;
case 101: goto opADDdir_BB_AA;
case 102: goto opADDdir_BB_AA;
case 103: goto opADDdir_BB_AA;
case 104: goto opADDdir_BB_AA;
case 105: goto opADDdir_BB_AA;
case 106: goto opADDdir_BB_AA;
case 107: goto opADDdir_BB_AA;
case 108: goto opADDdir_BB_AA;
case 109: goto opADDdir_BB_AA;
case 110: goto opADDdir_BB_AA;
case 111: goto opADDdir_BB_AA;
case 112: goto opSUBdir_BB_AA;
case 113: goto opSUBdir_BB_AA;
case 114: goto opSUBdir_BB_AA;
case 115: goto opSUBdir_BB_AA;
case 116: goto opSUBdir_BB_AA;
case 117: goto opSUBdir_BB_AA;
case 118: goto opSUBdir_BB_AA;
case 119: goto opSUBdir_BB_AA;
case 120: goto opSUBdir_BB_AA;
case 121: goto opSUBdir_BB_AA;
case 122: goto opSUBdir_BB_AA;
case 123: goto opSUBdir_BB_AA;
case 124: goto opSUBdir_BB_AA;
case 125: goto opSUBdir_BB_AA;
case 126: goto opSUBdir_BB_AA;
case 127: goto opSUBdir_BB_AA;
case 128: goto opLDPimm_BB_A;
case 129: goto opLDPimm_BB_A;
case 130: goto opLDPimm_BB_A;
case 131: goto opLDPimm_BB_A;
case 132: goto opLDPimm_BB_A;
case 133: goto opLDPimm_BB_A;
case 134: goto opLDPimm_BB_A;
case 135: goto opLDPimm_BB_A;
case 136: goto opLDPimm_BB_A;
case 137: goto opLDPimm_BB_A;
case 138: goto opLDPimm_BB_A;
case 139: goto opLDPimm_BB_A;
case 140: goto opLDPimm_BB_A;
case 141: goto opLDPimm_BB_A;
case 142: goto opLDPimm_BB_A;
case 143: goto opLDPimm_BB_A;
case 144: goto tOUT_BB_A;         /* redirector */
case 145: goto tOUT_BB_A;         /* redirector */
case 146: goto tOUT_BB_A;         /* redirector */
case 147: goto tOUT_BB_A;         /* redirector */
case 148: goto tOUT_BB_A;         /* redirector */
case 149: goto tOUT_BB_A;         /* redirector */
case 150: goto tOUT_BB_A;         /* redirector */
case 151: goto tOUT_BB_A;         /* redirector */
case 152: goto tOUT_BB_A;         /* redirector */
case 153: goto tOUT_BB_A;         /* redirector */
case 154: goto tOUT_BB_A;         /* redirector */
case 155: goto tOUT_BB_A;         /* redirector */
case 156: goto tOUT_BB_A;         /* redirector */
case 157: goto tOUT_BB_A;         /* redirector */
case 158: goto tOUT_BB_A;         /* redirector */
case 159: goto tOUT_BB_A;         /* redirector */
case 160: goto opLDAdir_BB_AA;
case 161: goto opLDAdir_BB_AA;
case 162: goto opLDAdir_BB_AA;
case 163: goto opLDAdir_BB_AA;
case 164: goto opLDAdir_BB_AA;
case 165: goto opLDAdir_BB_AA;
case 166: goto opLDAdir_BB_AA;
case 167: goto opLDAdir_BB_AA;
case 168: goto opLDAdir_BB_AA;
case 169: goto opLDAdir_BB_AA;
case 170: goto opLDAdir_BB_AA;
case 171: goto opLDAdir_BB_AA;
case 172: goto opLDAdir_BB_AA;
case 173: goto opLDAdir_BB_AA;
case 174: goto opLDAdir_BB_AA;
case 175: goto opLDAdir_BB_AA;
case 176: goto opCMPdir_BB_AA;
case 177: goto opCMPdir_BB_AA;
case 178: goto opCMPdir_BB_AA;
case 179: goto opCMPdir_BB_AA;
case 180: goto opCMPdir_BB_AA;
case 181: goto opCMPdir_BB_AA;
case 182: goto opCMPdir_BB_AA;
case 183: goto opCMPdir_BB_AA;
case 184: goto opCMPdir_BB_AA;
case 185: goto opCMPdir_BB_AA;
case 186: goto opCMPdir_BB_AA;
case 187: goto opCMPdir_BB_AA;
case 188: goto opCMPdir_BB_AA;
case 189: goto opCMPdir_BB_AA;
case 190: goto opCMPdir_BB_AA;
case 191: goto opCMPdir_BB_AA;
case 192: goto opLDIdir_BB_A;
case 193: goto opLDIdir_BB_A;
case 194: goto opLDIdir_BB_A;
case 195: goto opLDIdir_BB_A;
case 196: goto opLDIdir_BB_A;
case 197: goto opLDIdir_BB_A;
case 198: goto opLDIdir_BB_A;
case 199: goto opLDIdir_BB_A;
case 200: goto opLDIdir_BB_A;
case 201: goto opLDIdir_BB_A;
case 202: goto opLDIdir_BB_A;
case 203: goto opLDIdir_BB_A;
case 204: goto opLDIdir_BB_A;
case 205: goto opLDIdir_BB_A;
case 206: goto opLDIdir_BB_A;
case 207: goto opLDIdir_BB_A;
case 208: goto opSTAdir_BB_A;
case 209: goto opSTAdir_BB_A;
case 210: goto opSTAdir_BB_A;
case 211: goto opSTAdir_BB_A;
case 212: goto opSTAdir_BB_A;
case 213: goto opSTAdir_BB_A;
case 214: goto opSTAdir_BB_A;
case 215: goto opSTAdir_BB_A;
case 216: goto opSTAdir_BB_A;
case 217: goto opSTAdir_BB_A;
case 218: goto opSTAdir_BB_A;
case 219: goto opSTAdir_BB_A;
case 220: goto opSTAdir_BB_A;
case 221: goto opSTAdir_BB_A;
case 222: goto opSTAdir_BB_A;
case 223: goto opSTAdir_BB_A;
case 224: goto opVDR_BB_A;
case 225: goto opLDJirg_BB_A;
case 226: goto opXLT_BB_AA;
case 227: goto opMULirg_BB_AA;
case 228: goto opLLT_BB_AA;
case 229: goto opWAI_BB_A;
case 230: goto opSTAirg_BB_A;
case 231: goto opADDirg_BB_AA;
case 232: goto opSUBirg_BB_AA;
case 233: goto opANDirg_BB_AA;
case 234: goto opLDAirg_BB_AA;
case 235: goto opLSRe_BB_AA;
case 236: goto opLSLe_BB_AA;
case 237: goto opASRe_BB_AA;
case 238: goto opASRDe_BB_AA;
case 239: goto opLSLDe_BB_AA;
case 240: goto opVIN_BB_A;
case 241: goto opLDJirg_BB_A;
case 242: goto opXLT_BB_AA;
case 243: goto opMULirg_BB_AA;
case 244: goto opLLT_BB_AA;
case 245: goto opWAI_BB_A;
case 246: goto opSTAirg_BB_A;
case 247: goto opAWDirg_BB_AA;
case 248: goto opSUBirg_BB_AA;
case 249: goto opANDirg_BB_AA;
case 250: goto opLDAirg_BB_AA;
case 251: goto opLSRf_BB_AA;
case 252: goto opLSLf_BB_AA;
case 253: goto opASRf_BB_AA;
case 254: goto opASRDf_BB_AA;
case 255: goto opLSLDf_BB_AA;
} /* switch on opcode */




