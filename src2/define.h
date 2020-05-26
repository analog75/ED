/*****************************************************************************
*   Title      : define.h
*   Desc       : ASM definition
*   Author     : HyunJin Kim
*   Ver        : 2018.02.27 - 0.1
*                2018.04.13 - 0.2 Diagonal mode is added
*                2018.04.14 - 0.3 Zigzag mode is added
*                2018.04.15 - 0.4 Pruning mode is added
*                2019.01.12 - 0.5 seq_k is added
*   Description:  
*      
****************************************************************************/
#ifndef DEFINE_H  // Avoid redefinition error
#define DEFINE_H
// "_" means user-defined data type
// Define user defined data type

// SS denotes "schedule and stretch"
typedef enum { FALSE = 0, TRUE = 1 } _BOOL;
//typedef enum { SEQ = 0, SEQP = 1, DIAGONAL = 2, DIAGONALP = 3, 
//               ZIGZAG = 4, ZIGZAGP = 5, PRUNING = 6, PRUNINGP = 7 } MODE_TYPE;
typedef enum { SEQ = 0, DIAGONAL = 1, DIAGONAL_PRUNGING = 2, DIAGONAL_PRUNGING_BIT = 3 } MODE_TYPE;


#endif // End of Avoid redefinition error




