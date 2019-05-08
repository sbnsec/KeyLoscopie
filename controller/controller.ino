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
//  Serial.print(str.lenght());
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

char getScancode(char c) {

    switch(c) {

    case 'a' :
        c =  0x1C;
        break;
    case 'b' :
        c =  0x32;
        break;
    case 'c' :
        c =  0x21;
        break;
    case 'd' :
        c =  0x23;
        break;
    case 'e' :
        c =  0x24;
        break;
    case 'f' :
        c =  0x2B;
        break;
    case 'g' :
        c =  0x34;
        break;
    case 'h' :
        c =  0x33;
        break;
    case 'i' :
        c =  0x43;
        break;
    case 'j' :
        c =  0x3B;
        break;
    case 'k' :
        c =  0x42;
        break;
    case 'l' :
        c =  0x4B;
        break;
    case 'm' :
        c =  0x3A;
        break;
    case 'n' :
        c =  0x31;
        break;
    case 'o' :
        c =  0x44;
        break;
    case 'p' :
        c =  0x4D;
        break;
    case 'q' :
        c =  0x15;
        break;
    case 'r' :
        c =  0x2D;
        break;
    case 's' :
        c =  0x1B;
        break;
    case 't' :
        c =  0x2C;
        break;
    case 'u' :
        c =  0x3C;
        break;
    case 'v' :
        c =  0x2A;
        break;
    case 'w' :
        c =  0x1D;
        break;
    case 'x' :
        c =  0x22;
        break;
    case 'y' :
        c =  0x35;
        break;
    case 'z' :
        c =  0x1A;
        break;

    }
    return c;
}

void loop()
{

    int nbBytes = 0;
    String message="";
    char m;
    if ((nbBytes = Serial.available()) > 0) {
            // read the incoming byte:
            for (int i = 0; i < (nbBytes); i++){
              m=(char)Serial.read();
              message+=m;
              incomingByte += getScancode(m);
            }
      
            Serial.print(message);
            if(message.equals("win\n")){
              sendCustom("\xE0\x1F\xE0\xF0\x1F");
              Serial.print('enter');
              }else
            sendCustom(incomingByte);
            
    }
    
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      buf[buflen]='\0';
	    // Message with a good checksum received, dump it.
       Serial.print((char*)buf);
    // Serial.println((char*)buf); 
	   // driver.printBuffer("Got:", buf, buflen);
    }
}
