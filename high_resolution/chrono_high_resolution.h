/**
  ******************************************************************************
  * @file           : chrono_high_resolution.h
  * @brief          : Chrono high resolution module header file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 FaraabinCo.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  *
  * https://faraabinco.ir/
  * https://github.com/FaraabinCo
  *
  ******************************************************************************
  @verbatim
  
  For more information on how to use this module, please refer to the implementation
  of the module in chrono_high_resolution.c file.

  @endverbatim
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHRONO_HIGH_RESOLUTION_H
#define CHRONO_HIGH_RESOLUTION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "chrono_high_resolution_config.h"

/* Exported defines ----------------------------------------------------------*/
/**
 * @brief Define result type & value of chrono functions.
 * 
 */
typedef uint8_t chrono_high_resolution_res_t;

#define CHRONO_HIGH_RESOLUTION_OK                     (0U)
#define CHRONO_HIGH_RESOLUTION_ERROR_TICK_TOP_ZERO    (1U)
#define CHRONO_HIGH_RESOLUTION_ERROR_TICK_TO_NS_ZERO  (2U)
#define CHRONO_HIGH_RESOLUTION_ERROR_TICK_PTR_ERROR   (3U)

/* Exported macro ------------------------------------------------------------*/
/** @defgroup TIME_MACROS Time macros
 *  @{
 */

/**
 * @brief Returns current tick value as the number of ticks, since the start of the tick generator.
 *       
 */
#define tickHr_()                 fChronoHighResolution_GetTick()

/**
 * @brief Returns the number of nanoseconds passed since the initialization of the chrono module.
 * @note This macro is not re-entrant. Use critical section if needed.
 */
#define nanos_()                  fChronoHighResolution_GetContinuousTickNs()

/**
 * @brief Generates a delay for the amount of time (in microseconds) specified as input parameter.
 * 
 */
#define delayNanoseconds_(delay_) fChronoHighResolution_DelayNs(delay_)

/**
 * @brief Measures the elapsed time since the last call to tic_().
 *        This macro initiates a high-resolution timer to measure the elapsed time
 *        for a specific code section. The elapsed time can be retrieved using the
 *        tocNs_() macro.
 * 
 * @note To accurately account for timing overhead, use the 
 *       fChronoHighResolution_MeasureTicTocOverheadNs_() function. The actual
 *       elapsed time is calculated by subtracting the measured overhead from the
 *       time recorded by the tic and toc macros.
 */
#define ticHr_(name_) \
  sChronoHighResolution __ticHr_toc_##name_##__;\
  fChronoHighResolution_Start(&(__ticHr_toc_##name_##__))
  
#define tocNs_(name_) fChronoHighResolution_ElapsedNs(&(__ticHr_toc_##name_##__))

#define fChronoHighResolution_MeasureTicTocOverheadNs_(iter_, overheadNs_) \
  do {\
    float64_t sum_ = 0.0;\
    for(uint32_t __i__ = 0; __i__ < iter_; __i__++) {\
      ticHr_(__tic_toc_overhead__);\
      sum_ += tocNs_(__tic_toc_overhead__);\
    }\
    overheadNs_ = ((sum_) / (iter_));\
  }while(0)

/** @} */ //End of TIME_MACROS

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Definition of bool_t.
 * 
 */
#ifndef bool_t__
#define bool_t__

typedef bool bool_t;

#ifndef FALSE
#define FALSE ((bool_t)0)
#endif  //FALSE

#ifndef TRUE
#define TRUE  ((bool_t)1)
#endif  //TRUE

#endif  //bool_t

/**
 * @brief Definition of float32_t.
 * 
 */
#ifndef float32_t__
#define float32_t__
typedef float   float32_t;
#endif  //float32_t__

/**
 * @brief Definition of float32_t.
 * 
 */
#ifndef float64_t__
#define float64_t__
typedef double  float64_t;
#endif  //float64_t__

/**
 * @brief Definition of the ChronoHighResolution object.
 * 
 * @note Users of the OBJECTIVE_API need to create a variable of type sChronoHighResolution in their application.
 * 
 */
typedef struct {
  
  bool_t _run;                        /*!< Holds the RUN state of the chrono object.
                                           When this parameter is false, the chrono is in stop mode, and time measurement functions will return 0.
                                           This parameter is private and is set by the chrono API. Users must not change its value. */

  high_resolution_tick_t _startTick;  /*!< Holds the start time of the chrono.
                                           This parameter is private and is set by the chrono API. Users must not change its value. */

  high_resolution_tick_t _stopTick;   /*!< Holds the stop time of the chrono.
                                           This parameter is private and is set by the chrono API. Users must not change its value. */

  high_resolution_tick_t _timeout;    /*!< Holds the timeout length.
                                           This parameter is private and is set by the chrono API. Users must not change its value. */
  
  bool_t _isTimeout;                  /*!< Holds the timeout state of the chrono object.
                                           This parameter is private and is set by the chrono API. Users must not change its value. */

}sChronoHighResolution;

/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/** @defgroup INIT_FUNCTION Initialization functions
 *  @{
 */

/**
 * @brief Initializes chrono high resolution module in based on specified parameters.
 * 
 * @attention This function MUST be called before using any other function in this library.
 * 
 * @param tickValue If module is configured with TICK_TYPE_VARIABLE, pointer to the memory location that contains the tick value,
 *                  otherwise pointer to the function that returns tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */

#if (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_VARIABLE)

  chrono_high_resolution_res_t fChronoHighResolution_Init(volatile high_resolution_tick_t *tickValue);

#elif (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_FUNCTION)
  
  typedef high_resolution_tick_t(*fpTick_t)(void);
  chrono_high_resolution_res_t fChronoHighResolution_Init(high_resolution_tick_t(*fpTickValue)(void));

#else

  #error "CHRONO_HIGH_RESOLUTION_TICK_TYPE must be defined"

#endif
/** @} */ //End of INIT_FUNCTION

/** @defgroup FUNCTIONAL_API Functional API in chrono module
 *  @brief These functions don't require an object of type sChronoHighResolution.
 *  @note The functional API in this module is re-entrant.
 *  @{
 */

/**
 * @brief Returns the availability of corresponding tick after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is corresponding tick available.
 */
bool_t fChronoHighResolution_IsTickNsAvailable(void);

/**
 * @brief Get ram usage by module.
 * 
 * @retval ram usage in byte.
 */
uint32_t fChronoHighResolution_GetRamUsage(void);

/**
 * @brief Get current tick.
 * 
 * @retval tick: Current tick (raw)
 */
high_resolution_tick_t fChronoHighResolution_GetTick(void);

/**
 * @brief Returns the amount of time converted to nanoseconds since calling fChronoHighResolution_Init().
 * 
 * @note This functions is not re-entrant. Use critical section if needed.
 * 
 * @retval timeLength: Time length since calling fChronoHighResolution_Init() in nanoseconds.
 */
timeNs_t fChronoHighResolution_GetContinuousTickNs(void);

/**
 * @brief Get Tick top value.
 * 
 * @retval topValue: Tick top value
 */
high_resolution_tick_t fChronoHighResolution_GetTickTopValue(void);

/**
 * @brief Get Tick init value.
 * 
 * @retval topValue: Tick init value
 */
high_resolution_tick_t fChronoHighResolution_GetTickInitValue(void);

/**
 * @brief Returns the coefficient for converting one tick to nanoseconds.
 * 
 * @retval tickToNsCoef: Tick-to-nanoseconds coefficient
 */
float64_t fChronoHighResolution_GetTickToNsCoef(void);

/**
 * @brief Returns the pointer to the tick generator.
 * 
 * @retval tickPointer: Pointer to the tick generator.
 */
#if (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_VARIABLE)

  volatile high_resolution_tick_t* fChronoHighResolution_GetTickPointer(void);

#elif (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_FUNCTION)

  fpTick_t fChronoHighResolution_GetTickPointer(void);

#else

  #error "CHRONO_HIGH_RESOLUTION_TICK_TYPE must be defined"

#endif

/**
 * @brief Returns the maximum measurable time interval that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval
 */

/** @defgroup MAX_MEASURABLE_TIME
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the maximum time interval that can be measured with the tick generator, until the tick generator reaches its top value.
 *  @{
 */

timeNs_t fChronoHighResolution_GetMaxMeasurableTimeNs(void);

/** @} */ //End of MAX_MEASURABLE_TIME

/** @defgroup TIME_SPAN
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the time length between two time ticks.
 *  @{
 */

timeNs_t fChronoHighResolution_TimeSpanNs(high_resolution_tick_t startTick, high_resolution_tick_t endTick);
high_resolution_tick_t fChronoHighResolution_TimeSpanTick(high_resolution_tick_t startTick, high_resolution_tick_t endTick);

/** @} */ //End of TIME_SPAN

/** @defgroup DELAY
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions generate delay.
 *  @{
 */

void fChronoHighResolution_DelayNs(timeNs_t delayNs);

/** @} */ //End of DELAY

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChronoHighResolution. Users must declare one to use this API.
 *  @{
 */

/** @defgroup CHRONO_CMD
 *  @ingroup OBJECTIVE_API
 *  @brief These functions are used to start and stop the chrono object. fChronoHighResolution_Start() puts object in run state for time measurement and
 *         fChronoHighResolution_Stop() stops the chrono object.
 *  @{
 */

void fChronoHighResolution_Start(sChronoHighResolution * const me);
void fChronoHighResolution_Stop(sChronoHighResolution * const me);

/** @} */ //End of DELAY

/** @defgroup ELAPSED
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the elapsed time since the start of the measurement using fChronoHighResolution_Start().
 *  @{
 */

timeNs_t fChronoHighResolution_ElapsedNs(sChronoHighResolution const * const me);

/** @} */ //End of ELAPSED

/** @defgroup LEFT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the remaining time until the chrono object reaches its timeout.
 *  @{
 */
timeNs_t fChronoHighResolution_LeftNs(sChronoHighResolution * const me);

/** @} */ //End of LEFT

/** @defgroup TIMEOUT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions are used for measuring timeout.
 *  @{
 */

void fChronoHighResolution_StartTimeoutNs(sChronoHighResolution * const me, timeNs_t timeout);
bool_t fChronoHighResolution_IsTimeout(sChronoHighResolution * const me);

/** @} */ //End of TIMEOUT

/** @defgroup INTERVAL
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the time interval between two consecutive calls.
 *  @{
 */

timeNs_t fChronoHighResolution_IntervalNs(sChronoHighResolution * const me);

/** @} */ //End of INTERVAL

/** @} */ //End of OBJECTIVE_API

/* Exported variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_HIGH_RESOLUTION_H */

/************************ Copyright (c) 2025 FaraabinCo *****END OF FILE****/
