
#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define	SPI_READY_MASK (1 << 8)
#else
#define	SPI_READY_MASK (1 << 16)
#endif

uint8_t spi_start_tx(volatile uint16_t *port)
{
  uint32_t in;

  ((uint8_t *)port)[1] = 0x80;
  do {
    in = *port;
  } while ((in & SPI_READY_MASK) == 0);

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
  return (in & 0xff);
#else
  return (in >> 24);
#endif
}


uint8_t spi_rxtx(volatile uint16_t *port, uint8_t _data)
{
  uint32_t in;

  *(uint8_t *)port = _data;
  do {
    in = *port;
  } while ((in & SPI_READY_MASK) == 0);

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
  return (in & 0xff);
#else
  return (in >> 24);
#endif
}

