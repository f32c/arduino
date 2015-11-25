// -*- coding: utf-8 -*-
/************************************************************************************************
 *
 *  File:           const.h
 *
 *  Compiler:       HT-PICC18 STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    
 *
 *  History:        15.05.2009 -    File created
 *
 ************************************************************************************************/
#ifndef _CONST_H
#define _CONST_H



//******************************************************************************
//  
//      Includes
// 
//******************************************************************************

//#include <pic18.h>
#include "osa.h"




//******************************************************************************
//  
//      Constants
// 
//******************************************************************************


//------------------------------------------------------------------------------
//  general purpose constants
//------------------------------------------------------------------------------

#define false   0
#define true    1


#define     Hz      * 256L * 256 / (31500/2) + 0.5
#define     ms_OS   / 16


//------------------------------------------------------------------------------
//  pin constants
//------------------------------------------------------------------------------

#define pin_BUTTON_UP       3
#define pin_BUTTON_DOWN     2
#define pin_BUTTON_LEFT     1
#define pin_BUTTON_RIGHT    0
#define pin_BUTTON_FIRE     16

//------------------------------------------------------------------------------
//  Color constants (depend of color controlling pins)
//------------------------------------------------------------------------------

#define CL_BRIGHTNESS   0x08

#define CL_BLACK        0
#define CL_BLUE         1
#define CL_GREEN        2
#define CL_CYAN         3
#define CL_RED          4
#define CL_MAGENTA      5
#define CL_YELLOW       6
#define CL_WHITE        7
#define CL_BLACK2       (CL_BLACK | CL_BRIGHTNESS)
#define CL_LTBLUE       (CL_BLUE | CL_BRIGHTNESS)
#define CL_LTGREEN      (CL_GREEN | CL_BRIGHTNESS)
#define CL_LTCYAN       (CL_CYAN | CL_BRIGHTNESS)
#define CL_LTRED        (CL_RED | CL_BRIGHTNESS)
#define CL_LTMAGENTA    (CL_MAGENTA | CL_BRIGHTNESS)
#define CL_LTYELLOW     (CL_YELLOW | CL_BRIGHTNESS)
#define CL_LTWHITE      (CL_WHITE | CL_BRIGHTNESS)



//------------------------------------------------------------------------------
//  Buttons definitions
//------------------------------------------------------------------------------
#define BUTTON_UP       (m_cButtons & 0x01)
#define BUTTON_DOWN     (m_cButtons & 0x02)
#define BUTTON_LEFT     (m_cButtons & 0x04)
#define BUTTON_RIGHT    (m_cButtons & 0x08)
#define BUTTON_FIRE     (m_cButtons & 0x10)

//******************************************************************************
//  
//      Variables
// 
//******************************************************************************

extern near          char  i_cStep;
extern near unsigned int   i_wRandom;
extern               char  m_cButtons;


//******************************************************************************
// 
//      THE END (const.h)
// 
//******************************************************************************

#endif

