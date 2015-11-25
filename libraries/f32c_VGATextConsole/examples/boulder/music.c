// -*- coding: utf-8 -*-
/************************************************************************************************
 *
 *  File:           music.c
 *
 *  Compilers:      HT-PICC18 STD
 *                  
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    This file contains code for playng a music and to rpouce game
 *                  sound effects
 *
 *  History:        15.05.2009 -    File created
 *
 ************************************************************************************************/



//******************************************************************************
//  
//      Includes
// 
//******************************************************************************

#include "const.h"
#include "osa.h"
#include "music.h"


#include "freq.h"
#include "musicdata.h"



//******************************************************************************
//  
//      Variables
// 
//******************************************************************************

        OST_SMSG            smsg_GameSound;     // Simple msg to get command
                                                // from Task_GameStep
        TYPE_MUSIC_CHANNEL  M[4];               // Music control variables

near    TYPE_SOUND          s[5];               // Synthesizer variables. Used
                                                // in interrupt to produce sounds.


//******************************************************************************
//  
//      Function prototypes
// 
//******************************************************************************

void    InitSound (TYPE_SOUND *ps);
bit     WorkMusic (TYPE_MUSIC_CHANNEL *pm);
void    InitMusicChannel (TYPE_MUSIC_CHANNEL *pm, TYPE_SOUND *ps, const char *pnotes);


//******************************************************************************
//  
//      Functions
// 
//******************************************************************************



/********************************************************************************
 *
 *  TASK: Task_Music
 *
 *------------------------------------------------------------------------------
 *
 *  description:    generate music
 *
 ********************************************************************************/

void Task_Music (void)
{
    static char s_cTemp;
    static bit s_bNeedRestart;
    


    s_bNeedRestart = 1;

    // for (;;)
    {
        if (s_bNeedRestart)
        {
            InitMusicChannel(&M[0], &s[0], notes_ch0);
            InitMusicChannel(&M[1], &s[1], notes_ch1);
            InitMusicChannel(&M[2], &s[2], notes_ch2);
            InitMusicChannel(&M[3], &s[3], notes_ch3);
            s_bNeedRestart = 0;
        }
        OS_Bsem_Wait(BS_MUSIC_START);
        OS_Bsem_Set(BS_MUSIC_START);

        OS_Delay(112 ms_OS);

        s_cTemp = 0;

        s_cTemp |= WorkMusic(&M[0]);     
        OS_Yield();

        s_cTemp |= WorkMusic(&M[1]);     
        OS_Yield();

        s_cTemp |= WorkMusic(&M[2]);     
        OS_Yield();

        s_cTemp |= WorkMusic(&M[3]);     
        OS_Yield();

        if (!s_cTemp) s_bNeedRestart = 1;


    }
}





/********************************************************************************
 *
 *  TASK: Task_GameSound
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Produce game sounds
 *
 ********************************************************************************/

void Task_GameSound (void)
{
    static OST_SMSG sound;
    static char n;

    InitSound(&s[4]);

    for (;;)
    {
        OS_SMsg_Wait(smsg_GameSound, sound);
        OS_Delay(1);
        switch (sound)
        {
            case GAME_SOUND_STEP:
            
                s[4].nFreq          = (int)(50 Hz);
                s[4].cNoizeVolume   = 0;
                s[4].nPhase         = 0;
                s[4].cVolume        = 90;
                OS_Delay(1);
                break;

            case GAME_SOUND_EAT_DIRT:
            
                s[4].nFreq          = (int)(100 Hz);
                s[4].cNoizeVolume   = 40;
                s[4].nPhase         = 0;
                s[4].cVolume        = 30;
                OS_Delay(1);
                break;

            case GAME_SOUND_EAT_JEWEL:
            
                s[4].nFreq          = (int)(5000 Hz);
                s[4].cNoizeVolume   = 0;
                s[4].nPhase         = 0;
                s[4].cVolume        = 100;
                OS_Delay(1);
                break;

            case GAME_SOUND_BOULDER_FELD:
            
                s[4].nFreq          = (int)(100 Hz) + (i_wRandom & 0x3F);
                s[4].cNoizeVolume   = 20;
                s[4].nPhase         = 0;
                s[4].cVolume        = 80;
                OS_Delay(1);
                break;

            case GAME_SOUND_JEWEL_FELD:
            
                s[4].nFreq          = (int)(2500 Hz) + (i_wRandom & 0x1FF);
                s[4].cNoizeVolume   = 0;
                s[4].nPhase         = 0;
                s[4].cVolume        = 60;
                s[4].cVolumeStep    = 10;
                s[4].cState         = 1;
                OS_Delay(7);
                break;

            case GAME_SOUND_PUSH:
            
                s[4].nFreq          = (int)(100 Hz);
                s[4].cNoizeVolume   = 20;
                s[4].nPhase         = 0;
                s[4].cVolume        = 100;
                s[4].cFreqShift     = 10;
                OS_Delay(2);
                break;

            case GAME_SOUND_BANG:
            
                s[4].nFreq          = (int)(80 Hz);
                s[4].cNoizeVolume   = 40;
                s[4].nPhase         = 0;
                s[4].cVolume        = 80;
                s[4].cFreqShift     = (int)(5 Hz);
                s[4].cVolumeStep    = 10;
                s[4].cNoizeVolumeStep  = 5;
                s[4].cState         = 1 | 2;

                OS_Delay(8);
                break;

            case GAME_SOUND_BANG_JEWEL:
            
                s[4].nFreq          = (int)(80 Hz);
                s[4].cNoizeVolume   = 40;
                s[4].nPhase         = 0;
                s[4].cVolume        = 80;
                s[4].cFreqShift     = (int)(5 Hz);
                s[4].cVolumeStep    = 10;
                s[4].cNoizeVolumeStep  = 5;
                s[4].cState         = 1 | 2;

                OS_Delay(4);
                s[4].nFreq          = (int)(3000 Hz);
                s[4].cNoizeVolume   = 40;
                s[4].nPhase         = 0;
                s[4].cVolume        = 40;
                s[4].cVolumeStep    = 10;
                s[4].cFreqShift     = 0;
                s[4].cNoizeVolumeStep  = 5;
                s[4].cState         = 1 | 2;

                OS_Delay(4);
                break;

            case GAME_SOUND_HOME:
                s[4].nFreq          = (int)(2000 Hz);
                s[4].cNoizeVolume   = 0;
                s[4].nPhase         = 0;
                s[4].cVolume        = 100;
                s[4].cFreqShift     = (int)(15 Hz);
                s[4].cVolumeStep    = 1;
                s[4].cState         = 1 | 2;
                OS_Delay(2000 ms_OS);
                break;

            case GAME_SOUND_COMPLETE:
                s[4].nFreq          = (int)(2000 Hz);
                s[4].cNoizeVolume   = 0;
                s[4].nPhase         = 0;
                s[4].cVolume        = 100;
                s[4].cFreqShift     = 255;
                OS_Delay(4);
                break;

            case GAME_SOUND_BORN:
                s[4].nFreq          = (int)(1000 Hz);
                s[4].cNoizeVolume   = 10;
                s[4].nPhase         = 0;
                s[4].cVolume        = 50;
                s[4].cFreqShift     = 255;
                OS_Delay(8);
                break;

        }

        // Stop sound 

        s[4].cVolume = 0;
        s[4].cNoizeVolume = 0;
        s[4].cFreqShift = 0;
        s[4].cState = 0;
    }
}






/********************************************************************************
 *
 *  Function:       WorkMusic
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Get note from notelist and form synthesizer variable
 *                  for current channel
 *
 *  parameters:     TYPE_MUSIC_CHANNEL *M   - pointer to music channel control 
 *
 *  on return:      char = 0 - end of notes detected
 *                       = 1 - else
 *
 ********************************************************************************/


bit WorkMusic (TYPE_MUSIC_CHANNEL *pm)
{
    char   cmd, n;
    static bit OK;
    unsigned int f;

    if (pm->cPause)     // Channel makes a pause
    {
        pm->cPause--;
        return 1;
    }

    OK = 0;
    while (!OK)
    {
        cmd = pm->notelist[pm->nCurNote++]; // Get next note

        //------------------------------------------------------------
        // Check: is it note or control command
        //------------------------------------------------------------
        if (cmd & 0x80)             // this is command
        {       
            n = cmd & 0xE0;
            switch (n) 
            {

                //------------------------------------------------------------------------------
                case SOUND_CMD_PAUSE:       // Make pause for several ticks
                case SOUND_CMD_PLAY_MORE:   // Continue playing several ticks

                    if (!(cmd & 0x10)) pm->s->cVolume = 0;
                    n = (cmd & 0xF);
                    OK = 1;
                    break;

                //------------------------------------------------------------------------------
                case SOUND_CMD_SET_BASE:  // Set lowest octave

                    pm->cBaseNote = (char)(cmd & 0x1F) * 12;
                    break;

                //------------------------------------------------------------------------------
                case SOUND_CMD_REPEAT:    // Repeat fragment

                    if (!(cmd & 0x1F)) {     // Set repeat marker
                        pm->cRepeatCounter = 0;
                        pm->nRepeatPosition = pm->nCurNote;  // (save current position as marker)
                    } else {                               // Repeat from marker
                        if (!pm->cRepeatCounter)
                        {
                            pm->cRepeatCounter = (cmd & 0x0F);
                            pm->nCurNote = pm->nRepeatPosition;  // Restore position of marker
                        } else {
                            if (--pm->cRepeatCounter)
                            {
                                pm->nCurNote = pm->nRepeatPosition;  // Restore position of marker
                            } else {
                                pm->nRepeatPosition = pm->nCurNote;
                            }
                        }
                    }
                    break;

                //------------------------------------------------------------------------------
                case SOUND_CMD_STOP:      // Stop channel

                    pm->nCurNote--;
                    return 0;
            }
        //------------------------------------------------------------
        } else {                // this is note
        //------------------------------------------------------------
            n = ((cmd & 0x60) >> 5);                    // bits 5 and 6 - duration
            f = Freq[pm->cBaseNote + (cmd & 0x1F)];     // Set new note frequency

            pm->s->cVolume = 0;                         // Turn sound off
            pm->s->nFreq = f;                           // Modify sound parameters
            pm->s->nPhase = 0;
            pm->s->cVolumeStep = pm->cMasterVolumeStep;
            pm->s->cVolume = pm->cMasterVolume;         // Turn sound on

            break;
        }
        //------------------------------------------------------------
    }
    pm->cPause = n;
    return 1;

}




/********************************************************************************
 *
 *  Function:       InitSound
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Initialize synthesiser control variable 
 *
 *  parameters:     TYPE_SOUND *S
 *
 *  on return:      void
 *
 ********************************************************************************/

void InitSound (TYPE_SOUND *ps)
{
    ps->nPhase = 0;
    ps->nFreq = 0;
    ps->cVolume = 0;
    ps->cState = 1 | 4;  
    ps->cVolumeStep = 0;
}



/********************************************************************************
 *
 *  Function:       InitMusicChannel
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Assign notelist and synthesizer control variable to
 *                  music control variable. Set all parameters at begin.
 *
 *  parameters:     TYPE_MUSIC_CHANNEL *pm, TYPE_SOUND *ps
 *
 *  on return:      void
 *
 ********************************************************************************/

void InitMusicChannel (TYPE_MUSIC_CHANNEL *pm, TYPE_SOUND *ps, const char *pnotes)
{
    InitSound(ps);
    pm->s = ps;
    pm->cMasterVolumeStep = 12;
    pm->cMasterVolume = 120;
    pm->cPause = 0;
    pm->cBaseNote = 0;

    pm->nCurNote = 0;
    pm->nRepeatPosition = 0;
    pm->cRepeatCounter = 0;

    pm->notelist = pnotes;

}
//******************************************************************************
// 
//      THE END (music.c)
// 
//******************************************************************************

