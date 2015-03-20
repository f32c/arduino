
extern "C" {
#include <io.h>
#include <mips/asm.h>
#include <mips/cpuregs.h>
#include <sys/isr.h>
}

static int tsc_next;
static int irq7_ticks;

static int
tsc_isr(void)
{
  
  irq7_ticks++;
  tsc_next += VARIANT_MCK;
  mtc0_macro(tsc_next, MIPS_COP_0_COMPARE);
  return (1);
}

static struct isr_link tsc_isr_link = {NULL, &tsc_isr};

void setup() {
  // put your setup code here, to run once:
  isr_register_handler(7, &tsc_isr_link);
  mfc0_macro(tsc_next, MIPS_COP_0_COUNT);
  tsc_next += VARIANT_MCK;
  mtc0_macro(tsc_next, MIPS_COP_0_COMPARE);
  asm("ei");
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(irq7_ticks);
  delay(100);
}
