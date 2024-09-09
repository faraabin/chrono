/**
  ******************************************************************************
  * @file           : chrono_private.h
  * @brief          :
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 FaraabinCo.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component
  * 
  * https://faraabinco.ir/
  * https://github.com/FaraabinCo
  *
  ******************************************************************************
  * @verbatim
  * @endverbatim
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHRONO_PRIVATE_H
#define CHRONO_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private defines -----------------------------------------------------------*/
/** @defgroup PRIVATE_DEFINES
 * @note These defines are used internally.
 *  @{
 */
#define TICK_TYPE_VARIABLE    (1U)
#define TICK_TYPE_FUNCTION    (2U)

#define TICK_COUNTERMODE_UP   (0U)
#define TICK_COUNTERMODE_DOWN (1U)
/** @} */ //End of PRIVATE_DEFINES

#ifdef __cplusplus
}
#endif

#endif /* CHRONO_PRIVATE_H */

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
