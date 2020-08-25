
#include "serialio.h"
uint32_t output;
char str[10];
serialio serial;

void setup() {
    // Serial.begin(9600);
    // delay(1000);
    serial.begin(9600);
    delay(2000);
    serial.send_uint8(131);
}

void loop() {
   

    if(serial.backup_complete()) {
        serial.show_buffer();
        // Serial.println(serial.parseInt());
        
        
        // serial.parseString(str,10); // very careful with this function, do not give a bigger number than actual 
        // Serial.println(str);
        
        // Serial.println(serial.parseFloat());
    }

}


void serialEvent() {
    // serial.copyBuffer(5); // this function copies the buffer till '/r' (not '/n');
}