#include <MIDI.h>

/*
// FOR testing with usb-midi:
// edit ~/Arduino/libraries/MIDI_Library/src/midi_Settings.h
// BaudRate = 38400
// start bridge from alsa to midi serial
// ttymidi -b 38400 -v -s /dev/ttyUSB0
// composer: "musescore"
// use "midish" to sequence MIDI songs
// edit .midishrc and set target MIDI stream to ttymidi
// dnew 0 "ttymidi:1" wo
// run midish. At its prompt load MIDI file and play:
// import "ChildInTime.mid"
// p
// keyboard playing:
// vmpk is soft-MIDI keyboard,
// Edit->Connections->Output MIDI connection:
// select "Midi Through:0" OK,
// then select "ttymidi:1" OK
// press vmpk keys and LED will turn on when a key is pressed
// route events from USB drawbar slider to ttymidi
// aconnect 24:0 128:1
*/

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 13
int led_indicator_pin = LED;
uint8_t led_value = 0;
volatile uint32_t *led_indicator_pointer; // address of LED MMIO register

volatile uint32_t *voice = (uint32_t *)0xFFFFFBB0; // voices
volatile uint32_t *pitch  = (uint32_t *)0xFFFFFBB4; // frequency for prev written voice
int16_t volume[128], target[128];
int8_t transpose = 0; // number of meantones to shift +/-
uint8_t request_freq_table = 1;
uint8_t request_voices_volume_recalculate = 0;
uint32_t freq[128]; // freqency list
const int C_pbm_shift = 24, C_pbm_range = 16384;
uint8_t bend_meantones = 2; // 2 is default, can have range 1-127
uint32_t *pbm; // pitch bend multiplier 0..16383
double pb_inc[128], pb_start[128]; // pitchbend increment and start for each integer range 0-127
uint8_t request_pitch_bend_range = 1; // request recalculation of pitch bend range

uint8_t active_parameter[2] = {127,127}; // MSB[1], LSB[0] of currently active parameter
// see http://www.philrees.co.uk/nrpnq.htm
// 0,0: pitch bend range
// 0,1: fine tuning
// 0,2: coarse tuning
// 0,3: tuning program select
// 0,4: tuning bank select
// 127,127: cancel active parameter

// constants for frequency table caculation
const int C_clk_freq = 50000000; // Hz system clock
const float C_ref_freq = 440.0; // Hz reference tone
const int C_ref_octave = 5; // Octave 4 (MIDI voice 0 is C-1, octave -1)
const int C_ref_tone = 9; // Tone A
const int C_pa_data_bits = 32;
const int C_voice_addr_bits = 7; // 128 voices MIDI standard
const int C_tones_per_octave = 12; // tones per octave, MIDI standard
const float C_cents_per_octave = 1200.0; // cents (tuning) per octave

// Quarter comma temperament (16th century standard)
const float C_temperament_quarter_comma[C_tones_per_octave] =
{ // Quarter comma temperament, 16th century classic
         0.0,        //  0 C
        76.0,        //  1 C#
       193.2,        //  2 D
       310.3,        //  3 Eb
       386.3,        //  4 E
       503.4,        //  5 F
       579.5,        //  6 F#
       696.6,        //  7 G
       772.6,        //  8 G#
       889.7,        //  9 A
      1006.8,        // 10 Bb
      1082.9         // 11 B
};

// Bach's tuning http://www.larips.com/
const float C_temperament_bach[C_tones_per_octave] =
{ // Bach Well Tempered Clavier 1722
         5.9,        //  0 C
       103.9,        //  1 C#
       202.0,        //  2 D
       303.9,        //  3 Eb
       398.0,        //  4 E
       507.8,        //  5 F
       602.0,        //  6 F#
       703.9,        //  7 G
       803.9,        //  8 G#
       900.0,        //  9 A
      1003.9,        // 10 Bb
      1100.0         // 11 B
};

// Standard MIDI has equal temperament (100 cents each)
// Hammond tried to make it, but tonewheels were slightly off-tune
// because a wheel could have no more than 192 teeth.
// Real notes must be a little bit off-tune in order to sound correct :)
const float C_temperament_hammond[C_tones_per_octave] =
{ // Hammond temperament
         0.0,        //  0 C
        99.89267627, //  1 C#
       200.7760963,  //  2 D
       300.488157,   //  3 Eb
       400.180858,   //  4 E
       499.8955969,  //  5 F
       600.6025772,  //  6 F#
       700.5966375,  //  7 G
       799.8695005,  //  8 G#
       900.5764808,  //  9 A
      1000.29122,    // 10 Bb
      1099.983921    // 11 B
};

const float C_temperament_ben_johnston[C_tones_per_octave] =
{ // Ben Johnston's Suite for Microtonal Piano (1977)
         0.0,  //  0 C
       105.0,  //  1 C#
       203.9,  //  2 D
       297.5,  //  3 Eb
       386.3,  //  4 E
       470.8,  //  5 F
       551.3,  //  6 F#
       702.0,  //  7 G
       840.5,  //  8 G#
       905.9,  //  9 A
       968.8,  // 10 Bb
      1088.3   // 11 B
};

const float C_temperament_equal[C_tones_per_octave] =
{ // Equal temperament
         0.0,  //  0 C
       100.0,  //  1 C#
       200.0,  //  2 D
       300.0,  //  3 Eb
       400.0,  //  4 E
       500.0,  //  5 F
       600.0,  //  6 F#
       700.0,  //  7 G
       800.0,  //  8 G#
       900.0,  //  9 A
      1000.0,  // 10 Bb
      1100.0   // 11 B
};

const float *C_temperament = C_temperament_hammond;

// tuning constants (not true constants anymore)
float C_base_freq, C_octave_to_ref, C_tuning_cents;
int C_shift_octave;

// this must be called initially and after each re-tuning
// it is recommended to be called also after
// changing of temperament if referent
// note A (440Hz) changes its cents value.
void tuning_constants_init(void)
{
  // calculate base frequency, this is lowest possible A, meantone_temperament #9
  // downshifting by (C_voice_addr_bits+C_pa_data_bits) is moved to C_shift_octave to avoid floating underflow here
  C_base_freq = C_clk_freq*pow(2.0,C_temperament[C_ref_tone]/C_cents_per_octave);
  // calculate how many octaves (floating point) we need to go up to reach C_ref_freq
  C_octave_to_ref = log(C_ref_freq/C_base_freq)/log(2.0);
  // convert real C_octave_to_ref into octave integer and cents tuning
  // now shift by (C_voice_addr_bits+C_pa_data_bits)
  C_shift_octave = (int)(floor(C_octave_to_ref))+C_voice_addr_bits+C_pa_data_bits-C_ref_octave;
  C_tuning_cents = C_cents_per_octave*(C_octave_to_ref-floor(C_octave_to_ref));
}

uint64_t db_sine1x    = 0x800000000L; // key + 0
uint64_t db_sine3x    = 0x080000000L; // key + 19
uint64_t db_sine2x    = 0x008000000L; // key + 12
uint64_t db_sine4x    = 0x000800000L; // key + 24
uint64_t db_sine6x    = 0x000080000L; // key + 31
uint64_t db_sine8x    = 0x000008000L; // key + 36
uint64_t db_sine10x   = 0x000000800L; // key + 40
uint64_t db_sine12x   = 0x000000080L; // key + 43
uint64_t db_sine16x   = 0x000000008L; // key + 48

uint64_t db_rockorgan = 0x888000000L;
uint64_t db_metalorgan= 0x875050000L;
uint64_t db_brojack   = 0x800000888L;
uint64_t db_vocalist  = 0x784300000L;
uint64_t db_childintime_upper = 0x784300000L; // really 0x777000000 or vocalist is better?
uint64_t db_childintime_lower = 0x745201000L;
uint64_t db_starwars_upper = 0x811100000L;
uint64_t db_starwars_lower = 0x800140000L;
uint64_t db_civilwar_upper = 0x720000000L;
uint64_t db_civilwar_lower = 0x745201000L;

uint64_t reg_upper = db_childintime_upper;
uint64_t reg_lower = db_childintime_lower;

//  0 keys - 1x frequency
// 19 keys - 3x frequency
// 12 keys - 2x frequency
// 24 keys - 4x frequency
// 31 keys - 6x frequency
// 36 keys - 8x frequency
// 40 keys - 10x frequency
// 43 keys - 12x frequency
// 48 keys - 16x frequency
const int C_drawbar_count = 9;
int8_t drawbar_voice[9] = {0,19,12,24,31,36,40,43,48}; // voice offset for drawbars

int key_volume = 1; // key volume 1-7
uint8_t last_pitch = 0; // last note played, for the pitch bend

const int C_voice_num = 1<<C_voice_addr_bits;

int16_t active_keys[C_voice_num], active_bend[C_voice_num]; // tracks currently active keys and their bending

// calculate tuned frequencies (phase advances per clock)
#if 0
void freq_init(int transpose)
{
  int i;
  for(i = 0; i < (1 << C_voice_addr_bits); i++)
  {
    int octave = i / C_tones_per_octave;
    int meantone = i % C_tones_per_octave;
    int j = (i - transpose) % (1 << C_voice_addr_bits);
    *voice = j;
    freq[j] = pow(2.0, C_shift_octave+octave+(C_temperament[meantone]+C_tuning_cents)/C_cents_per_octave)+0.5;
    *pitch = freq[j];
  }
}
#endif

// background recalculate note frequency table
// after changing of the temperament
void freq_table_background(void)
{
  static int16_t i = -1; // running voice num counter if positive we are recalculating 
  if(request_freq_table != 0)
  {
    i = C_voice_num;
    request_freq_table = 0; // clear request flag
  }
  if(i <= 0) // we're done
  {
    return;
  }
  i--;
  int octave = i / C_tones_per_octave;
  int meantone = i % C_tones_per_octave;
  uint16_t j = (i - transpose) & ((1 << C_voice_addr_bits)-1);
  *voice = j | (volume[j] << 8);
  freq[j] = pow(2.0, C_shift_octave+octave+(C_temperament[meantone]+C_tuning_cents)/C_cents_per_octave)+0.5;
  *pitch = freq[j];
}

// calculate 128 start values for various pitchbend ranges
void pitch_bend_init(void)
{
  int i, j;
  for(j = 0; j < 128; j++)
  {
    i = j > 0 ? j : 2; // pitch bend 0 is the same as 2
    pb_inc[i] = pow(2.0, 1.0/((double)(12*C_pbm_range/2))*(double)(i));
    pb_start[i] = pow(2.0, (double)(-(C_pbm_range/2))/((double)(12*C_pbm_range/2))*(double)(i) + (double)(C_pbm_shift));
  }

  // allocate table for pitch bend range change
  pbm = (uint32_t *)malloc(sizeof(uint32_t) * C_pbm_range);
  for(i = 0; i < C_pbm_range; i++)
    pbm[i] = 1 << C_pbm_shift; // neutral intial value (multiply by 1)
}

// background updating of the pitch bend range table
void pitch_bend_background(void)
{
  static int16_t i = 0; // the running counter
  static double increment, pitch_bend;
  // on each invocation, this will process one table entry from 16384
  if(request_pitch_bend_range != 0)
  {
    i = 0; // setting current voice counter will start recalculation
    increment = pb_inc[bend_meantones];
    pitch_bend = pb_start[bend_meantones];
    request_pitch_bend_range = 0; // clear request flag
  }
  if(i >= C_pbm_range) // we're done
    return;
  // pitch bend frequency multiplier
  // default bend range is +-1 halftone (100 cents, 1/12 octave)
  // this calculation takes time so it's backgrounded
  // (but it's still too slow so serial port looses bytes)
  // pbm[i] = pow(2.0, (double)(i-(C_pbm_range/2))/((double)(12*C_pbm_range/2))*(double)(bend_meantones) + (double)(C_pbm_shift));
  // delay(1); // not even 1 ms delay is tolerated here
  pbm[i] = pitch_bend + 0.5;
  pitch_bend *= increment;
  i++; // increment
}

// this will background-refresh each voice volume
// setting, in case of some intermediate miscalculation
// or after each change of drawbar settings
void voices_volume_recalculate_background()
{
  static int16_t i = -1; // running voice num counter if positive we are recalculating 
  if(request_voices_volume_recalculate != 0)
  {
    i = C_voice_num;
    request_voices_volume_recalculate = 0; // clear request flag
  }
  if(i <= 0) // we're done
  {
    return;
  }
  i--; // decrement
  // for voice i, accumulate key volumes thru all relevant drawbars
  int32_t voice_vol = 0;
  int16_t j; // drawbar counter
  for(j = 0; j < C_drawbar_count; j++)
  {
    int key_num = i - drawbar_voice[j]; // some lower pitch key may contribute to this voice
    if(key_num >= 0 && key_num <= 127)
    {
      int16_t key_vol = active_keys[key_num]; // volume contribution of the key
      if(key_vol)
      {
        // determine drawbar value for this key
        uint64_t r = key_num < 60 ? reg_lower : reg_upper;
        // downshift drawbar value to position at bit 0
        r >>= 4*(C_drawbar_count-1-j);
        // delete all upper bits to get individual drawbar value
        uint8_t db_val = r & 0xF;
        if(db_val)
        {
          int16_t db_volume = (1 << db_val)/2; // 2^n function to linear volume
          voice_vol += key_vol * db_volume; // multiply with key value
        }
      }
    }
  }
  volume[i] = voice_vol;
  // apply recalculated volume to synth voice volume register
  *voice = i | (voice_vol << 8);
}

// will mute everything and stop any stuck key
void reset_keys()
{
  int i;
  for(i = 0; i < C_voice_num; i++)
  {
    active_keys[i] = 0;
    active_bend[i] = 0;
    volume[i] = 0;
    *voice = i;
    led_value = 0;
    *led_indicator_pointer = 0;
  }
}

// key press: set of voice volumes according to the registration
// bend: 0 no bend, -8192 down 1 octave, +8192 up 1 octave
// bending range can be changed by control command
void key(uint8_t key, int16_t vol, int16_t bend, uint8_t apply, uint64_t registration)
{
  int i;
  uint64_t r = registration;
  uint8_t db_val;
  int16_t db_volume;
  int8_t v; // voice number
  for(i = C_drawbar_count-1; i >= 0; i--)
  {
    db_val = r & 15;
    r >>= 4;
    v = key+drawbar_voice[i]; // in case of overflow >127, "v" becomes negative
    if(v >= 0 && db_val != 0) // if "v" is not negative means no overflow
    {
      db_volume = (1 << db_val)/2;
      volume[v] += vol * db_volume;
      if(apply != 0)
      {
        *voice = v | (volume[v] << 8);
        if(bend == 0)
        {
          *pitch = freq[v];
        }
        else
        {
          int16_t pitchbend = bend + 8192;
          if(bend < -8192) pitchbend = 0;
          if(bend > 8191) pitchbend = 16383;
          uint64_t fbend = ((uint64_t)(freq[v]) * (uint64_t)(pbm[pitchbend])) >> C_pbm_shift;
          *pitch = fbend;
        }
      }
    }
  }
}

// -----------------------------------------------------------------------------
// callback functions will be automatically called when a NoteOn, NoteOff,
// PitchBend or Control message is received.
// It must be a void-returning function with the correct parameters,
// see documentation here:
// http://arduinomidilib.fortyseveneffects.com/a00022.html


void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  static uint8_t recalc = 0;
  recalc++;
    if(channel != 10) // everything except drum channel
    {
      #if 1
      if(active_keys[pitch] <= 0)
      {
        active_keys[pitch] = 0;
        return; // nothing to do, key is already off
      }
      #endif
      key(pitch, -key_volume, 0, 1, pitch < 60 ? reg_lower : reg_upper);
      active_keys[pitch] -= key_volume;
      active_bend[pitch] = 0;
      #if 1
      if(recalc == 0) // every 256 notes request recalculation of all voices
        request_voices_volume_recalculate = 1;
      #endif
      led_value ^= pitch;
      *led_indicator_pointer = led_value;
    }
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    if(channel != 10) // everything except drum channel
    {
      if(velocity == 0)
      {
        // 0-velocity is the same as note off
        handleNoteOff(channel, pitch, velocity);
        return;
      }
      #if 0
      if(active_keys[pitch] > 0)
        return; // key already pressed
      #endif
      key(pitch, key_volume, 0, 1, pitch < 60 ? reg_lower : reg_upper);
      active_keys[pitch] += key_volume;
      active_bend[pitch] = 0;
      led_value ^= pitch;
      *led_indicator_pointer = led_value;
      last_pitch = pitch; // for pitch bend
    }
}

void handlePitchBend(byte channel, int bend)
{
  key(last_pitch, 0, bend, 1, last_pitch < 60 ? reg_lower : reg_upper);
  active_bend[last_pitch] = bend;
}

void pitch_bend_range_change(byte channel, byte number, byte value)
{
  switch(number) // controller number
  {
    case 100: // Registered Parameter LSB
      active_parameter[0] = value;
      break;
    case 101: // Registered Parameter MSB
      active_parameter[1] = value;
      break;
    case 6: // Data Entry MSB
      if(active_parameter[1] == 0 && active_parameter[0] == 0)
      {
        // change pitch bend range, value in meantones
        #if 0 // we have already 0->2 in tables pb_inc, pb_starts
        if(value == 0)
          bend_meantones = 2; // 0 is the same as 2
        else
        #endif
          bend_meantones = value;
        // attention - lengthy math, after changing bend_meantones,
        // the whole pitch bend table must be recalculated in the backgroud
        request_pitch_bend_range = 1;
      }
      break;
    case 38: // Data Entry LSB
      if(active_parameter[1] == 0 && active_parameter[0] == 0)
      {
        // optional, sets pitch bend range in cents
        // not yet implemented setting pitch bend range in cents
      }
      break;
  }
}

void drawbar_register_change(byte channel, byte number, byte value)
{
  uint8_t upper_drawbar_num, lower_drawbar_num;
  uint64_t db_val = (value+8)/16; // convert value range 0-127 -> 0-8
  uint8_t nshift; // number of bits to shift in drawbar register
  uint64_t regmask;
  #if 1
  if(channel != 1) // only channel 1 should carry drawbar change
    return;
  #endif
  // when registered parameters are disabled (127,127), drawbars will change
  if(active_parameter[1] == 127 && active_parameter[0] == 127)
  switch(number) // controller number
  {
    case 0: // 0-8: lower drawbar, sliders
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      lower_drawbar_num = number;
      nshift = 4*(8-lower_drawbar_num);
      regmask = ~(0xFULL << nshift); // reset bits which will change
      reg_lower = (reg_lower & regmask) | (db_val << nshift);
      request_voices_volume_recalculate = 1;
      break;
    case 16: // 16-23: upper drawbar turnknobs
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
      upper_drawbar_num = number-16;
      nshift = 4*(8-upper_drawbar_num);
      regmask = ~(0xFULL << nshift); // reset bits which will change
      reg_upper = (reg_upper & regmask) | (db_val << nshift);
      request_voices_volume_recalculate = 1;
      break;
  }
}

// temperament preset and reset keys
void emergency_reset_keys_control(byte channel, byte number, byte value)
{
  static uint8_t old_reset = 0;
  static uint8_t old_temperament_ben_johnston = 0;
  static uint8_t old_temperament_quarter_comma = 0;
  static uint8_t old_temperament_bach = 0;
  static uint8_t old_temperament_hammond = 0;
  static uint8_t old_temperament_equal = 0;
  switch(number) // controller number
  {
    case 46: // emergency reset: left key labeled "cycle" resets all notes (in case they got stuck
      if(value == 127 && old_reset == 0)
        reset_keys();
      old_reset = value;
      break;
    case 43: // key "<<" Ben Johnston's temperament 1977
      if(value == 127 && old_temperament_ben_johnston == 0)
      {
        C_temperament = C_temperament_ben_johnston;
        request_freq_table = 1;
      }
      old_temperament_ben_johnston = value;
      break;
    case 44: // key ">>" quarter comma temperament 16th century
      if(value == 127 && old_temperament_quarter_comma == 0)
      {
        C_temperament = C_temperament_quarter_comma;
        request_freq_table = 1;
      }
      old_temperament_quarter_comma = value;
      break;
    case 42: // key "[]" bach temperament 18th century
      if(value == 127 && old_temperament_bach == 0)
      {
        C_temperament = C_temperament_bach;
        request_freq_table = 1;
      }
      old_temperament_bach = value;
      break;
    case 41: // key ">" hammond temperament 20th century
      if(value == 127 && old_temperament_hammond == 0)
      {
        C_temperament = C_temperament_hammond;
        request_freq_table = 1;
      }
      old_temperament_hammond = value;
      break;
    case 45: // key "O" equal temperament contemporary
      if(value == 127 && old_temperament_equal == 0)
      {
        C_temperament = C_temperament_equal;
        request_freq_table = 1;
      }
      old_temperament_equal = value;
      break;
  }
}

void handleControlChange(byte channel, byte number, byte value)
{
  #if 1
  pitch_bend_range_change(channel, number, value);
  drawbar_register_change(channel, number, value);
  emergency_reset_keys_control(channel, number, value);
  #endif
}

// -----------------------------------------------------------------------------

void setup()
{
    pitch_bend_init();
    tuning_constants_init();
    led_indicator_pointer = portOutputRegister(digitalPinToPort(led_indicator_pin));
    reset_keys();

    #if 0
    *voice = 69 | (1000<<8);
    *pitch = 3000000;
    delay(500);
    *pitch = 4000000;
    delay(500);
    *voice = 69;
    #endif

    #if 0
    freq_init(0);
    key(69,7,0,1,db_sine1x);
    *led_indicator_pointer = 255;
    delay(500);
    *led_indicator_pointer = 0;
    key(69,-7,0,1,db_sine1x);
    #endif

    #if 0
    // key click test
    int i;
    for(i = 0; i < 40; i++)
    {
      key(75,6,0,1,db_sine1x);
      delay(100);
      key(75,-6,0,1,-db_sine1x);
      delay(10);
    }
    #endif

    // F32C specific by default is disabled but just in case...
    // Serial.setXoffXon(FALSE);
    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);

    // Handle the Pitch Bend
    MIDI.setHandlePitchBend(handlePitchBend);

    // Handle Control Change (may change pitch bend range)
    MIDI.setHandleControlChange(handleControlChange);

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop()
{
    // Call MIDI.read the fastest you can for real-time performance.
    MIDI.read();
    // There is no need to check if there are messages incoming
    // if they are bound to a Callback function.
    // The attached method will be called automatically
    // when the corresponding message has been received.
    freq_table_background();
    pitch_bend_background();
    voices_volume_recalculate_background();
}

/* TODO
[x] support setting range of pitch bend (bend_meantones)
[x] make pitch bend range change faster - request change process later
[x] register changing with MIDI slider keyboard controls
[x] when drawbars change, noteoff will not work (must recalculate all active notes)
[x] request drawbar change, process later
[ ] smooth register change (instead of 9-level ratcheted)
[ ] reschedule voices
[x] temperament preset keys: equal, hammond, bach etc.
[ ] after changed temperament, the tuning constants need
    to be recalculated because tuning reference note A (440Hz) may change
*/

