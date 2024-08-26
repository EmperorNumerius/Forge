/**
 * @file stepper.c 
 * @brief Implementation of interface for the TMC2209 stepper driver.
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

/** @addtogroup Forge
 * @{
 */

/** @addtogroup Stepper
 * @{
 */

#include "stepper.h"
#include "../HAL/stm32f4xx_hal.h"
#include <stdbool.h>
#include "../CMSIS-Core/cmsis_compiler.h"

static GPIO_InitTypeDef GPIO_InitStruct;

/**
 * @brief  Initializes the StepperConfig provided to the function. This involves ensuring that HAL is initialized, configuring all of the pins, setting cfg->_initialized to true, and then delaying one millisecond to ensure the driver has fully turned on. Please ensure that, before calling this function, the system clock and the peripheral clocks are configured so that the registers can be written to.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @retval None
 * @headerfile stepper.h
 */
void initStepper(StepperConfig *cfg)
{
    HAL_Init();

    GPIO_InitStruct.Pin = cfg->STEP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(cfg->STEPx, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = cfg->DIR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(cfg->DIRx, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = cfg->Enable_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(cfg->Enablex, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = cfg->DIAG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM; // Doesn't really matter, only matters for output
    HAL_GPIO_Init(cfg->DIAGx, &GPIO_InitStruct);

    if (cfg->maxHomingSteps == 0)
    {
        cfg->maxHomingSteps = DEFAULT_MAX_HOMING_STEPS;
    }

    HAL_Delay(131); // for StealthChop2

    cfg->_initialized = true;
    cfg->lastError = STEPPER_ERROR_NONE;
}

void _nopTimes(uint32_t num)
{
    for (uint32_t i = 0; i < num; i++)
    {
        __NOP();
    }
}

/**
 * @brief  Sets the direction of the stepper to the passed direction. If necessary, it initializes the stepper.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @param[in]  dir is a StepperDirection that should be one of STEP_DIR_0 or STEP_DIR_1. These have no direction(clockwise or counterclockwise) associated with them as this could be different for each stepper.
 * @retval None
 * @headerfile stepper.h
 */
void setDirectionStepper(StepperConfig *cfg, StepperDirection dir)
{
    if (!cfg->_initialized)
    {
        initStepper(cfg);
    }
    _nopTimes(4); // on a 168mhz clock, this will be at least(likely much more than) 24ns
    HAL_GPIO_WritePin(cfg->DIRx, cfg->DIR_Pin, dir);
    _nopTimes(4); // see note above
    cfg->direction = dir;
    cfg->lastError = STEPPER_ERROR_NONE;
}

/**
 * @brief  Steps the stepper a single time. If necessary, it initializes the stepper.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @retval None
 * @headerfile stepper.h
 */
void singleStepStepper(StepperConfig *cfg)
{
    if (!cfg->_initialized)
    {
        initStepper(cfg);
    }
    HAL_GPIO_WritePin(cfg->STEPx, cfg->STEP_Pin, GPIO_PIN_SET);
    _nopTimes(17); // on a 168mhz clock, this will be at least(likely much more than) 102ns
    HAL_GPIO_WritePin(cfg->STEPx, cfg->STEP_Pin, GPIO_PIN_RESET);
    _nopTimes(17); // see note above
    switch (cfg->direction)
    {
    case STEP_DIR_0:
        if (cfg->dir1IsClockwise)
        {
            cfg->currentPosition -= 1;
        }
        else
        {
            cfg->currentPosition += 1;
        }
        break;

    case STEP_DIR_1:
        if (cfg->dir1IsClockwise)
        {
            cfg->currentPosition += 1;
        }
        else
        {
            cfg->currentPosition -= 1;
        }
        break;

    default:
        break;
    }
    cfg->lastError = STEPPER_ERROR_NONE;
}

/**
 * @brief  Steps the stepper the specified number of times.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @retval None
 * @headerfile stepper.h
 */
void stepStepper(StepperConfig *cfg, uint32_t stepsNum)
{
    for (uint32_t i = 0; i < stepsNum; i++)
    {
        singleStepStepper(cfg);
    }
}

/**
 * @brief  Disables the stepper by setting the enable pin high. This also deinitalizes the stepper without deinitalizing the pins. If the stepper is uninitialized, then it sets `cfg->lastError` to `STEPPER_WARNING_UNINITIALIZED` and still attempts to disable the stepper(which will likely fail). Otherwise, it sets `cfg->lastError` to `STEPPER_ERROR_NONE`.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @retval None
 * @headerfile stepper.h
 */
void disableStepper(StepperConfig *cfg)
{
    if (!cfg->_initialized)
    {
        cfg->lastError = STEPPER_WARNING_UNINITIALIZED;
    }
    else
    {
        cfg->lastError = STEPPER_ERROR_NONE;
    }
    HAL_GPIO_WritePin(cfg->Enablex, cfg->Enable_Pin, GPIO_PIN_SET);
    cfg->_initialized = false;
}

/**
 * @brief  Returns if an error is being returned from the stepper driver. This is told through the DIAG pin from the cfg. If necessary, this initializes the driver.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @retval None
 * @headerfile stepper.h
 */
bool hasError(StepperConfig *cfg)
{
    if (!cfg->_initialized)
    {
        initStepper(cfg);
    }
    cfg->lastError = STEPPER_ERROR_NONE;
    return HAL_GPIO_ReadPin(cfg->DIAGx, cfg->DIAG_Pin) == GPIO_PIN_SET;
}

/**
 * @brief  Homes the stepper in the direction dir. To do this, it continuely steps in the specified direction until hasError reports an error or it reaches cfg->maxHomingSteps. If it reaches maxHomingSteps, then it sets `cfg->lastError` to `STEPPER_ERROR_REACHED_MAX_HOMING_STEPS`. Otherwise, it resets the stepper's saved position to zero and sets `cfg->lastError` to `STEPPER_ERROR_NONE`.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @param[in]  dir is the direction to home in.
 * @retval None
 * @headerfile stepper.h
 */
void homeStepper(StepperConfig *cfg, StepperDirection dir)
{
    if (!cfg->_initialized)
    {
        initStepper(cfg);
    }
    setDirectionStepper(cfg, dir);
    uint32_t numSteps = 0;
    while (!hasError(cfg))
    {
        if (numSteps > cfg->maxHomingSteps)
        {
            cfg->lastError = STEPPER_ERROR_REACHED_MAX_HOMING_STEPS;
            return;
        }
        singleStepStepper(cfg);
        numSteps++;
    }
    resetStepperPosition(cfg);
    cfg->lastError = STEPPER_ERROR_NONE;
}

/**
 * @brief  Resets the stepper's position to zero. If the stepper is uninitialized, then it sets cfg->lastError to `STEPPER_WARNING_UNINITIALIZED` and still resets the position. Otherwise, it sets `cfg->lastError` to `STEPPER_ERROR_NONE`.
 * @param[in]  cfg is a pointer to a StepperConfig that should have all of the pins set.
 * @retval None
 * @headerfile stepper.h
 */
void resetStepperPosition(StepperConfig *cfg)
{
    if (!cfg->_initialized)
    {
        cfg->lastError = STEPPER_WARNING_UNINITIALIZED;
    }
    else
    {
        cfg->lastError = STEPPER_ERROR_NONE;
    }
    cfg->currentPosition = 0;
}

/**
 * @}
 */

/**
 * @}
 */