class serialio{
	public: 

    
    void begin(uint16_t baud);
    void backup_buffer();
    void show_buffer();
    uint8_t backup_complete();
    uint32_t parseInt(void);
    void parse_float(void);
    void parseString(char* str);

    // void parse_string(&str);

    private:
    uint8_t serial_input_buffer[10];    // copy of the input serial buffer register
    uint8_t received_byte;
    uint8_t serial_input_buffer_index;
    uint8_t backup_done;

	
};
