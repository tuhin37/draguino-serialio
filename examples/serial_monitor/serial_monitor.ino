
#include "serialio.h"
uint32_t output;
char str[10];
serialio serial;

void setup() {
    // Serial.begin(9600);
    // delay(1000);
    serial.begin(9600);
}

void loop() {
   

    if(serial.backup_complete()) {
        serial.show_buffer();
        // output=serial.parseInt();
        // Serial.println(output);
        serial.parseString(str);
        Serial.println(str);
    }

}


void serialEvent() {
    serial.backup_buffer(); // this function copies the buffer till '/r' (not '/n');
}