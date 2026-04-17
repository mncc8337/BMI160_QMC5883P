#ifndef BMI160_QMC5883P_H
#define BMI160_QMC5883P_H

#include <BMI160Gen.h>

// QMC5883P specific I2C details
#define QMC5883P_I2C_ADDR       0x2C
#define QMC5883P_CHIP_ID_REG    0x00
#define QMC5883P_EXPECTED_ID    0x80

// QMC5883P Registers
#define QMC5883P_REG_OUT_X_L    0x01
// #define QMC5883P_REG_OUT_X_H    0x02
// #define QMC5883P_REG_OUT_Y_L    0x03
// #define QMC5883P_REG_OUT_Y_H    0x04
// #define QMC5883P_REG_OUT_Z_L    0x05
// #define QMC5883P_REG_OUT_Z_H    0x06
#define QMC5883P_REG_STATUS     0x09
#define QMC5883P_REG_CTRL1      0x0A
#define QMC5883P_REG_CTRL2      0x0B

enum QMC5883P_Mode {
    QMC_MODE_SUSPEND    = 0b00,
    QMC_MODE_NORMAL     = 0b01,
    QMC_MODE_SINGLE     = 0b10,
    QMC_MODE_CONTINUOUS = 0b11
};

enum QMC5883P_ODR {
    QMC_ODR_10HZ  = 0b00,
    QMC_ODR_50HZ  = 0b01,
    QMC_ODR_100HZ = 0b10,
    QMC_ODR_200HZ = 0b11
};

enum QMC5883P_OSR1 {
    QMC_OVERSAMPLING_X8 = 0b00,
    QMC_OVERSAMPLING_X4 = 0b01,
    QMC_OVERSAMPLING_X2 = 0b10,
    QMC_OVERSAMPLING_X1 = 0b11
};

enum QMC5883P_OSR2 {
    QMC_UNDERSAMPLING_X1 = 0b00,
    QMC_UNDERSAMPLING_X2 = 0b01,
    QMC_UNDERSAMPLING_X4 = 0b10,
    QMC_UNDERSAMPLING_X8 = 0b11
};

enum QMC5883P_Range {
    QMC_RANGE_30G = 0b00,
    QMC_RANGE_12G = 0b01,
    QMC_RANGE_8G  = 0b10,
    QMC_RANGE_2G  = 0b11
};

enum QMC5883P_SetReset {
    QMC_SET_RESET_ON  = 0b00,
    QMC_SET_ONLY_ON   = 0b01,
    QMC_SET_RESET_OFF = 0b10
};

// aliases
typedef enum {
    QMC5883P_ROTATION_0 = BMI160_MAG_ROTATION_0,
    QMC5883P_ROTATION_90 = BMI160_MAG_ROTATION_90,
    QMC5883P_ROTATION_180 = BMI160_MAG_ROTATION_180,
    QMC5883P_ROTATION_270 = BMI160_MAG_ROTATION_270,
} QMC5883PRotation;

class BMI160_QMC5883P {
public:
    BMI160_QMC5883P(BMI160Class& bmi);

    bool begin(QMC5883PRotation magRot);

    void getMagneticField(int16_t* x, int16_t* y, int16_t* z);
    void setMode(QMC5883P_Mode mode);
    void setODR(QMC5883P_ODR odr);
    void setOSR1(QMC5883P_OSR1 osr1);
    void setOSR2(QMC5883P_OSR2 osr2);
    void setRange(QMC5883P_Range range);
    void setSetResetMode(QMC5883P_SetReset sr_mode);

    void applySettings(); 

    QMC5883P_Mode getMode();
    QMC5883P_ODR getODR();
    QMC5883P_OSR1 getOSR1();
    QMC5883P_OSR2 getOSR2();
    QMC5883P_Range getRange();
    QMC5883P_SetReset getSetResetMode();

    void softReset();
    void triggerSelfTest();

    bool isDataReady();
    bool hasOverflow();

private:
    BMI160Class* _bmi;

    uint8_t _ctrl1;
    uint8_t _ctrl2;

    uint8_t readRegister(uint8_t reg);
};

#endif
