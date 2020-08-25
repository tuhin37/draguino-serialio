#include <arduino.h>
#include "serialio.h"

void serialio::begin(uint16_t baud) {	
	Serial.begin(baud);
    delay(100);
    // Serial.println("Constructor worked");
}


void serialio::copyBuffer(uint8_t mode) {
    if(mode==LINE_END) {
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
    else if(mode<=32 && Serial.available()==mode) {
        for(serial_input_buffer_index=0; serial_input_buffer_index<mode; serial_input_buffer_index++) {
            serial_input_buffer[serial_input_buffer_index]=Serial.read();
        }
        backup_done=1;
        index=0; 
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


void serialio::send_float(float inData) { 
    uint8_t bytes[sizeof(float)];
    *(float*)(bytes) = inData;  // convert float to bytes
    Serial.write(bytes[0]);
    Serial.write(bytes[1]);
    Serial.write(bytes[2]);
    Serial.write(bytes[3]);
    Serial.flush(); // wait for the transmission to complete
}


void serialio::send_uint32(uint32_t inData) {
    Serial.write((inData>>24) & 0xFF); // send most significat byte
    Serial.write((inData>>16) & 0xFF); // send second most significat byte
    Serial.write((inData>>8) & 0xFF); // send third most significat byte
    Serial.write((inData) & 0xFF); // send third least significat byte
    Serial.flush(); // wait for the transmission to complete
}


void serialio::send_uint16(uint16_t inData) {
    Serial.write((inData>>8) & 0xFF);
    Serial.write(inData & 0xFF);
}


void serialio::send_uint8(uint8_t inData) {
    Serial.write(inData);
}



float serialio::get_float() {
    if(serial_input_buffer_index-index>=4) {
        uint8_t bytes[sizeof(float)];
        float outData;
        bytes[0] = serial_input_buffer[index];
        bytes[1] = serial_input_buffer[index+1];
        bytes[2] = serial_input_buffer[index+2];
        bytes[3] = serial_input_buffer[index+3];
        outData = *(float*)(bytes);
        index+=4; // advace index for next bytes extracton
        if(index==serial_input_buffer_index) { // index point just out side of the input buffer copy
            backup_done=0;
            serial_input_buffer_index=0;    
        }
        return outData;
    }
    else {
        backup_done=0;
        serial_input_buffer_index=0;
        return 0;
    }
}


uint32_t serialio::get_uint32() {
    if(serial_input_buffer_index-index>=4) {
        uint32_t outData;
        outData = ((uint32_t)serial_input_buffer[index]) << 24;
        outData |= ((uint32_t)serial_input_buffer[index+1]) << 16;
        outData |= ((uint32_t)serial_input_buffer[index+2]) << 8;
        outData |= serial_input_buffer[index+3];
        index+=4;

        if(index==serial_input_buffer_index) { // no more bytes left 
            backup_done=0;
            serial_input_buffer_index=0;    
        }
        return outData;
    }
    else {
        backup_done=0;
        serial_input_buffer_index=0;
        return 0;
    }
}


uint16_t serialio::get_uint16() {
    if(serial_input_buffer_index-index>=2) { 
        uint16_t outData;
        outData |= (serial_input_buffer[index]) << 8;
        outData |= (serial_input_buffer[index+1]);
        index+=2;
        if(index==serial_input_buffer_index) { // no more bytes left 
            backup_done=0;
            serial_input_buffer_index=0;    
        }
        return outData;
    }
    else {
        backup_done=0;
        serial_input_buffer_index=0;
        return 0;
    }
}


uint8_t serialio::get_uint8() {
    uint8_t outData;
    if(serial_input_buffer_index-index>=1) { 
        outData=serial_input_buffer[index];
        index++;
        if(index==serial_input_buffer_index) { // no more bytes left 
            backup_done=0;
            serial_input_buffer_index=0;    
        }
        return outData;
    }
    else {
        backup_done=0;
        serial_input_buffer_index=0;
        return 0;
    }
}




