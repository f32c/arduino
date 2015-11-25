// -*- coding: utf-8 -*-
/************************************************************************************************
 *
 *  File:           game.h
 *
 *  Compilers:      HT-PICC STD
 *                  HT-PICC18 STD
 *                  Microchip C18
 *                  Microchip C30
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    
 *
 *  History:        15.05.2009 -    File created
 *
 ************************************************************************************************/

#ifndef _GAME_H
#define _GAME_H


//******************************************************************************
//  
//      Includes
// 
//******************************************************************************
#include "osa.h"


//******************************************************************************
//  
//      Constants
// 
//******************************************************************************

#define NUMBER_OF_MAPS      3
#define WIN_SIZE_X          16
#define WIN_SIZE_Y          12
#define MAP_SIZE_X          40
#define MAP_SIZE_Y          20

#define MOVING              0x20
#define DIR_MASK            0xC0
#define DIR_UP              0x80
#define DIR_LEFT            0xC0
#define DIR_DOWN            0x00
#define DIR_RIGHT           0x40

#define OBJECT_MASK         0x1C

#define OBJECT_SPACE            0x00
#define OBJECT_DIRT             0x01
#define OBJECT_BOULDER          0x02
#define OBJECT_WALL             0x03
#define OBJECT_TIT_WALL         0x04
#define OBJECT_TIT_WALL_BLANK   0x05
#define OBJECT_BORN             0x44
#define OBJECT_HOME             0x84
#define OBJECT_MAN              0x06
#define OBJECT_MAN_EYES         0x07
#define OBJECT_JEWEL            0x08
#define OBJECT_BUTTERFLY        0x0C
#define OBJECT_FIREFLY          0x10
#define OBJECT_LEFT_MAN         0x14
#define OBJECT_RIGHT_MAN        0x18
#define OBJECT_BANG             0x1C


/////////////////////////////////////////////////////////////////////////////
enum ACTIONS
{
    ACTION_NO_ACTION     ,
    ACTION_CAN_MOVE      ,
    ACTION_BANG          ,
    ACTION_BANG_JEWEL    ,
    ACTION_EAT_JEWEL     ,
    ACTION_EAT_DIRT      ,
    ACTION_BOULDER_FALLS ,
    ACTION_JEWEL_FALLS   ,
    ACTION_INC_DIRECTION ,
    ACTION_DEC_DIRECTION ,
    ACTION_EXCHANGE      ,
    ACTION_STOP_MOVE     ,
    ACTION_PUSH          ,
    ACTION_STOP_FLY      ,
    ACTION_GO_HOME       
};



//******************************************************************************
//  
//      Types
// 
//******************************************************************************


//******************************************************************************
//  
//      Variables
// 
//******************************************************************************


extern      unsigned char       m_Map[MAP_SIZE_Y][MAP_SIZE_X];
extern      char                m_strStatusBar[];
extern near char                m_cWinX, m_cWinY;
extern      unsigned short int  m_Colors[32];
extern      char                m_cCurMap;


//******************************************************************************
//  
//      Function prototypes
// 
//******************************************************************************

extern void Task_Animate (void);
extern void Task_Game (void);
extern void InitMap (char M);

//******************************************************************************
// 
//      THE END (game.h)
// 
//******************************************************************************

#endif

