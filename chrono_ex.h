/**
  ******************************************************************************
  * @file           : chrono_ex.h
  * @brief          : Chrono extension macros for runtime scaler, wait and etc.
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
  - It manages the rate of execution of code blocks during runtime.
  - All macros are based on chrono module.

  # How to use
  This section explains how to use the runtime_scaler module in your application.

  ## Configuration
  Before using these macros, follow these steps:
  - Clone the chrono module from this repository. https://github.com/faraabin/chrono.git
  - Ensure that the chrono module directory is defined in your project's include path.
  - Simply use the provided macros in your application to scale the rate of execution.
  
  ## Using macros
  There are two types of macros available for users:
  1- Run Part of Code Just Once:
    - Declare a RUN_ONCE variable using RUN_ONCE_DEF_().
    - Reset the state of this variable using RUN_ONCE_RESET_().
    - Create a code block with RUN_ONCE_(name_of_run_once_variable) and RUN_END_, and place your code inside this block.
    - Whenever the program reaches this code block, it executes the code only once.

  @code
  #include "runtime_scaler.h"

  //Declare a RUN_ONCE variable
  static RUN_ONCE_DEF_(test_once);

  int main(void)
  {
    //Reset RUN_ONCE variable status
    RUN_ONCE_RESET_(test_once);

    while(TRUE)
    {
      RUN_ONCE_(test_once)
      {
        //User code to run once.
        TestFunction();

        RUN_END_;
      }

      //Perform other parts of application
      //...

    }
  }
  @endcode

  2- Manage Rate of Execution:
    2-1- Declare RUN_EVERY statically:
      - Mark the start of the code block using one of the following macros:
        - RUN_EVERY_US_(): For microseconds intervals
        - RUN_EVERY_MS_(): For milliseconds intervals
        - RUN_EVERY_S_(): For seconds intervals
        - RUN_EVERY_US_FORCE_(): For microseconds intervals in forced mode
        - RUN_EVERY_MS_FORCE_(): For milliseconds intervals in forced mode
        - RUN_EVERY_S_FORCE_(): For seconds intervals in force mode
      - Specify the end of the code block using RUN_END_.
      - Place your application code inside this block to manage its execution relative to the main runtime.
    2-2- Declare RUN_EVERY as an object:
      - In order to use RUN_EVERY as an object, declare a chrono object of type sChrono.
      - Mark the start of the code block using one of the following macros:
        - RUN_EVERY_US_OBJ_(): For microseconds intervals
        - RUN_EVERY_MS_OBJ_(): For milliseconds intervals
        - RUN_EVERY_S_OBJ_(): For seconds intervals
        - RUN_EVERY_US_OBJ_FORCE_(): For microseconds intervals in forced mode
        - RUN_EVERY_MS_OBJ_FORCE_(): For milliseconds intervals in forced mode
        - RUN_EVERY_S_OBJ_FORCE_(): For seconds intervals in force mode
      - Use RUN_END_ to mark the end of the code block.
      - Place your application code inside this block to manage its execution relative to the main runtime.
  
  Below is a code example of how to use this module with static macros.
  
  @code
  #include "runtime_scaler.h"

  int main(void)
  {

    while(TRUE)
    {
      //User code to run every cycle
      //...

      RUN_EVERY_US_(microsecond, 1000)
      {
        //User code to run every 1000 microseconds.
        //...

        RUN_END_;
      }

      RUN_EVERY_US_(millisecond, 10)
      {
        //User code to run every 10 milliseconds.
        //...

        RUN_END_;
      }

      RUN_EVERY_US_(second, 5)
      {
        //User code to run every 5 seconds.
        //...

        RUN_END_;
      }
    }
  }
  @endcode

  Refer to the comments above each macro for more information on usage.
  @mainpage
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHRONO_EX_H
#define CHRONO_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "chrono.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup RUN_ONCE Run once macros
 *  @{
 */

/**
 * @brief Declares a RUN_ONCE variable.
 * 
 * @note User must declare this variable before using RUN_ONCE_().
 * 
 * @param name_ Given name to the RUN_ONCE variable.
 */
#define RUN_ONCE_DEF_(name_) bool_t __run_once_##name_##__ = TRUE

/**
 * @brief Resets the state of RUN_ONCE.
 * 
 * @note To allow RUN_ONCE to run again, the user needs to reset it.
 * 
 * @param name_ Name of the RUN_ONCE variable.
 */
#define RUN_ONCE_RESET_(name_) __run_once_##name_##__ = TRUE

/**
 * @brief This macro is used to mark the start of the code block for using RUN_ONCE.
 * 
 * @note User must use RUN_END_ to mark the end of the code block.
 * 
 * @param name_ Name of the RUN_ONCE variable.
 */
#define RUN_ONCE_(name_) \
  if(__run_once_##name_##__ == TRUE) {\
    __run_once_##name_##__ = FALSE;

/** @} */ //End of RUN_FIRST

/** @defgroup RUN_EVERY Run every macros
 *  @{
 */

/**
 * @brief Base macro for marking the start of a code block that runs at regular intervals.
 * 
 * @note It is essential to use the RUN_END_ macro to conclude the code block.
 * @note DO NOT USE this macro directly in your application. Instead, utilize appropriate wrappers such as RUN_EVERY_US_OBJ_(), RUN_EVERY_US_OBJ_FORCE_(), RUN_EVERY_US_(), RUN_EVERY_US_FORCE_(), etc.
 * 
 * @param name_ Name of the code block.
 * @param interval_ The time interval for executing the code block.
 * @param chrono_ A pointer to the chrono object.
 * @param ts_ The time sample of the RUN_EVERY object. Use Us for microseconds, Ms for milliseconds, and S for seconds.
 * @param force_ If TRUE, RUN_EVERY will execute the code block immediately; if FALSE, it will only initialize its chrono object and run the code block in the next interval.
 */
#define RUN_EVERY_BASE_(name_, interval_, chrono_, ts_, force_) \
    if((chrono_)->_run == FALSE) {\
      fChrono_StartTimeout##ts_((chrono_), force_ ? 0 : (interval_));\
    }\
    if((fChrono_IsTimeout((chrono_)) == TRUE) && (interval_ != 0)) {\
        time##ts_##_t name_##_elapsed##ts_ = fChrono_Elapsed##ts_((chrono_));\
        (void)(name_##_elapsed##ts_);\
        fChrono_StartTimeout##ts_((chrono_), (interval_));

/** @defgroup RUN_EVERY_US Macros with microsecond time scale
  * @ingroup RUN_EVERY
  * @{
  */

/**
 * @brief Objective RUN_EVERY macro with microsecond time interval. Marks the start of the code block.
 * 
 * @note Use this macro when you have already created a chrono object.
 *       Since RUN_EVERY_US_() creates its chrono object statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_US_() because all instances of that class will share a static chrono object.
 *       Instead of using RUN_EVERY_US_(), utilize RUN_EVERY_US_OBJ_() to pass the name of the chrono object.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. It only initializes the chrono object and puts it in the run state.
 *       User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param chrono_ Pointer to the chrono object.
 * @param intervalUs_ The time interval for running the code block in microseconds.
 * 
 */
#define RUN_EVERY_US_OBJ_(name_, chrono_, intervalUs_) RUN_EVERY_BASE_(name_, intervalUs_, chrono_, Us, FALSE)

/**
 * @brief Objective RUN_EVERY macro with microsecond time interval in FORCE mode. Marks the start of the code block.
 * 
 * @note Use this macro when you have already created a chrono object.
 *       Since RUN_EVERY_US_() creates its chrono object statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_US_() because all instances of that class will share a static chrono object.
 *       Instead of using RUN_EVERY_US_(), utilize RUN_EVERY_US_OBJ_() to pass the name of the chrono object.
 * 
 * @note This macro initializes the chrono object and puts it in the run state. It also forces RUN_EVERY to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param chrono_ Pointer to the chrono object.
 * @param intervalUs_ The time interval for running the code block in microseconds.
 * 
 */
#define RUN_EVERY_US_OBJ_FORCE_(name_, chrono_, intervalUs_) RUN_EVERY_BASE_(name_, intervalUs_, chrono_, Us, TRUE)

/**
 * @brief Static RUN_EVERY macro with microsecond time interval. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to deal with chrono object instantiation.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. It only initializes the chrono object and puts it in the run state.
 *       User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param intervalUs_ The time interval for running the code block in microseconds.
 * 
 */
#define RUN_EVERY_US_(name_, intervalUs_) \
    static sChrono __run_every_us_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
    RUN_EVERY_US_OBJ_(name_, &__run_every_us_##name_##__, intervalUs_)

/**
 * @brief Static RUN_EVERY macro with microsecond time interval in FORCE mode. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to deal with chrono object instantiation.
 * 
 * @note This macro initializes the chrono object and puts it in the run state. It also forces RUN_EVERY to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param intervalUs_ The time interval for running the code block in microseconds.
 * 
 */
#define RUN_EVERY_US_FORCE_(name_, intervalUs_) \
    static sChrono __run_every_us_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
    RUN_EVERY_US_OBJ_FORCE_(name_, &__run_every_us_##name_##__, intervalUs_)

/** @} */ //End of RUN_EVERY_US

/** @defgroup RUN_EVERY_MS Macros with millisecond time scale
  * @ingroup RUN_EVERY
  * @{
  */

/**
 * @brief Objective RUN_EVERY macro with millisecond time interval. Marks the start of the code block.
 * 
 * @note Use this macro when you have already created a chrono object.
 *       Since RUN_EVERY_MS_() creates its chrono object statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_MS_() because all instances of that class will share a static chrono object.
 *       Instead of using RUN_EVERY_MS_(), utilize RUN_EVERY_MS_OBJ_() to pass the name of the chrono object.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. It only initializes the chrono object and puts it in the run state.
 *       User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param chrono_ Pointer to the chrono object.
 * @param intervalUs_ The time interval for running the code block in milliseconds.
 * 
 */
#define RUN_EVERY_MS_OBJ_(name_, chrono_, intervalMs_) RUN_EVERY_BASE_(name_, intervalMs_, chrono_, Ms, FALSE)

/**
 * @brief Objective RUN_EVERY macro with millisecond time interval in FORCE mode. Marks the start of the code block.
 * 
 * @note Use this macro when you have already created a chrono object.
 *       Since RUN_EVERY_MS_() creates its chrono object statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_MS_() because all instances of that class will share a static chrono object.
 *       Instead of using RUN_EVERY_MS_(), utilize RUN_EVERY_MS_OBJ_() to pass the name of the chrono object.
 * 
 * @note This macro initializes the chrono object and puts it in the run state. It also forces RUN_EVERY to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param chrono_ Pointer to the chrono object.
 * @param intervalUs_ The time interval for running the code block in milliseconds.
 * 
 */
#define RUN_EVERY_MS_OBJ_FORCE_(name_, chrono_, intervalMs_) RUN_EVERY_BASE_(name_, intervalMs_, chrono_, Ms, TRUE)

/**
 * @brief Static RUN_EVERY macro with millisecond time interval. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to deal with chrono object instantiation.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. It only initializes the chrono object and puts it in the run state.
 *       User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param intervalUs_ The time interval for running the code block in milliseconds.
 * 
 */
#define RUN_EVERY_MS_(name_, intervalMs_) \
    static sChrono __run_every_ms_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
    RUN_EVERY_MS_OBJ_(name_, &__run_every_ms_##name_##__, intervalMs_)

/**
 * @brief Static RUN_EVERY macro with millisecond time interval in FORCE mode. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to deal with chrono object instantiation.
 * 
 * @note This macro initializes the chrono object and puts it in the run state. It also forces RUN_EVERY to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param intervalUs_ The time interval for running the code block in milliseconds.
 * 
 */
#define RUN_EVERY_MS_FORCE_(name_, intervalMs_) \
    static sChrono __run_every_ms_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
    RUN_EVERY_MS_OBJ_FORCE_(name_, &__run_every_ms_##name_##__, intervalMs_)

/** @} */ //End of RUN_EVERY_MS

/** @defgroup RUN_EVERY_S Macros with second time scale
  * @ingroup RUN_EVERY
  * @{
  */

/**
 * @brief Objective RUN_EVERY macro with second time interval. Marks the start of the code block.
 * 
 * @note Use this macro when you have already created a chrono object.
 *       Since RUN_EVERY_S_() creates its chrono object statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_S_() because all instances of that class will share a static chrono object.
 *       Instead of using RUN_EVERY_S_(), utilize RUN_EVERY_S__OBJ_() to pass the name of the chrono object.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. It only initializes the chrono object and puts it in the run state.
 *       User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param chrono_ Pointer to the chrono object.
 * @param intervalUs_ The time interval for running the code block in seconds.
 * 
 */
#define RUN_EVERY_S_OBJ_(name_, chrono_, intervalS_) RUN_EVERY_BASE_(name_, intervalS_, chrono_, S, FALSE)

/**
 * @brief Objective RUN_EVERY macro with second time interval in FORCE mode. Marks the start of the code block.
 * 
 * @note Use this macro when you have already created a chrono object.
 *       Since RUN_EVERY_S_() creates its chrono object statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_S_() because all instances of that class will share a static chrono object.
 *       Instead of using RUN_EVERY_S_(), utilize RUN_EVERY_S_OBJ_() to pass the name of the chrono object.
 * 
 * @note This macro initializes the chrono object and puts it in the run state. It also forces RUN_EVERY to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param chrono_ Pointer to the chrono object.
 * @param intervalUs_ The time interval for running the code block in seconds.
 * 
 */
#define RUN_EVERY_S_OBJ_FORCE_(name_, chrono_, intervalS_) RUN_EVERY_BASE_(name_, intervalS_, chrono_, S, TRUE)

/**
 * @brief Static RUN_EVERY macro with second time interval. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to deal with chrono object instantiation.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. It only initializes the chrono object and puts it in the run state.
 *       User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param intervalUs_ The time interval for running the code block in seconds.
 * 
 */
#define RUN_EVERY_S_(name_, intervalS_) \
    static sChrono __run_every_s_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
    RUN_EVERY_S_OBJ_(name_, &__run_every_s_##name_##__, intervalS_)

/**
 * @brief Static RUN_EVERY macro with second time interval in FORCE mode. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to deal with chrono object instantiation.
 * 
 * @note This macro initializes the chrono object and puts it in the run state. It also forces RUN_EVERY to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ Name of the code block.
 * @param intervalUs_ The time interval for running the code block in seconds.
 * 
 */
#define RUN_EVERY_S_FORCE_(name_, intervalS_) \
    static sChrono __run_every_s_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
    RUN_EVERY_S_OBJ_FORCE_(name_, &__run_every_s_##name_##__, intervalS_)

/** @} */ //End of RUN_EVERY_S

/** @defgroup RUN_EVERY_QTY Quantity based macros
  * @ingroup RUN_EVERY
  * @{
  */

/**
 * @brief Base macro for marking the start of a code block that runs every specified quantity of times.
 * 
 * @note It is essential to use the RUN_END_ macro to conclude the code block.
 * @note DO NOT USE this macro directly in your application. Instead, utilize appropriate wrappers such as RUN_EVERY_QTY_OBJ_(), RUN_EVERY_QTY_OBJ_FORCE_(), RUN_EVERY_QTY_(), RUN_EVERY_QTY_FORCE_(), etc.
 * 
 * @param qty_ The value that specifies how many times the code block will be executed.
 * @param cnt_ The name of the counter variable that holds how many times the program has reached the code block.
 * @param force_ If TRUE, RUN_EVERY_QTY will execute the code block immediately; if FALSE, it will only initialize its chrono object and run the code block in the next interval.
 */
#define RUN_EVERY_QTY_BASE_(qty_, cnt_, force_) \
  (cnt_)++;\
  if((!force_) && ((cnt_) == 1)) {\
    (cnt_)++;\
  }\
  if((((cnt_) >= (qty_ + 1)) || (force_ && (cnt_ == 1))) && (qty_ != 0)) {\
    (cnt_) = 1;

/**
 * @brief Objective RUN_EVERY_QTY macro. Marks the start of the code block.
 * 
 * @note Use this macro when you want to declare the counter variable (cnt_) yourself.
 *       Since RUN_EVERY_QTY_() creates its counter statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_QTY_() because all instances of that class will share a static counter.
 *       Instead of using RUN_EVERY_QTY_(), utilize RUN_EVERY_QTY_OBJ_() to pass the name of the counter variable.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param qty_ The value that specifies how many times the code block will be executed.
 * @param cnt_ The name of the counter variable.
 */
#define RUN_EVERY_QTY_OBJ_(qty_, cnt_) RUN_EVERY_QTY_BASE_(qty_, cnt_, FALSE)

/**
 * @brief Objective RUN_EVERY_QTY macro in FORCE mode. Marks the start of the code block.
 * 
 * @note Use this macro when you want to declare the counter variable (cnt_) yourself.
 *       Since RUN_EVERY_QTY_() creates its counter statically, users can't use it when an object-oriented approach is used to write the code.
 *       For example, in a method of a class, users can't use RUN_EVERY_QTY_() because all instances of that class will share a static counter.
 *       Instead of using RUN_EVERY_QTY_(), utilize RUN_EVERY_QTY_OBJ_() to pass the name of the counter variable.
 * 
 * @note This macro initializes the RUN_EVERY_QTY and also forces it to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param qty_ The value that specifies how many times the code block will be executed.
 * @param cnt_ The name of the counter variable.
 */
#define RUN_EVERY_QTY_OBJ_FORCE_(qty_, cnt_) RUN_EVERY_QTY_BASE_(qty_, cnt_, TRUE)

/**
 * @brief Static RUN_EVERY_QTY macro. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to create a counter variable. The macro itself will declare a counter statically.
 * 
 * @note This macro does not execute user code during the first encounter of the code block. User code will be executed in the subsequent intervals.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ The name of the code block.
 * @param qty_ The value that specifies how many times the code block will be executed.
 */
#define RUN_EVERY_QTY_(name_, qty_) \
  static uint32_t __run_every_qty_##name_##__ = 0;\
  RUN_EVERY_QTY_OBJ_(qty_, __run_every_qty_##name_##__)

/**
 * @brief Static RUN_EVERY_QTY macro. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to create a counter variable. The macro itself will declare a counter statically.
 * 
 * @note This macro initializes the RUN_EVERY_QTY and also forces it to run the user code during the first encounter.
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ The name of the code block.
 * @param qty_ The value that specifies how many times the code block will be executed.
 */
#define RUN_EVERY_QTY_FORCE_(name_, qty_) \
  static uint32_t __run_every_qty_##name_##__ = 0;\
  RUN_EVERY_QTY_OBJ_FORCE_(qty_, __run_every_qty_##name_##__)

/** @} */ //End of RUN_EVERY_QTY

/** @} */ //End of RUN_EVERY

/**
 * @brief Static RUN_AFTER_QTY macro. Marks the start of the code block.
 * 
 * @note Use this macro when you don't want to run code first n run.
 * 
 * @note It is essential to mark the end of the code block using the RUN_END_ macro.
 * 
 * @param name_ The name of the code block.
 * @param qty_ The value that specifies how many times the code block will be neglected.
 */
#define RUN_AFTER_QTY_(name_, qty_) \
  static uint32_t __run_after_qty_##name_##__ = 0;\
  static bool _is_run_after_qty_##name_##__ = FALSE;\
  if(!_is_run_after_qty_##name_##__) {\
    __run_after_qty_##name_##__++;\
    if(__run_after_qty_##name_##__++ >= (qty_)) {\
      __run_after_qty_##name_##__ = 0;\
      _is_run_after_qty_##name_##__ = TRUE;\
    }\
  } else {

#define RUN_AFTER_QTY_RESET_(name_) \
  _is_run_after_qty_##name_##__ = FALSE

#define RUN_END_ }

/**
 * @brief This macro wait code for a condition.
 *
 * @note Use this macro when you want to wait for condition, but should 
 *       have a timeout for prevent lock in while loop.
 */
#define WAIT_FOR_WITH_TIMEOUT_US_(name_, condition_, timeoutUs_, isTimeout_) \
  (isTimeout_) = FALSE; \
  static sChrono __wait_for_us_##name_##__ = {FALSE, 0, 0, 0, FALSE}; \
  fChrono_StartTimeoutUs(&(__wait_for_us_##name_##__), timeoutUs_); \
  while(!(condition_)) { \
    if(fChrono_IsTimeout(&(__wait_for_us_##name_##__))) { \
      (isTimeout_) = TRUE; \
      break; \
    } \
  }

/**
 * @brief This macro wait code for a condition.
 *
 * @note Use this macro when you want to wait for condition, but should 
 *       have a timeout for prevent lock in while loop.
 */
#define WAIT_FOR_WITH_TIMEOUT_MS_(name_, condition_, timeoutMs_, isTimeout_) \
  (isTimeout_) = FALSE; \
  static sChrono __wait_for_ms_##name_##__ = {FALSE, 0, 0, 0, FALSE}; \
  fChrono_StartTimeoutMs(&(__wait_for_ms_##name_##__), timeoutMs_); \
  while(!(condition_)) { \
    if(fChrono_IsTimeout(&(__wait_for_ms_##name_##__))) { \
      (isTimeout_) = TRUE; \
      break; \
    } \
  }

/**
 * @brief This macro wait code for a condition.
 *
 * @note Use this macro when you want to wait for condition, but should 
 *       have a timeout for prevent lock in while loop.
 */
#define WAIT_FOR_WITH_TIMEOUT_S_(name_, condition_, timeoutS_, isTimeout_) \
  (isTimeout_) = FALSE; \
  static sChrono __wait_for_s_##name_##__ = {FALSE, 0, 0, 0, FALSE}; \
  fChrono_StartTimeoutS(&(__wait_for_s_##name_##__), timeoutS_); \
  while(!(condition_)) { \
    if(fChrono_IsTimeout(&(__wait_for_s_##name_##__))) { \
      (isTimeout_) = TRUE; \
      break; \
    } \
  }

/**
 * @brief These macros set the out_pulse_ for a predefined duration_, if a rising edge
 *        is detected on the condition_ value.
 * 
 * @note To reset the pulser, condition_ must be FALSE for at least one execution period of the 
 *       PULSE_ONCE_FOR_XS_ macro.
 *
 */
#define PULSE_ONCE_FOR_US_(name_, condition_, duration_, out_pulse_)  \
  static bool_t pulse_latch_##name_##__ = FALSE;\
  static sChrono pulse_timer_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
  if(!(pulse_latch_##name_##__) && ((condition_) == TRUE)) {\
    pulse_latch_##name_##__ = TRUE;\
    fChrono_StartTimeoutUs(&(pulse_timer_##name_##__), duration_);\
    out_pulse_ = 1;\
  }\
  if(pulse_latch_##name_##__) {\
    if(fChrono_IsTimeout(&(pulse_timer_##name_##__))) {\
      out_pulse_ = 0;\
      fChrono_Stop(&(pulse_timer_##name_##__));\
    } else {\
      if((condition_) == FALSE) {\
        pulse_latch_##name_##__ = FALSE;\
      }\
    }\
  }

#define PULSE_ONCE_FOR_MS_(name_, condition_, duration_, out_pulse_)  \
  static bool_t pulse_latch_##name_##__ = FALSE;\
  static sChrono pulse_timer_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
  if(!(pulse_latch_##name_##__) && ((condition_) == TRUE)) {\
    pulse_latch_##name_##__ = TRUE;\
    fChrono_StartTimeoutMs(&(pulse_timer_##name_##__), duration_);\
    out_pulse_ = 1;\
  }\
  if(pulse_latch_##name_##__) {\
    if(fChrono_IsTimeout(&(pulse_timer_##name_##__))) {\
      out_pulse_ = 0;\
      fChrono_Stop(&(pulse_timer_##name_##__));\
    } else {\
      if((condition_) == FALSE) {\
        pulse_latch_##name_##__ = FALSE;\
      }\
    }\
  }

#define PULSE_ONCE_FOR_S_(name_, condition_, duration_, out_pulse_)  \
  static bool_t pulse_latch_##name_##__ = FALSE;\
  static sChrono pulse_timer_##name_##__ = {FALSE, 0, 0, 0, FALSE};\
  if(!(pulse_latch_##name_##__) && ((condition_) == TRUE)) {\
    pulse_latch_##name_##__ = TRUE;\
    fChrono_StartTimeoutS(&(pulse_timer_##name_##__), duration_);\
    out_pulse_ = 1;\
  }\
  if(pulse_latch_##name_##__) {\
    if(fChrono_IsTimeout(&(pulse_timer_##name_##__))) {\
      out_pulse_ = 0;\
      fChrono_Stop(&(pulse_timer_##name_##__));\
    } else {\
      if((condition_) == FALSE) {\
        pulse_latch_##name_##__ = FALSE;\
      }\
    }\
  }

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_EX_H */

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
