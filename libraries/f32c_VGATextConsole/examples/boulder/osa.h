#ifndef OSA_H
#define OSA_H

#include <stdint.h>

// DUMMY OS interface

#define OS_Init(x)
#define OS_Task_Create(x,y)
#define OS_EI()
#define OS_Run()
#define OS_Bsem_Set(x)
#define OS_Bsem_Wait(x)
#define OS_Flag_Wait_11(x,y)
#define OS_Flag_Set_0(x,y)
#define OS_SMsg_Send_Now(x,y)
#define OS_SMsg_Wait(x,y)
#define OS_Delay(x)
#define OS_Yield(x)

#define OST_SMSG int
#define BS_MAP_FREE 0
#define near
#define bit uint8_t
#define CLRWDT(x)

#endif
