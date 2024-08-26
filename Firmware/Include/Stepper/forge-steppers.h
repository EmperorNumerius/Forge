/**
 * @file forge-steppers.h
 * @brief Steppers in the Forge
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

#ifndef __FORGE_STEPPER_H
#define __FORGE_STEPPER_H

#include "stepper.h"
#include "../CMSIS-Core/cmsis_compiler.h"
#include "../HAL/stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern StepperConfig StepperX1;
extern StepperConfig StepperY1;
extern StepperConfig StepperZ1;
extern StepperConfig StepperE1;

void initForgeSteppers()
{
    StepperX1 = createStepperConfig(GPIOA, 7U, GPIOA, 1U, GPIOA, 6U, GPIOA, 0U, true, 0);

    StepperY1 = createStepperConfig(GPIOB, 0U, GPIOA, 2U, GPIOA, 4U, GPIOB, 1U, true, 0);

    StepperZ1 = createStepperConfig(GPIOH, 0U, GPIOC, 3U, GPIOC, 0U, GPIOH, 1U, true, 0);

    StepperE1 = createStepperConfig(GPIOB, 2U, GPIOA, 5U, GPIOB, 11U, GPIOB, 10U, true, 0);
}

void homeForgeXYZ()
{
    homeStepper(&StepperX1, STEP_DIR_0);
    homeStepper(&StepperY1, STEP_DIR_0);
    homeStepper(&StepperZ1, STEP_DIR_0);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FORGE_STEPPER_H */