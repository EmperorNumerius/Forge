/**
 * @file therm.c
 * @brief Reads a thermistor from the forge and reports it's approximate temperature in celsius
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

#include "../CMSIS-Core/cmsis_compiler.h"
#include "../HAL/stm32f4xx_hal.h"
#include <stdbool.h>

#include "therm.h"

static GPIO_InitTypeDef GPIO_InitStruct;
static ADC_HandleTypeDef AdcHandle;

void initThermistor(ThermistorConfig *cfg)
{
    ADC_ChannelConfTypeDef sConfig;
    HAL_Init();

    GPIO_InitStruct.Pin = cfg->Therm_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(cfg->Thermx, &GPIO_InitStruct);

    AdcHandle.Instance = cfg->Therm_ADC;

    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
    AdcHandle.Init.ScanConvMode = DISABLE;
    AdcHandle.Init.ContinuousConvMode = ENABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.NbrOfDiscConversion = 0;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion = 1;
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
    AdcHandle.Init.EOCSelection = DISABLE;
    if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
        cfg->lastError = THERM_ERROR_FAILED_ADC_INIT;
        return;
    }

    sConfig.Channel = cfg->Therm_ADC_Channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    cfg->_initialized = true;
    cfg->lastError = THERM_ERROR_NONE;
}

uint16_t readTemperature(ThermistorConfig *cfg)
{
    if (!cfg->_initialized)
        initThermistor(cfg);
}