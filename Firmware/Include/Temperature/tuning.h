/**
 * @file tuning.h
 * @brief Automatic tuning for a PID controller
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
*/

#ifndef TUNING_H
#define TUNING_H

#include "control.h"
#include "../DSP/Include/arm_math.h"
#include "../CMSIS-Core/cmsis_compiler.h"
#include "../HAL/stm32f4xx_hal.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

void initTuning(PIDControlConfig *cfg);
void tune(PIDControlConfig *cfg, float32_t target_temp, uint32_t max_cycles);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TUNING_H */