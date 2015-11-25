// -*- coding: utf-8 -*-
/************************************************************************************************
 *
 *  File:           music.h
 *
 *  Compilers:      HT-PICC18 STD
 * 
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    
 *
 *  History:        15.05.2009 -    File created
 *
 ************************************************************************************************/

#ifndef _MUSIC_H
#define _MUSIC_H

#include "osa.h"


//******************************************************************************
//  
//      Constants
// 
//******************************************************************************
//------------------------------------------------------------------------------
#define SOUND_CMD_PAUSE       0x80
#define SOUND_CMD_PLAY_MORE   0x90
#define SOUND_CMD_SET_BASE    0xA0
#define SOUND_CMD_PLAY_SHORT  0xB0  // Play 1/32
#define SOUND_CMD_REPEAT      0xC0
#define SOUND_CMD_STOP        0xE0

//------------------------------------------------------------------------------
#if 0
enum ENUM_NOTES {
    C0, C0_, D0, D0_, E0, F0, F0_, G0, G0_, A0, A0_, B0,
    C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1,
    C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2,
    C3, C3_, D3, D3_, E3, F3, F3_, G3, G3_, A3, A3_, B3,
    C4, C4_, D4, D4_, E4, F4, F4_, G4, G4_
};
#endif

//------------------------------------------------------------------------------
#if 1
enum ENUM_NOTES {
    c0, c0_, d0, d0_, e0, f0, f0_, g0, g0_, a0, a0_, b0,
    c1, c1_, d1, d1_, e1, f1, f1_, g1, g1_, a1, a1_, b1,
    c2, c2_, d2, d2_, e2, f2, f2_, g2, g2_, a2, a2_, b2,
    c3, c3_, d3, d3_, e3, f3, f3_, g3, g3_, a3, a3_, b3,
    c4, c4_, d4, d4_, e4, f4, f4_, g4, g4_
};
#endif

//------------------------------------------------------------------------------
#define play(note,duration)     (((duration-1) << 5) | note)
#define pause(duration)         (SOUND_CMD_PAUSE | (duration-1))
#define playmore(duration)      (SOUND_CMD_PLAY_MORE | (duration-1))
#define repeat(times)           (SOUND_CMD_REPEAT | times)
#define repeatmarker()          (SOUND_CMD_REPEAT)
#define setbase(baseoctave)     (SOUND_CMD_SET_BASE | baseoctave)
#define stop()                  (SOUND_CMD_STOP)

//#define l1                      0x00
#define l2                      0x20
#define l3                      0x40
#define l4                      0x60

#define L1                      (SOUND_CMD_PLAY_MORE | 0)
#define L2                      (SOUND_CMD_PLAY_MORE | 1)
#define L3                      (SOUND_CMD_PLAY_MORE | 2)
#define L4                      (SOUND_CMD_PLAY_MORE | 3)

#define p1                      pause(1)
#define p2                      pause(2)
#define p3                      pause(3)
#define p4                      pause(4)
#define p5                      pause(5)
#define p6                      pause(6)
#define p7                      pause(7)
#define p8                      pause(8)
#define p12                     pause(12)
#define p15                     pause(15)
#define p16                     pause(16)

#define m1                      playmore(1)
#define m2                      playmore(2)
#define m3                      playmore(3)
#define m4                      playmore(4)
#define m5                      playmore(5)
#define m6                      playmore(6)
#define m7                      playmore(7)
#define m8                      playmore(8)


enum GAME_SOUNDS_ENUM
{
    GAME_SOUND_STEP = 1,
    GAME_SOUND_EAT_DIRT,
    GAME_SOUND_BOULDER_FELD,
    GAME_SOUND_JEWEL_FELD,
    GAME_SOUND_EAT_JEWEL,
    GAME_SOUND_PUSH,
    GAME_SOUND_BANG,
    GAME_SOUND_BANG_JEWEL,
    GAME_SOUND_HOME,
    GAME_SOUND_COMPLETE,
    GAME_SOUND_BORN
};



//******************************************************************************
//  
//      Types
// 
//******************************************************************************

//------------------------------------------------------------------------------
//  Synthesizer controller
//------------------------------------------------------------------------------

typedef struct 
{
    unsigned int    nPhase;
    unsigned int    nFreq;
    unsigned char   cFreqShift;
    unsigned char   cVolume;
    unsigned char   cVolumeStep;
    unsigned char   cNoizeVolume;
    unsigned char   cNoizeVolumeStep;
    unsigned char   cState;
    
} TYPE_SOUND;


//------------------------------------------------------------------------------
//  Music controller
//------------------------------------------------------------------------------

typedef struct
{
    TYPE_SOUND      *s;
    unsigned char   cMasterVolume;
    unsigned char   cMasterVolumeStep;
    unsigned int    nCurNote;
    unsigned int    nRepeatPosition;
    unsigned char   cRepeatCounter;
    unsigned char   cPause;
    unsigned char   cBaseNote;
    const char *    notelist;

} TYPE_MUSIC_CHANNEL;


//******************************************************************************
//  
//      Variables
// 
//******************************************************************************

extern const int Freq[];            // Array of music frequencies
extern OST_SMSG  smsg_GameSound;    // Message to generate game sound effects

//******************************************************************************
//  
//      Function prototypes
// 
//******************************************************************************

extern void Task_Music (void);
extern void Task_GameSound (void);


//******************************************************************************
// 
//      THE END (music.h)
// 
//******************************************************************************
#endif

