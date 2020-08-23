#include <arduino.h>
#include "serialio.h"

void serialio::begin(uint16_t baud) {	
	Serial.begin(baud);
    delay(100);
    Serial.println("Constructor worked");
}


void serialio::backup_buffer() {
    received_byte=Serial.read();
    if(received_byte!=10) {
        serial_input_buffer[serial_input_buffer_index]=received_byte;
        serial_input_buffer_index++;
    }
    else {
        serial_input_buffer_index--;
        backup_done=1;
    }
}

void serialio::show_buffer() {
    uint8_t i;
    if(backup_done==0) {
        Serial.println("Backup incomplete");
    }
    else {
        Serial.print("[ ");
        for(i=0; i<serial_input_buffer_index; i++) {
            Serial.print(serial_input_buffer[i]);
            Serial.print(" ");
        }
        Serial.println("]");
    }
}


uint8_t serialio::backup_complete() {
    return backup_done;
}


uint32_t serialio::parseInt() {
    if(backup_done) {
        uint8_t i=serial_input_buffer_index-1;
        uint16_t multiplier=1;
        uint32_t tmp=0;
        while(i>=0 && i<=4 && serial_input_buffer[i]>=48 && serial_input_buffer[i]<=57) {
         
            tmp+=(serial_input_buffer[i]-48)*multiplier;
            i--;
            multiplier*=10;
        }
        // reset the flags
        backup_done=0;
        serial_input_buffer_index=0;
        return tmp;
    }
}






