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

// Define the timer handle and PWM channel handles
TIM_HandleTypeDef htim3;
TIM_OC_InitTypeDef sConfigOC;

// Function to initialize PWM on Timer 3, Channel 1, 2, and 3
void PWM_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t timerChannel) {
    __HAL_RCC_TIM3_CLK_ENABLE();  // Enable Timer 3 clock

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure GPIO for PWM pins
    GPIO_InitStruct.Pin = GPIO_Pin; // PA6 -> TIM3_CH1, PA7 -> TIM3_CH2
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

    // Configure Timer 3 for PWM
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 84 - 1;    // Prescaler value (84 MHz / 84 = 1 MHz timer frequency)
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000 - 1;     // Period value (1000 -> 1 kHz PWM frequency)
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(&htim3);

    // Configure PWM Channels
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;  // Initial duty cycle = 0%
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    // Initialize PWM channels
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, timerChannel);

    // Start PWM on all three channels
    HAL_TIM_PWM_Start(&htim3, timerChannel);
}

void PWM_SetDutyCycle(uint32_t channel, float32_t dutyCycle) {
    // Clamp dutyCycle to 0.0 - 1.0 range
    if (dutyCycle < 0.0f) dutyCycle = 0.0f;
    if (dutyCycle > 1.0f) dutyCycle = 1.0f;

    uint32_t pulse = (uint32_t)(htim3.Init.Period * dutyCycle); // Calculate pulse width

    // Set the pulse width for the corresponding channel
    switch (channel) {
        case TIM_CHANNEL_1:
            sConfigOC.Pulse = pulse;
            HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
            break;
        case TIM_CHANNEL_2:
            sConfigOC.Pulse = pulse;
            HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
            break;
        case TIM_CHANNEL_3:
            sConfigOC.Pulse = pulse;
            HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
            break;
        case TIM_CHANNEL_4:
            sConfigOC.Pulse = pulse;
            HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
            break;
        case TIM_CHANNEL_ALL:
            sConfigOC.Pulse = pulse;
            HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_ALL);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_ALL);
            break;
        default:
            // Invalid channel; do nothing
            break;
    }
}

TIM_HandleTypeDef htim2;

void initController(PIDControlConfig *cfg)
{
    HAL_Init();
    PWM_Init(cfg->HeaterMOSFETx, cfg->HeaterMOSFET_Pin, cfg->timerChannel);

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

    PWM_SetDutyCycle(cfg->timerChannel, out);
}