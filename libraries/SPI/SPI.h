#ifndef _SPI_H
#define _SPI_H

#include <stdlib.h>

// #define SPI_NOP __asm__ __volatile__ ("nop\n\t")
#define SPI_NOP

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define  SPI_READY_MASK (1 << 8)
#else
#define SPI_READY_MASK (1 << 16)
#endif

#define SPI_LSBFIRST 0
#define SPI_MSBFIRST 1

// modes not yet implemented in f32c hardware
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

// instantiating selected SPI bus, usage:
// SPIClass SPI_flash(SPI_FLASH);
// SPI_flash.begin();
// SPI_flash.begin(11); -- 11 is slave select pin, but used only for SPI_EXT
#define SPI_FLASH 0
#define SPI_SD 1
#define SPI_OLED 2
#define SPI_EXT 3
// default SPI bus when instantiating like this:
// SPIClass SPI;
#define SPI_DEFAULT SPI_EXT

class SPISettings
{
public:
    SPISettings() :_clock(5000000), _bitOrder(SPI_MSBFIRST), _dataMode(SPI_MODE0) {}
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) :_clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}
    uint32_t _clock;
    uint8_t  _bitOrder;
    uint8_t  _dataMode;
};

class SPIClass
{
private:
    int8_t _spi_num;
    volatile uint16_t * _spi;
    int8_t _ss;
    uint8_t _freq;
    void set_pin(uint8_t pin);
    void unset_pin(uint8_t pin);

public:
    SPIClass(uint8_t spi_bus=SPI_DEFAULT);
    void begin(int8_t ss=-1);
    void end();

    void setClock(uint32_t clock);
    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t dataMode);

    uint8_t transfer(uint8_t _data);
    void beginTransaction();
    void beginTransaction(SPISettings settings);
    void endTransaction();
};

extern SPIClass SPI;

#endif // _SPI_H
