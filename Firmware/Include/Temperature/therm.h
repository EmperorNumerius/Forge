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
        332.4000000, 257.6900000, 201.2700000, 158.3400000, 125.4200000,
        100.0000000, 80.2390000, 64.7760000, 52.5980000, 42.9500000,
        35.2620000, 29.1000000, 24.1360000, 20.1140000, 16.8410000,
        14.1640000, 11.9630000, 10.1470000, 8.6407000, 7.3867000,
        6.3383000, 5.4584000, 4.7170000, 4.0901000, 3.5581000,
        3.1050000, 2.7179000, 2.3861000, 2.1008000, 1.8548000,
        1.6419000, 1.4573000, 1.2967000, 1.1566000, 1.0341000,
        0.9266400, 0.8322400, 0.7490700, 0.6756400, 0.6106400,
        0.5529900, 0.5017500, 0.4561100, 0.4153700, 0.3789500,
        0.3463100, 0.3170100, 0.2906700, 0.2669300, 0.2455100,
        0.2261500, 0.2086200, 0.1927300, 0.1782900, 0.1651600,
        0.1531900, 0.1422800, 0.1323100, 0.1231900, 0.1148300,
        0.1071600};
    
    typedef enum
    {
        THERM_ERROR_NONE,
        THERM_ERROR_FAILED_ADC_INIT
    } ThermistorError;

    typedef struct
    {
        GPIO_TypeDef *Thermx;
        uint32_t Therm_Pin;
        ADC_TypeDef Therm_ADC;
        uint32_t Therm_ADC_Channel;

        bool _initialized; // NEVER touch this manually, other then to read it. this is set by initThermistor

        ThermistorError lastError;
    } ThermistorConfig;

    void initThermistor(ThermistorConfig *cfg);
    uint16_t readTemperature(ThermistorConfig *cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* THERM_H */