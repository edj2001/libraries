#include "SerialCRC.h"

swSerialCRC::swSerialCRC(uint8_t rxpin, uint8_t txpin, bool inverse) : SoftwareSerial(rxpin, txpin, inverse)
{
    crc = initialCRC;
  
  }  


swSerialCRC::print(char *instring)
  {
    Serial.print(instring);
	SoftwareSerial::print(instring);
    crc = crc32_string(crc,instring);
    
    }

swSerialCRC::println(char *instring)
  {
    print(instring);
    printlnCRC();
  }

  swSerialCRC::print(double var, int numdec)
  {
	char outstr[15];
	String(var, numdec).toCharArray(outstr,14);
	print(outstr);
    }

swSerialCRC::println(double var, int numdec)
  {
    print(var, numdec);
    printlnCRC();
  }
  
swSerialCRC::printlnCRC()
{
		Serial.print(":");
		Serial.println(crc,HEX);
		SoftwareSerial::print(":");
		SoftwareSerial::println(crc,HEX);
		crc = initialCRC;
}

