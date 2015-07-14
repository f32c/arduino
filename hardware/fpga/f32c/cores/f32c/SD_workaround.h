#ifndef _SD_workaround_h_
#define _SD_workaround_h_

#warning "Dirty SD_workaround.h included"
// Dirty workaround for getting
// Arduino SD card library to compile

// fake ARM architecture and define dummy SPI pins

#define __arm__
#define SS 0
#define MOSI 0
#define MISO 0
#define SCK 0

#endif
