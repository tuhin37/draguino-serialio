#define LINE_END 255


class serialio{
	public: 

    
    void begin(uint16_t baud);
    void copyBuffer(uint8_t);   // this makes a copy of the hardware input buffer
    void show_buffer();
    uint8_t backup_complete();
    uint32_t parseInt(void);
    float parseFloat(void);
    void parseString(char*, uint8_t);

    // void parse_string(&str);

    private:
    uint8_t serial_input_buffer[32];    // copy of the input serial buffer register
    uint8_t serial_input_buffer_index;
    uint8_t backup_done;
    uint32_t char_array2int(uint8_t, uint8_t);
	
};
