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
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_tim.h"

PIDControlConfig createController(
    ThermistorConfig *thermistorCfg,
    GPIO_TypeDef *HeaterMOSFETx,
    uint32_t HeaterMOSFET_Pin,
    float32_t K_p,
    float32_t K_i,
    float32_t K_d)
{
    PIDControlConfig out;
    out.thermistorCfg = thermistorCfg;
    out.HeaterMOSFETx = HeaterMOSFETx;
    out.HeaterMOSFET_Pin = HeaterMOSFET_Pin;
    out.K_p = K_p;
    out.K_i = K_i;
    out.K_d = K_d;
    return out;
}

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
TIM_OC_InitTypeDef sConfig;
#define PERIOD_VALUE (uint32_t)(100000)

bool setPWMPowerLevel(float32_t power)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, htim2.Init.Period * power);
}

TIM_HandleTypeDef htim2;

void initController(PIDControlConfig *cfg)
{
    HAL_Init();

    GPIO_InitStruct.Pin = cfg->HeaterMOSFET_Pin;  // Assuming PA5 is used for PWM output
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(cfg->HeaterMOSFETx, &GPIO_InitStruct);

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 83;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.Pulse = 0;  // Initial duty cycle (0%)
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }

    // GPIO configuration for TIM2_CH1 (e.g., PA5 on STM32F4xx)
    HAL_TIM_MspPostInit(&htim2);

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

    setPWMPowerLevel(out);
}