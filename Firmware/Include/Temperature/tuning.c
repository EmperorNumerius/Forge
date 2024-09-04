/**
 * @file tuning.c
 * @brief Automatic tuning for a PID controller
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

#include "tuning.h"

TIM_HandleTypeDef htim2; // Handle for TIM2

// Function to initialize TIM2 as a monotonic clock
void MonotonicClock_Init(void)
{
    // Enable the clock for TIM2 peripheral
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Configure TIM2
    htim2.Instance = TIM2;                               // Use TIM2
    htim2.Init.Prescaler = (SystemCoreClock / 1000) - 1; // Prescaler to get 1 KHz timer clock
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF; // Maximum 32-bit period
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    // Initialize TIM2
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        // Initialization error handling
        return;
    }

    // Start the timer in free-running mode
    if (HAL_TIM_Base_Start(&htim2) != HAL_OK)
    {
        // Start error handling
        return;
    }
}

// Function to get the current monotonic time in microseconds
uint32_t MonotonicClock_GetTime(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}

void initTuning(PIDControlConfig *cfg)
{
    initController(cfg);
}

void readTemps(PIDControlConfig *cfg, float32_t *out, uint32_t duration_secs)
{
    for (uint32_t i = 0; i < duration_secs; i++)
    {
        out[i] = readTemperature(cfg->thermistorCfg);

        for (uint32_t f = 0; f < 1000; f++)
        {
            singleStepController(cfg);
            HAL_Delay(1);
        }
    }
}

typedef struct
{
    float32_t period;
    float32_t amplitude;
} oscillation;

oscillation empty_oscillation = {0, 0};

oscillation getOscillations(float32_t *temps, uint32_t tempLength, float32_t target_temp, float32_t threshold)
{
    float32_t peaks[tempLength];
    uint32_t pi = 0;
    float32_t troughs[tempLength];
    uint32_t ti = 0;

    for (uint32_t i = 1; i < tempLength - 1; i++)
    {
        float32_t temp = temps[i];
        float32_t prev_temp = temps[i - 1];
        float32_t next_temp = temps[i + 1];

        if (temp > prev_temp && temp > next_temp && temp > target_temp + threshold)
        {
            peaks[pi] = temp;
            pi++;
        }
        else if (temp < prev_temp && temp < next_temp && temp < target_temp - threshold)
        {
            troughs[ti] = temp;
            ti++;
        }
    }

    if (pi >= 2 && ti >= 2)
    {
        float32_t total_period = 0.0, total_amplitude = 0.0;
        for (int i = 0; i < pi - 1; i++)
        {
            total_period += peaks[i + 1] - peaks[i];
        }
        for (int i = 0; i < ti && i < pi; i++)
        {
            float32_t out;
            float32_t in = temps[i] - temps[i + 1];
            arm_abs_f32(&in, &out, 100);
            total_amplitude += out / 2;
        }

        float32_t period = total_period / (pi - 1);
        float32_t amplitude = total_amplitude / (ti < pi ? ti : pi);
        oscillation outo = {period, amplitude};
        return outo;
    }
    else
    {
        return empty_oscillation;
    }
}

void adjust_PID_values(double *kp, double *ki, double *kd, double period, double amplitude)
{
    double kp_factor = 0.6;
    double ki_factor = 0.5;
    double kd_factor = 0.125;
    double acceptable_amplitude = 1.0; // Define an acceptable amplitude value

    if (amplitude > acceptable_amplitude)
    {
        *kp *= kp_factor; // Reduce P if oscillations are too large
    }
    else
    {
        *kp /= kp_factor; // Increase P if amplitude is acceptable
    }

    *ki = (2.0 * (*kp)) / period;
    *kd = (*kp * period) / 8.0;
}

bool is_stable(float32_t *temps, int size, float32_t target_temp, float32_t tolerance, float32_t stability_duration)
{
    int stable_count = 0;

    for (int i = 0; i < size; i++)
    {
        float32_t out;
        float32_t in = temps[i] - target_temp;
        arm_abs_f32(&in, &out, 100);
        if (out <= tolerance)
        {
            stable_count++;
        }
        else
        {
            stable_count = 0; // Reset count if temperature goes out of tolerance range
        }

        if (stable_count >= stability_duration)
        {
            return true; // Temperature is stable for the required duration
        }
    }

    return false;
}

void tune(PIDControlConfig *cfg, float32_t target_temp, uint32_t max_cycles, uint32_t duration_measure_temps_secs)
{
    for (uint32_t i = 0; i < max_cycles; i++)
    {
        PWM_SetDutyCycle(cfg->timerChannel, 1);
        while (readTemperature(cfg->thermistorCfg) < target_temp-0.05)
            ;
        
        float32_t temps[duration_measure_temps_secs];
        readTemps(cfg, temps, duration_measure_temps_secs);

        oscillation oscillations = getOscillations(temps, duration_measure_temps_secs, target_temp, 0.5f);

        adjust_PID_values(&cfg->K_p, &cfg->K_i, &cfg->K_d, oscillations.period, oscillations.amplitude);

        if (is_stable(temps, duration_measure_temps_secs, target_temp, 0.5, 100))
        {
            break;
        }
    }
}
