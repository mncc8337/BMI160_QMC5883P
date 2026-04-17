#include "BMI160_QMC5883P.h"

BMI160_QMC5883P::BMI160_QMC5883P(BMI160Class& bmi) {
    _bmi = &bmi;
    
    _ctrl1 = (QMC_UNDERSAMPLING_X1 << 6) | (QMC_OVERSAMPLING_X4 << 4) | (QMC_ODR_200HZ << 2) | QMC_MODE_CONTINUOUS;
    
    _ctrl2 = (QMC_RANGE_8G << 2) | QMC_SET_RESET_ON;
}

bool BMI160_QMC5883P::begin(QMC5883PRotation magRot) {
    if(!_bmi->initializeMagnetometer(
        QMC5883P_I2C_ADDR,
        QMC5883P_CHIP_ID_REG,
        QMC5883P_EXPECTED_ID
    )) {
        return false;
    }

    _bmi->setMagRotation((BMI160MagRotation)magRot);

    softReset();
    applySettings();

    _bmi->setMagRate(0x09); 
    _bmi->setMagDelay(0);

    _bmi->magDataMode(QMC5883P_REG_OUT_X_L, 0x00);

    return true;
}

void BMI160_QMC5883P::getMagneticField(int16_t* x, int16_t* y, int16_t* z) {
    _bmi->getMagneticField(x, y, z);
}

void BMI160_QMC5883P::setMode(QMC5883P_Mode mode) {
    _ctrl1 = (_ctrl1 & ~0b00000011) | mode;
}

void BMI160_QMC5883P::setODR(QMC5883P_ODR odr) {
    _ctrl1 = (_ctrl1 & ~0b00001100) | (odr << 2);
}

void BMI160_QMC5883P::setOSR1(QMC5883P_OSR1 osr1) {
    _ctrl1 = (_ctrl1 & ~0b00110000) | (osr1 << 4);
}

void BMI160_QMC5883P::setOSR2(QMC5883P_OSR2 osr2) {
    _ctrl1 = (_ctrl1 & ~0b11000000) | (osr2 << 6);
}

void BMI160_QMC5883P::setRange(QMC5883P_Range range) {
    _ctrl2 = (_ctrl2 & ~0b00001100) | (range << 2);
}

void BMI160_QMC5883P::setSetResetMode(QMC5883P_SetReset sr_mode) {
    _ctrl2 = (_ctrl2 & ~0b00000011) | sr_mode;
}

QMC5883P_Mode BMI160_QMC5883P::getMode() {
    return (QMC5883P_Mode)(_ctrl1 & 0b00000011);
}

QMC5883P_ODR BMI160_QMC5883P::getODR() {
    return (QMC5883P_ODR)((_ctrl1 >> 2) & 0b00000011);
}

QMC5883P_OSR1 BMI160_QMC5883P::getOSR1() {
    return (QMC5883P_OSR1)((_ctrl1 >> 4) & 0b00000011);
}

QMC5883P_OSR2 BMI160_QMC5883P::getOSR2() {
    return (QMC5883P_OSR2)((_ctrl1 >> 6) & 0b00000011);
}

QMC5883P_Range BMI160_QMC5883P::getRange() {
    return (QMC5883P_Range)((_ctrl2 >> 2) & 0b00000011);
}

QMC5883P_SetReset BMI160_QMC5883P::getSetResetMode() {
    return (QMC5883P_SetReset)(_ctrl2 & 0b00000011);
}

void BMI160_QMC5883P::applySettings() {
    _bmi->magSetupMode();
    delay(5);

    _bmi->writeMagRegister(QMC5883P_REG_CTRL2, _ctrl2);
    delay(1);
    _bmi->writeMagRegister(QMC5883P_REG_CTRL1, _ctrl1);
    delay(1);

    _bmi->magDataMode(QMC5883P_REG_OUT_X_L, 0x00);
}

void BMI160_QMC5883P::softReset() {
    _bmi->magSetupMode();
    delay(5);
    _bmi->writeMagRegister(QMC5883P_REG_CTRL2, 0x80); 
    delay(10);
    _bmi->magDataMode(QMC5883P_REG_OUT_X_L, 0x00);
}

void BMI160_QMC5883P::triggerSelfTest() {
    _bmi->magSetupMode();
    delay(5);
    uint8_t test_cmd = _ctrl2 | (1 << 6); 
    _bmi->writeMagRegister(QMC5883P_REG_CTRL2, test_cmd);
    delay(10);
    _bmi->magDataMode(QMC5883P_REG_OUT_X_L, 0x00);
}

uint8_t BMI160_QMC5883P::readRegister(uint8_t reg) {
    _bmi->magSetupMode();
    delay(2);
    
    _bmi->triggerMagRegistersRead(reg, BMI160_MAG_RD_BURST_1B);
    _bmi->waitTillDoneMagTrans();
    uint8_t val = _bmi->getReadMagRegister(0);

    _bmi->magDataMode(QMC5883P_REG_OUT_X_L, 0x00);
    
    return val;
}

bool BMI160_QMC5883P::isDataReady() {
    uint8_t status = readRegister(QMC5883P_REG_STATUS);
    return (status & 0x01);
}

bool BMI160_QMC5883P::hasOverflow() {
    uint8_t status = readRegister(QMC5883P_REG_STATUS);
    return (status & 0x02) >> 1;
}
