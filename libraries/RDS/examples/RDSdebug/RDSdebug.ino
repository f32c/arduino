/*
This is a test program that dynamically updates RDS message.
AUTHOR=EMARD
LICENSE=GPL
*/

#include <RDS.h>

RDS rds = RDS();

uint16_t pi = 0xCAFE;
char ps[9] = "TEST1234";
char rt[65] = "ABCDEFGH";

void setup() {
  int i;
  for(i = 0; i < sizeof(rt)-1; i++)
    rt[i] = '@'+i; // ascii map
  /* Setup initial RDS text */
  rds.set_pi(pi); // station numeric ID
  rds.set_ps(ps); // 8-char text, displayed as station name
  rds.set_rt(rt); // 64-char text, not every radio displays it
  Serial.begin(115200);
}

struct rdsdebug {
  int rds:7, dummy1:1;
  int pilot:7, dummy2:1;
  int stereo:7, dummy3:1;
};

void loop()
{
  static uint8_t number;
  char line[100];
  volatile uint32_t *rdsdebug = (volatile uint32_t *)0xFFFFFC00;
  int8_t rds, pilot, stereo;
  volatile struct rdsdebug *debug = (volatile struct rdsdebug *)rdsdebug;
 

  snprintf(line, sizeof(line), 
    "%08x stereo=%3d pilot=%3d rds=%3d", 
    *rdsdebug, debug->stereo, debug->pilot, debug->rds);
  Serial.println(line);

  delay(10); // wait 2 seconds
  number++; // increment number
}
