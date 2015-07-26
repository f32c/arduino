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

void loop()
{
  static uint8_t number;

  snprintf(ps, sizeof(ps), "TEST%04d", number % 10000);
  snprintf(rt, sizeof(rt), "%05d Zzz...", number % 100000);

  // apply local strings to rds class
  rds.set_ps(ps);
  rds.set_rt(rt);

  // strings changed but not yet transmitting
  rds.send(); // send (update transmitter buffer)

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
