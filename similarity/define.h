/*****************************************************************************
*   Title      : define.h
*   Desc       : ASM definition
*   Author     : HyunJin Kim
*   Description:  
*      
****************************************************************************/
#ifndef DEFINE_H  // Avoid redefinition error
#define DEFINE_H
// "_" means user-defined data type
// Define user defined data type

// SS denotes "schedule and stretch"
typedef enum { FALSE = 0, TRUE = 1 } _BOOL;
typedef enum { SEQ = 0, DIAGONAL = 1, DIAGONAL_PRUNGING = 2 } MODE_TYPE;


#endif // End of Avoid redefinition error




