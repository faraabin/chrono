/**
  ******************************************************************************
  * @file           : chrono_config.h
  * @brief          : Chrono test module configuration file.
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

/* Private defines -----------------------------------------------------------*/
#define TEST_GROUP_1  (1U)
#define TEST_GROUP_2  (2U)
#define TEST_GROUP_3  (3U)
#define TEST_GROUP_4  (4U)
#define TEST_GROUP_5  (5U)
#define TEST_GROUP_6  (6U)
#define TEST_GROUP_7  (7U)
#define TEST_GROUP_8  (8U)
#define TEST_GROUP_9  (9U)
#define TEST_GROUP_10 (10U)

#define CHRONO_TESTS_DISABLED (0U)
#define CHRONO_TESTS_ENABLED  (1U)

/* Exported defines ----------------------------------------------------------*/
#define CHRONO_TEST_STATUS  CHRONO_TESTS_ENABLED

#if(CHRONO_TEST_STATUS == CHRONO_TESTS_ENABLED)
#define CHRONO_TEST_GROUP   TEST_GROUP_8
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_1)
/** @defgroup TEST_GROUP_1_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (0xFFFFFFFFU)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1000U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_UP

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_1_CONFIG
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_2)
/** @defgroup TEST_GROUP_2_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (0xFFFFFFFEU)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1000U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_UP

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_2_CONFIG
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_3)
/** @defgroup TEST_GROUP_3_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (0U)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1000U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_UP

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_3_CONFIG
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_4)
/** @defgroup TEST_GROUP_4_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (1000U)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (0U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_UP

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_4_CONFIG
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_5)
/** @defgroup TEST_GROUP_5_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (0xFFFFFFFFU)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1000000000U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_UP

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_5_CONFIG
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_6)
/** @defgroup TEST_GROUP_6_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (0xFFFFFFFFU)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1000000U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_UP

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_6_CONFIG
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_7)
/** @defgroup TEST_GROUP_7_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (0xFFFFFFFFU)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_UP

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_7_CONFIG
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_8)
/** @defgroup TEST_GROUP_8_CONFIG
 *  @{
 */

#define CHRONO_TICK_TYPE                TICK_TYPE_VARIABLE
#define CHRONO_TICK_TOP_VALUE           (0xFFFFFFFFU)
#define CHRONO_TICK_TO_NANOSECOND_COEF  (1000000000U)
#define CHRONO_TICK_COUNTERMODE         TICK_COUNTERMODE_DOWN

typedef uint32_t tick_t;
typedef uint32_t timeS_t;
typedef uint32_t timeMs_t;
typedef uint32_t timeUs_t;

/** @} */ //End of TEST_GROUP_8_CONFIG
#endif

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_CONFIG_H */

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
