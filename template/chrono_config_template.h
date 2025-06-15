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

#include <stdint.h>

/* Exported defines ----------------------------------------------------------*/
/**
  * @brief Selects the method to read the current tick: from a memory address (hardware tick) or by calling a function (software tick).
  *
  * @note As an example if a timer generates the tick and reading its counter register is possible, set CHRONO_TICK_TYPE to TICK_TYPE_VARIABLE.
  *
  * @note If reading tick value from the an address in memory is not possible, and a function will return current tick value,
  *       set CHRONO_TICK_TYPE to TICK_TYPE_FUNCTION. In this case, chrono module will call this function whenever it needs to get the tick.
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
#define CHRONO_TICK_TO_NANOSECOND_COEF  (100U)

/**
  * @brief Select the counter mode of the tick generator.
  *
  * @note If the counter starts from zero to CHRONO_TICK_TOP_VALUE then define it as TICK_COUNTERMODE_UP.
  * 
  * @note if it counts downward from CHRONO_TICK_TOP_VALUE to zero then define it as TICK_COUNTERMODE_DOWN.
  */
#define CHRONO_TICK_COUNTERMODE TICK_COUNTERMODE_UP

/**
 * @brief Comment for disable assertion for increase performance
 * 
 */
#define CHRONO_USE_FULL_ASSERT

/**
 * @brief User typedefs for measurements in chrono module.
 * 
 * @note Modify these types based on your application.
 * 
 */
typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_CONFIG_H */

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
