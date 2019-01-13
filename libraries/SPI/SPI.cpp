#include <SPI.h>
#include <Arduino.h>

SPIClass::SPIClass(uint8_t spi_bus)
    :_spi_num(spi_bus)
    ,_spi(NULL)
    ,_ss(-1)
    ,_freq(1) // default is minimum SPI speed, about 400 kHz
{}

void SPIClass::begin(int8_t ss)
{
    _ss = ss;
    _spi = (volatile uint16_t *)(0xFFFFFB40 + (_spi_num * 0x10));
    ((volatile uint8_t *)_spi)[1] = _freq;
}

void SPIClass::end() {
}

void SPIClass::setClock(uint32_t clock)
{
    _freq = (uint8_t)(clock/(F_CPU/256));
    ((volatile uint8_t *)_spi)[1] = _freq;
    SPI_NOP;
}

void SPIClass::setBitOrder(uint8_t bitOrder) {
}

void SPIClass::setDataMode(uint8_t dataMode) {
}

uint8_t SPIClass::transfer(uint8_t _data)
{
    uint32_t in;
    ((volatile uint8_t *)_spi)[0] = _data;
    do {
        in = *_spi;
        SPI_NOP;
    } while ((in & SPI_READY_MASK) == 0);

    #if (_BYTE_ORDER == _LITTLE_ENDIAN)
        return (in & 0xff);
    #else
        return (in >> 24);
    #endif
}

void SPIClass::set_pin(uint8_t pin)
{
    if (_spi_num == SPI_EXT) {
        digitalWrite(pin, LOW);
    }
}

void SPIClass::unset_pin(uint8_t pin)
{
    if (_spi_num == SPI_EXT) {
        digitalWrite(pin, HIGH);
    }
}

void SPIClass::beginTransaction(SPISettings settings) {
    setClock(settings._clock);
    beginTransaction();
}

void SPIClass::beginTransaction() {
    ((volatile uint8_t *)_spi)[1] = 0x80;
    while ((*_spi & SPI_READY_MASK) == 0);
    ((volatile uint8_t *)_spi)[1] = _freq;
    unset_pin(_ss);
}

void SPIClass::endTransaction() {
    set_pin(_ss);
}
