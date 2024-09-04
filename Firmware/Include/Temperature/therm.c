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

ThermistorConfig createThermistorConfig(GPIO_TypeDef *Thermx,
                                        uint32_t Therm_Pin,
                                        ADC_TypeDef *Therm_ADC,
                                        uint32_t Therm_ADC_Channel)
{
    ThermistorConfig out;
    out.Thermx = Thermx;
    out.Therm_Pin = Therm_Pin;
    out.Therm_ADC = Therm_ADC;
    out.Therm_ADC_Channel = Therm_ADC_Channel;
    return out;
}

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
    AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
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

    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        cfg->lastError = THERM_ERROR_FAILED_ADC_CHAN_INIT;
        return;
    }

    if (HAL_ADC_Start_DMA(&AdcHandle, &cfg->_uhADCxConvertedValue, 1) != HAL_OK)
    {
        cfg->lastError = THERM_ERROR_FAILED_START_CONV;
        return;
    }

    cfg->_initialized = true;
    cfg->lastError = THERM_ERROR_NONE;
    HAL_Delay(100); // just want to make sure that it's finished converting, this is probably giant overkill
}

float32_t readTemperature(ThermistorConfig *cfg)
{
    initThermistor(cfg);
    cfg->lastError = THERM_ERROR_NONE;
    float32_t voltage_therm = ((float32_t)cfg->_uhADCxConvertedValue / 4095.0f) * 3.3f;
    float32_t resistance = 4700.0f * ((3.3f - voltage_therm) / voltage_therm);

    float32_t closest_high_val = _TemperatureValueTable[0];
    float32_t closest_high_temp = _TemperatureKeyTable[0];

    float32_t closest_low_val = _TemperatureValueTable[60];
    float32_t closest_low_temp = _TemperatureKeyTable[60];

    cfg->lastCertainty = CERTAINTY_HIGHER;

    for (uint32_t i = 0; i < 61; i++)
    {
        if (_TemperatureValueTable[i] > resistance)
        {
            closest_high_val = _TemperatureValueTable[i];
            closest_high_temp = _TemperatureKeyTable[i];
        }

        if (_TemperatureValueTable[i] < resistance)
        {
            closest_low_val = _TemperatureValueTable[i];
            closest_low_temp = _TemperatureKeyTable[i];
        }

        if (_TemperatureValueTable[i] == resistance)
            return _TemperatureKeyTable[i];
    }

    cfg->lastCertainty = CERTAINTY_LOWER;

    return closest_low_temp + ((closest_high_temp - closest_low_temp) / (closest_high_val - closest_low_val) * (resistance - closest_low_val));
}