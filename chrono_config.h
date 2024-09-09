/**
  ******************************************************************************
  * @file           : chrono_config.h
  * @brief          : Chrono module configuration file.
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHRONO_CONFIG_H
#define CHRONO_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "chrono_private.h"
#include "test\chrono_test_private.h"

#include <stdint.h>

#if (CHRONO_TEST_STATUS == CHRONO_TESTS_DISABLED)
/* Exported defines ----------------------------------------------------------*/
/**
  * @brief Select either reading current tick from an address in memory or by calling a function.
  *
  * @note In cases that a timer is used to generate tick and reading its counter register is possible to get current tick,
  *       set CHRONO_TICK_TYPE to TICK_TYPE_VARIABLE.
  *
  * @note In cases that user wants to generate the tick and reading its value is not possible by dereferencing the tick address,
  *       value of current tick can be read via calling a function.
  */
#define CHRONO_TICK_TYPE  TICK_TYPE_VARIABLE

/**
 * @brief Set the top value that the tick generator can count.
 * 
 * @note For example, if you have a timer that counts upto 0xFFFFFFFFU, CHRONO_TICK_TOP_VALUE should be this value.
 * 
 */
#define CHRONO_TICK_TOP_VALUE   (0xFFFFFFFFU)

/**
 * @brief Set the coefficient for converting one tick of the tick generator to nanoseconds.
 * 
 * @note For example, if you have a timer that counts with 1MHz frequency, CHRONO_TICK_TO_NANOSECOND_COEF should be 1000.
 * 
 */
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1000U)

/**
  * @brief Select the counter mode of the tick generator. Possible modes are TICK_COUNTERMODE_UP if the counter starts from zero to CHRONO_TICK_TOP_VALUE
  *        and TICK_COUNTERMODE_DOWN if it counts downward from CHRONO_TICK_TOP_VALUE to zero.
  *
  */
#define CHRONO_TICK_COUNTERMODE TICK_COUNTERMODE_UP

/**
 * @brief User typedef for measurements in chrono module
 * 
 * @note Modify these types based on your application.
 * 
 */
typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

#endif /* CHRONO_TEST_STATUS */

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_CONFIG_H */

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
