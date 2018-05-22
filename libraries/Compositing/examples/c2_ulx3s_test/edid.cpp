//VGA writer to write eeproms in vga displays
//Apr 2011 Kieran Levin kiram9@yahoo.com 
//If you want to modify some data scroll down and modify the bytes in command 'c'
//Then load from display - change custom bytes, generate checksum, write to save values back(r,c,g,w)
//See also http://en.wikipedia.org/wiki/Extended_display_identification_data
/*Help Commands:
  r=read hex
  b=read binary
  l=load binary from serial(gets 128 bytes of binary data from serial then returns)
  w=write
  d=displaybuffer(binary)
  e=print detailed info
  g=generatechecksum
  t=check checksum
  c=change custom bytes (compiled in)
Connection 
Arduino         VGA   DVI
Analog 4(Data)  12    7  
Analog 5(Clk)   15    6
DIO 2           14          
Power5V         9      14
GND             11     15
*/

#include <SoftwareWire.h>
extern SoftwareWire Wire; // defined in RTC

#define BYTE HEX

#define LED2 8
#define LED18 9

int i; 
int inByte = 0;         // incoming serial byte
int count = 0; 
char edidarray[128];

void edid_setup()
{
  Wire.setClock(100000L); // 100000-400000
  Wire.begin();        // join i2c bus (address optional for master)
  // Serial.begin(115200);  // start serial for output
  
  //write to vclk pin on pin 2 
  pinMode(LED2, OUTPUT);  
  pinMode(LED18, OUTPUT);  
  
  //This puts i2c eeproms that are also clocked off the vsync into R/W mode
  digitalWrite(LED18, HIGH);    // set the LED off
  for(i = 0; i < 20; i++){
    digitalWrite(LED2, HIGH);   // set the LED on
    delay(10);                  // wait for a second
    digitalWrite(LED2, LOW);    // set the LED off
    delay(10);
  }
  digitalWrite(LED2, HIGH);   // set the LED on
  delay(10);
  digitalWrite(LED18, LOW);    // set the LED off
  delay(10);
  digitalWrite(LED2, LOW);   // set the LED on
  
  for(i = 0; i < 10; i++){
    digitalWrite(LED2, HIGH);   // set the LED on
    delay(10);                  // wait for a second
    digitalWrite(LED2, LOW);    // set the LED off
    delay(10);
  }
  //disable writing to eeprom 
  digitalWrite(LED2, LOW);    // set the LED off
  Serial.println("EDID Reader Writer C 2011 Kieran Levin kiram9@yahoo.com");
}

int edidreadbytes(char * array)
{
  byte mycount = 0; 
      Wire.beginTransmission(B1010000);
      Wire.write(byte(mycount));
      Wire.endTransmission();
  for(int j = 0; j < 128; j++){
     Wire.requestFrom(B1010000, 1);
     if (Wire.available())    // slave may send less than requested
     { 
       array[j] = Wire.read(); // receive a byte as character
       mycount++;
     }
     delay(10);
  }
 return mycount; 
}
int edidwritebytes(char * array){
  byte mycount = 0;
  digitalWrite(LED2, HIGH);    // set the LED off
  delay(10);
  for(byte j =0; j < 128;j++){
      Wire.beginTransmission(B1010000);
      #if 1
      Wire.write(byte(j));
      Wire.write(byte(array[j]));
      Wire.endTransmission();
      #endif
     delay(10);
  }
 digitalWrite(LED2, LOW);    // set the LED off
 return mycount; 
}
void printDetailedinfo(){
Serial.println("Detailed Information:");
Serial.print("\tManufacture ID \t");
Serial.println(*((uint16_t *)(edidarray+8)));
Serial.print("\tProduct ID code \t");
Serial.println(*((uint16_t *)(edidarray+10)));
Serial.print("\tSerial Number \t");
Serial.println(*((uint32_t *)(edidarray+12)));
Serial.print("\tWeek of Manufacture \t");
Serial.println(edidarray[16]);
Serial.print("\tYear of manufacture \t");
Serial.println(edidarray[17] + 1990);
Serial.print("\tEdid Version\t");
Serial.println(edidarray[18]);
Serial.print("\tEdid Revision \t");
Serial.println(edidarray[19]);
Serial.print("\tEstablished Timing 1 \t");
Serial.println(0x00FF & edidarray[35],BIN);
Serial.print("\tEstablished Timing 2 \t");
Serial.println(0x00FF & edidarray[36],BIN);
  for(int i = 38; i < 53; i +=2){
    Serial.print("Standard Timing ID \t");
    Serial.println(i);
    Serial.print("\tHoriz Res \t");
    Serial.println(248+8*(0x00FF & edidarray[i]));
    Serial.print("\tAspect Ratio \t");
      switch(B11000000 & edidarray[i+1]){
       case B00000000: 
         if (edidarray[19] < 3)
         Serial.println("1:1");
         else
         Serial.println("16:10");
        break;
       case B01000000: 
        Serial.println("4:3");
        break;
       case B10000000: 
        Serial.println("5:4");
        break;
       case B11000000: 
        Serial.println("16:9");
        break;
      }
    Serial.print("\tVertical Freq \t");  
    Serial.println(60+(B00111111 & edidarray[i+1]));
  }
}
byte edidchecksum(char * array){
  byte checksum = 0; 
  for (int i = 0; i < 128; i++){
   checksum += array[i];
  }
  return(checksum);
}
void checkchecksum(){
  byte checksum = 0; 
  for (int i = 0; i < 128; i++){
   checksum += edidarray[i];
  }
  Serial.print("\tChecksum (should = 0) \t");
  Serial.println(checksum,HEX);
   
}
void generatechecksum(){
  Serial.println("Generating Checksum");
  byte checksum = 0; 
  for (int i = 0; i < 127; i++){
   checksum += edidarray[i];
  }
  edidarray[127] = 256 - checksum;
  checkchecksum();
  
}
void edid_loop()
{
  
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    switch (inByte){
    
      case 'r':  
      count = 0; 
        Serial.println("EEPROM:");
        count = edidreadbytes(edidarray);
        for (i = 0; i < count; i++){
          Serial.print(edidarray[i],HEX);
          Serial.print(',');
          delay(1);
        }
        Serial.println();
        Serial.print("Read ");
        Serial.print(count);
        Serial.println(" bytes");
      break; 
      case 'b': 
      count = 0; 
        Serial.println("EEPROM:");
        count = edidreadbytes(edidarray);
        for (i = 0; i < count; i++){
          Serial.print(edidarray[i],BYTE);
          delay(1);
        }
        Serial.println();
        Serial.print("Read ");
        Serial.print(count);
        Serial.println(" bytes");
       break;
       case 'l':
         count = 0; 
         while (count < 128){
             if (Serial.available() > 0) {
                // get incoming byte:
                edidarray[count] = Serial.read();
                count++;
             }
         }
         Serial.println("Done Loading Bytes press W to write to eeprom ");
       break; 
       case 'w':
         edidwritebytes(edidarray);
         Serial.println("Wrote 128 Bytes to eeprom ");
       break; 
       case 'd': 
         Serial.println("Contents of buffer");
        for (i = 0; i < 128; i++){
          Serial.print(edidarray[i]);
          delay(1);
        }
       break; 
       case 'e':
         printDetailedinfo();
       break;
      case 'g':
         generatechecksum();
       break;
       case 't':
         checkchecksum();
       break;
      case 'c':
        /******************************************************/
        //Change custom bytes here  
        edidarray[0] = 0;
        //edidarray[49] = edidarray[49] | B11000000; //change aspect ratio to 16x9
        edidarray[48] = 139;
        edidarray[49] = 0 | B11000000; //change aspect ratio to 16x9 60hz resolution 
        //Add on a few more custom resolutions here 
        edidarray[50] = 129;
        edidarray[51] = 12 | B11000000; //change aspect ratio to 16x9 72hz resolution 
        edidarray[52] = 129; // change resolution to 1368
        edidarray[53] = 0 | B11000000; //change aspect ratio to 16x9 60hz resolution         
       break;
      case 'h':
//      Serial.println("Help:\n\tr=read hex\n\tb=read binary\n\tl=load binary from serial(gets 128 bytes)\n\tw=write\n\td=displaybuffer(binary)\n\te=print detailed info\n\tg=generatechecksum\n\tt=check checksum\n\tc=change custom bytes (compiled in)");
       break;
    
       default: 
       break; 
    }
  }
}

void edid_read(char *a)
{
  for(byte i = 0; i < 128; i++)
    edidarray[i] = i;
  int edid_count = edidreadbytes(edidarray);
  byte edid_checksum = edidchecksum(edidarray);
  sprintf(a, "EDID EEPROM:%d CRC=%02x %s\n",
    edid_count,
    edid_checksum,
    edid_checksum == 0 ? "OK" : "ERROR"
  );
}

