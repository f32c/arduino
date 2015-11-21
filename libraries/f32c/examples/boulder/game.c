/************************************************************************************************
 *
 *  File:           game.c
 *
 *  Compilers:      HT-PICC18 STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    "Boulder Dash" game rules and animation.
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
#include "game.h"
#include "music.h"


#include "gamedata.h"       // file contains game maps and messages



//******************************************************************************
//  
//      Variables
// 
//******************************************************************************

unsigned short int   m_Colors[32];      // Array of colors for each game object

unsigned char       m_Map[MAP_SIZE_Y][MAP_SIZE_X];  // Game map

        char m_cCurMap;                 // Current active map
        char m_strStatusBar[33];        // text string displayed at the bottom

near    char m_cWinX, m_cWinY;          // Current map position in window
        bit  m_bScrMovedX;              
        bit  m_bScrMovedY;

near    char m_cManX, m_cManY;          // Position of man in map array
        char m_cBangX, m_cBangY;        // Poition of bang epicenter
        bit  m_bBlink;                  // Blink screen when all jewels picked
        bit  m_bBlinked;                

        char m_cNeededJewels;           // How maby jewels we need
        char m_cJewels;                 // How many jewels we got
        char m_cBornTimer;              // Born after 20 cycles after map was loaded

        bit  m_bAlive;                  // Man is alive and active (born and not dead)
        bit  m_bEnd;                    // Level complete or man is dead


//******************************************************************************
//  
//      Function prototypes
// 
//******************************************************************************

void    str_cpy                 (char *dst, const char *src);
void    SetMap                  (char y, char x, char value);
char    GetMap                  (char y, char x);
void    CorrectWindowPositionX  (char dx);
void    CorrectWindowPositionY  (char dy);
char    Object                  (char a);
void    ByteToString            (char byte, char *str, char digits);
void    MakeStatusBar           (void);
void    LoadColors              (char M);
void    InitMap                 (char M);


//******************************************************************************
//  
//      Functions
// 
//******************************************************************************




/********************************************************************************
 *
 *  TASK: Task_Animate
 *
 *------------------------------------------------------------------------------
 *
 *  description:    This task performs animation of all animated objects on
 *                  visible area of game map
 * 
 *                  Get control every fourth VGA-cadr
 *
 ********************************************************************************/

void Task_Animate (void)
{
    static      bit     bSlow = 0;      // Some objects animated slow
    static      char    s_cPhase = 0;   // Animation phase: 0..3
    static near char    x, y;           // Declared as 'near' to increaze speed
    static near char    A, B;           // Current map point and object
    static      char   *pmap;

                char    temp;

    //for (;;)
    {
        CLRWDT();
        OS_Bsem_Set(BS_MAP_FREE);       // Free map

        OS_Flag_Wait_11(i_cStep, 0x04);
        OS_Flag_Set_0(i_cStep, 0x04);

        OS_Bsem_Wait(BS_MAP_FREE);

        bSlow = !bSlow;
        if (!bSlow) s_cPhase++;


        //------------------------------------------------------------------------------
        //  When all jewels are picked, change all colors to light white to
        //  immitate a flash blink.
        //------------------------------------------------------------------------------
        
        if (m_bBlinked)
        {
            LoadColors(m_cCurMap);
            m_bBlinked = 0;
        }
        //------------------------------------------------------------------------------
        //  Restore colors after flashing
        //------------------------------------------------------------------------------
        
        if (m_bBlink)                                       // Зажигаем вспышку
        {
            for (x = 0; x < 32; x++) m_Colors[x] = CL_LTWHITE;
            m_bBlinked = 1; // Говорим, что ее нужно погасить при следующем вызове
            m_bBlink = 0;
        }
        

        //------------------------------------------------------------------------------
        //  Make animation for all elements on map
        //------------------------------------------------------------------------------
        
        pmap = (char*)m_Map;

        for (y = 0; y < MAP_SIZE_Y; y++) 
        {   
            OS_Yield();                       
            for (x = 0; x < MAP_SIZE_X; x++) 
            {
                A = *pmap;                      // Current element
                B = A & 0x1F;
                if (B < 4) goto NEXT;           // Space, dirt, boulder, wall
                if (B  == 0x1F) goto NEXT;      // Last stage of bang
                
                if (B == OBJECT_MAN_EYES && bSlow)
                {   
                    A = OBJECT_MAN;
                    goto CONTINUE;
                }
                
                B &= 0x1C;
                if (B == OBJECT_LEFT_MAN || B == OBJECT_RIGHT_MAN || (B==OBJECT_BANG && bSlow))
                {
                    // Running man
                    temp = A + 1;
                    A &= ~3;
                    A |= (temp & 3);
                    goto CONTINUE;
                }
                
                
                if (!bSlow) continue;
                
                if (B == OBJECT_JEWEL || B == OBJECT_BUTTERFLY || B == OBJECT_FIREFLY)
                {
                    A &= ~3;
                    A |= (s_cPhase & 3);
                    goto CONTINUE;
                }
                
                if (B == OBJECT_TIT_WALL && (A & 0xC0))     // Home
                {
                    if (((A & 0x80) && (m_cNeededJewels <= m_cJewels)) || 
                        ((A & 0x40) && m_cBornTimer))
                    {
                        A ^= 1;
                        goto CONTINUE;
                    }
                }
            CONTINUE:;
                *pmap = A;
            NEXT:;
                pmap++;
            }
        }
    }
}




/********************************************************************************
 *
 *  TASK: Task_Game
 *
 *------------------------------------------------------------------------------
 *
 *  description:    
 *
 ********************************************************************************/

void Task_Game (void)
{
    static      char    s_cKeys;        // Keyboard command
    static      char    s_cLastKeys = 0;// Previous command
    static      bit     bButton;        // Fire button state

    static near char    x, y, yy, xx;

                char    A,          // Object at porition X
                        B;          // Object style for A. These styles are calculated
                                    // once per cycle, to increase speed of GameStep
                char    A_top,      // Object above A
                        A_bottom,   // Object under A
                        A_move,     // Object at position where A wants move to.
        
                        B_top,      
                        B_bottom, 
                        B_move;

    static near char    s_cAction;  // Action performing on moving object

    static      bit     bDirChanged;// Addition flag for fireflies and butterflies
                                    // to change direction of moving
    static      bit     bBangJewel; // This bit tells that jewel should appear after bang
    static near char    s_cSound;   // Sound type on current stage

    static near char    n,m;        // addition temp variables
    static near char    temp;
    


    //for (;;)
    {
        OS_Bsem_Set(BS_MAP_FREE);       // Free "Map" resource

        OS_Flag_Wait_11(i_cStep, 0x08); // Run every eight VGA-cadr
        OS_Flag_Set_0(i_cStep, 0x08);

        OS_Bsem_Wait(BS_MAP_FREE);      // Wait for resource "Map" became free
        
        s_cKeys = 0;
        bButton = 0;


        //------------------------------------------------------------------------------
        // All button pressed: Restart level
        //------------------------------------------------------------------------------
    
        if (BUTTON_UP && BUTTON_LEFT && BUTTON_RIGHT && BUTTON_DOWN) 
        {    
            if (m_bAlive)
            {
                m_bAlive = false;
                m_bEnd = true;
                m_Map[m_cManY][m_cManX] = OBJECT_BANG;
                str_cpy(m_strStatusBar, STR_GAME_OVER);
            }    
        }
        
        //------------------------------------------------------------------------------
        //  Fire button after dead - reborn
        //------------------------------------------------------------------------------

        if (BUTTON_FIRE && m_bEnd) 
        {
            if (BUTTON_UP && BUTTON_LEFT && BUTTON_RIGHT && BUTTON_DOWN) 
            {    
                m_cCurMap++;
                if (m_cCurMap >= NUMBER_OF_MAPS) m_cCurMap = 0;
                InitMap(m_cCurMap);
            }
            if (!BUTTON_UP && !BUTTON_LEFT && !BUTTON_RIGHT && !BUTTON_DOWN) InitMap(m_cCurMap);
            
        }



        //------------------------------------------------------------------------------
        //  When man alive, check keyoard and make the man run
        //------------------------------------------------------------------------------
        
        if (m_bAlive)
        {
            OS_Bsem_Set(BS_MUSIC_START);            // First time music can start only
                                                    // after born
            
            if (BUTTON_UP)     s_cKeys = 1;
            if (BUTTON_DOWN)   s_cKeys = 2;
            if (BUTTON_LEFT)   s_cKeys = 3;
            if (BUTTON_RIGHT)  s_cKeys = 4;
            if (BUTTON_FIRE)   bButton = 1;
            

            // Select man picture: standing, running left, running right
            temp = 0;
            switch (s_cKeys & 0xF)
            {
                case 0:     
                    temp = OBJECT_MAN | (((i_wRandom & 7) == 7)?1:0);    
                    break;
                case 1:     
                    if (s_cLastKeys != s_cKeys) temp = OBJECT_LEFT_MAN  | MOVING | DIR_UP;
                    break; 

                case 2:     
                    if (s_cLastKeys != s_cKeys) temp = OBJECT_RIGHT_MAN | MOVING | DIR_DOWN; 
                    break;

                case 3:     
                    if (s_cLastKeys != s_cKeys) temp = OBJECT_LEFT_MAN  | MOVING | DIR_LEFT; 
                    break;

                case 4:     
                    if (s_cLastKeys != s_cKeys) temp = OBJECT_RIGHT_MAN | MOVING | DIR_RIGHT;
                    break;
            }

            if (temp) SetMap(m_cManY, m_cManX, temp);

        } else {            // Man is not born yet
            m_cBornTimer--;
        }

        s_cLastKeys = s_cKeys;   


        //------------------------------------------------------------------------------
        //  Make window position correction
        //------------------------------------------------------------------------------
        
        if (m_bScrMovedX)
        {
            m_bScrMovedX = false;
            if (!s_cKeys) CorrectWindowPositionX(5);
        }

        if (m_bScrMovedY)
        {
            m_bScrMovedY = false;
            if (!s_cKeys) CorrectWindowPositionY(4);
        }

        OS_Yield();

        s_cSound = 0;

        //------------------------------------------------------------------------------
        //  Check all elements in map
        //------------------------------------------------------------------------------

        for (y = 18; y > 0; y--)
        {    
            OS_Yield();

            for (x = 38; x > 0; x--)
            {
                
                CLRWDT();
                bDirChanged = false;
                bBangJewel = false;
                
            RETRY:
                A        = GetMap(y, x);

                if (A == (OBJECT_SPACE | 0x80)) // When object SPACE have bit 7 set, 
                {                               // nither boulder nor jewel can fall here
                    SetMap(y,x, OBJECT_SPACE);  // This state of SPACE can be active 
                    continue;                   // per one game step only.
                }
                if (A == OBJECT_SPACE) continue;
                if (A == OBJECT_WALL) continue;
                if (A == OBJECT_DIRT) continue;

                B        = Object(A);

                //******************************************************************************
                if (A & MOVING)         // Check moving object
                //******************************************************************************
                {
                    xx = x;             // Calculate position to move to
                    yy = y;
                    switch (A & DIR_MASK)
                    {
                        //------------------------------------------------------------------------------
                        case DIR_UP: 
                        //------------------------------------------------------------------------------
                            yy--;
                            break;

                        //------------------------------------------------------------------------------
                        case DIR_DOWN: 
                        //------------------------------------------------------------------------------
                            yy++;
                            break;

                        //------------------------------------------------------------------------------
                        case DIR_LEFT: 
                        //------------------------------------------------------------------------------
                            xx--;
                            break;

                        //------------------------------------------------------------------------------
                        case DIR_RIGHT: 
                        //------------------------------------------------------------------------------
                            xx++;
                            break;
                    }

                    // Point where object moves to
                    A_move = GetMap(yy, xx);    
                    B_move = Object(A_move);
                        
                    //**************************************************************************
                    //  Check what the action should we do
                    //**************************************************************************
                    
                    s_cAction = ACTION_NO_ACTION;

                    switch (B)
                    {
                    //------------------------------------------------------------------------------
                    case OBJECT_MAN:        // man
                    case OBJECT_MAN_EYES:   // man
                    case OBJECT_LEFT_MAN:   // man
                    case OBJECT_RIGHT_MAN:  // man

                        //------------------------------------------------------------------------------
                        //  Check for push boulder
                        //------------------------------------------------------------------------------
                        if (B_move == OBJECT_BOULDER)
                        {
                            if (y != yy) break;
                            if (x > xx && GetMap(y,xx-1)==OBJECT_SPACE) s_cAction = ACTION_PUSH;
                            if (x < xx && GetMap(y,xx+1)==OBJECT_SPACE) s_cAction = ACTION_PUSH;
                        }


                        if (B_move == OBJECT_TIT_WALL)
                        {      
                            // Check for HOME and there is enough jewels
                            if ((A_move & 0x80) && (m_cJewels >= m_cNeededJewels)) 
                            {    
                                s_cAction = ACTION_GO_HOME; 
                            }
                            break;
                        }

                        if (B_move == OBJECT_WALL)      s_cAction = ACTION_NO_ACTION;
                        if (B_move == OBJECT_SPACE)     s_cAction = ACTION_CAN_MOVE;
                        if (B_move == OBJECT_DIRT)      s_cAction = ACTION_EAT_DIRT;
                        if (B_move == OBJECT_BANG)      s_cAction = ACTION_NO_ACTION;
                        if (B_move == OBJECT_JEWEL)     s_cAction = ACTION_EAT_JEWEL;


                        break;

                    //------------------------------------------------------------------------------
                    case OBJECT_BUTTERFLY:
                    case OBJECT_FIREFLY:

                        //------------------------------------------------------------------------------
                        // Check for man touching
                        //------------------------------------------------------------------------------
                        
                        if (m_bAlive)
                        {
                            if ((m_cManY == y && (m_cManX == (x+1) || m_cManX == (x-1))) ||
                                (m_cManX == x && (m_cManY == (y+1) || m_cManY == (y-1))))
                            {
                                // Set bang position
                                m_cBangX = m_cManX;
                                m_cBangY = m_cManY;
                                s_cAction = ACTION_BANG;
                                if (B == OBJECT_BUTTERFLY) s_cAction = ACTION_BANG_JEWEL;
                                break;
                            }
                        }

                        if (B_move == OBJECT_SPACE) 
                        {    
                            s_cAction = ACTION_CAN_MOVE;
                            break;
                        }

                        //------------------------------------------------------------------------------
                        //  When firefly and butterfly meat face to face, they just 
                        //  replaced one by one (they should be moved in opposit directions
                        //------------------------------------------------------------------------------
                        
                        if (B_move == OBJECT_FIREFLY || B_move == OBJECT_BUTTERFLY)
                        {
                            if (B != B_move) if (((A^A_move) & DIR_MASK)==0x80) 
                            {
                                s_cAction = ACTION_EXCHANGE;
                                break;
                            }
                        }

                        //------------------------------------------------------------------------------
                        // Check for falling boulder or jewel above
                        //------------------------------------------------------------------------------
                        
                        A_top = GetMap(y-1, x);
                        B_top = A_top & 0x1F;

                        if (B_top == OBJECT_BOULDER || (B_top & 0x1C)== OBJECT_JEWEL )
                        {
                            if ((A_top & MOVING)) 
                            {    
                                s_cAction = ACTION_STOP_FLY;
                                break;
                            }
                        }
                        
                        s_cAction = ACTION_DEC_DIRECTION;
                        if (B == OBJECT_BUTTERFLY) s_cAction = ACTION_INC_DIRECTION;
                        break;

                    //------------------------------------------------------------------------------
                    case OBJECT_BOULDER:
                    case OBJECT_JEWEL:

                        m_cBangX = x;
                        m_cBangY = y + 1;
                        s_cAction = ACTION_STOP_MOVE;
                        B_bottom = Object(GetMap(y+1, x));
                        if (B_bottom == OBJECT_SPACE)     s_cAction = ACTION_CAN_MOVE;
                        if (B_bottom == OBJECT_MAN)       s_cAction = ACTION_BANG;
                        if (B_bottom == OBJECT_LEFT_MAN)  s_cAction = ACTION_BANG;
                        if (B_bottom == OBJECT_RIGHT_MAN) s_cAction = ACTION_BANG;
                        if (B_bottom == OBJECT_FIREFLY)   s_cAction = ACTION_BANG;
                        if (B_bottom == OBJECT_BUTTERFLY) s_cAction = ACTION_BANG_JEWEL;
                        break;

                    }   // switch (B)
                    

                    //******************************************************************************
                    //  Now we have a variable s_cAction which contains what should we do
                    //******************************************************************************


                    switch (s_cAction)
                    {
                        //------------------------------------------------------------------------------
                        case ACTION_NO_ACTION:  
                        //------------------------------------------------------------------------------
                            break;
                
                        //------------------------------------------------------------------------------
                        case ACTION_PUSH:
                        //------------------------------------------------------------------------------

                            if (i_wRandom & 3) break;
                            if (x > xx) SetMap(y, xx-1, OBJECT_BOULDER);
                            if (x < xx) SetMap(y, xx+1, OBJECT_BOULDER);
    
                            if (s_cSound < GAME_SOUND_PUSH) s_cSound = GAME_SOUND_PUSH;
    
                            goto MOVE;
                
                
                        //------------------------------------------------------------------------------
                        case ACTION_EAT_JEWEL:
                        //------------------------------------------------------------------------------

                            m_cJewels++;
                            if (m_cJewels == m_cNeededJewels) 
                            {    
                                m_bBlink = 1;
                                if (s_cSound < GAME_SOUND_COMPLETE) s_cSound = GAME_SOUND_COMPLETE;
                            }
                            if (s_cSound < GAME_SOUND_EAT_JEWEL) s_cSound = GAME_SOUND_EAT_JEWEL;
                            goto MOVE;
                
                        //------------------------------------------------------------------------------
                        case ACTION_GO_HOME:
                        //------------------------------------------------------------------------------

                            m_bAlive = 0;
                            m_bEnd = 1;
                            str_cpy(m_strStatusBar, STR_YOU_WIN);
                            A &= ~MOVING;
                            SetMap(y, x, A);

                            m_cCurMap++;
                            if (m_cCurMap >= NUMBER_OF_MAPS) m_cCurMap = 0;

                            if (s_cSound < GAME_SOUND_HOME) s_cSound = GAME_SOUND_HOME;
                            goto MOVE;
                    
                        
                        //------------------------------------------------------------------------------
                        case ACTION_EAT_DIRT:
                        //------------------------------------------------------------------------------
                            if (s_cSound < GAME_SOUND_EAT_DIRT) s_cSound = GAME_SOUND_EAT_DIRT;
                            goto MOVE;

                        //------------------------------------------------------------------------------
                        case ACTION_CAN_MOVE:
                        //------------------------------------------------------------------------------
                MOVE:
                            SetMap(y, x, OBJECT_SPACE);
                            
                            if (x == m_cManX && y == m_cManY) 
                            {
                                if (bButton && m_cManX==x && m_cManY==y)
                                {
                                    SetMap(yy, xx, OBJECT_SPACE);
                                    xx = x;
                                    yy = y;
                                }

                                m_cManX = xx;
                                m_cManY = yy;
                                
                                CorrectWindowPositionX(2);
                                CorrectWindowPositionY(2);
                                
                                if (s_cSound < GAME_SOUND_STEP) s_cSound = GAME_SOUND_STEP;
                            }

                            if (yy < y || xx < x) A &= ~MOVING; // To prevent multiply moving
                                                                // of same object


                            if (B == OBJECT_BUTTERFLY)  A -= 0x40;
                            if (B == OBJECT_FIREFLY)    A += 0x40;
                            SetMap(yy, xx, A);
                            break;
                
                        //------------------------------------------------------------------------------
                        case ACTION_EXCHANGE:
                        //------------------------------------------------------------------------------
                            SetMap(y, x, A_move);
                            SetMap(yy, xx, A);
                            break;
                
                        //------------------------------------------------------------------------------
                        case ACTION_INC_DIRECTION:
                        //------------------------------------------------------------------------------
                            if (!bDirChanged)
                            {
                                A += 0x40;
                                SetMap(y, x, A);
                                bDirChanged = true;
                                goto RETRY;
                            }
                            break;
                        //------------------------------------------------------------------------------
                        case ACTION_DEC_DIRECTION:
                        //------------------------------------------------------------------------------
                            if (!bDirChanged)
                            {
                                A -= 0x40;
                                SetMap(y, x, A);
                                bDirChanged = true;
                                goto RETRY;
                            }
                            break;
                    
                        //------------------------------------------------------------------------------
                        case ACTION_STOP_MOVE:
                        //------------------------------------------------------------------------------
                            A &= ~MOVING;
                            SetMap(y, x, A);
                            if (B == OBJECT_JEWEL)
                            {
                                if (s_cSound < GAME_SOUND_JEWEL_FELD) s_cSound = GAME_SOUND_JEWEL_FELD;
                            } else {
                                if (s_cSound < GAME_SOUND_BOULDER_FELD)s_cSound = GAME_SOUND_BOULDER_FELD; 
                            }
                            goto RETRY;
                    
                        //------------------------------------------------------------------------------
                        case ACTION_STOP_FLY:
                        //------------------------------------------------------------------------------
                            A &= ~MOVING;
                            SetMap(y, x, A);
                            break;
                    
                    
                        //------------------------------------------------------------------------------
                        case ACTION_BANG_JEWEL:
                        //------------------------------------------------------------------------------
                            if (s_cSound < GAME_SOUND_BANG_JEWEL) s_cSound = GAME_SOUND_BANG_JEWEL;
                            bBangJewel = true;
                            
                        //------------------------------------------------------------------------------
                        case ACTION_BANG:
                        //------------------------------------------------------------------------------
                            if (!bBangJewel) 
                                if (s_cSound < GAME_SOUND_BANG) s_cSound = GAME_SOUND_BANG;

                            for (n = m_cBangX - 1; n <= m_cBangX + 1; n++) 
                            {        
                                for (m = m_cBangY-1; m <= m_cBangY+1; m++)
                                {
                                    if (GetMap(m, n) != OBJECT_TIT_WALL)
                                    {
                                        if (n == m_cManX && m == m_cManY) 
                                        { 
                                            if (!m_bAlive) continue;   
                                            m_bAlive = 0;
                                            m_bEnd = true;
                                            str_cpy(m_strStatusBar, STR_GAME_OVER);
                                        }
                                        temp =  OBJECT_BANG;
                                        if (bBangJewel) temp |= 0x80;
                                        SetMap(m, n, temp);
                                        
                                    };
                                }
                            }    
                            break;
                    };  // switch s_cAction

                //******************************************************************************
                } else {            // not MOVING object
                //******************************************************************************

                    switch (B)
                    {
                        //------------------------------------------------------------------------------
                        case OBJECT_LEFT_MAN:   // Restore moving state to objects that should move
                        case OBJECT_RIGHT_MAN:  
                             if (!m_bAlive) break;
                        case OBJECT_BUTTERFLY:  
                        case OBJECT_FIREFLY:;   

                             m_Map[y][x] |= MOVING;
                             break;

                        //------------------------------------------------------------------------------
                        case OBJECT_BOULDER:
                        case OBJECT_JEWEL:

                            B_bottom = Object(A_bottom = GetMap(y+1, x));

                            if (B_bottom == OBJECT_SPACE)   // Check for start falling down
                             {
                                 A |= MOVING;
                                 A |= DIR_DOWN;
                                 SetMap(y, x, A);
                             } else {

                                 // Check for falling from a boulder, a jewel or a wall
                                 
                                 if ((B_bottom == OBJECT_BOULDER || 
                                      B_bottom == OBJECT_WALL || 
                                      B_bottom == OBJECT_JEWEL) && !(A_bottom & MOVING))
                                 {

                                     if (GetMap(y  ,x-1) == OBJECT_SPACE && 
                                         GetMap(y+1,x-1) == OBJECT_SPACE)
                                     {
                                         temp = Object(GetMap(y-1, x-1));
                                         // Не даем камню уйти влево, если что-то
                                         // должно падать сверху
                                         if (temp == OBJECT_BOULDER) break;
                                         if (temp == OBJECT_JEWEL)   break;
                    
                                         SetMap(y, x-1, A); // Здесь не нужно ставить флаг 
                                                            // движения, т.к. эта клетка будет
                                                            // преверена следюущим же шагом (т.к.
                                                            // просматриваем справа налево)
                                                                   
                                         SetMap(y,x, OBJECT_SPACE | 0x80);  // Признак того, что 
                                                                            // пока еще нельзя 
                                                                            // ронять сюда камень 
                                         break;
                                     }

                                     if (GetMap(y  ,x+1) == OBJECT_SPACE && 
                                         GetMap(y+1,x+1) == OBJECT_SPACE) 
                                     {
                                         temp = Object(GetMap(y-1,x+1));
                                         // Не даем камню уйти влево, если что-то
                                         // должно падать сверху
                                         if (temp == OBJECT_BOULDER) break;
                                         if (temp == OBJECT_JEWEL)   break;
                    
                                         SetMap(y,x+1, A | MOVING | DIR_DOWN);
                                         SetMap(y,x, OBJECT_SPACE | 0x80);
                                         break;
                                     }
                                 }
                             }
                             break;
                            
                        //------------------------------------------------------------------------------
                        case OBJECT_HOME:
                        case OBJECT_TIT_WALL:

                            if ((A & 0x80) && (m_cNeededJewels <= m_cJewels))
                            {
                    
                            }
                            if ((A & 0x40) && !m_cBornTimer)
                            {
                                SetMap(y,x, OBJECT_BANG | 0x40);
                                if (s_cSound < GAME_SOUND_BORN) s_cSound = GAME_SOUND_BORN;
                            }
                            break;
                    }   // switch (B)

                
                    if ((A & 0x1F) == (OBJECT_BANG | 3))    // last stage of bang
                    {
                        if (A & 0x40)           // Borning
                        {
                            A = OBJECT_MAN;
                            m_cManX = x;
                            m_cManY = y;
                            m_bAlive = 1;
                        } else {
                            if (A & 0x80)   A = OBJECT_JEWEL;   // Bang with jewels
                            else            A = OBJECT_SPACE;   // Bang without jewels
                        }
                        SetMap(y, x, A);
                    }

                //******************************************************************************
                }   // MOVING
                //******************************************************************************
            }   // for x
        }   // for y

        if (s_cSound) OS_SMsg_Send_Now(smsg_GameSound, s_cSound);
        if (m_bAlive) MakeStatusBar();
    }
}



//******************************************************************************
//  
//  Copy string from ROM into RAM
// 
//******************************************************************************

void str_cpy (char *dst, const char *src)
{
    while (*dst++ = *src++);
}



//******************************************************************************
//  
//  Get value of map array at position y,x
//  (used to reduce code size)
// 
//******************************************************************************

char GetMap (char y, char x)
{
    return m_Map[y][x];
}



//******************************************************************************
//  
//  Set value of map array at position y,x
//  (used to reduce code size)
// 
//******************************************************************************

void SetMap (char y, char x, char value)
{
    m_Map[y][x] = value;
}



//******************************************************************************
//  
//  Get object's ID.
// 
//******************************************************************************

char Object (char a)
{
    char b;
    b = a & 0x1F;
    if (b >= 0x4) b &= 0xFE;
    if (b >= 0x8) b &= 0xFD;
    return b;
}



//******************************************************************************
//
//  Correct visual map area accordind to man position
//  dx - minimal allowed distance between man and screen border
// 
//******************************************************************************

void CorrectWindowPositionX (char dx)
{
    if (m_cManX < (m_cWinX + dx) && m_cWinX > 0)
    {
        m_cWinX--; 
        m_bScrMovedX = true;
    }
    if (m_cManX > (m_cWinX + WIN_SIZE_X - 1 - dx) && m_cWinX < (MAP_SIZE_X - WIN_SIZE_X))   
    {
        m_cWinX++; 
        m_bScrMovedX = true;
    }
} 



//******************************************************************************
//
//  Correct visual map area accordind to man position
//  dy - minimal allowed distance between man and screen border
// 
//******************************************************************************

void CorrectWindowPositionY (char dy)
{
    if (m_cManY < (m_cWinY + dy) && m_cWinY > 0)         
    {
        m_cWinY--; 
        m_bScrMovedY = true;
    }
    if (m_cManY > (m_cWinY + WIN_SIZE_Y-1 - dy) && m_cWinY < (MAP_SIZE_Y - WIN_SIZE_Y))   
    {
        m_cWinY++; 
        m_bScrMovedY = true;
    }
} 


//******************************************************************************
//  
//  Pseudo printf 
// 
//******************************************************************************

void ByteToString (char byte, char *str, char digits)
{
    char a100, a10, a1;
    
    a100 = a10 = a1 = 0;
    while (byte >=100) 
    {
        a100++;
        byte -= 100;
    }
    while (byte >=10) 
    {
        a10++;
        byte -= 10;
    }
    a1 = byte;
    if (digits >=3) *str++ = a100 + '0';
    if (digits >=2) *str++ = a10 + '0';
    if (digits >=1) *str++ = a1 + '0';

}



//******************************************************************************
//  
//  Forming string for status bar
// 
//******************************************************************************

void MakeStatusBar (void)
{
    ByteToString(m_cJewels, m_strStatusBar + 7, 3);
    ByteToString(m_cNeededJewels, m_strStatusBar + 17, 3);
    ByteToString(m_cManX, m_strStatusBar + 25, 2);
    ByteToString(m_cManY, m_strStatusBar + 30, 2);
}



//******************************************************************************
// 
//  Load colors from  MAP constant (first line in array
// 
//******************************************************************************

void LoadColors (char M)
{
    char x;
    for (x = 0; x < 32; x++) m_Colors[x] = MAP[M][0][x];
    m_Colors[5] = m_Colors[4];
    m_Colors[7] = m_Colors[6];
    m_Colors[9] = m_Colors[10] = m_Colors[11] = m_Colors[8];
    m_Colors[13] = m_Colors[14] = m_Colors[15] = m_Colors[12];
    m_Colors[17] = m_Colors[18] = m_Colors[19] = m_Colors[16];
    for (x = 20; x < 28; x++) m_Colors[x] = m_Colors[6];
    m_Colors[29] = m_Colors[30] = m_Colors[31] = m_Colors[28];
}



//******************************************************************************
//  
//  Copy map data from ROM into RAM
//  Set all game variables at default.
//  Load colors.
// 
//******************************************************************************

void InitMap (char M)
{
    char x, y;
    char c;
    char a;

    for (y = 0; y < MAP_SIZE_Y; y++)
    {
        for (x = 0; x < MAP_SIZE_X; x++)
        {
            c = MAP[M][y+1][x];
            a = 0;
            switch (c)
            {
            case ' ':   a = OBJECT_SPACE;   break;  // space
            case '.':   a = OBJECT_DIRT;   break;  // dirt
            case 'o':
            case 'O':   a = OBJECT_BOULDER;   break;  // boulder
            case '=':   a = OBJECT_WALL;   break;  // wall
            case '+':   a = OBJECT_JEWEL;   break;  // jewel
            case 'm':
            case 'M':   a = OBJECT_BORN; m_cManX = x;  m_cManY = y;  break;  // man
            case 'h':
            case 'H':   a = OBJECT_HOME;   break;  // man
            case 'x':   
            case 'X':   a = OBJECT_BUTTERFLY;   break;  // butterfly
            case 'f':   
            case 'F':   a = OBJECT_FIREFLY;   break;  // firefly
            case 0x23:  a = OBJECT_TIT_WALL;   break;  // titanium wall

            }
            SetMap(y, x, a);
        }
    }

    LoadColors(M);


    m_cBornTimer = 20;
    m_cNeededJewels = MAP[M][0][32];
    m_cJewels = 0;
    m_bScrMovedX = 1;   // Уходим на нужное место
    m_bScrMovedY = 1;
    m_bAlive = 0;
    str_cpy(m_strStatusBar, STR_STATUS_BAR);
    m_bEnd = false;
}



//******************************************************************************
// 
//      THE END (game.c)
// 
//******************************************************************************


