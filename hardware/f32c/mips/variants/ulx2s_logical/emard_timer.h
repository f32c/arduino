
#ifndef _f32c_mips_emard_timer_h
#define _f32c_mips_emard_timer_h

#include <stdint.h>

/* CPU clock in Hz */
// #define CLOCK 81207000L
#define TIMER_CLOCK  VARIANT_MCK
/* bits of the timer counter C_bits */
#define TIMER_BITS 12
/* register of size more or equal max(C_bits, C_bits+C_pres), max 32 bits */
#define R_TYPE uint32_t
/* number of prescaler bits from vhd C_pres */
#define PRESCALER_BITS 10
/* number of fractional period bits from vhd C_period_frac */
#define FRAC_BITS 0

/* timer base address in i/o space */
#define EMARD_TIMADR -128

/* timer registers */
#define TC_COUNTER    0
#define TC_INCREMENT  1
#define TC_INC_MIN    2
#define TC_INC_MAX    3
#define TC_PERIOD     TC_INC_MIN
#define TC_FRACTIONAL TC_INC_MAX
#define TC_OCP1_START 4
#define TC_OCP1_STOP  5
#define TC_OCP2_START 6
#define TC_OCP2_STOP  7
#define TC_ICP1_START 10
#define TC_ICP1_STOP  11
#define TC_ICP2_START 8
#define TC_ICP2_STOP  9
#define TC_ICP1       12
#define TC_ICP2       13
#define TC_CONTROL    14
#define TC_APPLY      15

/* timer control bits */
#define TCTRL_IF_OCP1 (0)
#define TCTRL_IF_OCP2 (1)
#define TCTRL_IF_ICP1 (3)
#define TCTRL_IF_ICP2 (2)
#define TCTRL_AND_OR_OCP1 (4)
#define TCTRL_AND_OR_OCP2 (5)
#define TCTRL_AND_OR_ICP1 (7)
#define TCTRL_AND_OR_ICP2 (6)
#define TCTRL_IE_OCP1 (8)
#define TCTRL_IE_OCP2 (9)
#define TCTRL_IE_ICP1 (11)
#define TCTRL_IE_ICP2 (10)
#define TCTRL_XOR_OCP1 (12)
#define TCTRL_XOR_OCP2 (13)
#define TCTRL_XOR_ICP1 (15)
#define TCTRL_XOR_ICP2 (14)
#define TCTRL_AFCEN_ICP1 (16)
#define TCTRL_AFCEN_ICP2 (18)
#define TCTRL_AFCINV_ICP1 (17)
#define TCTRL_AFCINV_ICP2 (19)
#define TCTRL_ENABLE_OCP1 (20)
#define TCTRL_ENABLE_OCP2 (21)
#define TCTRL_ENABLE_ICP1 (23)
#define TCTRL_ENABLE_ICP2 (22)

extern volatile uint32_t *EMARD_TIMER; 
#define EMARD_TIMER_ADDRESS (uint32_t *)(EMARD_TIMADR)

/* timer pwm channel intializer 
 * for bitmask struct
 */
#define VARIANT_PWM_CHANNEL_ENABLE \
{ \
  {                                          \
      (1<<TCTRL_ENABLE_OCP1)  /* control_and */  \
    | (1<<TCTRL_ENABLE_OCP1),                \
      (1<<TCTRL_ENABLE_OCP1), /* control_or */   \
      (1<<TC_CONTROL)         /* apply */        \
    | (1<<TC_INCREMENT)                      \
    | (1<<TC_OCP1_START) | (1<<TC_OCP1_STOP) \
    | (0<<TC_OCP2_START) | (0<<TC_OCP2_STOP) \
    | (0<<TC_ICP1_START) | (0<<TC_ICP1_STOP) \
    | (0<<TC_ICP2_START) | (0<<TC_ICP2_STOP) \
    | (0<<TC_INC_MIN)    | (0<<TC_INC_MAX)   \
    | (0<<TC_ICP1)       | (0<<TC_ICP2),     \
    /* indexes of start/stop register */     \
    TC_OCP1_START, TC_OCP1_STOP              \
  },                                         \
  {                                          \
      (1<<TCTRL_ENABLE_OCP2)  /* control_and */  \
    | (1<<TCTRL_ENABLE_OCP2),                \
      (1<<TCTRL_ENABLE_OCP2), /* control_or */   \
      (1<<TC_CONTROL)         /* apply */        \
    | (1<<TC_INCREMENT)                      \
    | (0<<TC_OCP1_START) | (0<<TC_OCP1_STOP) \
    | (1<<TC_OCP2_START) | (1<<TC_OCP2_STOP) \
    | (0<<TC_ICP1_START) | (0<<TC_ICP1_STOP) \
    | (0<<TC_ICP2_START) | (0<<TC_ICP2_STOP) \
    | (0<<TC_INC_MIN)    | (0<<TC_INC_MAX)   \
    | (0<<TC_ICP1)       | (0<<TC_ICP2),     \
    /* indexes of start/stop register */     \
    TC_OCP2_START, TC_OCP2_STOP              \
  },                                         \
}

/*
 *  timer interrupt
 */
#define VARIANT_TIMER_INTERRUPT 4

#endif /* _f32c_mips_emard_timer_h */
