
#include "serialio.h"
uint32_t output;

serialio serial;

void setup() {
    // Serial.begin(9600);
    // delay(1000);
    serial.begin(9600);
}

void loop() {
   

    if(serial.backup_complete()) {
        serial.show_buffer();
        output=serial.parseInt();
        Serial.println(output);
    }

}


void serialEvent() {
    serial.backup_buffer();
}