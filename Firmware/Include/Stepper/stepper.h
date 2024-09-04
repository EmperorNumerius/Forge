/**
 * @file stepper.h
 * @brief Interface to the TMC2209 stepper driver.
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

#ifndef __STEPPER_H
#define __STEPPER_H

#include "../HAL/stm32f4xx_hal.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define DEFAULT_MAX_HOMING_STEPS 5000

    /**
     * @brief Stores an error related to at least one function in the stepper library.
     */
    typedef enum
    {
        STEPPER_ERROR_NONE = 0,
        STEPPER_ERROR_REACHED_MAX_HOMING_STEPS,
        STEPPER_WARNING_UNINITIALIZED,
        STEPPER_REACHED_MAX_POS,
        STEPPER_REACHED_MIN_POS,
        STEPPER_INVALID_HOMING_SPEED
    } StepperError;

    /**
     * @brief Stores configuration data about a TMC2209 stepper driver.
     */
    typedef struct
    {
        GPIO_TypeDef *STEPx;
        uint32_t STEP_Pin;

        GPIO_TypeDef *DIRx;
        uint32_t DIR_Pin;

        GPIO_TypeDef *Enablex;
        uint32_t Enable_Pin;

        GPIO_TypeDef *DIAGx;
        uint32_t DIAG_Pin;

        bool _initialized; // NEVER touch this manually, other then to read it. this is set by initStepper and disableStepper.

        int32_t currentPosition;
        StepperDirection direction;
        bool dir1IsClockwise;

        uint32_t maxHomingSteps; // Stores the maximum number of steps used in homing. See more details at the homeStepper function.
        uint32_t homingSpeed;

        /**
         * @brief Stores the last error caused by one of the stepper functions.
         * @note For functions that don't mention lastError, assume that they reset its value to STEPPER_ERROR_NONE.
         */
        StepperError lastError;

        int32_t minPosition; // Inclusive
        int32_t maxPosition; // Inclusive
    } StepperConfig;

    typedef enum
    {
        STEP_DIR_0 = GPIO_PIN_RESET,
        STEP_DIR_1 = GPIO_PIN_SET
    } StepperDirection;

    StepperConfig createStepperConfig(GPIO_TypeDef *STEPx,
                                      uint32_t STEP_Pin,

                                      GPIO_TypeDef *DIRx,
                                      uint32_t DIR_Pin,

                                      GPIO_TypeDef *Enablex,
                                      uint32_t Enable_Pin,

                                      GPIO_TypeDef *DIAGx,
                                      uint32_t DIAG_Pin,

                                      bool dir1IsClockwise,

                                      uint32_t maxHomingSteps,
                                      uint32_t homingSpeed,

                                      uint32_t minPosition,
                                      uint32_t maxPosition);

    void initStepper(StepperConfig *cfg);

    void setDirectionStepper(StepperConfig *cfg, StepperDirection dir);

    void singleStepStepper(StepperConfig *cfg);

    void stepStepper(StepperConfig *cfg, uint32_t stepsNum);

    void disableStepper(StepperConfig *cfg);

    bool hasError(StepperConfig *cfg);

    void homeStepper(StepperConfig *cfg, StepperDirection dir);

    void resetStepperPosition(StepperConfig *cfg);

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