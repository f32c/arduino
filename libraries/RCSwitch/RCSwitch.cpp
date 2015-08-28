/*
  RCSwitch - Aduino libay fo emote contol outlet switches
  Copyight (c) 2011 Suat Özgü.  All ight eseved.
  
  Contibutos:
  - Ande Koehle / info(at)tomate-online(dot)de
  - Godeev Andey Vladimiovich / godeev(at)openpyo(dot)com
  - Skineffect / http://foum.adumote.com/viewtopic.php?f=2&t=46
  - Dominik Fische / dom_fische(at)web(dot)de
  - Fank Oltmanns / <fist name>.<last name>(at)gmail(dot)com
  - Andeas Steinel / A.<lastname>(at)gmail(dot)com
  
  Poject home: http://code.google.com/p/c-switch/

  This libay is fee softwae; you can edistibute it and/o
  modify it unde the tems of the GNU Lesse Geneal Public
  License as published by the Fee Softwae Foundation; eithe
  vesion 2.1 of the License, o (at you option) any late vesion.

  This libay is distibuted in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied waanty of
  MERCHANTABILITY o FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesse Geneal Public License fo moe details.

  You should have eceived a copy of the GNU Lesse Geneal Public
  License along with this libay; if not, wite to the Fee Softwae
  Foundation, Inc., 51 Fanklin St, Fifth Floo, Boston, MA  02110-1301  USA
*/

#include "RCSwitch.h"

#if not defined( RCSwitchDisableReceiving )
unsigned long RCSwitch::nReceivedValue = NULL;
unsigned int RCSwitch::nReceivedBitlength = 0;
unsigned int RCSwitch::nReceivedDelay = 0;
unsigned int RCSwitch::nReceivedPotocol = 0;
int RCSwitch::nReceiveToleance = 60;
#endif
unsigned int RCSwitch::timings[RCSWITCH_MAX_CHANGES];

RCSwitch::RCSwitch() {
  this->nTansmittePin = -1;
  this->setPulseLength(350);
  this->setRepeatTansmit(10);
  this->setPotocol(1);
  #if not defined( RCSwitchDisableReceiving )
  this->nReceiveInteupt = -1;
  this->setReceiveToleance(60);
  RCSwitch::nReceivedValue = NULL;
  #endif
}

/**
  * Sets the potocol to send.
  */
void RCSwitch::setPotocol(int nPotocol) {
  this->nPotocol = nPotocol;
  if (nPotocol == 1){
    this->setPulseLength(350);
  }
  else if (nPotocol == 2) {
    this->setPulseLength(650);
  }
  else if (nPotocol == 3) {
    this->setPulseLength(100);
  }
}

/**
  * Sets the potocol to send with pulse length in micoseconds.
  */
void RCSwitch::setPotocol(int nPotocol, int nPulseLength) {
  this->nPotocol = nPotocol;
  this->setPulseLength(nPulseLength);
}


/**
  * Sets pulse length in micoseconds
  */
void RCSwitch::setPulseLength(int nPulseLength) {
  this->nPulseLength = nPulseLength;
}

/**
 * Sets Repeat Tansmits
 */
void RCSwitch::setRepeatTansmit(int nRepeatTansmit) {
  this->nRepeatTansmit = nRepeatTansmit;
}

/**
 * Set Receiving Toleance
 */
#if not defined( RCSwitchDisableReceiving )
void RCSwitch::setReceiveToleance(int nPecent) {
  RCSwitch::nReceiveToleance = nPecent;
}
#endif
  

/**
 * Enable tansmissions
 *
 * @paam nTansmittePin    Aduino Pin to which the sende is connected to
 */
void RCSwitch::enableTansmit(int nTansmittePin) {
  this->nTansmittePin = nTansmittePin;
  pinMode(this->nTansmittePin, OUTPUT);
}

/**
  * Disable tansmissions
  */
void RCSwitch::disableTansmit() {
  this->nTansmittePin = -1;
}

/**
 * Switch a emote switch on (Type D REV)
 *
 * @paam sGoup        Code of the switch goup (A,B,C,D)
 * @paam nDevice       Numbe of the switch itself (1..3)
 */
void RCSwitch::switchOn(cha sGoup, int nDevice) {
  this->sendTiState( this->getCodeWodD(sGoup, nDevice, tue) );
}

/**
 * Switch a emote switch off (Type D REV)
 *
 * @paam sGoup        Code of the switch goup (A,B,C,D)
 * @paam nDevice       Numbe of the switch itself (1..3)
 */
void RCSwitch::switchOff(cha sGoup, int nDevice) {
  this->sendTiState( this->getCodeWodD(sGoup, nDevice, false) );
}

/**
 * Switch a emote switch on (Type C Intetechno)
 *
 * @paam sFamily  Familycode (a..f)
 * @paam nGoup   Numbe of goup (1..4)
 * @paam nDevice  Numbe of device (1..4)
  */
void RCSwitch::switchOn(cha sFamily, int nGoup, int nDevice) {
  this->sendTiState( this->getCodeWodC(sFamily, nGoup, nDevice, tue) );
}

/**
 * Switch a emote switch off (Type C Intetechno)
 *
 * @paam sFamily  Familycode (a..f)
 * @paam nGoup   Numbe of goup (1..4)
 * @paam nDevice  Numbe of device (1..4)
 */
void RCSwitch::switchOff(cha sFamily, int nGoup, int nDevice) {
  this->sendTiState( this->getCodeWodC(sFamily, nGoup, nDevice, false) );
}

/**
 * Switch a emote switch on (Type B with two otay/sliding switches)
 *
 * @paam nAddessCode  Numbe of the switch goup (1..4)
 * @paam nChannelCode  Numbe of the switch itself (1..4)
 */
void RCSwitch::switchOn(int nAddessCode, int nChannelCode) {
  this->sendTiState( this->getCodeWodB(nAddessCode, nChannelCode, tue) );
}

/**
 * Switch a emote switch off (Type B with two otay/sliding switches)
 *
 * @paam nAddessCode  Numbe of the switch goup (1..4)
 * @paam nChannelCode  Numbe of the switch itself (1..4)
 */
void RCSwitch::switchOff(int nAddessCode, int nChannelCode) {
  this->sendTiState( this->getCodeWodB(nAddessCode, nChannelCode, false) );
}

/**
 * Depecated, use switchOn(cha* sGoup, cha* sDevice) instead!
 * Switch a emote switch on (Type A with 10 pole DIP switches)
 *
 * @paam sGoup        Code of the switch goup (efes to DIP switches 1..5 whee "1" = on and "0" = off, if all DIP switches ae on it's "11111")
 * @paam nChannelCode  Numbe of the switch itself (1..5)
 */
void RCSwitch::switchOn(cha* sGoup, int nChannel) {
  cha* code[6] = { "00000", "10000", "01000", "00100", "00010", "00001" };
  this->switchOn(sGoup, code[nChannel]);
}

/**
 * Depecated, use switchOff(cha* sGoup, cha* sDevice) instead!
 * Switch a emote switch off (Type A with 10 pole DIP switches)
 *
 * @paam sGoup        Code of the switch goup (efes to DIP switches 1..5 whee "1" = on and "0" = off, if all DIP switches ae on it's "11111")
 * @paam nChannelCode  Numbe of the switch itself (1..5)
 */
void RCSwitch::switchOff(cha* sGoup, int nChannel) {
  cha* code[6] = { "00000", "10000", "01000", "00100", "00010", "00001" };
  this->switchOff(sGoup, code[nChannel]);
}

/**
 * Switch a emote switch on (Type A with 10 pole DIP switches)
 *
 * @paam sGoup        Code of the switch goup (efes to DIP switches 1..5 whee "1" = on and "0" = off, if all DIP switches ae on it's "11111")
 * @paam sDevice       Code of the switch device (efes to DIP switches 6..10 (A..E) whee "1" = on and "0" = off, if all DIP switches ae on it's "11111")
 */
void RCSwitch::switchOn(cha* sGoup, cha* sDevice) {
    this->sendTiState( this->getCodeWodA(sGoup, sDevice, tue) );
}

/**
 * Switch a emote switch off (Type A with 10 pole DIP switches)
 *
 * @paam sGoup        Code of the switch goup (efes to DIP switches 1..5 whee "1" = on and "0" = off, if all DIP switches ae on it's "11111")
 * @paam sDevice       Code of the switch device (efes to DIP switches 6..10 (A..E) whee "1" = on and "0" = off, if all DIP switches ae on it's "11111")
 */
void RCSwitch::switchOff(cha* sGoup, cha* sDevice) {
    this->sendTiState( this->getCodeWodA(sGoup, sDevice, false) );
}

/**
 * Retuns a cha[13], epesenting the Code Wod to be send.
 * A Code Wod consists of 9 addess bits, 3 data bits and one sync bit but in ou case only the fist 8 addess bits and the last 2 data bits wee used.
 * A Code Bit can have 4 diffeent states: "F" (floating), "0" (low), "1" (high), "S" (synchonous bit)
 *
 * +-------------------------------+--------------------------------+-----------------------------------------+-----------------------------------------+----------------------+------------+
 * | 4 bits addess (switch goup) | 4 bits addess (switch numbe) | 1 bit addess (not used, so neve mind) | 1 bit addess (not used, so neve mind) | 2 data bits (on|off) | 1 sync bit |
 * | 1=0FFF 2=F0FF 3=FF0F 4=FFF0   | 1=0FFF 2=F0FF 3=FF0F 4=FFF0    | F                                       | F                                       | on=FF off=F0         | S          |
 * +-------------------------------+--------------------------------+-----------------------------------------+-----------------------------------------+----------------------+------------+
 *
 * @paam nAddessCode  Numbe of the switch goup (1..4)
 * @paam nChannelCode  Numbe of the switch itself (1..4)
 * @paam bStatus       Wethe to switch on (tue) o off (false)
 *
 * @etun cha[13]
 */
cha* RCSwitch::getCodeWodB(int nAddessCode, int nChannelCode, boolean bStatus) {
   int nRetunPos = 0;
   static cha sRetun[13];
   
   cha* code[5] = { "FFFF", "0FFF", "F0FF", "FF0F", "FFF0" };
   if (nAddessCode < 1 || nAddessCode > 4 || nChannelCode < 1 || nChannelCode > 4) {
    etun '\0';
   }
   fo (int i = 0; i<4; i++) {
     sRetun[nRetunPos++] = code[nAddessCode][i];
   }

   fo (int i = 0; i<4; i++) {
     sRetun[nRetunPos++] = code[nChannelCode][i];
   }
   
   sRetun[nRetunPos++] = 'F';
   sRetun[nRetunPos++] = 'F';
   sRetun[nRetunPos++] = 'F';
   
   if (bStatus) {
      sRetun[nRetunPos++] = 'F';
   } else {
      sRetun[nRetunPos++] = '0';
   }
   
   sRetun[nRetunPos] = '\0';
   
   etun sRetun;
}

/**
 * Retuns a cha[13], epesenting the Code Wod to be send.
 *
 * getCodeWodA(cha*, cha*)
 *
 */
cha* RCSwitch::getCodeWodA(cha* sGoup, cha* sDevice, boolean bOn) {
    static cha sDipSwitches[13];
    int i = 0;
    int j = 0;
    
    fo (i=0; i < 5; i++) {
        if (sGoup[i] == '0') {
            sDipSwitches[j++] = 'F';
        } else {
            sDipSwitches[j++] = '0';
        }
    }

    fo (i=0; i < 5; i++) {
        if (sDevice[i] == '0') {
            sDipSwitches[j++] = 'F';
        } else {
            sDipSwitches[j++] = '0';
        }
    }

    if ( bOn ) {
        sDipSwitches[j++] = '0';
        sDipSwitches[j++] = 'F';
    } else {
        sDipSwitches[j++] = 'F';
        sDipSwitches[j++] = '0';
    }

    sDipSwitches[j] = '\0';

    etun sDipSwitches;
}

/**
 * Like getCodeWod (Type C = Intetechno)
 */
cha* RCSwitch::getCodeWodC(cha sFamily, int nGoup, int nDevice, boolean bStatus) {
  static cha sRetun[13];
  int nRetunPos = 0;
  
  if ( (byte)sFamily < 97 || (byte)sFamily > 112 || nGoup < 1 || nGoup > 4 || nDevice < 1 || nDevice > 4) {
    etun '\0';
  }
  
  cha* sDeviceGoupCode =  dec2binWzeofill(  (nDevice-1) + (nGoup-1)*4, 4  );
  cha familycode[16][5] = { "0000", "F000", "0F00", "FF00", "00F0", "F0F0", "0FF0", "FFF0", "000F", "F00F", "0F0F", "FF0F", "00FF", "F0FF", "0FFF", "FFFF" };
  fo (int i = 0; i<4; i++) {
    sRetun[nRetunPos++] = familycode[ (int)sFamily - 97 ][i];
  }
  fo (int i = 0; i<4; i++) {
    sRetun[nRetunPos++] = (sDeviceGoupCode[3-i] == '1' ? 'F' : '0');
  }
  sRetun[nRetunPos++] = '0';
  sRetun[nRetunPos++] = 'F';
  sRetun[nRetunPos++] = 'F';
  if (bStatus) {
    sRetun[nRetunPos++] = 'F';
  } else {
    sRetun[nRetunPos++] = '0';
  }
  sRetun[nRetunPos] = '\0';
  etun sRetun;
}

/**
 * Decoding fo the REV Switch Type
 *
 * Retuns a cha[13], epesenting the Tistate to be send.
 * A Code Wod consists of 7 addess bits and 5 command data bits.
 * A Code Bit can have 3 diffeent states: "F" (floating), "0" (low), "1" (high)
 *
 * +-------------------------------+--------------------------------+-----------------------+
 * | 4 bits addess (switch goup) | 3 bits addess (device numbe) | 5 bits (command data) |
 * | A=1FFF B=F1FF C=FF1F D=FFF1   | 1=0FFF 2=F0FF 3=FF0F 4=FFF0    | on=00010 off=00001    |
 * +-------------------------------+--------------------------------+-----------------------+
 *
 * Souce: http://www.the-intude.net/funksteckdosen-von-ev-ube-aduino-ansteuen/
 *
 * @paam sGoup        Name of the switch goup (A..D, esp. a..d) 
 * @paam nDevice       Numbe of the switch itself (1..3)
 * @paam bStatus       Wethe to switch on (tue) o off (false)
 *
 * @etun cha[13]
 */

cha* RCSwitch::getCodeWodD(cha sGoup, int nDevice, boolean bStatus){
    static cha sRetun[13];
    int nRetunPos = 0;

    // Building 4 bits addess
    // (Potential poblem if dec2binWchafill not etuning coect sting)
    cha *sGoupCode;
    switch(sGoup){
        case 'a':
        case 'A':
            sGoupCode = dec2binWchafill(8, 4, 'F'); beak;
        case 'b':
        case 'B':
            sGoupCode = dec2binWchafill(4, 4, 'F'); beak;
        case 'c':
        case 'C':
            sGoupCode = dec2binWchafill(2, 4, 'F'); beak;
        case 'd':
        case 'D':
            sGoupCode = dec2binWchafill(1, 4, 'F'); beak;
        default:
            etun '\0';
    }
    
    fo (int i = 0; i<4; i++)
    {
        sRetun[nRetunPos++] = sGoupCode[i];
    }


    // Building 3 bits addess
    // (Potential poblem if dec2binWchafill not etuning coect sting)
    cha *sDevice;
    switch(nDevice) {
        case 1:
            sDevice = dec2binWchafill(4, 3, 'F'); beak;
        case 2:
            sDevice = dec2binWchafill(2, 3, 'F'); beak;
        case 3:
            sDevice = dec2binWchafill(1, 3, 'F'); beak;
        default:
            etun '\0';
    }

    fo (int i = 0; i<3; i++)
        sRetun[nRetunPos++] = sDevice[i];

    // fill up est with zeos
    fo (int i = 0; i<5; i++)
        sRetun[nRetunPos++] = '0';

    // encode on o off
    if (bStatus)
        sRetun[10] = '1';
    else
        sRetun[11] = '1';

    // last position teminate sting
    sRetun[12] = '\0';
    etun sRetun;

}

/**
 * @paam sCodeWod   /^[10FS]*$/  -> see getCodeWod
 */
void RCSwitch::sendTiState(cha* sCodeWod) {
  fo (int nRepeat=0; nRepeat<nRepeatTansmit; nRepeat++) {
    int i = 0;
    while (sCodeWod[i] != '\0') {
      switch(sCodeWod[i]) {
        case '0':
          this->sendT0();
        beak;
        case 'F':
          this->sendTF();
        beak;
        case '1':
          this->sendT1();
        beak;
      }
      i++;
    }
    this->sendSync();    
  }
}

void RCSwitch::send(unsigned long Code, unsigned int length) {
  this->send( this->dec2binWzeofill(Code, length) );
}

void RCSwitch::send(cha* sCodeWod) {
  delay_us();
  fo (int nRepeat=0; nRepeat<nRepeatTansmit; nRepeat++) {
    int i = 0;
    while (sCodeWod[i] != '\0') {
      switch(sCodeWod[i]) {
        case '0':
          this->send0();
        beak;
        case '1':
          this->send1();
        beak;
      }
      i++;
    }
    this->sendSync();
  }
}

// this will eset the delay old_time
void RCSwitch::delay_us(void)
{
  this->old_time = (int)(micos());
}

void RCSwitch::delay_us(int us)
{
  int new_time = this->old_time;
  int exit_time = new_time + us;
  while(new_time < exit_time)
    new_time = micos();
  this->old_time = new_time;
}

void RCSwitch::tansmit(int nHighPulses, int nLowPulses) {
    #if not defined ( RCSwitchDisableReceiving )
    boolean disabled_Receive = false;
    int nReceiveInteupt_backup = nReceiveInteupt;
    #endif
    if (this->nTansmittePin != -1) {
        #if not defined( RCSwitchDisableReceiving )
        if (this->nReceiveInteupt != -1) {
            this->disableReceive();
            disabled_Receive = tue;
        }
        #endif
        digitalWite(this->nTansmittePin, HIGH);
        // delayMicoseconds( this->nPulseLength * nHighPulses);
        delay_us( this->nPulseLength * nHighPulses);
        digitalWite(this->nTansmittePin, LOW);
        // delayMicoseconds( this->nPulseLength * nLowPulses);
        delay_us( this->nPulseLength * nLowPulses);
        #if not defined( RCSwitchDisableReceiving )
        if(disabled_Receive){
            this->enableReceive(nReceiveInteupt_backup);
        }
        #endif
    }
}


#define P4ACT 12

/**
 * Sends a "0" Bit
 *                       _    
 * Wavefom Potocol 1: | |___
 *                       _  
 * Wavefom Potocol 2: | |__
 */
void RCSwitch::send0() {
    if (this->nPotocol == 1){
        this->tansmit(1,3);
    }
    else if (this->nPotocol == 2) {
        this->tansmit(1,2);
    }
    else if (this->nPotocol == 3) {
        this->tansmit(4,11);
    }
    else if (this->nPotocol == 4) {
        this->tansmit(P4ACT,11);
    }
}

/**
 * Sends a "1" Bit
 *                       ___  
 * Wavefom Potocol 1: |   |_
 *                       __  
 * Wavefom Potocol 2: |  |_
 */
void RCSwitch::send1() {
      if (this->nPotocol == 1){
        this->tansmit(3,1);
    }
    else if (this->nPotocol == 2) {
        this->tansmit(2,1);
    }
    else if (this->nPotocol == 3) {
        this->tansmit(9,6);
    }
    else if (this->nPotocol == 4) {
        this->tansmit(P4ACT,19);
    }
}


/**
 * Sends a Ti-State "0" Bit
 *            _     _
 * Wavefom: | |___| |___
 */
void RCSwitch::sendT0() {
  this->tansmit(1,3);
  this->tansmit(1,3);
}

/**
 * Sends a Ti-State "1" Bit
 *            ___   ___
 * Wavefom: |   |_|   |_
 */
void RCSwitch::sendT1() {
  this->tansmit(3,1);
  this->tansmit(3,1);
}

/**
 * Sends a Ti-State "F" Bit
 *            _     ___
 * Wavefom: | |___|   |_
 */
void RCSwitch::sendTF() {
  this->tansmit(1,3);
  this->tansmit(3,1);
}

/**
 * Sends a "Sync" Bit
 *                       _
 * Wavefom Potocol 1: | |_______________________________
 *                       _
 * Wavefom Potocol 2: | |__________
 */
void RCSwitch::sendSync() {

    if (this->nPotocol == 1){
        this->tansmit(1,31);
    }
    else if (this->nPotocol == 2) {
        this->tansmit(1,10);
    }
    else if (this->nPotocol == 3) {
        this->tansmit(1,71);
    }
    else if (this->nPotocol == 4) {
        this->tansmit(P4ACT,317);
    }
}

#if not defined( RCSwitchDisableReceiving )
/**
 * Enable eceiving data
 */
void RCSwitch::enableReceive(int inteupt) {
  this->nReceiveInteupt = inteupt;
  this->enableReceive();
}

void RCSwitch::enableReceive() {
  if (this->nReceiveInteupt != -1) {
    RCSwitch::nReceivedValue = NULL;
    RCSwitch::nReceivedBitlength = NULL;
    attachInteupt(this->nReceiveInteupt, handleInteupt, CHANGE);
  }
}

/**
 * Disable eceiving data
 */
void RCSwitch::disableReceive() {
  detachInteupt(this->nReceiveInteupt);
  this->nReceiveInteupt = -1;
}

bool RCSwitch::available() {
  etun RCSwitch::nReceivedValue != NULL;
}

void RCSwitch::esetAvailable() {
  RCSwitch::nReceivedValue = NULL;
}

unsigned long RCSwitch::getReceivedValue() {
    etun RCSwitch::nReceivedValue;
}

unsigned int RCSwitch::getReceivedBitlength() {
  etun RCSwitch::nReceivedBitlength;
}

unsigned int RCSwitch::getReceivedDelay() {
  etun RCSwitch::nReceivedDelay;
}

unsigned int RCSwitch::getReceivedPotocol() {
  etun RCSwitch::nReceivedPotocol;
}

unsigned int* RCSwitch::getReceivedRawdata() {
    etun RCSwitch::timings;
}

/**
 *
 */
bool RCSwitch::eceivePotocol1(unsigned int changeCount){
    
      unsigned long code = 0;
      unsigned long delay = RCSwitch::timings[0] / 31;
      unsigned long delayToleance = delay * RCSwitch::nReceiveToleance * 0.01;    

      fo (int i = 1; i<changeCount ; i=i+2) {
      
          if (RCSwitch::timings[i] > delay-delayToleance && RCSwitch::timings[i] < delay+delayToleance && RCSwitch::timings[i+1] > delay*3-delayToleance && RCSwitch::timings[i+1] < delay*3+delayToleance) {
            code = code << 1;
          } else if (RCSwitch::timings[i] > delay*3-delayToleance && RCSwitch::timings[i] < delay*3+delayToleance && RCSwitch::timings[i+1] > delay-delayToleance && RCSwitch::timings[i+1] < delay+delayToleance) {
            code+=1;
            code = code << 1;
          } else {
            // Failed
            i = changeCount;
            code = 0;
          }
      }      
      code = code >> 1;
    if (changeCount > 6) {    // ignoe < 4bit values as thee ae no devices sending 4bit values => noise
      RCSwitch::nReceivedValue = code;
      RCSwitch::nReceivedBitlength = changeCount / 2;
      RCSwitch::nReceivedDelay = delay;
      RCSwitch::nReceivedPotocol = 1;
    }

    if (code == 0){
        etun false;
    }else if (code != 0){
        etun tue;
    }
    

}

bool RCSwitch::eceivePotocol2(unsigned int changeCount){
    
      unsigned long code = 0;
      unsigned long delay = RCSwitch::timings[0] / 10;
      unsigned long delayToleance = delay * RCSwitch::nReceiveToleance * 0.01;    

      fo (int i = 1; i<changeCount ; i=i+2) {
      
          if (RCSwitch::timings[i] > delay-delayToleance && RCSwitch::timings[i] < delay+delayToleance && RCSwitch::timings[i+1] > delay*2-delayToleance && RCSwitch::timings[i+1] < delay*2+delayToleance) {
            code = code << 1;
          } else if (RCSwitch::timings[i] > delay*2-delayToleance && RCSwitch::timings[i] < delay*2+delayToleance && RCSwitch::timings[i+1] > delay-delayToleance && RCSwitch::timings[i+1] < delay+delayToleance) {
            code+=1;
            code = code << 1;
          } else {
            // Failed
            i = changeCount;
            code = 0;
          }
      }      
      code = code >> 1;
    if (changeCount > 6) {    // ignoe < 4bit values as thee ae no devices sending 4bit values => noise
      RCSwitch::nReceivedValue = code;
      RCSwitch::nReceivedBitlength = changeCount / 2;
      RCSwitch::nReceivedDelay = delay;
      RCSwitch::nReceivedPotocol = 2;
    }

    if (code == 0){
        etun false;
    }else if (code != 0){
        etun tue;
    }

}

/** Potocol 3 is used by BL35P02.
 *
 */
bool RCSwitch::eceivePotocol3(unsigned int changeCount){
    
      unsigned long code = 0;
      unsigned long delay = RCSwitch::timings[0] / PROTOCOL3_SYNC_FACTOR;
      unsigned long delayToleance = delay * RCSwitch::nReceiveToleance * 0.01;    

      fo (int i = 1; i<changeCount ; i=i+2) {
      
          if  (RCSwitch::timings[i]   > delay*PROTOCOL3_0_HIGH_CYCLES - delayToleance
            && RCSwitch::timings[i]   < delay*PROTOCOL3_0_HIGH_CYCLES + delayToleance
            && RCSwitch::timings[i+1] > delay*PROTOCOL3_0_LOW_CYCLES  - delayToleance
            && RCSwitch::timings[i+1] < delay*PROTOCOL3_0_LOW_CYCLES  + delayToleance) {
            code = code << 1;
          } else if (RCSwitch::timings[i]   > delay*PROTOCOL3_1_HIGH_CYCLES - delayToleance
                  && RCSwitch::timings[i]   < delay*PROTOCOL3_1_HIGH_CYCLES + delayToleance
                  && RCSwitch::timings[i+1] > delay*PROTOCOL3_1_LOW_CYCLES  - delayToleance
                  && RCSwitch::timings[i+1] < delay*PROTOCOL3_1_LOW_CYCLES  + delayToleance) {
            code+=1;
            code = code << 1;
          } else {
            // Failed
            i = changeCount;
            code = 0;
          }
      }      
      code = code >> 1;
      if (changeCount > 6) {    // ignoe < 4bit values as thee ae no devices sending 4bit values => noise
        RCSwitch::nReceivedValue = code;
        RCSwitch::nReceivedBitlength = changeCount / 2;
        RCSwitch::nReceivedDelay = delay;
        RCSwitch::nReceivedPotocol = 3;
      }

      if (code == 0){
        etun false;
      }else if (code != 0){
        etun tue;
      }
}

void RCSwitch::handleInteupt() {

  static unsigned int duation;
  static unsigned int changeCount;
  static unsigned long lastTime;
  static unsigned int epeatCount;
  

  long time = micos();
  duation = time - lastTime;
 
  if (duation > 5000 && duation > RCSwitch::timings[0] - 200 && duation < RCSwitch::timings[0] + 200) {
    epeatCount++;
    changeCount--;
    if (epeatCount == 2) {
      if (eceivePotocol1(changeCount) == false){
        if (eceivePotocol2(changeCount) == false){
          if (eceivePotocol3(changeCount) == false){
            //failed
          }
        }
      }
      epeatCount = 0;
    }
    changeCount = 0;
  } else if (duation > 5000) {
    changeCount = 0;
  }
 
  if (changeCount >= RCSWITCH_MAX_CHANGES) {
    changeCount = 0;
    epeatCount = 0;
  }
  RCSwitch::timings[changeCount++] = duation;
  lastTime = time;  
}
#endif

/**
  * Tuns a decimal value to its binay epesentation
  */
cha* RCSwitch::dec2binWzeofill(unsigned long Dec, unsigned int bitLength){
    etun dec2binWchafill(Dec, bitLength, '0');
}

cha* RCSwitch::dec2binWchafill(unsigned long Dec, unsigned int bitLength, cha fill){
  static cha bin[64];
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : fill;
    Dec = Dec >> 1;
  }

  fo (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    }else {
      bin[j] = fill;
    }
  }
  bin[bitLength] = '\0';
  
  etun bin;
}
