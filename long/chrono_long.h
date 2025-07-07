/**
  ******************************************************************************
  * @file           : chrono_long.h
  * @brief          : ChronoLong module header file.
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
  @verbatim
  
  For more information on how to use this module, please refer to the implementation
  of the module in chrono_long.c file.

  @endverbatim
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHRONO_LONG_H
#define CHRONO_LONG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "chrono.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct {
  
  bool_t _run;            /*!< Holds the RUN state of the chronoLong object.
                             When this parameter is false, the chronoLong is in stop mode, and time measurement functions will return 0.
                             This parameter is private and is set by the chronoLong API. Users must not change its value. */

  uint64_t _startTimeMs;  /*!< Holds the start time of the chronoLong in milliseconds.
                             This parameter is private and is set by the chronoLong API. Users must not change its value. */
  
  uint64_t _timeoutMs;    /*!< Holds the timeout length.
                             This parameter is private and is set by the chrono API. Users must not change its value. */
  
  bool_t _isTimeout;      /*!< Holds the timeout state of the chronoLong object.
                             This parameter is private and is set by the chrono API. Users must not change its value. */
  
}sChronoLong;

/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/** @defgroup OBJECTIVE_API Objective API in the chrono_long module
 *  @brief These functions require an object of type sChronoLong. Users must declare one to use this API.
 *  @{
 */

/** @defgroup CHRONO_CMD
 *  @ingroup OBJECTIVE_API
 *  @brief These functions are used to start and stop the chrono object. fChronoLong_Start() puts object in run state for time measurement and
 *         fChronoLong_Stop() stops the chronoLong object.
 *  @{
 */

void fChronoLong_Start(sChronoLong * const me);
void fChronoLong_Stop(sChronoLong * const me);

/** @} */ //End of DELAY

/** @defgroup ELAPSED
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the elapsed time since the start of the measurement using fChronoLong_Start().
 *  @{
 */

uint64_t fChronoLong_ElapsedS(sChronoLong const * const me);
uint64_t fChronoLong_ElapsedMs(sChronoLong const * const me);

/** @} */ //End of ELAPSED

/** @defgroup LEFT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the remaining time until the chronoLong object reaches its timeout.
 *  @{
 */
uint64_t fChronoLong_LeftS(sChronoLong * const me);
uint64_t fChronoLong_LeftMs(sChronoLong * const me);

/** @} */ //End of LEFT

/** @defgroup TIMEOUT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions are used for measuring timeout.
 *  @{
 */

void fChronoLong_StartTimeoutS(sChronoLong * const me, uint64_t timeout);
void fChronoLong_StartTimeoutMs(sChronoLong * const me, uint64_t timeout);
bool_t fChronoLong_IsTimeout(sChronoLong * const me);

/** @} */ //End of TIMEOUT

/** @} */ //End of OBJECTIVE_API

/* Exported variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_LONG_H */

/************************ Copyright (c) 2024 FaraabinCo *****END OF FILE****/
