/* FM antenna test
 * Tune FM radio to 107.9 MHz
 * Bring ULX3S within 1 m distance of radio antenna
 * You should hear silence.
 * RDS display should show: "TEST1234"
 * RDS long message (optional on some radios): "@ABCDEFGHIJ.."
 */
 
#include <RDS.h>

RDS rds = RDS();

uint16_t pi = 0xCAFE;
char ps[9] = "TEST1234";
char rt[65] = "ABCDEFGH";

void rds_init() {
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
}
