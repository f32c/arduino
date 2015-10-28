/*  f32c_PS2Keyboard library example
  
  f32c_PS2Keyboard now requries both pins specified for begin()

  bool keyboard_detected = keyboard.begin();
  
  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
  
  Like the Original library and example this is under LGPL license.
  
  Modified by Cuninganreset@gmail.com on 2010-03-22
  Modified by Paul Stoffregen <paul@pjrc.com> June 2010
  Modified by Xark for f32c https://hackaday.io/Xark Oct 2015
*/

#include <f32c_PS2Keyboard.h>

PS2Keyboard keyboard;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  delay(5 * 1000);

  bool detected = false;
  while (!detected)
  {
    Serial.println("f32c PS/2 keyboard port test.");
    delay(2 * 1000);
    detected = keyboard.begin();
    Serial.println(detected ? "Keyboard was detected" : "No keyboard detected");
  }
  Serial.println("Start typing, PS/2 keys will be echoed.");
  Serial.println("");
}

void loop()
{
  if (keyboard.available()) {
    
    // read the next key
    uint8_t c = keyboard.read();	
    
    if (c == PS2_ENTER) {
      Serial.println();
    } else if (c == PS2_TAB) {
      Serial.print("[Tab]");
    } else if (c == PS2_ESC) {
      Serial.print("[ESC]");
    } else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");
    } else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } else if (c == PS2_DELETE) {
      Serial.print("[Del]");
    } else if (c == PS2_INSERT) {
      Serial.print("[Insert]");
    } else if (c == PS2_HOME) {
      Serial.print("[Home]");
    } else if (c == PS2_END) {
      Serial.print("[End]");
    } else if (c == PS2_BACKSPACE) {
      Serial.print("[Backspace]");
    } else if (c == PS2_F1) {
      Serial.print("[F1]");
    } else if (c == PS2_F2) {
      Serial.print("[F2]");
    } else if (c == PS2_F3) {
      Serial.print("[F3]");
    } else if (c == PS2_F4) {
      Serial.print("[F4]");
    } else if (c == PS2_F5) {
      Serial.print("[F5]");
    } else if (c == PS2_F6) {
      Serial.print("[F6]");
    } else if (c == PS2_F7) {
      Serial.print("[F7]");
    } else if (c == PS2_F8) {
      Serial.print("[F8]");
    } else if (c == PS2_F9) {
      Serial.print("[F9]");
    } else if (c == PS2_F10) {
      Serial.print("[F10]");
    } else if (c == PS2_F11) {
      Serial.print("[F11]");
    } else if (c == PS2_F12) {
      Serial.print("[F12]");
    } else if (c) {
      if (c < ' ')
      {
        Serial.print("[0x");
        Serial.print((uint8_t)c, HEX);
        Serial.print("]");
      }
      else
      {
        // otherwise, just print all normal characters
        Serial.print((char)c);
      }
    }
  }
}