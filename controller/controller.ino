// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver;
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 

String incomingByte;

char* strToChar(String s) {
  unsigned int bufSize = s.length() + 1; //String length + null terminator
  char* ret = new char[bufSize];
  s.toCharArray(ret, bufSize);
  return ret;
}

void sendCustom(String str) {
  char *msg = strToChar(str);
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  incomingByte = "";
  free(msg);
}

void setup()
{
#ifdef RH_HAVE_SERIAL
    Serial.begin(9600);	  // Debugging only
    Serial.println("init Serial : Done");
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
	;
#endif
}

void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    int nbBytes = 0;

    if ((nbBytes = Serial.available()) > 0) {
            // read the incoming byte:
            for (int i = 0; i < nbBytes; i++)
              incomingByte += (char)Serial.read();

            // say what you got:
            Serial.print("Sent : ");
            Serial.println(incomingByte);
            sendCustom(incomingByte);
            
    }

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
	    // Message with a good checksum received, dump it.
	    driver.printBuffer("Got:", buf, buflen);
    }
}
