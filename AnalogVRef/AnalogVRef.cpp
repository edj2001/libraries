
#include <AnalogVRef.h>


/*


*/

//
//  www.blinkenlight.net
//
//  Copyright 2013 Udo Klein
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see http://www.gnu.org/licenses/
 
long read_vin_mv() {
    // data sheet 24.9.1 ADC Multiplexer Selection Register
    ADMUX = (1<<REFS0) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1);
   // wait for adc to settle 
    delay(5); 
          
    // trigger conversion     
    ADCSRA |= 1<<ADSC;
     
    // wait for conversion to finish
    while (ADCSRA & (1<<ADSC)) {};
     
    // read result
    uint16_t adc;
    adc = ADCL;
    adc |= ADCH<<8;
     
    // data sheet 24.7 ADC Conversion Result
    // ADC = vref * 1024 / vin
    // notice that vin and vref have exchanged roles because we 
    // measure the reference voltage against vin
    // ==> vin [V] = vref [V] * 1024 / adc
    // ==> vin [mV] = 1.1 * 1000 * 1024 / adc
    return 1126400L / adc;
}
 

 /* alternative from http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/ */
 
 /*
 simply measure your Vcc with a voltmeter and with our readVcc() function. Then, replace the constant 1125300L with a new constant:

internal1.1Ref = 1.1 * Vcc1 (per voltmeter) / Vcc2 (per readVcc() function)
scale_constant = internal1.1Ref * 1023 * 1000

This calibrated value will be good for the AVR chip measured only, and may be subject to temperature variation. Feel free to experiment with your own measurements.
 */
 
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
 /*
 it took me a while to figure-out the problem, but on MEGA 2560, immediately after analogRead(A8), ADCL started returning zero. So every attempt to read from A8-A16 on Arduino MEGA will damage the functionality of readVcc().
I’ve resolved the problem by adding:
ADCSRB = 0;
just before
delay(2);


 */
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
 /*
 the solution for this issue is to add:
ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
ADCSRB &= ~_BV(MUX5); // Without this the function always returns -1 on the ATmega2560


 */
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}