/**
 * @file therm.h
 * @brief Reads a thermistor from the forge and reports it's approximate temperature in celsius
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

#ifndef THERM_H
#define THERM_H

#include "../DSP/Include/arm_math.h"
#include "../CMSIS-Core/cmsis_compiler.h"
#include "../HAL/stm32f4xx_hal.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    static const uint16_t _TemperatureKeyTable[61] = { // To regenerate, cd to this directory and `node tableparser.mjs`
        0, 5, 10, 15, 20,
        25, 30, 35, 40, 45,
        50, 55, 60, 65, 70,
        75, 80, 85, 90, 95,
        100, 105, 110, 115, 120,
        125, 130, 135, 140, 145,
        150, 155, 160, 165, 170,
        175, 180, 185, 190, 195,
        200, 205, 210, 215, 220,
        225, 230, 235, 240, 245,
        250, 255, 260, 265, 270,
        275, 280, 285, 290, 295,
        300};

    static const float32_t _TemperatureValueTable[61] = { // To regenerate, cd to this directory and `node tableparser.mjs`
        332400.0000000, 257690.0000000, 201270.0000000, 158340.0000000, 125420.0000000,
        100000.0000000, 80239.0000000, 64776.0000000, 52598.0000000, 42950.0000000,
        35262.0000000, 29100.0000000, 24136.0000000, 20114.0000000, 16841.0000000,
        14164.0000000, 11963.0000000, 10147.0000000, 8640.7000000, 7386.7000000,
        6338.3000000, 5458.4000000, 4717.0000000, 4090.1000000, 3558.1000000,
        3105.0000000, 2717.9000000, 2386.1000000, 2100.8000000, 1854.8000000,
        1641.9000000, 1457.3000000, 1296.7000000, 1156.6000000, 1034.1000000,
        926.6400000, 832.2400000, 749.0700000, 675.6400000, 610.6400000,
        552.9900000, 501.7500000, 456.1100000, 415.3700000, 378.9500000,
        346.3100000, 317.0100000, 290.6700000, 266.9300000, 245.5100000,
        226.1500000, 208.6200000, 192.7300000, 178.2900000, 165.1600000,
        153.1900000, 142.2800000, 132.3100000, 123.1900000, 114.8300000,
        107.1600000};

    typedef enum
    {
        THERM_ERROR_NONE,
        THERM_ERROR_FAILED_ADC_INIT,
        THERM_ERROR_FAILED_ADC_CHAN_INIT,
        THERM_ERROR_FAILED_START_CONV
    } ThermistorError;

    typedef enum
    {
        CERTAINTY_HIGHER, // Got this from the datasheet table; likely to be more accurate
        CERTAINTY_LOWER   // Calculated this using linear interpolation and the two nearest values to the resistance value
    } ThermistorTempCertainty;

    typedef struct
    {
        GPIO_TypeDef *Thermx;
        uint32_t Therm_Pin;
        ADC_TypeDef *Therm_ADC;
        uint32_t Therm_ADC_Channel;
        __IO uint32_t _uhADCxConvertedValue;

        bool _initialized; // NEVER touch this manually, other then to read it. this is set by initThermistor

        ThermistorError lastError;
        ThermistorTempCertainty lastCertainty;
    } ThermistorConfig;

    void initThermistor(ThermistorConfig *cfg);
    uint16_t readTemperature(ThermistorConfig *cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* THERM_H */