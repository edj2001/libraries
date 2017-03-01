#ifndef CRC_h
#define CRC_h

	#include "Arduino.h"

	unsigned long crc32_update(unsigned long crc, byte data);
	unsigned long crc32_string(char *s);
	unsigned long crc32_string(unsigned long crc, char *sdata);


#endif
