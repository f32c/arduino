// Dummy synth example
// Press buttons and hear some sounds.
// For a more useful synth, see "midisynth" example.

volatile uint32_t *voice = (uint32_t *)0xFFFFFBB0; // voices
volatile uint32_t *pitch  = (uint32_t *)0xFFFFFBB4; // frequency for prev written voice

// Initialize each of 128 sinewave channels
// to some frequency calculated in for-loop.
// All channels initially muted (off).
void setup() {
  for(int i = 0; i < 128; i++)
  {
    *voice = i | (0<<8); // channel number or'd with volume 0 (mute)
    *pitch = 1000000+2000*i; // frequency coefficient for the channel
  }
}

// When a button is pressed, volume of 
// a channel corresponding to the pressed button
// is set to some audible volume.
void loop()
{
  for(int i = 1; i <= 6; i++)
    *voice = (6*i) // channel number in lower byte
           | (digitalRead(i)*1000<<8); // amplitude=1000 during keypress
}
