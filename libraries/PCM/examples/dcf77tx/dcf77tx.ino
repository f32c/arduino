/*
 * (c) 2003 maZZoo_SPIDER_MONKEY_gmx_DOT_de
 * License: GPLv2, see http://www.gnu.org/licenses/gpl.txt
 * transmits DCF77 time information via arduino pin
 * Modified for Arduino by EMARD
 * Currently noone reported success with
 * setting DCF77 clock with this
 */
#include <math.h>
#include <PCM.h>
#include <RDS.h>
PCM pcm = PCM();
// RDS rds = RDS();

struct sample
{
  int16_t ch[2]; // 0:left and 1:right channel, signed 16 bit integers
};

const int       fC = 77500;  /* 77.5kHz carrier-frequency */
//const int       fC = 1600;  /* audible 7750 carrier-frequency */ 
const int nperiod = 1; // number of sinewave periods in the buffer

// crude malloc()
volatile struct sample *buf = (volatile struct sample *) 0x80080000;
int buflen = 64; // number of samples in the buffer

#ifndef uint
#  define uint unsigned int
#endif
#ifndef ulong
#  define ulong unsigned long
#endif

#define LED 13

// PCM amplitude
#define VOLUME 32000

void sinewave() {
  // set audio parameters
  int i, vol = VOLUME, rate = fC*buflen; // buflen*fC;
  pcm.dma((uint32_t *)buf, buflen);
  pcm.rate(rate);
  pcm.volume(0,0);
  for(i = 0; i < buflen; i++)
  {
    int16_t left  = 32767 * sin(2*M_PI/buflen * i * nperiod);
    int16_t right = 32767 * sin(2*M_PI/buflen * i * nperiod);
    uint16_t mono;
    // left = right = 0;
    // mono = left+right;
    buf[i].ch[0] = left;
    buf[i].ch[1] = right;
  }
  // pcm.volume(vol, vol);
  // rds.Hz(107900000); // play tone on FM radio (use vol 20000)
}

void wave(uint8_t percent)
{
  int16_t vol = VOLUME * percent/100;
  pcm.volume(vol, vol);
}

int countbits_ul(unsigned long v){
  int ret = 0;
  unsigned int i=0;
  
  for (; i<8*sizeof(unsigned long); i++){
    if (v&1) ret++;
    v>>=1;
  }
  return ret;
}

inline int parity_min_hour(uint lo, uint hi){
  int ret = 0; /* even parity */
  ret ^= (lo & 1) ? 1:0;
  ret ^= (lo & 2) ? 1:0;
  ret ^= (lo & 4) ? 1:0;
  ret ^= (lo & 8) ? 1:0;
  ret ^= (hi & 1) ? 1:0;
  ret ^= (hi & 2) ? 1:0;
  ret ^= (hi & 4) ? 1:0; /* will be zero in case of hours */

  return ret;
}

inline int parity_day_thru_year(uint daylo,   uint dayhi, uint weekday,
				uint monthlo, uint monthhi,
				uint yearlo,  uint yearhi ){
  int ret = 0; /* even parity */
  ret ^= (daylo & 1) ? 1:0;
  ret ^= (daylo & 2) ? 1:0;
  ret ^= (daylo & 4) ? 1:0;
  ret ^= (daylo & 8) ? 1:0;
  ret ^= (dayhi & 1) ? 1:0;
  ret ^= (dayhi & 2) ? 1:0;
  
  ret ^= (weekday & 1) ? 1:0;
  ret ^= (weekday & 2) ? 1:0;
  ret ^= (weekday & 4) ? 1:0;
  
  ret ^= (monthlo & 1) ? 1:0;
  ret ^= (monthlo & 2) ? 1:0;
  ret ^= (monthlo & 4) ? 1:0;
  ret ^= (monthlo & 8) ? 1:0;
  ret ^= (monthhi & 1) ? 1:0;

  ret ^= (yearlo & 1) ? 1:0;
  ret ^= (yearlo & 2) ? 1:0;
  ret ^= (yearlo & 4) ? 1:0;
  ret ^= (yearlo & 8) ? 1:0;
  ret ^= (yearhi & 1) ? 1:0;
  
  return ret;
}

void setTimestring(char t[], uint min, uint hour, uint day, uint weekday, uint month, uint year){
  
  char * pt;
  uint minlow;
  uint minhi;
  uint hourlow;
  uint hourhi;
  uint daylow;
  uint dayhi;
  uint monthlow;
  uint monthhi;
  uint yearlow;
  uint yearhi;

  memset( t, '0', 60*sizeof(char));

  t[18] = '1'; // CET
  t[20] = '1';

  pt = t+21;
  
  minlow = min%10;
  
  *pt += (minlow & 1)>>0; pt++; // Minute Einer
  *pt += (minlow & 2)>>1; pt++;
  *pt += (minlow & 4)>>2; pt++;
  *pt += (minlow & 8)>>3; pt++;
  
  minhi = min - minlow;
  minhi /= 10;
  
  *pt += (minhi & 1)>>0; pt++; // Minute Zehner
  *pt += (minhi & 2)>>1; pt++;
  *pt += (minhi & 4)>>2; pt++;
  
  *pt += parity_min_hour(minlow, minhi); pt++; // P1
  
  hourlow = hour%10;
  
  *pt += (hourlow & 1)>>0; pt++; // Stunde Einer
  *pt += (hourlow & 2)>>1; pt++;
  *pt += (hourlow & 4)>>2; pt++;
  *pt += (hourlow & 8)>>3; pt++;
  
  hourhi = hour - hourlow;
  hourhi /= 10;
  
  *pt += (hourhi & 1)>>0; pt++; // Stunde Zehner
  *pt += (hourhi & 2)>>1; pt++;
  
  *pt += parity_min_hour(hourlow, hourhi); pt++; // P2
  
  daylow = day%10;
  
  *pt += (daylow & 1)>>0; pt++; // Tag Einer
  *pt += (daylow & 2)>>1; pt++;
  *pt += (daylow & 4)>>2; pt++;
  *pt += (daylow & 8)>>3; pt++;
  
  dayhi = day - daylow;
  dayhi /= 10;
  
  *pt += (dayhi & 1)>>0; pt++; // Tag Zehner
  *pt += (dayhi & 2)>>1; pt++;
  
  *pt += (weekday & 1)>>0; pt++; // Wochentag
  *pt += (weekday & 2)>>1; pt++;
  *pt += (weekday & 4)>>2; pt++;
  
  monthlow = month%10;
  
  *pt += (monthlow & 1)>>0; pt++; // Monat Einer
  *pt += (monthlow & 2)>>1; pt++;
  *pt += (monthlow & 4)>>2; pt++;
  *pt += (monthlow & 8)>>3; pt++;
  
  monthhi = month - monthlow;
  monthhi /= 10;
  
  *pt += (monthhi & 1)>>0; pt++; // Monat Zehner
  
  yearlow = year%10;

  *pt += (yearlow & 1)>>0; pt++; // Jahr Einer
  *pt += (yearlow & 2)>>1; pt++;
  *pt += (yearlow & 4)>>2; pt++;
  *pt += (yearlow & 8)>>3; pt++;
  
  yearhi = year - yearlow;
  yearhi /= 10;
  
  *pt += (yearhi & 1)>>0; pt++; // Jahr Einer
  *pt += (yearhi & 2)>>1; pt++;
  *pt += (yearhi & 4)>>2; pt++;
  *pt += (yearhi & 8)>>3; pt++;

  *pt += parity_day_thru_year(daylow, dayhi, weekday, monthlow, monthhi, yearlow, yearhi); pt++; // P3
  
  *pt = '\0'; pt++; // sync-bit
}

int32_t next_micros; // absolute microseconds of the next second
/* wait for 1 complete second,
transmit pulse (width in microseconds)
*/
static inline void pulse(uint8_t width)
{
  int32_t next_pulse = next_micros + 1000*width;
  while( ((int32_t) (micros()-next_micros) ) < 0);
  if(width)
  {
    wave(15); // 15% of full amplitude
    digitalWrite(LED, HIGH);
    while( ((int32_t) (micros()-next_pulse) ) < 0);
  }
  wave(100); // full amplitude
  digitalWrite(LED, LOW);
  next_micros += 1000000L; // next second
}

  char timestring[60];         /* time information for one minute */
  char *p;

  uint currmin = 23;           /* adjust the default time here */
  uint hour    = 23;
  uint day     = 1;
  uint weekday = 1;
  uint month   = 12;
  uint year    = 4;            /* 0-99 */

  uint8_t tv_last;
  uint8_t tv_dbg;

void setup()
{
  Serial.begin(115200);
  sinewave();
  timestring[59]='\0';
  Serial.println("          1         2         3         4         5");
  Serial.println("012345678901234567890123456789012345678901234567890123456789");
  next_micros = micros();
}

void loop()
{
  char c;
  int  i;
  char infostring[50];
  
    /* sync bit, last second of previous minute */
    pulse(0); // wait full second, no pulse
    // after sync bit we have almost 1 full second to 
    // calculate next time string
    setTimestring(timestring, currmin, hour, day, weekday, month, year);
    // Serial.print(timestring);
    sprintf(infostring, "\n20%02d-%02d-%02d %02d:%02d ", year, month, day, hour, currmin);
    Serial.print(infostring);

    /*
    in each minute:
    first 59 from 60 seconds have pulse
    last second is only a pause without pulse
    */
    for(i = 0; i < 59; i++)
    {
      char bit = timestring[i];
      pulse(bit == '1' ? 200 : 100);
      Serial.print(bit);
    }

    currmin++; /* next minute */
    if (currmin==60){currmin=0;hour++;}
    if (hour   ==24){hour=0;   day++;}
    if (day    ==32){day=1;}           // FIXME there's more ...
}
