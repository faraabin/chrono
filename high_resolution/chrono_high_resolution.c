/**
  ******************************************************************************
  * @file           : chrono_high_resolution.c
  * @brief          : High-Resolution Chrono Module for Time Measurement.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 FaraabinCo.
  * All rights reserved.
  *
  * This software is licensed under the terms outlined in the LICENSE file
  * located in the root directory of this software component.
  *
  * For more information, visit:
  * https://faraabinco.ir/
  * https://github.com/FaraabinCo
  *
  ******************************************************************************
  @mainpage
  
  This module serves as a high-resolution variant of the standard chrono module,
  specifically designed for measuring time in nanoseconds. While the original chrono
  module adheres to MISRA compliance, it does not support fractional ticks for nanosecond
  measurements. This high-resolution version bypasses that limitation, allowing users
  to measure time with fractional ticks when converted to nanoseconds.

  The primary objective of this module is to facilitate precise time measurements
  on a nanosecond scale. Users can leverage high-frequency timers, such as the CPU cycle
  counter available in ARM microcontrollers, to serve as the tick generator for this module.

  @mainpage
  */

/* Includes ------------------------------------------------------------------*/
#include "chrono_high_resolution.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/**
 * @brief Returns the time length between "a" and "b".
 * 
 * @note Chronologically, "a" must be a time tick after "b", but the value of "a" could be less than "b" i.e., when tick overflows.
 * 
 */
#if(CHRONO_HIGH_RESOLUTION_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
#define ELAPSED_(a, b)  (((a) >= (b)) ? ((a) - (b)) : ((_chrono.TickTopValue) - ((b) - (a))))
#elif(CHRONO_HIGH_RESOLUTION_TICK_COUNTERMODE == TICK_COUNTERMODE_DOWN)
#define ELAPSED_(a, b)  (((a) <= (b)) ? ((b) - (a)) : ((_chrono.TickTopValue) - ((a) - (b))))
#else
#error "CHRONO_HIGH_RESOLUTION_TICK_COUNTERMODE should be either TICK_COUNTERMODE_UP or TICK_COUNTERMODE_DOWN in chrono_high_resolution_config.h file."
#endif

/**
 * @brief Define GET_TICK_() macro for internal use.
 * 
 */
#if (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_VARIABLE)
  #define GET_TICK_() (*_chrono.pTickValue)
#elif (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_FUNCTION)
  #define GET_TICK_() (_chrono.GetTickValue())
#else
  #define GET_TICK_() (high_resolution_tick_t)0;
#endif

/**
 * @brief Define START_() macro for internal use.
 * 
 */
#define START_(chrono_) \
  (chrono_)->_startTick = GET_TICK_();\
  (chrono_)->_run = TRUE

/**
 * @brief Define DELAY_TICK_() macro for internal use.
 * 
 */
#define DELAY_TICK_(startTick_, delayTick_) \
  high_resolution_tick_t currentTick = GET_TICK_(); \
  while(ELAPSED_(currentTick, (startTick_)) < (delayTick_)) { \
    currentTick = GET_TICK_(); \
  }

  /**
   * @brief Define TIMESPAN_() macro for internal use.
   * 
   */
#define TIMESPAN_NS_(startTick_, endTick_) ((timeNs_t)(ELAPSED_((endTick_), (startTick_))) / _chrono.NsToTickCoef)

/**
 * @brief Define assert macros.
 * 
 */
#ifdef CHRONO_HIGH_RESOLUTION_USE_FULL_ASSERT
  #define ASSERT_(condition_) if(!(condition_)) {return;}
  #define ASSERT_RETURN_(condition_, return_) if(!(condition_)) {return (return_);}
#else
  #define ASSERT_(condition_)
  #define ASSERT_RETURN_(condition_, return_)
#endif

#define ASSERT_NOT_NULL_(pointer_) ASSERT_((pointer_) != NULL)
#define ASSERT_NOT_NULL_RETURN_(pointer_, return_) ASSERT_RETURN_((pointer_) != NULL, (return_))

/* Private typedef -----------------------------------------------------------*/
typedef struct {

  bool_t Init;
  
  float64_t TickToNsCoef;                         // Multiplier that converts ticks to nanosecond.

  float64_t NsToTickCoef;                         // Convert nanoseconds into the number of ticks.

  high_resolution_tick_t TickTopValue;            // The tick top value. This private value is set by calling fChronoHighResolution_Init().

  high_resolution_tick_t TickInitValue;           // The tick init value. This private value is tick value when calling fChronoHighResolution_Init().

  sChronoHighResolution ChronoTickNs;             // **chrono** object that holds the tick value in nanoseconds since initializing the module using fChronoHighResolution_Init().

  float64_t ContinuousTickNs;                     // Time length in nanoseconds since initializing the chrono module.

#if (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_VARIABLE)

  volatile high_resolution_tick_t *pTickValue;    // Pointer to an unsigned integer that hold current value of the tick.

#elif (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_FUNCTION)

  high_resolution_tick_t(*fpGetTickValue)(void);  // Function pointer to a function that returns current value of the tick.

#endif

}sChronoInternal;

/* Private variables ---------------------------------------------------------*/
static sChronoInternal _chrono = {
  
  .Init = FALSE,
  .TickToNsCoef = 1U,
#if (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_VARIABLE)
  .pTickValue = NULL,
#elif (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_FUNCTION)
  .GetTickValue = NULL
#endif
};

/* Private function prototypes -----------------------------------------------*/
/* Variables -----------------------------------------------------------------*/

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                          ##### Exported Functions #####                          ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/** @defgroup INIT_FUNCTION Initialization functions
 *  @{
 */

#if (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_VARIABLE)

/**
 * @brief Initializes chrono module in VARIABLE mode based on specified parameters.
 * 
 * @attention This function must be called before using any other function in this library.
 * 
 * @note This function is only available when CHRONO_HIGH_RESOLUTION_TICK_TYPE is TICK_TYPE_VARIABLE.
 *
 * @note When configuring the chrono module with the TICK_TYPE_VARIABLE, the module fetches the current tick from the tick generator by dereferencing
 *       a memory address as high_resolution_tick_t. The pointer to this memory location is provided by the user via the tickValue argument.
 *       A practical use case for this configuration is when you have a spare timer available. You can start this timer in free run mode,
 *       allowing it to continuously count ticks. By reading the value from the corresponding COUNTER register, you can obtain the current tick value.
 *       This approach provides an efficient and accurate way to track time intervals and perform time-related operations in your embedded system.
 * 
 * @param tickValue Pointer to the memory location that contains the tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
chrono_high_resolution_res_t fChronoHighResolution_Init(volatile high_resolution_tick_t *tickValue) {
  
  _chrono.Init = FALSE;

  if(CHRONO_HIGH_RESOLUTION_TICK_TOP_VALUE == 0U) {
    return CHRONO_HIGH_RESOLUTION_ERROR_TICK_TOP_ZERO;
  }

  if(CHRONO_HIGH_RESOLUTION_TICK_TO_NANOSECOND_COEF == 0.0) {
    return CHRONO_HIGH_RESOLUTION_ERROR_TICK_TO_NS_ZERO;
  }
  
  if(tickValue == NULL) {
    return CHRONO_HIGH_RESOLUTION_ERROR_TICK_PTR_ERROR;
  }
  
  _chrono.TickTopValue = CHRONO_HIGH_RESOLUTION_TICK_TOP_VALUE;
  _chrono.TickToNsCoef = CHRONO_HIGH_RESOLUTION_TICK_TO_NANOSECOND_COEF;
  _chrono.pTickValue = tickValue;

  _chrono.NsToTickCoef = (1.0 / CHRONO_HIGH_RESOLUTION_TICK_TO_NANOSECOND_COEF);
  
  _chrono.ContinuousTickNs = 0U;
  
  _chrono.Init = TRUE;
  
  START_(&_chrono.ChronoTickNs);

  _chrono.TickInitValue = GET_TICK_();

  return CHRONO_HIGH_RESOLUTION_OK;
}
#elif (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_FUNCTION)

/**
 * @brief Initializes chrono module in FUNCTION mode based on specified parameters.
 * 
 * @attention This function must be called before using any other function in this library.
 * 
 * @note This function is only available when CHRONO_HIGH_RESOLUTION_TICK_TYPE is TICK_TYPE_FUNCTION.
 *
 * @note When configuring the chrono module in this manner, the module always invokes a function that returns the current tick as high_resolution_tick_t.
 *       The user provides a pointer to this function as an input argument when calling fChronoHighResolution_Init(). This approach allows the chrono module to dynamically fetch the current tick value from the user-defined function,
 *       ensuring accurate and efficient timekeeping. Whether you're using a timer or a custom tick generator, this flexibility enables precise time measurements in your embedded system.
 * 
 * @param fpTickValue Pointer to the function that returns tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
chrono_high_resolution_res_t fChronoHighResolution_Init(high_resolution_tick_t(*fpTickValue)(void)) {

  _chrono.Init = FALSE;

  if(CHRONO_HIGH_RESOLUTION_TICK_TOP_VALUE == 0U) {
    return CHRONO_HIGH_RESOLUTION_ERROR_TICK_TOP_ZERO;
  }

  if(CHRONO_HIGH_RESOLUTION_TICK_TO_NANOSECOND_COEF == 0U) {
    return CHRONO_HIGH_RESOLUTION_ERROR_TICK_TO_NS_ZERO;
  }
  
  if(fpTickValue == NULL) {
    return CHRONO_HIGH_RESOLUTION_ERROR_TICK_PTR_ERROR;
  }
  
  _chrono.TickTopValue = CHRONO_HIGH_RESOLUTION_TICK_TOP_VALUE;
  _chrono.TickToNsCoef = CHRONO_HIGH_RESOLUTION_TICK_TO_NANOSECOND_COEF;
  _chrono.GetTickValue = fpTickValue;

  _chrono.NsToTickCoef = (1000U / CHRONO_HIGH_RESOLUTION_TICK_TO_NANOSECOND_COEF);

  _chrono.ContinuousTickNs = 0U;

  _chrono.Init = TRUE;
  
  START_(&_chrono.ChronoTickNs);

  _chrono.TickInitValue = GET_TICK_();

  return CHRONO_HIGH_RESOLUTION_OK;
}
#endif

/** @} */ //End of INIT_FUNCTION

/** @defgroup FUNCTIONAL_API Functional API in chrono module
 *  @brief These functions don't require an object of type sChronoHighResolution.
 *  @note The functional API in this module is re-entrant.
 *  @{
 */

/**
 * @brief Returns the availability of tick in nanosecond after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is nanoseconds tick available.
 */
bool_t fChronoHighResolution_IsTickNsAvailable(void) {
  
  return _chrono.Init;
}

/**
 * @brief Get ram usage by module.
 * 
 * @retval ram usage in byte.
 */
uint32_t fChronoHighResolution_GetRamUsage(void) {
  
  return sizeof(sChronoInternal);
}

/**
 * @brief Get current tick.
 * 
 * @note based on CHRONO_HIGH_RESOLUTION_TICK_TYPE two cases are possible:
 *         In TICK_TYPE_VARIABLE mode, tick is returned by dereferencing the address of tick value as high_resolution_tick_t.
 *         In TICK_TYPE_FUNCTION mode, tick is returned by calling a user defined function that generates tick value.
 *
 * @retval tick: Current tick (raw)
 */
high_resolution_tick_t fChronoHighResolution_GetTick(void) {

  ASSERT_RETURN_(_chrono.Init, (high_resolution_tick_t)0);
  
  return GET_TICK_();
}

/**
 * @brief Returns the continuous tick value converted to nanoseconds since calling fChronoHighResolution_Init().
 * 
 * @attention Always remember to call this function before tick generator overflows. For example if tick generator reaches its topValue in 8.9 seconds,
 *            make sure fChronoHighResolution_GetContinuousTickNs() is called once in this interval.
 *
 * @retval continuousTickTime: Time length since calling fChronoHighResolution_Init() in nanoseconds
 */
timeNs_t fChronoHighResolution_GetContinuousTickNs(void) {

  ASSERT_RETURN_(_chrono.Init, 0);
  
  _chrono.ContinuousTickNs += (timeNs_t)fChronoHighResolution_ElapsedNs(&_chrono.ChronoTickNs);
  
  START_(&_chrono.ChronoTickNs);
  
  return _chrono.ContinuousTickNs;
}

/**
 * @brief Get Tick top value.
 * 
 * @note User can get adjusted tick top value, using this function. It returns the same value that was taken from user in fChronoHighResolution_Init().
 * 
 * @retval topValue: Tick top value
 */
high_resolution_tick_t fChronoHighResolution_GetTickTopValue(void) {
  
  ASSERT_RETURN_(_chrono.Init, (high_resolution_tick_t)0);
  
  return _chrono.TickTopValue;
}

/**
 * @brief Get Tick init value.
 * 
 * @note User can get tick init value, using this function. It returns the tick value when calling fChronoHighResolution_Init().
 * 
 * @retval initValue: Tick init value
 */
high_resolution_tick_t fChronoHighResolution_GetTickInitValue(void) {
  
  ASSERT_RETURN_(_chrono.Init, (high_resolution_tick_t)0);
  
  return _chrono.TickInitValue;
}

/**
 * @brief Returns the coefficient for converting one tick to nanoseconds.
 * 
 * @note This coefficient is used to convert ticks (which are unitless) to time lengths with proper units.
 * 
 * @note The **chrono** module cannot measure time lengths smaller than this multiplier.
 * 
 * @retval tickToNsCoef: Tick-to-nanoseconds coefficient
 */
float64_t fChronoHighResolution_GetTickToNsCoef(void) {
  
  ASSERT_RETURN_(_chrono.Init, 0);
  
  return _chrono.TickToNsCoef;
}

/**
 * @brief Returns the pointer to the tick generator.
 * 
 * @retval tickPointer: Pointer to the tick generator.
 */
#if (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_VARIABLE)

volatile high_resolution_tick_t* fChronoHighResolution_GetTickPointer(void) {
  
  ASSERT_RETURN_(_chrono.Init, NULL);

  return _chrono.pTickValue;
}

#elif (CHRONO_HIGH_RESOLUTION_TICK_TYPE == TICK_TYPE_FUNCTION)

fpTick_t fChronoHighResolution_GetTickPointer(void) {
  
  ASSERT_RETURN_(_chrono.Init, NULL);
  
  return _chrono.GetTickValue;
}

#else
#error "CHRONO_HIGH_RESOLUTION_TICK_TYPE must be defined"
#endif

/**
 * @brief Returns the maximum measurable time interval in nanoseocnds that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval (nanoseconds)
 */
timeNs_t fChronoHighResolution_GetMaxMeasurableTimeNs(void) {
  
  ASSERT_RETURN_(_chrono.Init, (timeNs_t)0);
  
  return (timeNs_t)_chrono.TickTopValue / _chrono.TickToNsCoef;
}

/**
 * @brief Returns the time length in nanoseconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (nanoseconds)
 */
timeNs_t fChronoHighResolution_TimeSpanNs(high_resolution_tick_t startTick, high_resolution_tick_t endTick) {
  
  ASSERT_RETURN_(_chrono.Init, (timeNs_t)0);
  
  return TIMESPAN_NS_(startTick, endTick);
}

/**
 * @brief Returns the time length in ticks between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (ticks)
 */
high_resolution_tick_t fChronoHighResolution_TimeSpanTick(high_resolution_tick_t startTick, high_resolution_tick_t endTick) {
  
  ASSERT_RETURN_(_chrono.Init, (high_resolution_tick_t)0);
  
  return (high_resolution_tick_t)(ELAPSED_(endTick, startTick));
}

/**
 * @brief Generates a delay in nanoseconds.
 * 
 * @note Call this function, and it will terminate when the elapsed time is greater than the input argument.
 * 
 * @param delayMs Delay time length (nanoseconds)
 */
void fChronoHighResolution_DelayNs(timeNs_t delayNs) {

  ASSERT_(_chrono.Init);
  
  high_resolution_tick_t startTick = GET_TICK_();
  high_resolution_tick_t delayTick = delayNs * _chrono.NsToTickCoef;
  
  DELAY_TICK_(startTick, delayTick);
}

/** @} */ //End of FUNCTIONAL_API

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChronoHighResolution. Users must declare one to use them.
 *  @{
 */

/**
 * @brief Starts the chrono object for measuring time.
 * 
 * @note This function puts the chrono object in the run state.
 *       Functions like fChronoHighResolution_ElapsedS() or fChronoHighResolution_IntervalNs() only return measured values if there is a start time tag already.
 *       Therefore, this function must be called before any time measurement.
 * 
 * @param me Pointer to the chrono object
 */
void fChronoHighResolution_Start(sChronoHighResolution * const me) {
  
  ASSERT_(_chrono.Init);
  ASSERT_NOT_NULL_(me);
  
  me->_startTick = GET_TICK_();
  me->_run = TRUE;
}

/**
 * @brief Stops the chrono object to signal the end of time measurement.
 * 
 * @note This function stops the chrono object and puts it in the stop state. Measurements are not valid in this case, and corresponding functions return 0.
 * 
 * @param me Pointer to the chrono object
 */
void fChronoHighResolution_Stop(sChronoHighResolution * const me) {

  ASSERT_(_chrono.Init);
  ASSERT_NOT_NULL_(me);
  
  me->_stopTick = GET_TICK_();
  me->_run = FALSE;
  me->_isTimeout = FALSE;

}

/**
 * @brief Returns the elapsed time in nanoseconds since the start of the measurement using fChronoHighResolution_Start().
 * 
 * @note fChronoHighResolution_Start() must be called before using this function. It returns the time passed since the start.
 * 
 * @param me Pointer to the chrono object
 * @retval elapsed: Elapsed time since starting the chrono object (nanoseconds)
 */
timeNs_t fChronoHighResolution_ElapsedNs(sChronoHighResolution const * const me) {
  
  ASSERT_RETURN_(_chrono.Init, (timeNs_t)0);
  ASSERT_NOT_NULL_RETURN_(me, (timeNs_t)0);

  if(!me->_run) {
    return TIMESPAN_NS_(me->_startTick, me->_stopTick);
  }
  
  high_resolution_tick_t currentTick = GET_TICK_();
  return (timeNs_t)((timeNs_t)(ELAPSED_(currentTick, me->_startTick)) / _chrono.NsToTickCoef);
}

/**
 * @brief Returns the remaining time in nanoseconds until the chrono object reaches its timeout.
 * 
 * @note Before using this function, ensure that fChronoHighResolution_StartTimeoutNs() has been called to start the measurement.
 * 
 * @note This function works correctly if a timeout has occurred, returning zero in that case.
 *       However, if it is called after the timeout, and the tick values have surpassed the start tick, the returned value is no longer valid.
 * 
 * @param me Pointer to the chrono object
 * @retval timeLeft: Time length until timeout (nanoseconds)
 */
timeNs_t fChronoHighResolution_LeftNs(sChronoHighResolution * const me) {
  
  ASSERT_RETURN_(_chrono.Init, (timeNs_t)0);
  ASSERT_NOT_NULL_RETURN_(me, (timeNs_t)0);

  if(!me->_run) {
    return (timeNs_t)0;
  }
  
  if(me->_isTimeout) {
    return (timeNs_t)0;
  }
  
  high_resolution_tick_t currentTick = GET_TICK_();
  high_resolution_tick_t elapsedTick = ELAPSED_(currentTick, me->_startTick);
  timeNs_t elapsed = (timeNs_t)((timeNs_t)(elapsedTick) / _chrono.NsToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeNs_t)0;
  } else {
    return ((me->_timeout / _chrono.NsToTickCoef) - elapsed);
  }
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (nanoseconds)
 */
void fChronoHighResolution_StartTimeoutNs(sChronoHighResolution * const me, timeNs_t timeout) {

  ASSERT_(_chrono.Init);
  ASSERT_NOT_NULL_(me);
  
  me->_timeout = (high_resolution_tick_t)(timeout * (timeNs_t)_chrono.NsToTickCoef);
  me->_isTimeout = FALSE;
  
  START_(me);
}

/**
 * @brief Returns the timeout status of the chrono object.
 * 
 * @note Before using this function, ensure that fChronoHighResolution_StartTimeoutNs() has been called to start the measurement.
 * 
 * @param me Pointer to the chrono object
 * @retval isTimeout: TRUE if the chrono is timed out, otherwise returns FALSE
 */
bool_t fChronoHighResolution_IsTimeout(sChronoHighResolution * const me) {
  
  ASSERT_RETURN_(_chrono.Init, FALSE);
  ASSERT_NOT_NULL_RETURN_(me, FALSE);

  if(!me->_run) {
    return FALSE;
  }
  
  high_resolution_tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return TRUE;
  }
  
  high_resolution_tick_t currentTick = GET_TICK_();
  if(ELAPSED_(currentTick, startTick) >= me->_timeout) {
    me->_isTimeout = TRUE;
  } else {
    return FALSE;
  }

  return TRUE;
}

/**
 * @brief Returns the time interval in nanoseconds between two consecutive calls to this function.
 * 
 * @note Before using this function, ensure that fChronoHighResolution_Start() has been called first. Whenever this function is called, it measures the interval
 *       between the current tick and the last time fChronoHighResolution_IntervalNs() was called.
 *
 * @note For the first call, it measures the interval between fChronoHighResolution_Start() and the first fChronoHighResolution_IntervalNs() function calls.
 * 
 * @note A practical use case is measuring the time interval during the execution of a periodic function.
 *       By invoking fChronoHighResolution_IntervalNs() at the beginning of the periodic function, you can accurately measure the interval between consecutive runs.
 * 
 * @param me Pointer to the chrono object
 * @retval interval: Time interval between two consecutive calls of the function (nanoseconds)
 */
timeNs_t fChronoHighResolution_IntervalNs(sChronoHighResolution * const me) {
  
  ASSERT_RETURN_(_chrono.Init, (timeNs_t)0);
  ASSERT_NOT_NULL_RETURN_(me, (timeNs_t)0);

  if(!me->_run) {
    return (timeNs_t)0;
  }
  
  high_resolution_tick_t currentTick = GET_TICK_();
  high_resolution_tick_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeNs_t)(ELAPSED_(currentTick, startTick)) / _chrono.NsToTickCoef);
}

/** @} */ //End of OBJECTIVE_API

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/************************ Copyright (c) 2025 FaraabinCo *****END OF FILE****/
