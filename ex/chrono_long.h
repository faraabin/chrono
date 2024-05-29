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
#ifndef __chrono_long_H
#define __chrono_long_H

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
/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/** @defgroup OBJECTIVE_API Objective API in the chrono_long module
 *  @brief These functions require an object of type sChrono. Users must declare one to use this API.
 *  @{
 */

/** @defgroup CHRONO_CMD
 *  @ingroup OBJECTIVE_API
 *  @brief These functions are used to start and stop the chrono object. fChronoLong_Start() puts object in run state for time measurement and
 *         fChronoLong_Stop() stops the chrono object.
 *  @{
 */

void fChronoLong_Start(sChrono * const me);
void fChronoLong_Stop(sChrono * const me);

/** @} */ //End of DELAY

/** @defgroup ELAPSED
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the elapsed time since the start of the measurement using fChronoLong_Start().
 *  @{
 */

timeS_t fChronoLong_ElapsedS(sChrono * const me);
timeMs_t fChronoLong_ElapsedMs(sChrono * const me);

/** @} */ //End of ELAPSED

/** @defgroup LEFT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions return the remaining time until the chrono object reaches its timeout.
 *  @{
 */
timeS_t fChronoLong_LeftS(sChrono * const me);
timeMs_t fChronoLong_LeftMs(sChrono * const me);

/** @} */ //End of LEFT

/** @defgroup TIMEOUT
 *  @ingroup FUNCTIONAL_API
 *  @brief These functions are used for measuring timeout.
 *  @{
 */

void fChronoLong_StartTimeoutS(sChrono * const me, timeS_t timeout);
void fChronoLong_StartTimeoutMs(sChrono * const me, timeMs_t timeout);
bool fChronoLong_IsTimeout(sChrono * const me);

/** @} */ //End of TIMEOUT

/** @} */ //End of OBJECTIVE_API

/* Exported variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __CHRONO_LONG_H */

/************************ Copyright (c) 2024 FaraabinCo *****END OF FILE****/
