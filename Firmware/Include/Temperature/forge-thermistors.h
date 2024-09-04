/**
 * @file forge-thermistors.h
 * @brief Thermistors in the forge
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
*/

#ifndef FORGE_THERMISTORS_H
#define FORGE_THERMISTORS_H

#include "therm.h"
#include "../CMSIS-Core/cmsis_compiler.h"
#include "../HAL/stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern ThermistorConfig T0;
extern ThermistorConfig T1;
extern ThermistorConfig T2;

void initThermistors(void)
{
    T0 = createThermistorConfig(GPIOC, GPIO_PIN_4, ADC1, 14);
    T1 = createThermistorConfig(GPIOC, GPIO_PIN_5, ADC1, 15);
    T2 = createThermistorConfig(GPIOB, GPIO_PIN_0, ADC1, 8);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORGE_THERMISTORS_H */