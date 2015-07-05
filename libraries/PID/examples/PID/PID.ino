#include <PID.h>

#define N 4

void PID_print(int i)
{
  char line[255];

  sprintf(line, "PID%d SP=%9d PV=%9d P=%3d I=%3d D=%3d OUT=%5d",
    i, PID[i].sp, PID[i].pv, PID[i].p, PID[i].i, PID[i].d, PID[i].out
  ); 
  Serial.println(line);
}

void PID_hex(void)
{
  char line[255];
  uint32_t *rp = (uint32_t *) PID;
  
  for(int j = 0; j < 4; j++)
  {
    sprintf(line, "PID%d %08x",
      j, rp[j]
    ); 
    Serial.println(line);
  }
}

void setup() {
  int i;

  for(i = 0; i < N; i++)
  {
    PID[i].sp = 0;
    PID[i].p = 10;
    PID[i].i = 11;
    PID[i].d =  1;
  }
  Serial.begin(115200);
}

void loop() {
  int ip;
  static int sp, state;
  int phase;
  int interval = 6;
  int jump[4] = {100000, -100000, 110000, -110000};

  for(ip = 0; ip < N; ip++)
  {
    phase = state & ((1<<interval)-1);
    /*
    each pid will have different offset
    */
    if(phase == 0)
      PID[ip].sp += (state >> interval) & 1 ? -jump[ip] : jump[ip];
    if(phase < 8)
      PID_print(ip);
  }
  delay(200);
  state++;
}
