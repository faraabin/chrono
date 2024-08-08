/**
  ******************************************************************************
  * @file           : chrono.h
  * @brief          : Chrono module header file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 FaraabinCo.
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
  of the module in chrono.c file.

  @endverbatim
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHRONO_H
#define CHRONO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "chrono_config.h"

/* Exported defines ----------------------------------------------------------*/
#define CHRONO_OK                     0
#define CHRONO_ERROR_TICK_TOP_ZERO    1
#define CHRONO_ERROR_TICK_TO_NS_ZERO  2
#define CHRONO_ERROR_TICK_PTR_ERROR   3

/* Exported macro ------------------------------------------------------------*/
/** @defgroup TIME_MACROS Time macros
 *  @{
 */

/**
 * @brief Returns current tick value a the number of ticks, since the start of the tick generator.
 *       
 */
#define tick_()		fChrono_GetTick()

/**
 * @brief Returns the number of microseconds passed since the initialization of the chrono module.
 * @note This macro is not re-entrant. if this condition may be happen, user should use critical section
 *       for prevent it.
 */
#define micros_()	fChrono_GetContinuousTickUs()

/**
 * @brief Returns the number of milliseconds passed since the initialization of the chrono module.
 * @note This macro is not re-entrant. if this condition may be happen, user should use critical section
 *       for prevent it.
 */
#define millis_()	fChrono_GetContinuousTickMs()

/**
 * @brief Returns the number of seconds passed since the initialization of the chrono module.
 * @note This macro is not re-entrant. if this condition may be happen, user should use critical section
 *       for prevent it.
 */
#define seconds_()	fChrono_GetContinuousTickS()

/**
 * @brief Generates a delay for the amount of time (in microseconds) specified as input parameter.
 * 
 */
#define delayMicroseconds_(delay_)	fChrono_DelayUs(delay_)

/**
 * @brief Generates a delay for the amount of time (in milliseconds) specified as input parameter.
 * 
 */
#define delayMilliseconds_(delay_)	fChrono_DelayMs(delay_)

/**
 * @brief Generates a delay for the amount of time (in second) specified as input parameter.
 * 
 */
#define delaySeconds_(delay_)	fChrono_DelayS(delay_)

/**
 * @brief Measures elapsed time since a call to tic_().
 * 
 */
#define tic_(name_) \
  sChrono __tic_toc_##name_##__;\
  fChrono_Start(&(__tic_toc_##name_##__))
  
#define tocUs_(name_) fChrono_ElapsedUs(&(__tic_toc_##name_##__))
#define tocMs_(name_) fChrono_ElapsedMs(&(__tic_toc_##name_##__))
#define tocS_(name_)  fChrono_ElapsedS(&(__tic_toc_##name_##__))

/** @} */ //End of TIME_MACROS

/* Exported types ------------------------------------------------------------*/
/**
  * @brief Definition of bool_t for misra compliance.
  * 
  */
#ifndef bool_t
typedef bool bool_t;

#define FALSE ((bool_t)0)
#define TRUE  ((bool_t)1)
#endif

/**
  * @brief Definition of the Chrono object.
  * 
  * @note Users of the OBJECTIVE_API need to create a variable of type sChrono in their application.
  * 
  */
typedef struct {
  
  bool_t _run;            /*!< Holds the RUN state of the chrono object.
                             When this parameter is false, the chrono is in stop mode, and time measurement functions will return 0.
                             This parameter is private and is set by the chrono API. Users must not change its value. */

  tick_t _startTick;    /*!< Holds the start time of the chrono.
                             This parameter is private and is set by the chrono API. Users must not change its value. */

  tick_t _timeout;      /*!< Holds the timeout length.
                             This parameter is private and is set by the chrono API. Users must not change its value. */
  
  bool_t _isTimeout;      /*!< Holds the timeout state of the chrono object.
                             This parameter is private and is set by the chrono API. Users must not change its value. */
}sChrono;

/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/** @defgroup INIT_FUNCTION Initialization functions
 *  @{
 */

/**
 * @brief Initializes chrono module in based on specified parameters.
 * 
 * @attention This function MUST be called before using any other function in this library.
 * 
 * @param tickTopValue The maximum number of counts that the tick generator can count (starting from zero).
 * @param tickToNsCoef Coefficient for converting one tick to nanoseconds.
 * @param tickValue If module is configured with TICK_TYPE_VARIABLE, pointer to the memory location that contains the tick value,
 *                  otherwise pointer to the function that returns tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */

#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

  uint8_t fChrono_Init(tick_t tickTopValue, uint32_t tickToNsCoef, volatile tick_t *tickValue);

#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)
  
  typedef tick_t(*fpTick_t)(void);
  uint8_t fChrono_Init(tick_t tickTopValue, uint32_t tickToNsCoef, tick_t(*fpTickValue)(void));

#else

  #error "CHRONO_TICK_TYPE must be defined"

#endif
/** @} */ //End of INIT_FUNCTION

/** @defgroup FUNCTIONAL_API Functional API in chrono module
 *  @brief These functions don't require an object of type sChrono.
 *  @note The functional API in this module is re-entrant.
 *  @{
 */

/**
 * @brief Returns the availability of corresponding tick after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is corresponding tick available.
 */
bool_t fChrono_IsTickUsAvailable(void);
bool_t fChrono_IsTickMsAvailable(void);
bool_t fChrono_IsTickSAvailable(void);

/**
 * @brief Get current tick.
 * 
 * @retval tick: Current tick (raw)
 */
tick_t fChrono_GetTick(void);

/**
 * @brief Returns the amount of time converted to microseconds, milliseconds & seconds since calling fChrono_Init().
 * @note This functions are not re-entrant. if this condition may be happen, user should use critical section
 *       for prevent it.
 * @retval timeLength: Time length since calling fChrono_Init() in microseconds, milliseconds & seconds
 */
uint64_t fChrono_GetContinuousTickUs(void);
uint64_t fChrono_GetContinuousTickMs(void);
uint64_t fChrono_GetContinuousTickS(void);

/**
 * @brief Get Tick top value.
 * 
 * @retval topValue: Tick top value
 */
tick_t fChrono_GetTickTopValue(void);

/**
 * @brief Returns the coefficient for converting one tick to nanoseconds.
 * 
 * @retval tickToNsCoef: Tick-to-nanoseconds coefficient
 */
uint32_t fChrono_GetTickToNsCoef(void);

/**
 * @brief Returns the pointer to the tick generator.
 * 
 * @retval tickPointer: Pointer to the tick generator.
 */
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

  volatile tick_t* fChrono_GetTickPointer(void);

#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)

  fpTick_t fChrono_GetTickPointer(void);

#else

  #error "CHRONO_TICK_TYPE must be defined"

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

timeS_t fChrono_GetMaxMeasurableTimeS(void);
timeMs_t fChrono_GetMaxMeasurableTimeMs(void);
timeUs_t fChrono_GetMaxMeasurableTimeUs(void);

/** @} */ //End of MAX_MEASURABLE_TIME

/** @defgroup TIME_SPAN
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the time length between two time ticks.
 *  @{
 */

timeS_t fChrono_TimeSpanS(tick_t startTick, tick_t endTick);
timeMs_t fChrono_TimeSpanMs(tick_t startTick, tick_t endTick);
timeUs_t fChrono_TimeSpanUs(tick_t startTick, tick_t endTick);
tick_t fChrono_TimeSpanTick(tick_t startTick, tick_t endTick);

/** @} */ //End of TIME_SPAN

/** @defgroup DELAY
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions generate delay.
 *  @{
 */

void fChrono_DelayS(timeS_t delayS);
void fChrono_DelayMs(timeMs_t delayMs);
void fChrono_DelayUs(timeUs_t delayUs);

/** @} */ //End of DELAY

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChrono. Users must declare one to use this API.
 *  @{
 */

/** @defgroup CHRONO_CMD
 *  @ingroup OBJECTIVE_API
 *  @brief These functions are used to start and stop the chrono object. fChrono_Start() puts object in run state for time measurement and
 *         fChrono_Stop() stops the chrono object.
 *  @{
 */

void fChrono_Start(sChrono * const me);
void fChrono_Stop(sChrono * const me);

/** @} */ //End of DELAY

/** @defgroup ELAPSED
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the elapsed time since the start of the measurement using fChrono_Start().
 *  @{
 */

timeS_t fChrono_ElapsedS(sChrono const * const me);
timeMs_t fChrono_ElapsedMs(sChrono const * const me);
timeUs_t fChrono_ElapsedUs(sChrono const * const me);

/** @} */ //End of ELAPSED

/** @defgroup LEFT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the remaining time until the chrono object reaches its timeout.
 *  @{
 */
timeS_t fChrono_LeftS(sChrono * const me);
timeMs_t fChrono_LeftMs(sChrono * const me);
timeUs_t fChrono_LeftUs(sChrono * const me);

/** @} */ //End of LEFT

/** @defgroup TIMEOUT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions are used for measuring timeout.
 *  @{
 */

void fChrono_StartTimeoutS(sChrono * const me, timeS_t timeout);
void fChrono_StartTimeoutMs(sChrono * const me, timeMs_t timeout);
void fChrono_StartTimeoutUs(sChrono * const me, timeUs_t timeout);
bool_t fChrono_IsTimeout(sChrono * const me);

/** @} */ //End of TIMEOUT

/** @defgroup INTERVAL
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the time interval between two consecutive calls.
 *  @{
 */

timeS_t fChrono_IntervalS(sChrono * const me);
timeMs_t fChrono_IntervalMs(sChrono * const me);
timeUs_t fChrono_IntervalUs(sChrono * const me);

/** @} */ //End of INTERVAL

/** @} */ //End of OBJECTIVE_API

/* Exported variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_H */

/************************ Copyright (c) 2024 FaraabinCo *****END OF FILE****/
