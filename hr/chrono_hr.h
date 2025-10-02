/**
  ******************************************************************************
  * @file           : chrono_hr.h
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
  
  @endverbatim
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHRONO_HR_H
#define CHRONO_HR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "chrono_hr_config.h"

/* Exported defines ----------------------------------------------------------*/
/**
 * @brief Define result type & value of chrono high resolution functions.
 * 
 */
typedef uint8_t chrono_hr_res_t;

#define CHRONO_HR_OK                     (0U)
#define CHRONO_HR_ERROR_TICK_TOP_ZERO    (1U)
#define CHRONO_HR_ERROR_TICK_TO_NS_ZERO  (2U)
#define CHRONO_HR_ERROR_TICK_PTR_ERROR   (3U)

/* Exported macro ------------------------------------------------------------*/
/** @defgroup TIME_MACROS Time macros
 *  @{
 */

/**
 * @brief Returns current tick value as the number of ticks, since the start of the tick generator.
 *       
 */
#define tickHr_()                 fChronoHr_GetTick()

/**
 * @brief Returns the number of nanoseconds passed since the initialization of the chrono module.
 * @note This macro is not re-entrant. Use critical section if needed.
 */
#define nanos_()                  fChronoHr_GetContinuousTickNs()

/**
 * @brief Generates a delay for the amount of time (in microseconds) specified as input parameter.
 * 
 */
#define delayNanoseconds_(delay_) fChronoHr_DelayNs(delay_)

/**
 * @brief Measures the elapsed time since the last call to tic_().
 *        This macro initiates a high-resolution timer to measure the elapsed time
 *        for a specific code section. The elapsed time can be retrieved using the
 *        tocNs_() macro.
 * 
 */
#define ticHr_(name_) \
  sChronoHr __ticHr_toc_##name_##__;\
  fChronoHr_Start(&(__ticHr_toc_##name_##__))
  
#define tocNs_(name_) fChronoHr_ElapsedNs(&(__ticHr_toc_##name_##__))

#define fChronoHr_MeasureAverageOverheadOfTicTocNs_() fChronoHr_MeasureAverageOverheadOfTicTocNs(100)

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
 * @note Users of the OBJECTIVE_API need to create a variable of type sChronoHr in their application.
 * 
 */
typedef struct {
  
  bool_t _run;          /*!< Holds the RUN state of the chrono object.
                             When this parameter is false, the chrono is in stop mode, and time measurement functions will return 0.
                             This parameter is private and is set by the chrono API. Users must not change its value. */

  tick_hr_t _startTick; /*!< Holds the start time of the chrono.
                             This parameter is private and is set by the chrono API. Users must not change its value. */

  tick_hr_t _stopTick;  /*!< Holds the stop time of the chrono.
                             This parameter is private and is set by the chrono API. Users must not change its value. */

  tick_hr_t _timeout;   /*!< Holds the timeout length.
                             This parameter is private and is set by the chrono API. Users must not change its value. */
  
  bool_t _isTimeout;    /*!< Holds the timeout state of the chrono object.
                             This parameter is private and is set by the chrono API. Users must not change its value. */

}sChronoHr;

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

#if (CHRONO_HR_TICK_TYPE == TICK_TYPE_VARIABLE)

  chrono_hr_res_t fChronoHr_Init(volatile tick_hr_t *tickValue);

#elif (CHRONO_HR_TICK_TYPE == TICK_TYPE_FUNCTION)
  
  typedef tick_hr_t(*fpTick_t)(void);
  chrono_hr_res_t fChronoHr_Init(tick_hr_t(*fpTickValue)(void));

#else

  #error "CHRONO_HR_TICK_TYPE must be defined"

#endif
/** @} */ //End of INIT_FUNCTION

/** @defgroup FUNCTIONAL_API Functional API in chrono module
 *  @brief These functions don't require an object of type sChronoHr.
 *  @note The functional API in this module is re-entrant.
 *  @{
 */

/**
 * @brief Returns the availability of corresponding tick after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is corresponding tick available.
 */
bool_t fChronoHr_IsTickNsAvailable(void);

/**
 * @brief Get ram usage by module.
 * 
 * @retval ram usage in byte.
 */
uint32_t fChronoHr_GetRamUsage(void);

/**
 * @brief Get current tick.
 * 
 * @retval tick: Current tick (raw)
 */
tick_hr_t fChronoHr_GetTick(void);

/**
 * @brief Returns the amount of time converted to nanoseconds since calling fChronoHr_Init().
 * 
 * @note This functions is not re-entrant. Use critical section if needed.
 * 
 * @retval timeLength: Time length since calling fChronoHr_Init() in nanoseconds.
 */
timeNs_t fChronoHr_GetContinuousTickNs(void);

/**
 * @brief Get Tick top value.
 * 
 * @retval topValue: Tick top value
 */
tick_hr_t fChronoHr_GetTickTopValue(void);

/**
 * @brief Get Tick init value.
 * 
 * @retval topValue: Tick init value
 */
tick_hr_t fChronoHr_GetTickInitValue(void);

/**
 * @brief Returns the coefficient for converting one tick to nanoseconds.
 * 
 * @retval tickToNsCoef: Tick-to-nanoseconds coefficient
 */
float64_t fChronoHr_GetTickToNsCoef(void);

/**
 * @brief Returns the pointer to the tick generator.
 * 
 * @retval tickPointer: Pointer to the tick generator.
 */
#if (CHRONO_HR_TICK_TYPE == TICK_TYPE_VARIABLE)

  volatile tick_hr_t* fChronoHr_GetTickPointer(void);

#elif (CHRONO_HR_TICK_TYPE == TICK_TYPE_FUNCTION)

  fpTick_t fChronoHr_GetTickPointer(void);

#else

  #error "CHRONO_HR_TICK_TYPE must be defined"

#endif

/** @defgroup MAX_MEASURABLE_TIME
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the maximum time interval that can be measured with the tick generator, until the tick generator reaches its top value.
 *  @{
  */
 
timeNs_t fChronoHr_GetMaxMeasurableTimeNs(void);

/** @} */ //End of MAX_MEASURABLE_TIME

/** @defgroup AVG_OVERHEAD_TIC_TOC
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the maximum time interval that can be measured with the tick generator, until the tick generator reaches its top value.
 *  @{
  */
 
timeNs_t fChronoHr_MeasureAverageOverheadOfTicTocNs(uint32_t averageQty);

/** @} */ //End of AVG_OVERHEAD_TIC_TOC

/** @defgroup TIME_SPAN
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the time length between two time ticks.
 *  @{
 */

timeNs_t fChronoHr_TimeSpanNs(tick_hr_t startTick, tick_hr_t endTick);
tick_hr_t fChronoHr_TimeSpanTick(tick_hr_t startTick, tick_hr_t endTick);

/** @} */ //End of TIME_SPAN

/** @defgroup DELAY
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions generate delay.
 *  @{
 */

void fChronoHr_DelayNs(timeNs_t delayNs);

/** @} */ //End of DELAY

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChronoHr. Users must declare one to use this API.
 *  @{
 */

/** @defgroup CHRONO_CMD
 *  @ingroup OBJECTIVE_API
 *  @brief These functions are used to start and stop the chrono object. fChronoHr_Start() puts object in run state for time measurement and
 *         fChronoHr_Stop() stops the chrono object.
 *  @{
 */

void fChronoHr_Start(sChronoHr * const me);
void fChronoHr_Stop(sChronoHr * const me);

/** @} */ //End of DELAY

/** @defgroup ELAPSED
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the elapsed time since the start of the measurement using fChronoHr_Start().
 *  @{
 */

timeNs_t fChronoHr_ElapsedNs(sChronoHr const * const me);

/** @} */ //End of ELAPSED

/** @defgroup LEFT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the remaining time until the chrono object reaches its timeout.
 *  @{
 */
timeNs_t fChronoHr_LeftNs(sChronoHr * const me);

/** @} */ //End of LEFT

/** @defgroup TIMEOUT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions are used for measuring timeout.
 *  @{
 */

void fChronoHr_StartTimeoutNs(sChronoHr * const me, timeNs_t timeout);
bool_t fChronoHr_IsTimeout(sChronoHr * const me);

/** @} */ //End of TIMEOUT

/** @defgroup INTERVAL
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the time interval between two consecutive calls.
 *  @{
 */

timeNs_t fChronoHr_IntervalNs(sChronoHr * const me);

/** @} */ //End of INTERVAL

/** @} */ //End of OBJECTIVE_API

/* Exported variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_HR_H */

/************************ Copyright (c) 2025 FaraabinCo *****END OF FILE****/
