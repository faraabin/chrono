/**
  ******************************************************************************
  * @file           : chrono.c
  * @brief          : Chrono module.
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
  @mainpage
  
  # Features
  - Easily connects to a tick generator for measuring time intervals.
  - Designed with an objective approach, allowing users to create as many chrono objects as needed.
  - Supports time measurements such as calculating intervals, elapsed times, and timeouts, making it easy to use.

  # How to use
  This section explains how to configure the chrono module, create an instance, and use it in your application.

  ## Configuration
  1- In chrono_config.h file, select the appropriate CHRONO_TICK_TYPE:
    - Choose TICK_TYPE_VARIABLE if you want to provide an address of the memory location where the tick value can be read.
    This address is passed to fChrono_Init() as a pointer to a variable of type tick_t.
    - Alternatively, select TICK_TYPE_FUNCTION if you prefer to obtain the current tick by calling a function.
    In this case, you must provide a function that returns the current tick as tick_t. The address of this function is passed to fChrono_Init().
  
  2- In chrono_config.h file, set the value of CHRONO_TICK_TOP_VALUE. This should be the top value that your tick generator can count upto.

  3- In chrono_config.h file, set the value of CHRONO_TICK_TO_NANOSECOND_COEF. This is the amount of time that one tick increment takes.

  4- In chrono_config.h file, set the CHRONO_TICK_COUNTERMODE which is the counting mode of your tick generator.
    - Select TICK_COUNTERMODE_UP if tick generator up counts from zero to CHRONO_TICK_TOP_VALUE.
    - Select TICK_COUNTERMODE_DOWN if tick generator down counts from CHRONO_TICK_TOP_VALUE to zero.
  
  5- In chrono_config.h file, specify type of the variables that chrono module uses for time measuremenst. These types are:
    - tick_t: Type of the tick value. Depending on your application, this type can be uint16_t, uint32_t, etc.
    - timeS_t: Type of the variable that is used to measure time lengths in seconds.
    - timeMs_t: Type of the variable that is used to measure time lengths in milliseconds.
    - timeUs_t: Type of the variable that is used to measure time lengths in microseconds.
  It's important to consider proper data types used for measuring time intervals in your application. Here are some key points:
    - Atomic access to memory for obtaining tickValue.
    - Maximum measurable time length and its accuracy in long time measurements.
  If you have a 16-bit MCU its better to configure chrono with a 16-bit time tick, but keep in mind that maximum measurable time is limited compared to a 32-bit tick.
  Also, a variable of type double has better accuracy for time measurements than a float one.

  6- Initialize the chrono module by calling fChrono_Init(). You need to provide the following parameter:
    - Depending on the CHRONO_TICK_TYPE, provide either a pointer to a tick_t variable or a tick_t(*fptr)(void) function pointer
      that returns the current tick for the chrono module.
  
  Now the chrono module is ready to use!
  
  ## Using functions
  After initializing the module, there are two kinds of methods available for users.

  1- Functions that Require a Chrono Object (sChrono) as Input:
    - The following methods need an instantiated sChrono object:
      - fChrono_Start(), fChrono_Stop(), fChrono_ElapsedS(), fChrono_ElapsedMs(), fChrono_ElapsedUs(), fChrono_LeftS(),
        fChrono_LeftMs(), fChrono_LeftUs(), fChrono_StartTimeoutS(), fChrono_StartTimeoutMs(), fChrono_StartTimeoutUs(),
        fChrono_IsTimeout(), fChrono_IntervalS(), fChrono_IntervalMs() and fChrono_IntervalUs().
    - These methods are primarily used for creating time stamps and measuring time intervals. They require a variable of type sChrono to perform their tasks.
  
  2- Functions that Do Not Require a Chrono Object:
    - The following functions do not need a sChrono object:
      - fChrono_GetTick(), fChrono_GetContinuousTickMs(), fChrono_GetTickTopValue(), fChrono_GetTickToNsCoef(), fChrono_GetMaxMeasurableTimeMs(),
        fChrono_DelayS(), fChrono_DelayMs(), fChrono_DelayUs(), fChrono_TimeSpanS(), fChrono_TimeSpanMs() and fChrono_TimeSpanUs().
    - These functions are used for obtaining internal information from the chrono module, generating delays, and measuring time duration between two ticks.
      Since they serve general purposes, they do not require a specific instantiated sChrono object.
  
  ### Time measurement
  For measuring elapsed time from a user-defined time stamp:
  - Create a variable of type sChrono.
  - Start this chrono object using the fChrono_Start() function. This function puts the object in the run state and creates a time tag in the user's code.
  - Call one of the following functions:
    - fChrono_ElapsedS()
    - fChrono_ElapsedMs()
    - fChrono_ElapsedUs()
  - These functions measure the time length between the line of code where fChrono_Start() was called and the line of code where these measurement functions are called.
    They return the time length in their corresponding units.
  - Whenever a stop is needed for time measurement, the user can call fChrono_Stop() on the chrono object. This action resets all measurement results and puts the chrono in stop mode.

  ### Timeout
  To utilize the timeout functionality, follow these steps:
  - Create a variable of type sChrono.
  - Start this chrono object in timeout mode using the fChrono_StartTimeoutS(), fChrono_StartTimeoutMs() and fChrono_StartTimeoutUs() functions.
  - Query whether the chrono is timed out or not using fChrono_IsTimeout().
  - To stop the chrono object, call fChrono_Stop(). This action puts the object in the stop state, and checking its timeout state will return FALSE.
  - To determine the remaining time until the chrono times out, use fChrono_LeftS(), fChrono_LeftMs(), or fChrono_LeftUs().

  ### Generating time delay
  For creating time delays, there is no need to instantiate a chrono object. Simply call one of the following functions:
  - fChrono_DelayS()
  - fChrono_DelayMs()
  - fChrono_DelayUs()

  ### Measuring time intervals
  To measure time intervals:
  - Create a variable of type sChrono.
  - Start this chrono object using the fChrono_Start() function. This action puts the object in the run state and creates a time tag in your code.
  - In the part of the code where time interval measurement is needed, simply call fChrono_IntervalS(), fChrono_IntervalMs() or fChrono_IntervalUs().
    These functions return the time interval between two consecutive calls.
    - The first time you measure the interval, it returns the time length between fChrono_Start() and the first call to fChrono_IntervalS(), fChrono_IntervalMs or fChrono_IntervalUs().
    - Subsequent calls measure the time between consecutive invocations of the fChrono_IntervalS(), fChrono_IntervalMs() or fChrono_IntervalUs().

  ### Getting current tick from tick generator
  To obtain the current tick from the tick generator:
  - User can call fChrono_GetTick(). This function returns the current tick value.
  - Additionally, user can use the following functions to measure the time length between two ticks:
    - fChrono_TimeSpanS()
    - fChrono_TimeSpanMs()
    - fChrono_TimeSpanUs()
  - If the tick generator experiences an overflow (just once), these functions will handle it and return the correct result.
  - However, if the tick generator has overflowed more than once, the time span measurement is no longer valid.

  ******************************************************************************
  Below is an example of how you can use the chrono module API for a generic application.

  @code
  #include "chrono.h"

  //Assuming tick is generated with a 32-bit timer, top value is 0xFFFFFFFF and each tick takes 1000 nano-seconds.
  static volatile uint32_t *tickValue = &(TIM->CNT);

  int main(void) {
    // Initialize the chrono module
    fChrono_Init(tickValue);

    // Create a chrono object
    sChrono myChrono;

    // Start myChrono for measuring time 
    fChrono_Start(&myChrono);

    // Perform other operations in your code
    // ...

    // Measure elapsed time in milliseconds
    uint32_t elapsedMs = fChrono_ElapsedMs(&myChrono);
    printf("Elapsed time: %u ms\n", elapsedMs);

    // Get the current tick value
    uint32_t currentTick = fChrono_GetTick();
    printf("Current tick: %u\n", currentTick);

    // Generate a 1-second delay
    fChrono_DelayS(1);

    // Stop the chrono object
    fChrono_Stop(&myChrono);

    // Create another chrono objects
    sChrono intervalChrono;
    sChrono timeoutChrono;

    // Start the chrono objects
    fChrono_Start(&intervalChrono);
    fChrono_StartTimeoutMs(&intervalChrono, 5000);

    while(TRUE) {

      // Measure time interval
      float interval = fChrono_IntervalUs(&chronoPeriod);
      printf("Runtime interval: %f\n", interval);

      // Check if the chrono has timed out (assuming a timeout of 5000 ms)
      if(fChrono_IsTimeout(&myChrono)) {
        printf("Timeout occurred!\n");
      }
    }
  }

  @endcode
  @mainpage
  */

/* Includes ------------------------------------------------------------------*/
#include "chrono.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/**
 * @brief Returns the time length between "a" and "b".
 * 
 * @note Chronologically, "a" must be a time tick after "b", but the value of "a" could be less than "b" i.e., when tick overflows.
 * 
 */
#if(CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
#define ELAPSED_(a, b)  (((a) >= (b)) ? ((a) - (b)) : ((_chrono.TickTopValue) - ((b) - (a))))
#elif(CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_DOWN)
#define ELAPSED_(a, b)  (((a) <= (b)) ? ((b) - (a)) : ((_chrono.TickTopValue) - ((a) - (b))))
#else
#error "CHRONO_TICK_COUNTERMODE should be either TICK_COUNTERMODE_UP or TICK_COUNTERMODE_DOWN in chrono_config.h file."
#endif

/**
 * @brief Define GET_TICK_() macro for internal use.
 * 
 */
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)
  #define GET_TICK_() (*_chrono.pTickValue)
#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)
  #define GET_TICK_() (_chrono.GetTickValue())
#else
  #define GET_TICK_() (tick_t)0;
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
  tick_t currentTick = GET_TICK_(); \
  while(ELAPSED_(currentTick, (startTick_)) < (delayTick_)) { \
    currentTick = GET_TICK_(); \
  }

  /**
   * @brief Define TIMESPAN_() macro for internal use.
   * 
   */
#define TIMESPAN_S_(startTick_, endTick_) ((timeS_t)(ELAPSED_((endTick_), (startTick_))) / _chrono.SecToTickCoef)
#define TIMESPAN_MS_(startTick_, endTick_) ((timeMs_t)(ELAPSED_((endTick_), (startTick_))) / _chrono.MsToTickCoef)
#define TIMESPAN_US_(startTick_, endTick_) ((timeUs_t)(ELAPSED_((endTick_), (startTick_))) / _chrono.UsToTickCoef)

/**
 * @brief Define assert macros.
 * 
 */
#ifdef CHRONO_USE_FULL_ASSERT
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
	
	bool_t InitUs;
	
	bool_t InitMs;
	
	bool_t InitSec;

	uint32_t TickToNsCoef; // Multiplier that converts ticks to nanosecond.

	uint32_t UsToTickCoef; // Multiplier that converts microseconds to ticks.

	uint32_t MsToTickCoef; // Multiplier that converts milliseconds to ticks.

	uint32_t SecToTickCoef; // Multiplier that converts seconds to ticks.

	tick_t TickTopValue; // The tick top value. This private value is set by calling fChrono_Init().

	tick_t TickInitValue; // The tick init value. This private value is tick value when calling fChrono_Init().

	sChrono ChronoTickUs; // **chrono** object that holds the tick value in microseconds since initializing the module using fChrono_Init().

	sChrono ChronoTickMs; // **chrono** object that holds the tick value in milliseconds since initializing the module using fChrono_Init().

	sChrono ChronoTickS; // **chrono** object that holds the tick value in seconds since initializing the module using fChrono_Init().

	uint64_t ContinuousTickUs; // Time length in microseconds since initializing the chrono module.

	uint64_t ContinuousTickMs; // Time length in milliseconds since initializing the chrono module.

	uint64_t ContinuousTickS; // Time length in seconds since initializing the chrono module.

#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

	volatile tick_t *pTickValue; // Pointer to an unsigned integer that hold current value of the tick.

#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)

	tick_t(*GetTickValue)(void); // Function pointer to a function that returns current value of the tick.

#endif

}sChronoInternal;

/* Private variables ---------------------------------------------------------*/
static sChronoInternal _chrono = {
	
	.Init = FALSE,
	.InitUs = FALSE,
	.InitMs = FALSE,
	.InitSec = FALSE,
	.TickToNsCoef = 1U,
	.UsToTickCoef = 1U,
	.MsToTickCoef = 1U,
	.SecToTickCoef = 1U,
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)
	.pTickValue = NULL,
#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)
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

#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

/**
 * @brief Initializes chrono module in VARIABLE mode based on specified parameters.
 * 
 * @attention This function must be called before using any other function in this library.
 * 
 * @note This function is only available when CHRONO_TICK_TYPE is TICK_TYPE_VARIABLE.
 *
 * @note When configuring the chrono module with the TICK_TYPE_VARIABLE, the module fetches the current tick from the tick generator by dereferencing
 *       a memory address as tick_t. The pointer to this memory location is provided by the user via the tickValue argument.
 *       A practical use case for this configuration is when you have a spare timer available. You can start this timer in free run mode,
 *       allowing it to continuously count ticks. By reading the value from the corresponding COUNTER register, you can obtain the current tick value.
 *       This approach provides an efficient and accurate way to track time intervals and perform time-related operations in your embedded system.
 * 
 * @param tickValue Pointer to the memory location that contains the tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
uint8_t fChrono_Init(volatile tick_t *tickValue) {
  
  _chrono.Init = FALSE;
  _chrono.InitUs = FALSE;
  _chrono.InitMs = FALSE;
  _chrono.InitSec = FALSE;

  if(CHRONO_TICK_TOP_VALUE == 0U) {
    return CHRONO_ERROR_TICK_TOP_ZERO;      /* MISRA 2012 Rule 15.5 deviation */
  }

  if(CHRONO_TICK_TO_NANOSECOND_COEF == 0U) {
    return CHRONO_ERROR_TICK_TO_NS_ZERO;    /* MISRA 2012 Rule 15.5 deviation */
  }
  
  if(tickValue == NULL) {
    return CHRONO_ERROR_TICK_PTR_ERROR;     /* MISRA 2012 Rule 15.5 deviation */
  }
  
  _chrono.TickTopValue = CHRONO_TICK_TOP_VALUE;
  _chrono.TickToNsCoef = CHRONO_TICK_TO_NANOSECOND_COEF;
  _chrono.pTickValue = tickValue;

  _chrono.UsToTickCoef = (1000U / CHRONO_TICK_TO_NANOSECOND_COEF);
  _chrono.MsToTickCoef = (1000000U / CHRONO_TICK_TO_NANOSECOND_COEF);
  _chrono.SecToTickCoef = (1000000000U / CHRONO_TICK_TO_NANOSECOND_COEF);
  
  _chrono.ContinuousTickUs = 0U;
	_chrono.ContinuousTickMs = 0U;
	_chrono.ContinuousTickS = 0U;
	
  _chrono.Init = TRUE;
  _chrono.InitUs = (_chrono.UsToTickCoef != 0U);
  _chrono.InitMs = (_chrono.MsToTickCoef != 0U);
  _chrono.InitSec = (_chrono.SecToTickCoef != 0U);
  
	START_(&_chrono.ChronoTickUs);
	START_(&_chrono.ChronoTickMs);
	START_(&_chrono.ChronoTickS);

  _chrono.TickInitValue = GET_TICK_();

  return CHRONO_OK;
}
#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)

/**
 * @brief Initializes chrono module in FUNCTION mode based on specified parameters.
 * 
 * @attention This function must be called before using any other function in this library.
 * 
 * @note This function is only available when CHRONO_TICK_TYPE is TICK_TYPE_FUNCTION.
 *
 * @note When configuring the chrono module in this manner, the module always invokes a function that returns the current tick as tick_t.
 *       The user provides a pointer to this function as an input argument when calling fChrono_Init(). This approach allows the chrono module to dynamically fetch the current tick value from the user-defined function,
 *       ensuring accurate and efficient timekeeping. Whether you're using a timer or a custom tick generator, this flexibility enables precise time measurements in your embedded system.
 * 
 * @param fpTickValue Pointer to the function that returns tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
uint8_t fChrono_Init(tick_t(*fpTickValue)(void)) {

  _chrono.Init = FALSE;
  _chrono.InitUs = FALSE;
  _chrono.InitMs = FALSE;
  _chrono.InitSec = FALSE;

  if(CHRONO_TICK_TOP_VALUE == 0U) {
    return CHRONO_ERROR_TICK_TOP_ZERO; /* MISRA 2012 Rule 15.5 deviation */
  }

  if(CHRONO_TICK_TO_NANOSECOND_COEF == 0U) {
    return CHRONO_ERROR_TICK_TO_NS_ZERO; /* MISRA 2012 Rule 15.5 deviation */
  }
  
  if(fpTickValue == NULL) {
    return CHRONO_ERROR_TICK_PTR_ERROR; /* MISRA 2012 Rule 15.5 deviation */
  }
  
  _chrono.TickTopValue = CHRONO_TICK_TOP_VALUE;
  _chrono.TickToNsCoef = CHRONO_TICK_TO_NANOSECOND_COEF;
  _chrono.GetTickValue = fpTickValue;

  _chrono.UsToTickCoef = (1000U / CHRONO_TICK_TO_NANOSECOND_COEF);
  _chrono.MsToTickCoef = (1000000U / CHRONO_TICK_TO_NANOSECOND_COEF);
  _chrono.SecToTickCoef = (1000000000U / CHRONO_TICK_TO_NANOSECOND_COEF);

  _chrono.ContinuousTickUs = 0U;
	_chrono.ContinuousTickMs = 0U;
	_chrono.ContinuousTickS = 0U;
	
  _chrono.Init = TRUE;
  _chrono.InitUs = (_chrono.UsToTickCoef != 0U);
  _chrono.InitMs = (_chrono.MsToTickCoef != 0U);
  _chrono.InitSec = (_chrono.SecToTickCoef != 0U);
  
	START_(&_chrono.ChronoTickUs);
	START_(&_chrono.ChronoTickMs);
	START_(&_chrono.ChronoTickS);

  _chrono.TickInitValue = GET_TICK_();

  return CHRONO_OK;
}
#endif

/** @} */ //End of INIT_FUNCTION

/** @defgroup FUNCTIONAL_API Functional API in chrono module
 *  @brief These functions don't require an object of type sChrono.
 *  @note The functional API in this module is re-entrant.
 *  @{
 */

/**
 * @brief Returns the availability of tick in microsecond after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is microseconds tick available.
 */
bool_t fChrono_IsTickUsAvailable(void) {
  
  if(!_chrono.Init) {
    return false;
  }
  
  return _chrono.InitUs;
}

/**
 * @brief Returns the availability of tick in milliseconds after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is milliseconds tick available.
 */
bool_t fChrono_IsTickMsAvailable(void) {
  
  if(!_chrono.Init) {
    return false;
  }
  
  return _chrono.InitMs;
}

/**
 * @brief Returns the availability of tick in milliseconds after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is milliseconds tick available.
 */
bool_t fChrono_IsTickSAvailable(void) {
  
  ASSERT_RETURN_(_chrono.Init, false);
  
  return _chrono.InitSec;
}

/**
 * @brief Get ram usage by module.
 * 
 * @retval ram usage in byte.
 */
uint32_t fChrono_GetRamUsage(void) {
	
	return sizeof(sChronoInternal);
}

/**
 * @brief Get current tick.
 * 
 * @note based on CHRONO_TICK_TYPE two cases are possible:
 *         In TICK_TYPE_VARIABLE mode, tick is returned by dereferencing the address of tick value as tick_t.
 *         In TICK_TYPE_FUNCTION mode, tick is returned by calling a user defined function that generates tick value.
 *
 * @retval tick: Current tick (raw)
 */
tick_t fChrono_GetTick(void) {

  ASSERT_RETURN_(_chrono.Init, (tick_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return GET_TICK_();
}

/**
 * @brief Returns the continuous tick value converted to microseconds since calling fChrono_Init().
 * 
 * @attention Always remember to call this function before tick generator overflows. For example if tick generator reaches its topValue in 71 minutes,
 *            make sure fChrono_GetContinuousTickUs() is called once in this interval.
 *
 * @retval continuousTickTime: Time length since calling fChrono_Init() in milliseconds
 */
uint64_t fChrono_GetContinuousTickUs(void) {

  ASSERT_RETURN_(_chrono.Init, 0); /* MISRA 2012 Rule 15.5 deviation */
  
  _chrono.ContinuousTickUs += (uint64_t)fChrono_ElapsedUs(&_chrono.ChronoTickUs);
  
  START_(&_chrono.ChronoTickUs);
  
  return _chrono.ContinuousTickUs;
}

/**
 * @brief Returns the continuous tick value converted to milliseconds since calling fChrono_Init().
 * 
 * @attention Always remember to call this function before tick generator overflows. For example if tick generator reaches its topValue in 71 minutes,
 *            make sure fChrono_GetContinuousTickMs() is called once in this interval.
 *
 * @retval continuousTickTime: Time length since calling fChrono_Init() in milliseconds
 */
uint64_t fChrono_GetContinuousTickMs(void) {

  ASSERT_RETURN_(_chrono.Init, 0); /* MISRA 2012 Rule 15.5 deviation */
  
  _chrono.ContinuousTickMs += (uint64_t)fChrono_ElapsedMs(&_chrono.ChronoTickMs);
  
  START_(&_chrono.ChronoTickMs);
  
  return _chrono.ContinuousTickMs;
}

/**
 * @brief Returns the continuous tick value converted to seconds since calling fChrono_Init().
 * 
 * @attention Always remember to call this function before tick generator overflows. For example if tick generator reaches its topValue in 71 minutes,
 *            make sure fChrono_GetContinuousTickS() is called once in this interval.
 *
 * @retval continuousTickTime: Time length since calling fChrono_Init() in milliseconds
 */
uint64_t fChrono_GetContinuousTickS(void) {

  ASSERT_RETURN_(_chrono.Init, 0); /* MISRA 2012 Rule 15.5 deviation */
  
  _chrono.ContinuousTickS += (uint64_t)fChrono_ElapsedS(&_chrono.ChronoTickS);
  
  START_(&_chrono.ChronoTickS);
  
  return _chrono.ContinuousTickS;
}

/**
 * @brief Get Tick top value.
 * 
 * @note User can get adjusted tick top value, using this function. It returns the same value that was taken from user in fChrono_Init().
 * 
 * @retval topValue: Tick top value
 */
tick_t fChrono_GetTickTopValue(void) {
  
  ASSERT_RETURN_(_chrono.Init, (tick_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return _chrono.TickTopValue;
}

/**
 * @brief Get Tick init value.
 * 
 * @note User can get tick init value, using this function. It returns the tick value when calling fChrono_Init().
 * 
 * @retval initValue: Tick init value
 */
tick_t fChrono_GetTickInitValue(void) {
  
  ASSERT_RETURN_(_chrono.Init, (tick_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return _chrono.TickInitValue;
}

/**
 * @brief Returns the coefficient for converting one tick to nanoseconds.
 * 
 * @note This coefficient is used to convert ticks (which are unitless) to time lengths with proper units.
 * 
 * @note The **chrono** module cannot measure time lengths smaller than this multiplier.
 * 
 * @note Other multipliers (tick to milliseconds and tick to seconds) are calculated based on this coefficient.
 * 
 * @retval tickToNsCoef: Tick-to-nanoseconds coefficient
 */
uint32_t fChrono_GetTickToNsCoef(void) {
  
  ASSERT_RETURN_(_chrono.Init, 0); /* MISRA 2012 Rule 15.5 deviation */
  
  return _chrono.TickToNsCoef;
}

/**
 * @brief Returns the pointer to the tick generator.
 * 
 * @retval tickPointer: Pointer to the tick generator.
 */
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

  volatile tick_t* fChrono_GetTickPointer(void) {
    
    ASSERT_RETURN_(_chrono.Init, NULL); /* MISRA 2012 Rule 15.5 deviation */
  
    return _chrono.pTickValue;
  }

#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)

  fpTick_t fChrono_GetTickPointer(void) {
    
    ASSERT_RETURN_(_chrono.Init, NULL); /* MISRA 2012 Rule 15.5 deviation */
    
    return _chrono.GetTickValue;
  }

#else

  #error "CHRONO_TICK_TYPE must be defined"

#endif

/**
 * @brief Returns the maximum measurable time interval in seocnds that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval (seconds)
 */
timeS_t fChrono_GetMaxMeasurableTimeS(void) {
  
  ASSERT_RETURN_(_chrono.InitSec, (timeS_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeS_t)_chrono.TickTopValue / _chrono.SecToTickCoef;
}

/**
 * @brief Returns the maximum measurable time interval in milliseocnds that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval (milliseocnds)
 */
timeMs_t fChrono_GetMaxMeasurableTimeMs(void) {
  
  ASSERT_RETURN_(_chrono.InitMs, (timeMs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeMs_t)_chrono.TickTopValue / _chrono.MsToTickCoef;
}

/**
 * @brief Returns the maximum measurable time interval in microseocnds that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval (microseocnds)
 */
timeUs_t fChrono_GetMaxMeasurableTimeUs(void) {
  
  ASSERT_RETURN_(_chrono.InitUs, (timeUs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeUs_t)_chrono.TickTopValue / _chrono.UsToTickCoef;
}

/**
 * @brief Returns the time length in seconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (seconds)
 */
timeS_t fChrono_TimeSpanS(tick_t startTick, tick_t endTick) {
  
  ASSERT_RETURN_(_chrono.InitSec, (timeS_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return TIMESPAN_S_(startTick, endTick);
}

/**
 * @brief Returns the time length in milliseconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (milliseconds)
 */
timeMs_t fChrono_TimeSpanMs(tick_t startTick, tick_t endTick) {
  
  ASSERT_RETURN_(_chrono.InitMs, (timeMs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return TIMESPAN_MS_(startTick, endTick);
}

/**
 * @brief Returns the time length in microseconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (microseconds)
 */
timeUs_t fChrono_TimeSpanUs(tick_t startTick, tick_t endTick) {
  
  ASSERT_RETURN_(_chrono.InitUs, (timeUs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return TIMESPAN_US_(startTick, endTick);
}

/**
 * @brief Returns the time length in ticks between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (ticks)
 */
tick_t fChrono_TimeSpanTick(tick_t startTick, tick_t endTick) {
  
  ASSERT_RETURN_(_chrono.Init, (tick_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (tick_t)(ELAPSED_(endTick, startTick));
}

/**
 * @brief Generates a delay in seconds.
 * 
 * @note Call this function, and it will terminate when the elapsed time is greater than the input argument.
 * 
 * @param delayS Delay time length (seconds)
 */
void fChrono_DelayS(timeS_t delayS) {

  ASSERT_(_chrono.InitSec); /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = GET_TICK_();
  tick_t delayTick = delayS * _chrono.SecToTickCoef;
  
  DELAY_TICK_(startTick, delayTick);
}

/**
 * @brief Generates a delay in milliseconds.
 * 
 * @note Call this function, and it will terminate when the elapsed time is greater than the input argument.
 * 
 * @param delayMs Delay time length (milliseconds)
 */
void fChrono_DelayMs(timeMs_t delayMs) {

  ASSERT_(_chrono.InitMs); /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = GET_TICK_();
  tick_t delayTick = delayMs * _chrono.MsToTickCoef;
  
  DELAY_TICK_(startTick, delayTick);
}

/**
 * @brief Generates a delay in microseconds.
 * 
 * @note Call this function, and it will terminate when the elapsed time is greater than the input argument.
 * 
 * @param delayMs Delay time length (microseconds)
 */
void fChrono_DelayUs(timeUs_t delayUs) {

  ASSERT_(_chrono.InitUs); /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = GET_TICK_();
  tick_t delayTick = delayUs * _chrono.UsToTickCoef;
  
  DELAY_TICK_(startTick, delayTick);
}

/** @} */ //End of FUNCTIONAL_API

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChrono. Users must declare one to use them.
 *  @{
 */

/**
 * @brief Starts the chrono object for measuring time.
 * 
 * @note This function puts the chrono object in the run state.
 *       Functions like fChrono_ElapsedS() or fChrono_IntervalUs() only return measured values if there is a start time tag already.
 *       Therefore, this function must be called before any time measurement.
 * 
 * @param me Pointer to the chrono object
 */
void fChrono_Start(sChrono * const me) {
  
  ASSERT_(_chrono.Init);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_(me);   /* MISRA 2012 Rule 15.5 deviation */
  
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
void fChrono_Stop(sChrono * const me) {

  ASSERT_(_chrono.Init);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_(me);   /* MISRA 2012 Rule 15.5 deviation */
  
  me->_stopTick = GET_TICK_();
  me->_run = FALSE;
  me->_isTimeout = FALSE;

}

/**
 * @brief Returns the elapsed time in seconds since the start of the measurement using fChrono_Start().
 * 
 * @note fChrono_Start() must be called before using this function. It returns the time passed since the start.
 * 
 * @param me Pointer to the chrono object
 * @retval elapsed: Elapsed time since starting the chrono object (seconds)
 */
timeS_t fChrono_ElapsedS(sChrono const * const me) {
  
  ASSERT_RETURN_(_chrono.InitSec, (timeS_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeS_t)0);      /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, TIMESPAN_S_(me->_startTick, me->_stopTick));               /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = GET_TICK_();
  return (timeS_t)((timeS_t)(ELAPSED_(currentTick, me->_startTick)) / _chrono.SecToTickCoef);
}

/**
 * @brief Returns the elapsed time in milliseconds since the start of the measurement using fChrono_Start().
 * 
 * @note fChrono_Start() must be called before using this function. It returns the time passed since the start.
 * 
 * @param me Pointer to the chrono object
 * @retval elapsed: Elapsed time since starting the chrono object (milliseconds)
 */
timeMs_t fChrono_ElapsedMs(sChrono const * const me) {
  
  ASSERT_RETURN_(_chrono.InitMs, (timeMs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeMs_t)0);     /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, TIMESPAN_MS_(me->_startTick, me->_stopTick));                /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = GET_TICK_();
  return (timeMs_t)((timeMs_t)(ELAPSED_(currentTick, me->_startTick)) / _chrono.MsToTickCoef);
}

/**
 * @brief Returns the elapsed time in microseconds since the start of the measurement using fChrono_Start().
 * 
 * @note fChrono_Start() must be called before using this function. It returns the time passed since the start.
 * 
 * @param me Pointer to the chrono object
 * @retval elapsed: Elapsed time since starting the chrono object (microseconds)
 */
timeUs_t fChrono_ElapsedUs(sChrono const * const me) {
  
  ASSERT_RETURN_(_chrono.InitUs, (timeUs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeUs_t)0);     /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, TIMESPAN_US_(me->_startTick, me->_stopTick));                /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = GET_TICK_();
  return (timeUs_t)((timeUs_t)(ELAPSED_(currentTick, me->_startTick)) / _chrono.UsToTickCoef);
}

/**
 * @brief Returns the remaining time in seconds until the chrono object reaches its timeout.
 * 
 * @note Before using this function, ensure that fChrono_StartTimeoutS(), fChrono_StartTimeoutMs() or fChrono_StartTimeoutUs() has been called to start the measurement.
 * 
 * @note This function works correctly if a timeout has occurred, returning zero in that case.
 *       However, if it is called after the timeout, and the tick values have surpassed the start tick, the returned value is no longer valid.
 * 
 * @param me Pointer to the chrono object
 * @retval timeLeft: Time length until timeout (seconds)
 */
timeS_t fChrono_LeftS(sChrono * const me) {
  
  ASSERT_RETURN_(_chrono.InitSec, (timeS_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeS_t)0);      /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, (timeS_t)0);         /* MISRA 2012 Rule 15.5 deviation */
  
  if(me->_isTimeout) {
    return (timeS_t)0;
  }
  
  tick_t currentTick = GET_TICK_();
  tick_t elapsedTick = ELAPSED_(currentTick, me->_startTick);
  timeS_t elapsed = (timeS_t)((timeS_t)(elapsedTick) / _chrono.SecToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeS_t)0;
  } else {
    return ((me->_timeout / _chrono.SecToTickCoef) - elapsed);
  }
}

/**
 * @brief Returns the remaining time in milliseconds until the chrono object reaches its timeout.
 * 
 * @note Before using this function, ensure that fChrono_StartTimeoutS(), fChrono_StartTimeoutMs() or fChrono_StartTimeoutUs() has been called to start the measurement.
 * 
 * @note This function works correctly if a timeout has occurred, returning zero in that case.
 *       However, if it is called after the timeout, and the tick values have surpassed the start tick, the returned value is no longer valid.
 * 
 * @param me Pointer to the chrono object
 * @retval timeLeft: Time length until timeout (milliseconds)
 */
timeMs_t fChrono_LeftMs(sChrono * const me) {
  
  ASSERT_RETURN_(_chrono.InitMs, (timeMs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeMs_t)0);     /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, (timeMs_t)0);        /* MISRA 2012 Rule 15.5 deviation */
  
  if(me->_isTimeout) {
    return (timeMs_t)0;
  }
  
  tick_t currentTick = GET_TICK_();
  tick_t elapsedTick = ELAPSED_(currentTick, me->_startTick);
  timeMs_t elapsed = (timeMs_t)((timeMs_t)(elapsedTick) / _chrono.MsToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeMs_t)0;
  } else {
    return ((me->_timeout / _chrono.MsToTickCoef) - elapsed);
  }
}

/**
 * @brief Returns the remaining time in microseconds until the chrono object reaches its timeout.
 * 
 * @note Before using this function, ensure that fChrono_StartTimeoutS(), fChrono_StartTimeoutMs() or fChrono_StartTimeoutUs() has been called to start the measurement.
 * 
 * @note This function works correctly if a timeout has occurred, returning zero in that case.
 *       However, if it is called after the timeout, and the tick values have surpassed the start tick, the returned value is no longer valid.
 * 
 * @param me Pointer to the chrono object
 * @retval timeLeft: Time length until timeout (microseconds)
 */
timeUs_t fChrono_LeftUs(sChrono * const me) {
  
  ASSERT_RETURN_(_chrono.InitUs, (timeUs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeUs_t)0);     /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, (timeUs_t)0);        /* MISRA 2012 Rule 15.5 deviation */
  
  if(me->_isTimeout) {
    return (timeUs_t)0;
  }
  
  tick_t currentTick = GET_TICK_();
  tick_t elapsedTick = ELAPSED_(currentTick, me->_startTick);
  timeUs_t elapsed = (timeUs_t)((timeUs_t)(elapsedTick) / _chrono.UsToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeUs_t)0;
  } else {
    return ((me->_timeout / _chrono.UsToTickCoef) - elapsed);
  }
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (seconds)
 */
void fChrono_StartTimeoutS(sChrono * const me, timeS_t timeout) {

  ASSERT_(_chrono.InitSec); /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_(me);     /* MISRA 2012 Rule 15.5 deviation */
  
  me->_timeout = (tick_t)(timeout * (timeS_t)_chrono.SecToTickCoef);
  me->_isTimeout = FALSE;
  
  START_(me);
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (milliseconds)
 */
void fChrono_StartTimeoutMs(sChrono * const me, timeMs_t timeout) {

  ASSERT_(_chrono.Init);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_(me);   /* MISRA 2012 Rule 15.5 deviation */
  
  me->_timeout = (tick_t)(timeout * (timeMs_t)_chrono.MsToTickCoef);
  me->_isTimeout = FALSE;
  
  START_(me);
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (microseconds)
 */
void fChrono_StartTimeoutUs(sChrono * const me, timeUs_t timeout) {

  ASSERT_(_chrono.InitUs);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_(me);     /* MISRA 2012 Rule 15.5 deviation */
  
  me->_timeout = (tick_t)(timeout * (timeUs_t)_chrono.UsToTickCoef);
  me->_isTimeout = FALSE;
  
  START_(me);
}

/**
 * @brief Returns the timeout status of the chrono object.
 * 
 * @note Before using this function, ensure that fChrono_StartTimeoutS(), fChrono_StartTimeoutMs() or fChrono_StartTimeoutUs() has been called to start the measurement.
 * 
 * @param me Pointer to the chrono object
 * @retval isTimeout: TRUE if the chrono is timed out, otherwise returns FALSE
 */
bool_t fChrono_IsTimeout(sChrono * const me) {
  
  ASSERT_RETURN_(_chrono.Init, FALSE); /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, FALSE);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, FALSE);     /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return TRUE;
  }
  
  tick_t currentTick = GET_TICK_();
  if(ELAPSED_(currentTick, startTick) >= me->_timeout) {
    me->_isTimeout = TRUE;
  } else {
    return FALSE;
  }

  return TRUE;
}

/**
 * @brief Returns the time interval in seconds between two consecutive calls to this function.
 * 
 * @note Before using this function, ensure that fChrono_Start() has been called first. Whenever this function is called, it measures the interval
 *       between the current tick and the last time fChrono_IntervalS() was called.
 *
 * @note For the first call, it measures the interval between fChrono_Start() and the first fChrono_IntervalS() function calls.
 * 
 * @note A practical use case is measuring the time interval during the execution of a periodic function.
 *       By invoking fChrono_IntervalS() at the beginning of the periodic function, you can accurately measure the interval between consecutive runs.
 * 
 * @param me Pointer to the chrono object
 * @retval interval: Time interval between two consecutive calls of the function (seconds)
 */
timeS_t fChrono_IntervalS(sChrono * const me) {
  
  ASSERT_RETURN_(_chrono.InitSec, (timeS_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeS_t)0);      /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, (timeS_t)0);         /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = GET_TICK_();
  tick_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeS_t)(ELAPSED_(currentTick, startTick)) / _chrono.SecToTickCoef);
}

/**
 * @brief Returns the time interval in milliseconds between two consecutive calls to this function.
 * 
 * @note Before using this function, ensure that fChrono_Start() has been called first. Whenever this function is called, it measures the interval
 *       between the current tick and the last time fChrono_IntervalMs() was called.
 *
 * @note For the first call, it measures the interval between fChrono_Start() and the first fChrono_IntervalMs() function calls.
 * 
 * @note A practical use case is measuring the time interval during the execution of a periodic function.
 *       By invoking fChrono_IntervalMs() at the beginning of the periodic function, you can accurately measure the interval between consecutive runs.
 * 
 * @param me Pointer to the chrono object
 * @retval interval: Time interval between two consecutive calls of the function (milliseconds)
 */
timeMs_t fChrono_IntervalMs(sChrono * const me) {
  
  ASSERT_RETURN_(_chrono.InitMs, (timeMs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeMs_t)0);     /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, (timeMs_t)0);        /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = GET_TICK_();
  tick_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeMs_t)(ELAPSED_(currentTick, startTick)) / _chrono.MsToTickCoef);
}

/**
 * @brief Returns the time interval in microseconds between two consecutive calls to this function.
 * 
 * @note Before using this function, ensure that fChrono_Start() has been called first. Whenever this function is called, it measures the interval
 *       between the current tick and the last time fChrono_IntervalUs() was called.
 *
 * @note For the first call, it measures the interval between fChrono_Start() and the first fChrono_IntervalUs() function calls.
 * 
 * @note A practical use case is measuring the time interval during the execution of a periodic function.
 *       By invoking fChrono_IntervalUs() at the beginning of the periodic function, you can accurately measure the interval between consecutive runs.
 * 
 * @param me Pointer to the chrono object
 * @retval interval: Time interval between two consecutive calls of the function (microseconds)
 */
timeUs_t fChrono_IntervalUs(sChrono * const me) {
  
  ASSERT_RETURN_(_chrono.InitUs, (timeUs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_NOT_NULL_RETURN_(me, (timeUs_t)0);     /* MISRA 2012 Rule 15.5 deviation */
  ASSERT_RETURN_(me->_run, (timeUs_t)0);        /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = GET_TICK_();
  tick_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeUs_t)(ELAPSED_(currentTick, startTick)) / _chrono.UsToTickCoef);
}

/** @} */ //End of OBJECTIVE_API

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/


/************************ Copyright (c) 2024 FaraabinCo *****END OF FILE****/
