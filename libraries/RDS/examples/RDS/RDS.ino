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
  rds.pi(pi); // station numeric ID
  rds.stereo(0); // 0-Inform over RDS that we send Mono, 1-Stereo
  rds.ta(0);  // 0-No, 1-Traffice Announcements
  rds.ps(ps); // 8-char text, displayed as station name
  rds.rt(rt); // 64-char text, not every radio displays it
  rds.Hz(107900000); // Hz carrier wave frequency
  rds.length(260); // bytes message length (260 default)
  Serial.begin(115200);
}

void loop()
{
  static uint8_t number;

  snprintf(ps, sizeof(ps), "TEST%04d", number % 10000);
  snprintf(rt, sizeof(rt), "%05d Zzz...", number % 100000);

  // send strings for transmission
  rds.ps(ps);
  rds.rt(rt);
  // rds.ct(2015,7,22,15,30,900);

  // print actual status on serial
  Serial.print("0x");
  Serial.print(pi, HEX);
  Serial.print(" ");
  Serial.print(ps);
  Serial.print(" ");
  Serial.println(rt);

  delay(2000); // wait 2 seconds
  number++; // increment number
}
