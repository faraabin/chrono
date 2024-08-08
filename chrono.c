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
  
  2- In chrono_config.h file, specify type of the variables that chrono module uses for time measuremenst. These types are:
    - tick_t: Type of the tick value. Depending on your application, this type can be uint16_t, uint32_t, etc.
    - timeS_t: Type of the variable that is used to measure time lengths in seconds.
    - timeMs_t: Type of the variable that is used to measure time lengths in milliseconds.
    - timeUs_t: Type of the variable that is used to measure time lengths in microseconds.
  It's important to consider proper data types used for measuring time intervals in your application. Here are some key points:
    - Atomic access to memory for obtaining tickValue.
    - Maximum measurable time length and its accuracy in long time measurements.
  If you have a 16-bit MCU its better to configure chrono with a 16-bit time tick, but keep in mind that maximum measurable time is limited compared to a 32-bit tick.
  Also, a variable of type double has better accuracy for time measurements than a float one.

  3- Initialize the chrono module by calling fChrono_Init(). You need to provide the following parameters:
    - tickTopValue: The maximum tick value that the tick generator can count.
    - tickToNsCoef: A multiplier for converting each tick to its corresponding time length in nanoseconds.
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
      Since they serve specific purposes, they do not require an instantiated sChrono object.
  
  ### Time measurement
  For measuring elapsed time from a user-defined time stamp:
  - Create a variable of type sChrono.
  - Start this chrono object using the fChrono_Start() function. This function puts the object in the run state and creates a time tag in the user's code.
  - Call one of the following functions:
    - fChrono_ElapsedS()
    - fChrono_ElapsedMs()
    - fChrono_ElapsedUs()
  - These functions measure the time length between the line of code where fChrono_Start() was called and the line of code where these measurement functions are called. They return the time length in their corresponding units.
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
  - In the part of the code where time interval measurement is needed, simply call fChrono_IntervalS(), fChrono_IntervalMs() or fChrono_IntervalUs(). These functions return the time interval between two consecutive calls.
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
    fChrono_Init(0xFFFFFFFF, 1000, tickValue);

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
#define ELAPSED_(a, b)  (((a) >= (b)) ? ((a) - (b)) : ((TickTopValue) - ((b) - (a))))

/**
 * @brief Checks whether the chrono object is in the run state or not.
 * 
 * @note If the chrono object is not running, this macro forces the API to return the values specified by "ret".
 * 
 */
#define CHECK_RUN_(ret)   if(!me->_run){return (ret);}

/**
 * @brief Checks whether the chrono module is initialized or not.
 * 
 * @note If the chrono object is not initialized, this macro forces the API to return the values specified by "ret".
 * 
 */
#define CHECK_INIT_RET_(ret)      if(!Init){return (ret);}
#define CHECK_INIT_US_RET_(ret)   if(!InitUs){return (ret);}
#define CHECK_INIT_MS_RET_(ret)   if(!InitMs){return (ret);}
#define CHECK_INIT_SEC_RET_(ret)  if(!InitSec){return (ret);}

/**
 * @brief Checks whether the chrono module is initialized or not.
 * 
 * @note If the chrono object is not initialized, this macro forces the API to return.
 * 
 */
#define CHECK_INIT_()     if(!Init){return;}
#define CHECK_INIT_US_()  if(!InitUs){return;}
#define CHECK_INIT_MS_()  if(!InitMs){return;}
#define CHECK_INIT_SEC_() if(!InitSec){return;}

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup CHRONO_PRIVATE_VARIABLES Private variables in chrono module.
 *  @{
 */

/**
 * @brief Initialization state of the chrono module.
 * 
 */
static bool_t Init = FALSE;
static bool_t InitUs = FALSE;
static bool_t InitMs = FALSE;
static bool_t InitSec = FALSE;

/** @defgroup CHRONO_PVT_COEFS Private chrono coefficients
  * @ingroup CHRONO_PRIVATE_VARIABLES
  * @{
  */

/**
 * @brief Multiplier that converts ticks to nanosecond.
 */
static uint32_t TickToNsCoef = 1U;

/**
 * @brief Multiplier that converts microseconds to ticks.
 */
static uint32_t UsToTickCoef = 1U;

/**
 * @brief Multiplier that converts milliseconds to ticks.
 */
static uint32_t MsToTickCoef = 1U;

/**
 * @brief Multiplier that converts seconds to ticks.
 */
static uint32_t SecToTickCoef = 1U;
/** @} */ //End of CHRONO_PVT_COEFS

/**
 * @brief The tick top value. This private value is set by calling fChrono_Init().
 * 
 */
static tick_t TickTopValue;

/**
 * @brief **chrono** object that holds the tick value in microseconds since initializing the module using fChrono_Init().
 * 
 */
static sChrono ChronoTickUs;

/**
 * @brief **chrono** object that holds the tick value in milliseconds since initializing the module using fChrono_Init().
 * 
 */
static sChrono ChronoTickMs;

/**
 * @brief **chrono** object that holds the tick value in seconds since initializing the module using fChrono_Init().
 * 
 */
static sChrono ChronoTickS;

/**
 * @brief Time length in microseconds since initializing the chrono module.
 * 
 * @note To get this value user must call fChrono_GetContinuousTickUs(). 
 * 
 */
static uint64_t ContinuousTickUs;

/**
 * @brief Time length in milliseconds since initializing the chrono module.
 * 
 * @note To get this value user must call fChrono_GetContinuousTickMs(). 
 * 
 */
static uint64_t ContinuousTickMs;

/**
 * @brief Time length in seconds since initializing the chrono module.
 * 
 * @note To get this value user must call fChrono_GetContinuousTickS(). 
 * 
 */
static uint64_t ContinuousTickS;

/** @defgroup CHRONO_TICK_POINTER Tick pointer for getting current tick value.
 *  @{
 */
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

/**
 * @brief Pointer to an unsigned integer that hold current value of the tick.
 * 
 * @note This variable is only available when CHRONO_TICK_TYPE is TICK_TYPE_32BIT_VARIABLE.
 * 
 */
static volatile tick_t *pTickValue;

#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)

/**
 * @brief Function pointer to a function that returns current value of the tick.
 * 
 * @note This function pointer is only available when CHRONO_TICK_TYPE is TICK_TYPE_FUNCTION.
 * 
 */
static tick_t(*GetTickValue)(void) = NULL;

#endif
/** @} */ //End of CHRONO_TICK_POINTER

/** @} */ //End of CHRONO_PRIVATE_VARIABLES

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
 * @param tickTopValue The maximum number of counts that the tick generator can count (starting from zero).
 * @param tickToNsCoef Coefficient for converting one tick to nanoseconds. Other multipliers (tick to milliseconds and tick to seconds) are calculated based on this coefficient.
 * @param tickValue Pointer to the memory location that contains the tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
uint8_t fChrono_Init(tick_t tickTopValue, uint32_t tickToNsCoef, volatile tick_t *tickValue) {
  
  Init = FALSE;
  InitUs = FALSE;
  InitMs = FALSE;
  InitSec = FALSE;

  if(tickTopValue == 0U) {
    return CHRONO_ERROR_TICK_TOP_ZERO; /* MISRA 2012 Rule 15.5 deviation */
  }

  if(tickToNsCoef == 0U) {
    return CHRONO_ERROR_TICK_TO_NS_ZERO; /* MISRA 2012 Rule 15.5 deviation */
  }
  
  if(tickValue == NULL) {
    return CHRONO_ERROR_TICK_PTR_ERROR; /* MISRA 2012 Rule 15.5 deviation */
  }
  
  TickTopValue = tickTopValue;
  TickToNsCoef = tickToNsCoef;
  pTickValue = tickValue;

  UsToTickCoef = (1000U / tickToNsCoef);
  MsToTickCoef = (1000000U / tickToNsCoef);
  SecToTickCoef = (1000000000U / tickToNsCoef);
  
  ContinuousTickUs = 0U;
	ContinuousTickMs = 0U;
	ContinuousTickS = 0U;
	
  Init = TRUE;
  InitUs = (UsToTickCoef != 0U);
  InitMs = (MsToTickCoef != 0U);
  InitSec = (SecToTickCoef != 0U);
  
	fChrono_Start(&ChronoTickUs);
	fChrono_Start(&ChronoTickMs);
	fChrono_Start(&ChronoTickS);

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
 * @param tickTopValue The maximum number of counts that the tick generator can count (starting from zero).
 * @param tickToNsCoef Coefficient for converting one tick to nanoseconds. Other multipliers (tick to milliseconds and tick to seconds) are calculated based on this coefficient.
 * @param fpTickValue Pointer to the function that returns tick value.
 * @retval initStatus: returns 0 if successful and 1 if it fails.
 */
uint8_t fChrono_Init(tick_t tickTopValue, uint32_t tickToNsCoef, tick_t(*fpTickValue)(void)) {

  Init = FALSE;
  InitUs = FALSE;
  InitMs = FALSE;
  InitSec = FALSE;

  if(tickTopValue == 0) {
    return CHRONO_ERROR_TICK_TOP_ZERO; /* MISRA 2012 Rule 15.5 deviation */
  }

  if(tickToNsCoef == 0) {
    return CHRONO_ERROR_TICK_TO_NS_ZERO; /* MISRA 2012 Rule 15.5 deviation */
  }
  
  if(fpTickValue == NULL) {
    return CHRONO_ERROR_TICK_PTR_ERROR; /* MISRA 2012 Rule 15.5 deviation */
  }

  TickTopValue = tickTopValue;
  TickToNsCoef = tickToNsCoef;
  GetTickValue = fpTickValue;
  
  UsToTickCoef = (1000U / tickToNsCoef);
  MsToTickCoef = (1000000U / tickToNsCoef);
  SecToTickCoef = (1000000000U / tickToNsCoef);

  ContinuousTickUs = 0U;
	ContinuousTickMs = 0U;
	ContinuousTickS = 0U;
	
  Init = TRUE;
  InitUs = (UsToTickCoef != 0U);
  InitMs = (MsToTickCoef != 0U);
  InitSec = (SecToTickCoef != 0U);
  
	fChrono_Start(&ChronoTickUs);
	fChrono_Start(&ChronoTickMs);
	fChrono_Start(&ChronoTickS);

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
  
  if(!Init) {
    return false;
  }
  
  return InitUs;
}

/**
 * @brief Returns the availability of tick in milliseconds after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is milliseconds tick available.
 */
bool_t fChrono_IsTickMsAvailable(void) {
  
  if(!Init) {
    return false;
  }
  
  return InitMs;
}

/**
 * @brief Returns the availability of tick in milliseconds after initialization of the chrono module.
 * 
 * @retval isTickAvailable: Is milliseconds tick available.
 */
bool_t fChrono_IsTickSAvailable(void) {
  
  if(!Init) {
    return false;
  }
  
  return InitSec;
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

  CHECK_INIT_RET_((tick_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

  return *pTickValue;

#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)

  return GetTickValue();

#else
  
  return (tick_t)0;
  
#endif
  
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
  
  ContinuousTickUs += (uint64_t)fChrono_ElapsedUs(&ChronoTickUs);
  
  fChrono_Start(&ChronoTickUs);
  
  return ContinuousTickUs;
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
  
  ContinuousTickMs += (uint64_t)fChrono_ElapsedMs(&ChronoTickMs);
  
  fChrono_Start(&ChronoTickMs);
  
  return ContinuousTickMs;
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
  
  ContinuousTickS += (uint64_t)fChrono_ElapsedS(&ChronoTickS);
  
  fChrono_Start(&ChronoTickS);
  
  return ContinuousTickS;
}

/**
 * @brief Get Tick top value.
 * 
 * @note User can get adjusted tick top value, using this function. It returns the same value that was taken from user in fChrono_Init().
 * 
 * @retval topValue: Tick top value
 */
tick_t fChrono_GetTickTopValue(void) {
  
  CHECK_INIT_RET_((tick_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return TickTopValue;
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
  
  CHECK_INIT_RET_(0U); /* MISRA 2012 Rule 15.5 deviation */
  
  return TickToNsCoef;
}

/**
 * @brief Returns the pointer to the tick generator.
 * 
 * @retval tickPointer: Pointer to the tick generator.
 */
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)

  volatile tick_t* fChrono_GetTickPointer(void) {
    
    CHECK_INIT_RET_(NULL);  /* MISRA 2012 Rule 15.5 deviation */
  
    return pTickValue;
  }

#elif (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)

  fpTick_t fChrono_GetTickPointer(void) {
    
    CHECK_INIT_RET_(NULL);
    
    return GetTickValue;
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
  
  CHECK_INIT_SEC_RET_((timeS_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeS_t)TickTopValue / SecToTickCoef;
}

/**
 * @brief Returns the maximum measurable time interval in milliseocnds that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval (milliseocnds)
 */
timeMs_t fChrono_GetMaxMeasurableTimeMs(void) {
  
  CHECK_INIT_MS_RET_((timeMs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeMs_t)TickTopValue / MsToTickCoef;
}

/**
 * @brief Returns the maximum measurable time interval in microseocnds that can be counted by the tick generator until it reaches its top value.
 * 
 * @retval maxTime: Maximum measurable time interval (microseocnds)
 */
timeUs_t fChrono_GetMaxMeasurableTimeUs(void) {
  
  CHECK_INIT_US_RET_((timeUs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeUs_t)TickTopValue / UsToTickCoef;
}

/**
 * @brief Returns the time length in seconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (seconds)
 */
timeS_t fChrono_TimeSpanS(tick_t startTick, tick_t endTick) {
  
  CHECK_INIT_SEC_RET_((timeS_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeS_t)(ELAPSED_(endTick, startTick)) / SecToTickCoef;
}

/**
 * @brief Returns the time length in milliseconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (milliseconds)
 */
timeMs_t fChrono_TimeSpanMs(tick_t startTick, tick_t endTick) {
  
  CHECK_INIT_MS_RET_((timeMs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeMs_t)(ELAPSED_(endTick, startTick)) / MsToTickCoef;
  
}

/**
 * @brief Returns the time length in microseconds between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (microseconds)
 */
timeUs_t fChrono_TimeSpanUs(tick_t startTick, tick_t endTick) {
  
  CHECK_INIT_US_RET_((timeUs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
  return (timeUs_t)(ELAPSED_(endTick, startTick)) / UsToTickCoef;
  
}

/**
 * @brief Returns the time length in ticks between two time ticks.
 * 
 * @param startTick Start tick
 * @param endTick End tick
 * @retval timeSpan: Time length between two time stamps (ticks)
 */
tick_t fChrono_TimeSpanTick(tick_t startTick, tick_t endTick) {
  
  CHECK_INIT_RET_((tick_t)0); /* MISRA 2012 Rule 15.5 deviation */
  
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

  CHECK_INIT_SEC_();  /* MISRA 2012 Rule 15.5 deviation */
  
  sChrono chrono = {0};
  
  fChrono_Start(&chrono);
  
  while(fChrono_ElapsedS(&chrono) < delayS) {};
}

/**
 * @brief Generates a delay in milliseconds.
 * 
 * @note Call this function, and it will terminate when the elapsed time is greater than the input argument.
 * 
 * @param delayMs Delay time length (milliseconds)
 */
void fChrono_DelayMs(timeMs_t delayMs) {

  CHECK_INIT_MS_();  /* MISRA 2012 Rule 15.5 deviation */
  
  sChrono chrono = {0};
  
  fChrono_Start(&chrono);
  
  while(fChrono_ElapsedMs(&chrono) < delayMs) {};
}

/**
 * @brief Generates a delay in microseconds.
 * 
 * @note Call this function, and it will terminate when the elapsed time is greater than the input argument.
 * 
 * @param delayMs Delay time length (microseconds)
 */
void fChrono_DelayUs(timeUs_t delayUs) {

  CHECK_INIT_US_();  /* MISRA 2012 Rule 15.5 deviation */
  
  sChrono chrono = {0};
  
  fChrono_Start(&chrono);
  
  while(fChrono_ElapsedUs(&chrono) < delayUs) {};
}

/** @} */ //End of FUNCTIONAL_API

/** @defgroup OBJECTIVE_API Objective API in the chrono module
 *  @brief These functions require an object of type sChrono. Users must declare one to use this API.
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

  CHECK_INIT_();  /* MISRA 2012 Rule 15.5 deviation */
  
  me->_startTick = fChrono_GetTick();
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
  
  CHECK_INIT_SEC_RET_((timeS_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeS_t)0);           /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  tick_t currentTick = fChrono_GetTick();
  
  return (timeS_t)((timeS_t)(ELAPSED_(currentTick, startTick)) / SecToTickCoef);
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
  
  CHECK_INIT_MS_RET_((timeMs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeMs_t)0);          /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  tick_t currentTick = fChrono_GetTick();
  
  return (timeMs_t)((timeMs_t)(ELAPSED_(currentTick, startTick)) / MsToTickCoef);
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
  
  CHECK_INIT_US_RET_((timeUs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeUs_t)0);          /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  tick_t currentTick = fChrono_GetTick();
  
  return (timeUs_t)((timeUs_t)(ELAPSED_(currentTick, startTick)) / UsToTickCoef);
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
  
  CHECK_INIT_SEC_RET_((timeS_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeS_t)0);           /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return (timeS_t)0;
  }
  
  tick_t currentTick = fChrono_GetTick();
  tick_t elapsedTick = ELAPSED_(currentTick, startTick);
  timeS_t elapsed = (timeS_t)((timeS_t)(elapsedTick) / SecToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeS_t)0;
  } else {
    return ((me->_timeout / SecToTickCoef) - elapsed);
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
  
  CHECK_INIT_MS_RET_((timeMs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeMs_t)0);         /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return (timeMs_t)0;
  }
  
  tick_t currentTick = fChrono_GetTick();
  tick_t elapsedTick = ELAPSED_(currentTick, startTick);
  timeMs_t elapsed = (timeMs_t)((timeMs_t)(elapsedTick) / MsToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeMs_t)0;
  } else {
    return ((me->_timeout / MsToTickCoef) - elapsed);
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
  
  CHECK_INIT_US_RET_((timeUs_t)0); /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeUs_t)0);         /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return (timeUs_t)0;
  }
  
  tick_t currentTick = fChrono_GetTick();
  tick_t elapsedTick = ELAPSED_(currentTick, startTick);
  timeUs_t elapsed = (timeUs_t)((timeUs_t)(elapsedTick) / UsToTickCoef);
  if(elapsedTick >= (me->_timeout)) {
    me->_isTimeout = TRUE;
    return (timeUs_t)0;
  } else {
    return ((me->_timeout / UsToTickCoef) - elapsed);
  }
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (seconds)
 */
void fChrono_StartTimeoutS(sChrono * const me, timeS_t timeout) {

  CHECK_INIT_SEC_(); /* MISRA 2012 Rule 15.5 deviation */
  //TODO: Assert for null me
  
  me->_timeout = (tick_t)(timeout * (timeS_t)SecToTickCoef);
  me->_isTimeout = FALSE;
  
  fChrono_Start(me);
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (milliseconds)
 */
void fChrono_StartTimeoutMs(sChrono * const me, timeMs_t timeout) {

  CHECK_INIT_MS_(); /* MISRA 2012 Rule 15.5 deviation */
  
  me->_timeout = (tick_t)(timeout * (timeMs_t)MsToTickCoef);
  me->_isTimeout = FALSE;
  
  fChrono_Start(me);
}

/**
 * @brief Starts the chrono object in timeout mode.
 * 
 * @param me Pointer to the chrono object
 * @param timeout Time length until the chrono object times out (microseconds)
 */
void fChrono_StartTimeoutUs(sChrono * const me, timeUs_t timeout) {

  CHECK_INIT_US_(); /* MISRA 2012 Rule 15.5 deviation */
  
  me->_timeout = (tick_t)(timeout * (timeUs_t)UsToTickCoef);
  me->_isTimeout = FALSE;
  
  fChrono_Start(me);
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
  
  CHECK_INIT_RET_(FALSE); /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_(FALSE);      /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t startTick = me->_startTick;
  
  if(me->_isTimeout) {
    return TRUE;
  }
  
  tick_t currentTick = fChrono_GetTick();
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
  
  CHECK_INIT_SEC_RET_((timeS_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeS_t)0);           /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = fChrono_GetTick();
  tick_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeS_t)(ELAPSED_(currentTick, startTick)) / SecToTickCoef);
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
  
  CHECK_INIT_MS_RET_((timeMs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeMs_t)0);          /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = fChrono_GetTick();
  tick_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeMs_t)(ELAPSED_(currentTick, startTick)) / MsToTickCoef);
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
  
  CHECK_INIT_US_RET_((timeUs_t)0);  /* MISRA 2012 Rule 15.5 deviation */
  CHECK_RUN_((timeUs_t)0);          /* MISRA 2012 Rule 15.5 deviation */
  
  tick_t currentTick = fChrono_GetTick();
  tick_t startTick = me->_startTick;
  me->_startTick = currentTick;
  
  return ((timeUs_t)(ELAPSED_(currentTick, startTick)) / UsToTickCoef);
}

/** @} */ //End of OBJECTIVE_API

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/


/************************ Copyright (c) 2024 FaraabinCo *****END OF FILE****/
