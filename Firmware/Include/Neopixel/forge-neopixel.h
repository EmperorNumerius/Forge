/**
 * @file forge-neopixel.h
 * @brief Neopixel in the Forge
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

#ifndef __FORGE_NEO_H
#define __FORGE_NEO_H

#include "neopixel.h"
#include "../HAL/stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern NeoPixelString neopixels;

    void initNeopixel(void)
    {
        neopixels = createNPS(3, GPIOA, GPIO_PIN_3, NEO_GRB);
    }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FORGE_NEO_H */