// Written ad-hoc for Forge by Arthur Beck/@ave

/** @addtogroup Forge
  * @{
  */

/** @addtogroup Stepper
  * @{
  */

#ifndef __STEPPER_H
#define __STEPPER_H

#include "../HAL/stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GPIO_TypeDef  *STEPx;
    GPIO_InitTypeDef *STEP_Pin;

    GPIO_TypeDef  *DIRx;
    GPIO_InitTypeDef *DIR_Pin;

    GPIO_TypeDef  *Enablex;
    GPIO_InitTypeDef *Enable_Pin;

    GPIO_TypeDef  *DIAGx;
    GPIO_InitTypeDef *DIAG_Pin;
} StepperConfig;

void initStepper(StepperConfig *cfg);

void singleStepStepper(StepperConfig *cfg);

void stepStepper(StepperConfig *cfg, uint32_t stepsNum);

void disableStepper(StepperConfig *cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STEPPER_H */
/**
  * @}
  */

/**
  * @}
  */