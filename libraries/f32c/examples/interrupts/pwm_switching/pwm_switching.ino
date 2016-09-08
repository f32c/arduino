/* PWM changing sequence
 * recommended clock 100MHz f32c CPU
 * PWM     output: OCP1 (dedicated pin) and led pin 9
 * monitor output: arduino pin 32
 */
int ocp1_led = 9;         // the pin for OCP1 channel (hardwired)
int ocp2_led = 10;         // the pin for OCP2 channel (hardwired)
int control_led1 = 13;
int control_led2 = 14;
int monitor_gpio = 32;
volatile uint8_t *monitor_pointer; // address of GPIO for fast access

static uint32_t icount = 0, iset = 0, nswitch = 0;

/*
 * -- timer hardware pins for ulx2s board
 *    icp(0) => j2_4 (JA7), icp(1) => j2_11 (JA3)
 *    ocp(0) => j2_3   led_pointer = (volatile uint8_t *) portOutputRegister(digitalPinToPort(led)); // clean way
(JA8), ocp(1) => j2_10 (JA4)
 */

struct S_pwm_set
{
  uint32_t increment;
  uint16_t start, stop, change;
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

enum { N_changes = sizeof(FSP)/sizeof(FSP[0]) };

#define PHASE_CENTER 2048
#define PHASE_FULL 4096
#define COUNTER_MASK 0xFFF

struct S_pwm_set pwm_set[N_changes];
/*
= 
{//freq,  start,              stop,               count
  {FREQ1, (PHASE_CENTER-DUTY1)&COUNTER_MASK, (PHASE_CENTER+DUTY1)&COUNTER_MASK, 3}, 
  {FREQ2, (PHASE_CENTER-DUTY2)&COUNTER_MASK, (PHASE_CENTER+DUTY2)&COUNTER_MASK, 3},
};
*/

void ocp2_isr(void)
{
  if(++icount >= nswitch)
  { // as soon as possible apply settings which are previously prepared
    EMARD_TIMER[TC_APPLY] = // atomic apply (freq,duty)
      (1<<TC_INCREMENT)
    | (1<<TC_OCP1_START)
    | (1<<TC_OCP1_STOP);
    *monitor_pointer = iset;
    nswitch = pwm_set[iset].count;
    if(++iset >= N_changes) // change set index and prepare next change
      iset = 0; // wraparound
    EMARD_TIMER[TC_INCREMENT] = pwm_set[iset].increment;
    EMARD_TIMER[TC_OCP1_START] = pwm_set[iset].start;
    EMARD_TIMER[TC_OCP1_STOP] = pwm_set[iset].stop;
    icount = 0; // reset count for the next interrupt
    // set triggering phase for ISR to make the next switch "smooth"
    EMARD_TIMER[TC_OCP2_START] = pwm_set[iset].change;
    EMARD_TIMER[TC_APPLY] = 1<<TC_OCP2_START;
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
    pwm_set[i].start = PHASE_CENTER - (uint64_t)(PHASE_FULL/2) * (FSP[i].dtc) / 100000;
    pwm_set[i].stop  = PHASE_CENTER + (uint64_t)(PHASE_FULL/2) * (FSP[i].dtc) / 100000;
    pwm_set[i].change = pwm_set[i].stop * 2/3; // experimentally determined change time
    if(FSP[i].f < 900000)
      pwm_set[i].count = FSP[i].n; // count (# of periods)
    else
      pwm_set[i].count = FSP[i].n * 7 / 16; // experimentally determined to compensate loss of interrupt counter
  }

  nswitch = pwm_set[0].count;
  // set initial timer registers to first PWM set
  EMARD_TIMER[TC_INCREMENT] = pwm_set[0].increment;
  EMARD_TIMER[TC_OCP1_START] = pwm_set[0].start;
  EMARD_TIMER[TC_OCP1_STOP] = pwm_set[0].stop;

  // ocp2 rising edge interrupt is used
  // precisely schedule time of the phase change of ocp1
  EMARD_TIMER[TC_OCP2_START] = 1000;
  EMARD_TIMER[TC_OCP2_STOP] = 2047;

  EMARD_TIMER[TC_APPLY] =
      (1<<TC_INCREMENT)
    | (1<<TC_OCP1_START)
    | (1<<TC_OCP1_STOP)
    | (1<<TC_OCP2_START)
    | (1<<TC_OCP2_STOP);

  EMARD_TIMER[TC_CONTROL] =
      (1<<TCTRL_AND_OR_OCP1)
    | (0<<TCTRL_ENABLE_OCP1)
    | (0<<TCTRL_IE_OCP1) // no interrupt for ocp1
    | (1<<TCTRL_IF_OCP1) // clear the interrupt flag
    | (1<<TCTRL_AND_OR_OCP2)
    | (1<<TCTRL_ENABLE_OCP2)
    | (1<<TCTRL_IE_OCP2) // ocp2 will trigger interrupt
    | (1<<TCTRL_IF_OCP2) // clear the interrupt flag
    ;
  attachInterrupt(ocp2_led, ocp2_isr, RISING); // rising edge of OCP2 will trigger interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

