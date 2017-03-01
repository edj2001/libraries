#ifndef SerialCRC_h
#define SerialCRC_h

#include <CRC.h>
#include <SoftwareSerial.h>
#include "Arduino.h"

class swSerialCRC : public SoftwareSerial
{
  public:
    swSerialCRC(uint8_t rxpin, uint8_t txpin, bool inverse = false);
    //~SerialCRC();    
    print(char *instring);
    println(char *instring);
	print(double, int=2);
	println(double, int=2);

  private:
  const unsigned long initialCRC = 0L;
  unsigned long crc;
  printlnCRC();
  //SoftwareSerial sser;
  };

#endif
