#include <arduino.h>
#include "serialio.h"

void serialio::begin(uint16_t baud) {	
	Serial.begin(baud);
    delay(100);
    Serial.println("Constructor worked");
}


void serialio::copyBuffer() {
    uint8_t received_byte=Serial.read();
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

// internal function
uint32_t serialio::char_array2int(uint8_t msd_index, uint8_t lsd_index) {
    uint32_t output=0;
    uint32_t multiplier=1;
    uint8_t i=lsd_index;
    while(i>=msd_index && i!=255 && serial_input_buffer[i]>=48 && serial_input_buffer[i]<58) {
        output+=(serial_input_buffer[i]-48)*multiplier;
        multiplier*=10;
        i--;
    }
    if(i==255 || i==msd_index-1) {
        return output;
    }
    else {
        return 0;
    }
}



uint32_t serialio::parseInt() {
    uint32_t output;
    if(backup_done) {
        output=serialio::char_array2int(0, serial_input_buffer_index-1);
        backup_done=0;
        serial_input_buffer_index=0;
        return (output);
    }
}



void serialio::parseString(char* str, uint8_t string_length) {
    char ch;
    uint8_t i=0;
    // clear the string array
    for(i=0; i<string_length; i++){
        *(str+i)=0;
    }
    for(i=0; i<serial_input_buffer_index; i++) {
        ch=(char)serial_input_buffer[i];
        strncat(str, &ch, 1); 
    }
    
}


float serialio::parseFloat() {
    uint8_t i,j;
    float output;
    // find decimal's index point
    for(i=0; i<serial_input_buffer_index; i++) {
        if(serial_input_buffer[i]==46) {
            break;
        }
    }
    // at this point i points to  the decimal point location
    // if i=len then decimal point was not found
    if(i>=serial_input_buffer_index-1) {
        backup_done=0;
        serial_input_buffer_index=0;
        output=serialio::char_array2int(0, i-1);
    }

    else {// decimal found at 
        output=(serialio::char_array2int(i+1, serial_input_buffer_index-1)); // get the post decimal part
        for(j=0; j<(serial_input_buffer_index-i-1); j++){
            output/=10;
        }
        output+=serialio::char_array2int(0, i-1); // get the pre decimal part
    }

    backup_done=0;
    serial_input_buffer_index=0;
    return output;
}






