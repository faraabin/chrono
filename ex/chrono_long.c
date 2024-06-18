/**
  ******************************************************************************
  * @file           : chrono_long.c
  * @brief          : ChronoLong module.
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
  */

/* Includes ------------------------------------------------------------------*/
#include "chrono_long.h"

#include "chrono_long_config.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/**
 * @brief Returns the time length between "a" and "b".
 * 
 * @note Chronologically, "a" must be a time tick after "b", but the value of "a" could be less than "b" (e.g., when tick overflows).
 * 
 */
#define ELAPSED_(a, b)  (((a) >= (b)) ? ((a) - (b)) : ((0xFFFFFFFF) - ((b) - (a))))

/**
 * @brief Checks whether the chrono object is in the run state or not.
 * 
 * @note If the chrono object is not running, this macro forces the API to return the values specified by "ret".
 * 
 */
#define CHECK_RUN_(ret)   if(!me->_run){return (ret);}

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Variables -----------------------------------------------------------------*/

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                          ##### Exported Functions #####                          ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChrono. Users must declare one to use this API.
 *  @{
 */

/**
 * @brief Starts the chrono object for measuring time.
 * 
 * @note This function puts the chrono object in the run state.
 *       Functions like fChronoLong_ElapsedS() or fChronoLong_IntervalUs() only return measured values if there is a start time tag already.
 *       Therefore, this function must be called before any time measurement.
 * 
 * @param me Pointer to the chrono object
 */
void fChronoLong_Start(sChrono * const me) {

  CHRONO_LONG_CRITICAL_SECTION_ENTER_;
  me->_startTick = (tick_t)millis_();
  CHRONO_LONG_CRITICAL_SECTION_EXIT_;

  me->_run = TRUE;
}

/**
 * @brief Stops the chrono object to signal the end of time measurement.
 * 
 * @note This function stops the chrono object and puts it in the stop state. Measurements are not valid in this case, and corresponding functions return 0.
 * 
 * @param me Pointer to the chrono object
 */
void fChronoLong_Stop(sChrono * const me) {
  
  me->_run = FALSE;
  me->_isTimeout = FALSE;
}

/**
 * @brief Returns the elapsed time in seconds since the start of the measurement using fChronoLong_Start().
 * 
 * @note fChronoLong_Start() must be called before using this function. It returns the time passed since the start.
 * 
 * @param me Pointer to the chrono object
 * @retval elapsed: Elapsed time since starting the chrono object (seconds)
 */
timeS_t fChronoLong_ElapsedS(sChrono const * const me) {
  
  CHECK_RUN_(0);  /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  CHRONO_LONG_CRITICAL_SECTION_ENTER_;
  uint64_t millis = millis_();
  CHRONO_LONG_CRITICAL_SECTION_EXIT_;

  return (timeS_t)((timeS_t)(ELAPSED_(millis, startTick)) * 0.001);
}

/**
 * @brief Returns the elapsed time in milliseconds since the start of the measurement using fChronoLong_Start().
 * 
 * @note fChronoLong_Start() must be called before using this function. It returns the time passed since the start.
 * 
 * @param me Pointer to the chrono object
 * @retval elapsed: Elapsed time since starting the chrono object (milliseconds)
 */
timeMs_t fChronoLong_ElapsedMs(sChrono const * const me) {
  
  CHECK_RUN_(0);  /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;

  CHRONO_LONG_CRITICAL_SECTION_ENTER_;
  uint64_t millis = millis_();
  CHRONO_LONG_CRITICAL_SECTION_EXIT_;
  
  return (timeMs_t)((timeMs_t)(ELAPSED_(millis, startTick)));
}

/**
 * @brief Returns the remaining time in seconds until the chrono object reaches its timeout.
 * 
 * @note Before using this function, ensure that fChronoLong_StartTimeoutS() or fChronoLong_StartTimeoutMs() has been called to start the measurement.
 * 
 * @note This function works correctly if a timeout has occurred, returning zero in that case.
 *       However, if it is called after the timeout, and the tick values have surpassed the start tick, the returned value is no longer valid.
 * 
 * @param me Pointer to the chrono object
 * @retval timeLeft: Time length until timeout (seconds)
 */
timeS_t fChronoLong_LeftS(sChrono * const me) {
  
  CHECK_RUN_(0);  /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return 0;
  }

  CHRONO_LONG_CRITICAL_SECTION_ENTER_;
  uint64_t millis = millis_();
  CHRONO_LONG_CRITICAL_SECTION_EXIT_;
    
  timeS_t elapsed = (timeS_t)((timeS_t)(ELAPSED_(millis, startTick)) * 0.001);
  if(elapsed >= (me->_timeout * 0.001)) {
    me->_isTimeout = TRUE;
    return 0;
  } else {
    return ((me->_timeout * 0.001) - elapsed);
  }
}

/**
 * @brief Returns the remaining time in milliseconds until the chrono object reaches its timeout.
 * 
 * @note Before using this function, ensure that fChronoLong_StartTimeoutS() or fChronoLong_StartTimeoutMs() has been called to start the measurement.
 * 
 * @note This function works correctly if a timeout has occurred, returning zero in that case.
 *       However, if it is called after the timeout, and the tick values have surpassed the start tick, the returned value is no longer valid.
 * 
 * @param me Pointer to the chrono object
 * @retval timeLeft: Time length until timeout (milliseconds)
 */
timeMs_t fChronoLong_LeftMs(sChrono * const me) {
  
  CHECK_RUN_(0);  /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return 0;
  }

  CHRONO_LONG_CRITICAL_SECTION_ENTER_;
  uint64_t millis = millis_();
  CHRONO_LONG_CRITICAL_SECTION_EXIT_;
    
  timeMs_t elapsed = (timeMs_t)((timeMs_t)(ELAPSED_(millis, startTick)));
  if(elapsed >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return 0;
  } else {
    return ((me->_timeout) - elapsed);
  }
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (seconds)
 */
void fChronoLong_StartTimeoutS(sChrono * const me, timeS_t timeout) {
  
  me->_timeout = (tick_t)(timeout / (timeS_t)0.001);
  me->_isTimeout = FALSE;
  
  fChronoLong_Start(me);
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (milliseconds)
 */
void fChronoLong_StartTimeoutMs(sChrono * const me, timeMs_t timeout) {
  
  me->_timeout = (tick_t)(timeout / (timeMs_t)1);
  me->_isTimeout = FALSE;
  
  fChronoLong_Start(me);
}

/**
 * @brief Returns the timeout status of the chrono object.
 * 
 * @note Before using this function, ensure that fChronoLong_StartTimeoutS() or fChronoLong_StartTimeoutMs() has been called to start the measurement.
 * 
 * @param me Pointer to the chrono object
 * @retval isTimeout: TRUE if the chrono is timed out, otherwise returns FALSE
 */
bool8_t fChronoLong_IsTimeout(sChrono * const me) {
  
  CHECK_RUN_(FALSE);  /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return TRUE;
  }

  CHRONO_LONG_CRITICAL_SECTION_ENTER_;
  uint64_t millis = millis_();
  CHRONO_LONG_CRITICAL_SECTION_EXIT_;

  if(ELAPSED_(millis, startTick) >= me->_timeout) {
    me->_isTimeout = TRUE;
  } else {
    return FALSE;
  }

  return TRUE;
}

/** @} */ //End of OBJECTIVE_API

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/


/************************ Copyright (c) 2024 FaraabinCo *****END OF FILE****/
