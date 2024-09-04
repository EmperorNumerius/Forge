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

    void initForgeSteppers(void)
    {
        StepperX1 = createStepperConfig(GPIOA, GPIO_PIN_7, GPIOA, GPIO_PIN_1, GPIOA, GPIO_PIN_6, GPIOA, GPIO_PIN_0, true, 0, 50, 0, 200);

        StepperY1 = createStepperConfig(GPIOB, GPIO_PIN_5, GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_4, GPIOB, GPIO_PIN_1, true, 0, 50, 0, 200);

        StepperZ1 = createStepperConfig(GPIOH, GPIO_PIN_0, GPIOC, GPIO_PIN_3, GPIOC, GPIO_PIN_0, GPIOH, GPIO_PIN_1, true, 0, 0, 1, 200);

        StepperE1 = createStepperConfig(GPIOB, GPIO_PIN_2, GPIOA, GPIO_PIN_5, GPIOB, GPIO_PIN_11, GPIOB, GPIO_PIN_10, true, 0, 0, 0, 0);
    }

    void homeForge()
    {
        homeStepper(&StepperX1, STEP_DIR_0);
        homeStepper(&StepperY1, STEP_DIR_0);
    }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FORGE_STEPPER_H */