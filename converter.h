// --------------------------------------------------------------------
// Copyright (c) 2010 by Terasic Technologies Inc. 
// --------------------------------------------------------------------
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------
//

#ifndef _CONVERTER_H
#define _CONVERTER_H

#define A_ADN 1                //3'b001,        //nucleotide "A"
#define G_ADN 2                //3'b010,        //nucleotide "G"
#define T_ADN 3                //3'b011,        //nucleotide "T"
#define C_ADN 4                //3'b100;        //nucleotide "C"
#define N_ADN 7                //3'b111;        //nucleotide "N"


#define PCIE_USER_BAR         PCIE_BAR0
#define PCIE_CMD_ADDR         0x00

#define MAX_SUBJECT_NUCLEAR_SIZE   128920
#define MAX_QUERY_NUCLEAR_SIZE     256        //256 nuclear

#define MEM_SIZE                   (128*1024) //128KB
#define MAX_QUERY_SIZE             (MAX_QUERY_NUCLEAR_SIZE*3/8)
#define MAX_SUBJECT_SIZE           (MAX_SUBJECT_NUCLEAR_SIZE*3/8)
//#define MAX_HIT_SCORE_SIZE         (MEM_SIZE - MAX_SUBJECT_SIZE - MAX_QUERY_SIZE)
#define MAX_HIT_SCORE_SIZE         (MEM_SIZE - (127*1024))

#define PCIE_MEM_QUERY_ADDR   0x20000   
#define PCIE_MEM_SUBJECT_ADDR (PCIE_MEM_QUERY_ADDR + MAX_QUERY_SIZE)
//#define PCIE_MEM_SCORE_ADDR   (PCIE_MEM_QUERY_ADDR + (127*1024))
#define PCIE_MEM_SCORE_ADDR   (261168)

#define DATABASE_FILENAME "drosoph.nt"  // 4689005 byte of data
#define DATABASE_LINE_WIDTH 200

#define WRITE_QUERY          0xAA
#define WRITE_SUBJECT        0xBB

#ifdef __cplusplus
extern "C"{
#endif
void printHexString(char indata[]);
int CovertQuery2Bit(char indata[], int indata_length, char outdata[]);


#ifdef __cplusplus
}
#endif



#endif /* _INC_PCIE_H */

