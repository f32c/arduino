/* PWM changing sequence
 * tested at 100MHz f32c CPU
 */
int ocp1_led = 9;         // the pin for OCP1 channel (hardwired)
int ocp2_led = 10;         // the pin for OCP2 channel (hardwired)
int control_led1 = 13;
int control_led2 = 14;
int monitor_gpio = 32;
volatile uint8_t *monitor_pointer; // address of GPIO for fast access

// patch defines to use OCP instead of ICP
int ocp3_led = 11; // the pin to which OCP3 interrupt attaches
#define OCP3 ICP2
#define TC_OCP3_START TC_ICP2_START
#define TC_OCP3_STOP TC_ICP2_STOP
#define TCTRL_IE_OCP3 TCTRL_IE_ICP2
#define TCTRL_IF_OCP3 TCTRL_IF_ICP2
#define TCTRL_ENABLE_OCP3 TCTRL_ENABLE_ICP2
#define TCTRL_AND_OR_OCP3 TCTRL_AND_OR_ICP2

int ocp4_led = 8; // the pin to which OCP4 interrupt attaches
#define OCP4 ICP1
#define TC_OCP4_START TC_ICP1_START


static uint32_t icount = 0, iset = 0, nswitch = 0;

/*
 * -- timer hardware pins for ulx2s board
 *    icp(0) => j2_4 (JA7), icp(1) => j2_11 (JA3)
 *    ocp(0) => j2_3 (JA8), ocp(1) => j2_10 (JA4)
 */

struct S_pwm_set
{
  uint32_t increment;
  uint16_t start[2], stop[2];
  uint16_t change;
  uint32_t count; // how many periods to keep this set
};

// human readable frequency change plan
struct S_frequency_switching_plan
{
  uint32_t f, dtc, n;
};

#if 1
struct S_frequency_switching_plan FSP[] =
{ // frequency, duty cycle,   count
  //        Hz,    /100000,      x1
  {     250000,          0,  250000 }, // OFF for 1 second
  {     250000,      45000,   25000 }, 
  {     250000,      45000,     500 },
  {     275000,      40000,      50 },
  {     250000,      45000,     500 },
  {     246000,      44000,      50 },
  {     250000,      45000,     500 },
  {     248000,      36000,      50 },
  {     250000,      45000,     500 },
  {     250000,      45000,     500 },
  {    1000000,      45000,      50 },
};
#endif

#if 0
// debugging sequence with short counts
// for easier counting on the scope
struct S_frequency_switching_plan FSP[] =
{ // frequency, duty cycle,   count
  //        Hz,    /100000,      x1
  {     250000,          0,       3 }, // OFF
  {     250000,      45000,       3 }, 
  {     250000,      45000,       3 },
  {     275000,      40000,       3 },
  {     250000,      45000,       3 },
  {     246000,      44000,       3 },
  {     250000,      45000,       3 },
  {     248000,      36000,       3 },
  {     250000,      45000,       3 },
  {     250000,      45000,       3 },
  {    1000000,      45000,      50 },
};
#endif

#if 0
// debugging sequence with short counts
// for easier counting on the scope
struct S_frequency_switching_plan FSP[] =
{ // frequency, duty cycle,   count
  //        Hz,    /100000,      x1
  {      25000,          0,      4 }, // OFF
  {      26000,      30000,      4 },
  {      27000,      45000,      4 },
  {      30000,      30000,      4 },
};
#endif

enum { N_changes = sizeof(FSP)/sizeof(FSP[0]) };

#define PHASE_FULL 4096
#define COUNTER_MASK 0xFFF

struct S_pwm_set pwm_set[N_changes];

void ocp3_isr(void)
{
  if(++icount >= nswitch)
  { // as soon as possible apply settings which are previously prepared
    EMARD_TIMER[TC_APPLY] = // atomic apply (freq,duty)
      (1<<TC_INCREMENT)
    | (1<<TC_OCP1_START)
    | (1<<TC_OCP1_STOP)
    | (1<<TC_OCP2_START)
    | (1<<TC_OCP2_STOP)
    | (1<<TC_OCP3_START)
    ;
    *monitor_pointer = iset;
    nswitch = pwm_set[iset].count;
    if(++iset >= N_changes) // change set index and prepare next change
      iset = 0; // wraparound
    EMARD_TIMER[TC_INCREMENT] = pwm_set[iset].increment;
    EMARD_TIMER[TC_OCP1_START] = pwm_set[iset].start[0];
    EMARD_TIMER[TC_OCP1_STOP] = pwm_set[iset].stop[0];
    EMARD_TIMER[TC_OCP2_START] = pwm_set[iset].start[1];
    EMARD_TIMER[TC_OCP2_STOP] = pwm_set[iset].stop[1];
    icount = 0; // reset count for the next interrupt
    // set triggering phase for ISR to make the next switch "smooth"
    EMARD_TIMER[TC_OCP3_START] = pwm_set[iset].change;
  }
}

void setup()
{
  int i;

  monitor_pointer = (volatile uint8_t *) portOutputRegister(digitalPinToPort(monitor_gpio)); // clean way for fast access
  pinMode(monitor_gpio, OUTPUT);

  // convert from human readable frequency switching plan 
  // to timer register switching PWM set
  for(i = 0; i < N_changes; i++)
  {
    pwm_set[i].increment = ((uint64_t)(FSP[i].f)) * (1<<22) / F_CPU;
    pwm_set[i].start[0] = (0            - (uint64_t)(PHASE_FULL/2) * (FSP[i].dtc) / 100000) & COUNTER_MASK;
    pwm_set[i].stop[0]  = (0            + (uint64_t)(PHASE_FULL/2) * (FSP[i].dtc) / 100000) & COUNTER_MASK;
    pwm_set[i].start[1] = (PHASE_FULL/2 - (uint64_t)(PHASE_FULL/2) * (FSP[i].dtc) / 100000) & COUNTER_MASK;
    pwm_set[i].stop[1]  = (PHASE_FULL/2 + (uint64_t)(PHASE_FULL/2) * (FSP[i].dtc) / 100000) & COUNTER_MASK;
    if(FSP[i].f < 900000)
      pwm_set[i].count = FSP[i].n; // count (# of periods)
    else
      pwm_set[i].count = FSP[i].n * 7 / 16; // experimentally determined to compensate loss of interrupt counter
  }

  for(i = 0; i < N_changes; i++)
  {
    int i_next = (i+1) % N_changes;
    #if 1
    if(pwm_set[i].stop[0] < pwm_set[i_next].stop[0])
      pwm_set[i].change = (pwm_set[i].stop[0] - 150) & COUNTER_MASK ;
    else
      pwm_set[i].change = (pwm_set[i].stop[1] - 150) & COUNTER_MASK;
    #endif
    #if 0
    if(pwm_set[i].stop[0] < 2048)
      pwm_set[i].change = pwm_set[i].stop[1];
    else
      pwm_set[i].change = pwm_set[i].stop[1];
    #endif
    // limit range to reliably trigger OCP3
    // ocp3 stop is 4095
    if(pwm_set[i].change < 7)
      pwm_set[i].change = 7;
    if(pwm_set[i].change > 4088)
      pwm_set[i].change = 4088;
  }

  nswitch = pwm_set[0].count;
  // set initial timer registers to first PWM set
  EMARD_TIMER[TC_INCREMENT] = pwm_set[0].increment;
  EMARD_TIMER[TC_OCP1_START] = pwm_set[0].start[0];
  EMARD_TIMER[TC_OCP1_STOP] = pwm_set[0].stop[0];
  EMARD_TIMER[TC_OCP2_START] = pwm_set[0].start[1];
  EMARD_TIMER[TC_OCP2_STOP] = pwm_set[0].stop[1];

  // ocp2 rising edge interrupt is used
  // precisely schedule time of the phase change of ocp1
  EMARD_TIMER[TC_OCP3_START] = pwm_set[0].change;
  EMARD_TIMER[TC_OCP3_STOP] = 4095;

  EMARD_TIMER[TC_APPLY] =
      (1<<TC_INCREMENT)
    | (1<<TC_OCP1_START)
    | (1<<TC_OCP1_STOP)
    | (1<<TC_OCP2_START)
    | (1<<TC_OCP2_STOP)
    | (1<<TC_OCP3_START)
    | (1<<TC_OCP3_STOP);

  EMARD_TIMER[TC_CONTROL] =
      (0<<TCTRL_AND_OR_OCP1) // "OR" mode (0-crossing)
    | (1<<TCTRL_ENABLE_OCP1) // enable output
    | (0<<TCTRL_IE_OCP1) // no interrupt for ocp1
    | (1<<TCTRL_IF_OCP1) // clear the interrupt flag
    | (1<<TCTRL_AND_OR_OCP2) // "AND" mode
    | (1<<TCTRL_ENABLE_OCP2) // enable output
    | (0<<TCTRL_IE_OCP2) // no interrupt for ocp2
    | (1<<TCTRL_IF_OCP2) // clear the interrupt flag
    | (1<<TCTRL_AND_OR_OCP3) // "AND" mode
    | (0<<TCTRL_ENABLE_OCP3) // no output
    | (0<<TCTRL_IE_OCP3) // ocp3 will trigger interrupt
    | (1<<TCTRL_IF_OCP3) // clear the interrupt flag
    ;
  attachInterrupt(ocp3_led, ocp3_isr, RISING); // rising edge of OCP3 will trigger interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

