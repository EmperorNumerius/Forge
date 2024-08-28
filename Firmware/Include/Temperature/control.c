/**
 * @file control.c
 * @brief Automatically turns on and off a heater based on a thermistor
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

#include "../DSP/Include/arm_math.h"
#include "../CMSIS-Core/cmsis_compiler.h"
#include "../HAL/stm32f4xx_hal.h"
#include <stdbool.h>
#include "control.h"

float32_t _derive(PIDControlConfig *cfg, float32_t x0)
{
    const float32_t delta = 1.0e-6f; // or similar
    float32_t x1 = x0 - delta;
    float32_t x2 = x0 + delta;
    float32_t y1 = getError(cfg, x1);
    float32_t y2 = getError(cfg, x2);
    return (y2 - y1) / (x2 - x1);
}

static GPIO_InitTypeDef GPIO_InitStruct;

void initController(PIDControlConfig *cfg)
{
    HAL_Init();

    GPIO_InitStruct.Pin = cfg->HeaterMOSFET_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(cfg->HeaterMOSFETx, &GPIO_InitStruct);

    initThermistor(cfg->thermistorCfg);

    cfg->_initialized = true;
}

float32_t getError(PIDControlConfig *cfg, float32_t t)
{
    if (roundf(t) == t)
    {
        return cfg->errors[(uint32_t)t];
    }

    float32_t closest_low_error = cfg->errors[0];
    float32_t closest_low_index = 0;
    float32_t closest_high_error = cfg->errors[0];
    float32_t closest_high_index = 0;

    for (uint32_t i = 0; i < t + 1; i++)
    {
        if (i > t)
        {
            closest_high_error = cfg->errors[i];
            closest_high_index = i;
        }

        if (i < t)
        {
            closest_low_error = cfg->errors[i];
            closest_low_index = i;
        }
    }

    return closest_low_error + ((closest_high_error - closest_low_error) / (closest_high_index - closest_low_index) * (t - closest_low_index));
}

void singleStepController(PIDControlConfig *cfg)
{
    if (!cfg->_initialized)
        initController(cfg);
    float32_t temp = readTemperature(cfg->thermistorCfg);

    float32_t error = cfg->target_temp - temp;
    cfg->errors[cfg->_t] = error;

    float32_t derivative = _derive(cfg, cfg->_t);

    cfg->_integral += error;

    cfg->_t++;

    float32_t out = (cfg->K_p * error) + (cfg->K_i * cfg->_integral) + (cfg->K_d * derivative);

    if (out > cfg->trigger_thresh)
    {
        HAL_GPIO_WritePin(cfg->HeaterMOSFETx, cfg->HeaterMOSFET_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(cfg->HeaterMOSFETx, cfg->HeaterMOSFET_Pin, GPIO_PIN_RESET);
    }
}