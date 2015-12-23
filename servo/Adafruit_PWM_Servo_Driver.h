class PWM {
    //Registers
    static const uint8_t __MODE1         = 0x00;
    static const uint8_t __MODE2         = 0x01;
    static const uint8_t __SUBADR1       = 0x02;
    static const uint8_t __SUBADR2       = 0x03;
    static const uint8_t __SUBADR3       = 0x04;
    static const uint8_t __PRESCALE      = 0xFE;
    static const uint8_t __LED0_ON_L     = 0x06;
    static const uint8_t __LED0_ON_H     = 0x07;
    static const uint8_t __LED0_OFF_L    = 0x08;
    static const uint8_t __LED0_OFF_H    = 0x09;
    static const uint8_t __ALL_LED_ON_L  = 0xFA;
    static const uint8_t __ALL_LED_ON_H  = 0xFB;
    static const uint8_t __ALL_LED_OFF_L = 0xFC;
    static const uint8_t __ALL_LED_OFF_H = 0xFD;

    //Bits
    static const uint8_t __RESTART       = 0x80;
    static const uint8_t __SLEEP         = 0x10;
    static const uint8_t __ALLCALL       = 0x01;
    static const uint8_t __INVRT         = 0x10;
    static const uint8_t __OUTDRV        = 0x04;

    //Other?
    static const uint8_t SWRST           = 0x06;

    uint8_t address; //I2C address the class is talking to
    uint32_t pwm_fd; //File descriptor for the i2c pwm device
    bool debug;

public:
    PWM(uint8_t addr = 0x40, bool debug = false);
    void setPWMFreq(uint32_t freq);
    void setPrescale(uint32_t scale);
    void setPWM(uint8_t channel, uint32_t on, uint32_t off);
    void setAllPWM(uint32_t on, uint32_t off);
    
};
