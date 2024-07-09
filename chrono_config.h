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

#include <stdint.h>

/* Exported defines ----------------------------------------------------------*/
#define TICK_TYPE_VARIABLE  (1)
#define TICK_TYPE_FUNCTION  (2)

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
 * @brief User typedef for measurements in chrono module
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
