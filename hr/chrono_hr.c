/**
  ******************************************************************************
  * @file           : chrono_hr.c
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
#include "chrono_hr.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/**
 * @brief Returns the time length between "a" and "b".
 * 
 * @note Chronologically, "a" must be a time tick after "b", but the value of "a" could be less than "b" i.e., when tick overflows.
 * 
 */
#if(CHRONO_HR_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
#define ELAPSED_(a, b)  (((a) >= (b)) ? ((a) - (b)) : ((_chronoHr.TickTopValue) - ((b) - (a))))
#elif(CHRONO_HR_TICK_COUNTERMODE == TICK_COUNTERMODE_DOWN)
#define ELAPSED_(a, b)  (((a) <= (b)) ? ((b) - (a)) : ((_chronoHr.TickTopValue) - ((a) - (b))))
#else
#error "CHRONO_HR_TICK_COUNTERMODE should be either TICK_COUNTERMODE_UP or TICK_COUNTERMODE_DOWN in chrono_hr_config.h file."
#endif

/**
 * @brief Define GET_TICK_() macro for internal use.
 * 
 */
#if (CHRONO_HR_TICK_TYPE == TICK_TYPE_VARIABLE)
  #define GET_TICK_() (*_chronoHr.pTickValue)
#elif (CHRONO_HR_TICK_TYPE == TICK_TYPE_FUNCTION)
  #define GET_TICK_() (_chronoHr.GetTickValue())
#else
  #define GET_TICK_() (tick_hr_t)0;
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
  tick_hr_t currentTick = GET_TICK_(); \
  while(ELAPSED_(currentTick, (startTick_)) < (delayTick_)) { \
    currentTick = GET_TICK_(); \
  }

  /**
   * @brief Define TIMESPAN_() macro for internal use.
   * 
   */
#define TIMESPAN_NS_(startTick_, endTick_) ((timeNs_t)(ELAPSED_((endTick_), (startTick_))) / _chronoHr.NsToTickCoef)

/**
 * @brief Define assert macros.
 * 
 */
#ifdef CHRONO_HR_USE_FULL_ASSERT
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

  bool_t Init;                      // Init flag of the module.
  
  float64_t TickToNsCoef;           // Multiplier that converts ticks to nanosecond.

  float64_t NsToTickCoef;           // Convert nanoseconds into the number of ticks.

  tick_hr_t TickTopValue;           // The tick top value. This private value is set by calling fChronoHr_Init().

  tick_hr_t TickInitValue;          // The tick init value. This private value is tick value when calling fChronoHr_Init().

  sChronoHr ChronoTickNs;           // **chrono** object that holds the tick value in nanoseconds since initializing the module using fChronoHr_Init().

  float64_t ContinuousTickNs;       // Time length in nanoseconds since initializing the chrono module.

#if (CHRONO_HR_TICK_TYPE == TICK_TYPE_VARIABLE)

  volatile tick_hr_t *pTickValue;   // Pointer to an unsigned integer that hold current value of the tick.

#elif (CHRONO_HR_TICK_TYPE == TICK_TYPE_FUNCTION)

  tick_hr_t(*fpGetTickValue)(void); // Function pointer to a function that returns current value of the tick.

#endif

}sChronoHrInternal;

/* Private variables ---------------------------------------------------------*/
static sChronoHrInternal _chronoHr = {
  
  .Init = FALSE,
  .TickToNsCoef = 1U,
#if (CHRONO_HR_TICK_TYPE == TICK_TYPE_VARIABLE)
  .pTickValue = NULL,
#elif (CHRONO_HR_TICK_TYPE == TICK_TYPE_FUNCTION)
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

#if (CHRONO_HR_TICK_TYPE == TICK_TYPE_VARIABLE)

/**
 * @brief Initializes chrono module in VARIABLE mode based on specified parameters.
 * 
 * @attention This function must be called before using any other function in this library.
 * 
 * @note This function is only available when CHRONO_HR_TICK_TYPE is TICK_TYPE_VARIABLE.
 *
 * @note When configuring the chrono module with the TICK_TYPE_VARIABLE, the module fetches the current tick from the tick generator by dereferencing
 *       a memory address as tick_hr_t. The pointer to this memory location is provided by the user via the tickValue argument.
 *       A practical use case for this configuration is when you have a spare timer available. You can start this timer in free run mode,
 *       allowing it to continuously count ticks. By reading the value from the corresponding COUNTER register, you can obtain the current tick value.
 *       This approach provides an efficient and accurate way to track time intervals and perform time-related operations in your embedded system.
 * 
 * @param tickValue Pointer to the memory location that contains the tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
chrono_hr_res_t fChronoHr_Init(volatile tick_hr_t *tickValue) {
  
  _chronoHr.Init = FALSE;

  if(CHRONO_HR_TICK_TOP_VALUE == 0U) {
    return CHRONO_HR_ERROR_TICK_TOP_ZERO;
  }

  if(CHRONO_HR_TICK_TO_NANOSECOND_COEF == 0.0) {
    return CHRONO_HR_ERROR_TICK_TO_NS_ZERO;
  }
  
  if(tickValue == NULL) {
    return CHRONO_HR_ERROR_TICK_PTR_ERROR;
  }
  
  _chronoHr.TickTopValue = CHRONO_HR_TICK_TOP_VALUE;
  _chronoHr.TickToNsCoef = CHRONO_HR_TICK_TO_NANOSECOND_COEF;
  _chronoHr.pTickValue = tickValue;

  _chronoHr.NsToTickCoef = (1.0 / CHRONO_HR_TICK_TO_NANOSECOND_COEF);
  
  _chronoHr.ContinuousTickNs = 0U;
  
  _chronoHr.Init = TRUE;
  
  START_(&_chronoHr.ChronoTickNs);

  _chronoHr.TickInitValue = GET_TICK_();

  return CHRONO_HR_OK;
}
#elif (CHRONO_HR_TICK_TYPE == TICK_TYPE_FUNCTION)

/**
 * @brief Initializes chrono module in FUNCTION mode based on specified parameters.
 * 
 * @attention This function must be called before using any other function in this library.
 * 
 * @note This function is only available when CHRONO_HR_TICK_TYPE is TICK_TYPE_FUNCTION.
 *
 * @note When configuring the chrono module in this manner, the module always invokes a function that returns the current tick as tick_hr_t.
 *       The user provides a pointer to this function as an input argument when calling fChronoHr_Init(). This approach allows the chrono module to dynamically fetch the current tick value from the user-defined function,
 *       ensuring accurate and efficient timekeeping. Whether you're using a timer or a custom tick generator, this flexibility enables precise time measurements in your embedded system.
 * 
 * @param fpTickValue Pointer to the function that returns tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
chrono_hr_res_t fChronoHr_Init(tick_hr_t(*fpTickValue)(void)) {

  _chronoHr.Init = FALSE;

  if(CHRONO_HR_TICK_TOP_VALUE == 0U) {
    return CHRONO_HR_ERROR_TICK_TOP_ZERO;
  }

  if(CHRONO_HR_TICK_TO_NANOSECOND_COEF == 0U) {
    return CHRONO_HR_ERROR_TICK_TO_NS_ZERO;
  }
  
  if(fpTickValue == NULL) {
    return CHRONO_HR_ERROR_TICK_PTR_ERROR;
  }
  
  _chronoHr.TickTopValue = CHRONO_HR_TICK_TOP_VALUE;
  _chronoHr.TickToNsCoef = CHRONO_HR_TICK_TO_NANOSECOND_COEF;
  _chronoHr.GetTickValue = fpTickValue;

  _chronoHr.NsToTickCoef = (1000U / CHRONO_HR_TICK_TO_NANOSECOND_COEF);

  _chronoHr.ContinuousTickNs = 0U;

  _chronoHr.Init = TRUE;
  
  START_(&_chronoHr.ChronoTickNs);

  _chronoHr.TickInitValue = GET_TICK_();

  return CHRONO_HR_OK;
}
#endif

/** @} */ //End of INIT_FUNCTION

/** @defgroup FUNCTIONAL_API Functional API in chrono module
 *  @brief These functions don't require an object of type sChronoHr.
 *  @note The functional API in this module is re-entrant.
 *  @{
 */

/**
 * @brief Returns the availability of tick in nanosecond after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is nanoseconds tick available.
 */
bool_t fChronoHr_IsTickNsAvailable(void) {
  
  return _chronoHr.Init;
}

/**
 * @brief Get ram usage by module.
 * 
 * @retval ram usage in byte.
 */
uint32_t fChronoHr_GetRamUsage(void) {
  
  return sizeof(sChronoHrInternal);
}

/**
 * @brief Get current tick.
 * 
 * @note based on CHRONO_HR_TICK_TYPE two cases are possible:
 *         In TICK_TYPE_VARIABLE mode, tick is returned by dereferencing the address of tick value as tick_hr_t.
 *         In TICK_TYPE_FUNCTION mode, tick is returned by calling a user defined function that generates tick value.
 *
 * @retval tick: Current tick (raw)
 */
tick_hr_t fChronoHr_GetTick(void) {

  ASSERT_RETURN_(_chronoHr.Init, (tick_hr_t)0);
  
  return GET_TICK_();
}

/**
 * @brief Returns the continuous tick value converted to nanoseconds since calling fChronoHr_Init().
 * 
 * @attention Always remember to call this function before tick generator overflows. For example if tick generator reaches its topValue in 8.9 seconds,
 *            make sure fChronoHr_GetContinuousTickNs() is called once in this interval.
 *
 * @retval continuousTickTime: Time length since calling fChronoHr_Init() in nanoseconds
 */
timeNs_t fChronoHr_GetContinuousTickNs(void) {

  ASSERT_RETURN_(_chronoHr.Init, 0);
  
  _chronoHr.ContinuousTickNs += (timeNs_t)fChronoHr_ElapsedNs(&_chronoHr.ChronoTickNs);
  
  START_(&_chronoHr.ChronoTickNs);
  
  return _chronoHr.ContinuousTickNs;
}

/**
 * @brief Get Tick top value.
 * 
 * @note User can get adjusted tick top value, using this function. It returns the same value that was taken from user in fChronoHr_Init().
 * 
 * @retval topValue: Tick top value
 */
tick_hr_t fChronoHr_GetTickTopValue(void) {
  
  ASSERT_RETURN_(_chronoHr.Init, (tick_hr_t)0);
  
  return _chronoHr.TickTopValue;
}

/**
 * @brief Get Tick init value.
 * 
 * @note User can get tick init value, using this function. It returns the tick value when calling fChronoHr_Init().
 * 
 * @retval initValue: Tick init value
 */
tick_hr_t fChronoHr_GetTickInitValue(void) {
  
  ASSERT_RETURN_(_chronoHr.Init, (tick_hr_t)0);
  
  return _chronoHr.TickInitValue;
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
float64_t fChronoHr_GetTickToNsCoef(void) {
  
  ASSERT_RETURN_(_chronoHr.Init, 0);
  
  return _chronoHr.TickToNsCoef;
}

/**
 * @brief Returns the pointer to the tick generator.
 * 
 * @retval tickPointer: Pointer to the tick generator.
 */
#if (CHRONO_HR_TICK_TYPE == TICK_TYPE_VARIABLE)

volatile tick_hr_t* fChronoHr_GetTickPointer(void) {
  
  ASSERT_RETURN_(_chronoHr.Init, NULL);

  return _chronoHr.pTickValue;
}

#elif (CHRONO_HR_TICK_TYPE == TICK_TYPE_FUNCTION)

fpTick_t fChronoHr_GetTickPointer(void) {
  
  ASSERT_RETURN_(_chronoHr.Init, NULL);
  
  return _chronoHr.GetTickValue;
}

#else
#error "CHRONO_HR_TICK_TYPE must be defined"
#endif

/**
 * @brief Returns the maximum measurable time interval in nanoseocnds that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval (nanoseconds)
 */
timeNs_t fChronoHr_GetMaxMeasurableTimeNs(void) {
  
  ASSERT_RETURN_(_chronoHr.Init, (timeNs_t)0);
  
  return (timeNs_t)_chronoHr.TickTopValue / _chronoHr.TickToNsCoef;
}

/**
 * @brief This function measures the average overhead that tic_() and tocNs_() macros have intrinsically.
 * 
 * @note To accurately account for timing overhead, use the fChronoHr_MeasureTicTocOverheadNs_() function.
 *       The actual elapsed time is calculated by subtracting the measured overhead from the time recorded by 
 *       the tic and toc macros.
 * 
 * @param averageQty Nummber of iterations to measure the overhead time for getting the average.
 * @return timeNs_t Average time overhead of the tic_ toc_ macros.
 */
timeNs_t fChronoHr_MeasureAverageOverheadOfTicTocNs(uint32_t averageQty) {
  
  ASSERT_RETURN_(_chronoHr.Init, (timeNs_t)0);

  float64_t sum = 0.0;
  for(uint32_t i = 0; i < averageQty; i++) {

    ticHr_();
    sum += tocNs_();
  
  }

  return ((sum) / (averageQty));\
}

/** @} */ //End of AVG_OVERHEAD_TIC_TOC

/**
 * @brief Returns the time length in nanoseconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (nanoseconds)
 */
timeNs_t fChronoHr_TimeSpanNs(tick_hr_t startTick, tick_hr_t endTick) {
  
  ASSERT_RETURN_(_chronoHr.Init, (timeNs_t)0);
  
  return TIMESPAN_NS_(startTick, endTick);
}

/**
 * @brief Returns the time length in ticks between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (ticks)
 */
tick_hr_t fChronoHr_TimeSpanTick(tick_hr_t startTick, tick_hr_t endTick) {
  
  ASSERT_RETURN_(_chronoHr.Init, (tick_hr_t)0);
  
  return (tick_hr_t)(ELAPSED_(endTick, startTick));
}

/**
 * @brief Generates a delay in nanoseconds.
 * 
 * @note Call this function, and it will terminate when the elapsed time is greater than the input argument.
 * 
 * @param delayMs Delay time length (nanoseconds)
 */
void fChronoHr_DelayNs(timeNs_t delayNs) {

  ASSERT_(_chronoHr.Init);
  
  tick_hr_t startTick = GET_TICK_();
  tick_hr_t delayTick = delayNs * _chronoHr.NsToTickCoef;
  
  DELAY_TICK_(startTick, delayTick);
}

/** @} */ //End of FUNCTIONAL_API

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChronoHr. Users must declare one to use them.
 *  @{
 */

/**
 * @brief Starts the chrono object for measuring time.
 * 
 * @note This function puts the chrono object in the run state.
 *       Functions like fChronoHr_ElapsedS() or fChronoHr_IntervalNs() only return measured values if there is a start time tag already.
 *       Therefore, this function must be called before any time measurement.
 * 
 * @param me Pointer to the chrono object
 */
void fChronoHr_Start(sChronoHr * const me) {
  
  ASSERT_(_chronoHr.Init);
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
void fChronoHr_Stop(sChronoHr * const me) {

  ASSERT_(_chronoHr.Init);
  ASSERT_NOT_NULL_(me);
  
  me->_stopTick = GET_TICK_();
  me->_run = FALSE;
  me->_isTimeout = FALSE;

}

/**
 * @brief Returns the elapsed time in nanoseconds since the start of the measurement using fChronoHr_Start().
 * 
 * @note fChronoHr_Start() must be called before using this function. It returns the time passed since the start.
 * 
 * @param me Pointer to the chrono object
 * @retval elapsed: Elapsed time since starting the chrono object (nanoseconds)
 */
timeNs_t fChronoHr_ElapsedNs(sChronoHr const * const me) {
  
  ASSERT_RETURN_(_chronoHr.Init, (timeNs_t)0);
  ASSERT_NOT_NULL_RETURN_(me, (timeNs_t)0);

  if(!me->_run) {
    return TIMESPAN_NS_(me->_startTick, me->_stopTick);
  }
  
  tick_hr_t currentTick = GET_TICK_();
  return (timeNs_t)((timeNs_t)(ELAPSED_(currentTick, me->_startTick)) / _chronoHr.NsToTickCoef);
}

/**
 * @brief Returns the remaining time in nanoseconds until the chrono object reaches its timeout.
 * 
 * @note Before using this function, ensure that fChronoHr_StartTimeoutNs() has been called to start the measurement.
 * 
 * @note This function works correctly if a timeout has occurred, returning zero in that case.
 *       However, if it is called after the timeout, and the tick values have surpassed the start tick, the returned value is no longer valid.
 * 
 * @param me Pointer to the chrono object
 * @retval timeLeft: Time length until timeout (nanoseconds)
 */
timeNs_t fChronoHr_LeftNs(sChronoHr * const me) {
  
  ASSERT_RETURN_(_chronoHr.Init, (timeNs_t)0);
  ASSERT_NOT_NULL_RETURN_(me, (timeNs_t)0);

  if(!me->_run) {
    return (timeNs_t)0;
  }
  
  if(me->_isTimeout) {
    return (timeNs_t)0;
  }
  
  tick_hr_t currentTick = GET_TICK_();
  tick_hr_t elapsedTick = ELAPSED_(currentTick, me->_startTick);
  timeNs_t elapsed = (timeNs_t)((timeNs_t)(elapsedTick) / _chronoHr.NsToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeNs_t)0;
  } else {
    return ((me->_timeout / _chronoHr.NsToTickCoef) - elapsed);
  }
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (nanoseconds)
 */
void fChronoHr_StartTimeoutNs(sChronoHr * const me, timeNs_t timeout) {

  ASSERT_(_chronoHr.Init);
  ASSERT_NOT_NULL_(me);
  
  me->_timeout = (tick_hr_t)(timeout * (timeNs_t)_chronoHr.NsToTickCoef);
  me->_isTimeout = FALSE;
  
  START_(me);
}

/**
 * @brief Returns the timeout status of the chrono object.
 * 
 * @note Before using this function, ensure that fChronoHr_StartTimeoutNs() has been called to start the measurement.
 * 
 * @param me Pointer to the chrono object
 * @retval isTimeout: TRUE if the chrono is timed out, otherwise returns FALSE
 */
bool_t fChronoHr_IsTimeout(sChronoHr * const me) {
  
  ASSERT_RETURN_(_chronoHr.Init, FALSE);
  ASSERT_NOT_NULL_RETURN_(me, FALSE);

  if(!me->_run) {
    return FALSE;
  }
  
  tick_hr_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return TRUE;
  }
  
  tick_hr_t currentTick = GET_TICK_();
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
 * @note Before using this function, ensure that fChronoHr_Start() has been called first. Whenever this function is called, it measures the interval
 *       between the current tick and the last time fChronoHr_IntervalNs() was called.
 *
 * @note For the first call, it measures the interval between fChronoHr_Start() and the first fChronoHr_IntervalNs() function calls.
 * 
 * @note A practical use case is measuring the time interval during the execution of a periodic function.
 *       By invoking fChronoHr_IntervalNs() at the beginning of the periodic function, you can accurately measure the interval between consecutive runs.
 * 
 * @param me Pointer to the chrono object
 * @retval interval: Time interval between two consecutive calls of the function (nanoseconds)
 */
timeNs_t fChronoHr_IntervalNs(sChronoHr * const me) {
  
  ASSERT_RETURN_(_chronoHr.Init, (timeNs_t)0);
  ASSERT_NOT_NULL_RETURN_(me, (timeNs_t)0);

  if(!me->_run) {
    return (timeNs_t)0;
  }
  
  tick_hr_t currentTick = GET_TICK_();
  tick_hr_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeNs_t)(ELAPSED_(currentTick, startTick)) / _chronoHr.NsToTickCoef);
}

/** @} */ //End of OBJECTIVE_API

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/************************ Copyright (c) 2025 FaraabinCo *****END OF FILE****/
