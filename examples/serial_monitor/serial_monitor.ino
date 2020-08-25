
#include "serialio.h"
uint32_t qword;
float fl;
uint16_t dword;
uint8_t one_byte;

char str[10];
serialio serial;





void setup() {
    // Serial.begin(9600);
    // delay(1000);
    serial.begin(9600);
    delay(2000);
    serial.send_uint32(67123);
    serial.send_uint8(37);
    serial.send_float(9192.63);
    serial.send_uint16(1023);
    
}

void loop() {
   

    if(serial.backup_complete()) {
        serial.show_buffer();
        
        
        // Serial.println(serial.parseInt());
        
        
        // serial.parseString(str,10); // very careful with this function, do not give a bigger number than actual 
        // Serial.println(str);
        
        // Serial.println(serial.parseFloat());

        qword=serial.get_uint32();
        one_byte=serial.get_uint8();
        fl=serial.get_float();
        dword=serial.get_uint16();

        Serial.print("Float: ");
        Serial.println(fl,3);

        Serial.print("u32: ");
        Serial.println(qword);

        Serial.print("u16: ");
        Serial.println(dword);

        Serial.print("u8: ");
        Serial.println(one_byte);
    }

}


void serialEvent() {
    serial.copyBuffer(11); // this function copies the buffer till '/r' (not '/n');
}