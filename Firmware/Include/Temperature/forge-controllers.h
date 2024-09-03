/**
 * @file forge-controllers.h
 * @brief Controllers for each of the forge thermistors
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
*/

#ifndef FORGE_CONTROL_H
#define FORGE_CONTROL_H

#include "control.h"
#include "forge-thermistors.h"
#include "tuning.h"
#include "../CMSIS-Core/cmsis_compiler.h"
#include "../HAL/stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern PIDControlConfig HeaterHotend;
extern PIDControlConfig HeaterBed;

void initHeaterControllers(void)
{
    initThermistors();
    HeaterHotend = createController(&T0, GPIOC, GPIO_PIN_11, 22.2f, 1.08f, 114.0f);
    HeaterBed = createController(&T1, GPIOC, GPIO_PIN_12, 10.0f, 0.023f, 305.0f);
}

void tuneHeaters(void)
{
    initHeaterControllers();
    initTuning(&HeaterHotend);
    tune(&HeaterHotend, 200, 30);
    tune(&HeaterBed, 60, 30);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORGE_CONTROL_H */