#include <Arduino.h>
#include <Wire.h>
#include "BMI160_QMC5883P.h"

BMI160_QMC5883P compass(BMI160);

void setup() {
    Serial.begin(115200);
    while(!Serial); 
    delay(1000);

    Wire.begin();

    Serial.println("Initializing BMI160...");
    if(!BMI160.begin(BMI160GenClass::I2C_MODE, 0x69)) {
        Serial.println("Failed to initialize BMI160");
        while(1);
    }

    Serial.println("Initializing QMC5883P...");
    if(!compass.begin(BMI160_MAG_ROTATION_0)) {
        Serial.println("Failed to initialize QMC5883P. Check wiring or ID.");
        while(1);
    }

    Serial.println("Configuring advanced settings...");
    
    compass.setMode(QMC_MODE_CONTINUOUS);
    compass.setODR(QMC_ODR_100HZ);
    compass.setOSR1(QMC_OSR1_8);
    compass.setRange(QMC_RNG_2G);
    compass.setSetResetMode(QMC_SET_RESET_ON);
    compass.applySettings();

    Serial.println("Setup Complete. Sensors are running.");
}

void loop() {
    int16_t magX, magY, magZ;

    compass.getMagneticField(&magX, &magY, &magZ);

    Serial.print("Mag [X: "); Serial.print(magX);
    Serial.print(" | Y: "); Serial.print(magY);
    Serial.print(" | Z: "); Serial.print(magZ);
    Serial.println(" ]");

    delay(100);
}
