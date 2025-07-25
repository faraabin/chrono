/**
 ******************************************************************************
 *  @file         chrono_fn.c
 *  @brief        ...
 *  @author       ...
 *  @version      0.0.0.1
 *  @date         yyyy/mm/dd
 *  @copyright    COPYRIGHT(c) 2018 MicroEmbeds
 ******************************************************************************
 * @attention
 *
 *
 *
 ******************************************************************************
 * DESCRIPTION:
 *
 ******************************************************************************
 * HOW TO USE:
 *
 ******************************************************************************
 @verbatim
 @endverbatim
*/

/* Includes ------------------------------------------------------------------*/
#include "chrono_fn.h"

#include "chrono.h"

#ifdef FARAABIN_ENABLE

/** @addtogroup Group_Name
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
FARAABIN_FUNCTION_GROUP_TYPE_DEF_(void*);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/*
===============================================================================
                       ##### Private Functios #####
===============================================================================*/
/**
  * @brief
  * @note  
  * @param  
  * @retval
*/
FARAABIN_FUNCTION_LOOP_(ChronoFn, Tick,
"type cmd help here") {
	
	FARAABIN_Function_LoopStartMs_(500) {
    
    FARAABIN_PrintfToFunction_("\nTick = %10lu", (unsigned long)fChrono_GetTick());
    
    FARAABIN_Function_LoopEnd_();
  }
	
	FARAABIN_FUNCTION_END_();
}

/**
  * @brief
  * @note
  * @param
  * @retval
*/
FARAABIN_FUNCTION_(ChronoFn, DelayUs,
"type cmd help here") {
	
	unsigned long value;
	FARAABIN_Function_GetParam_(1, "%lu", &value);

	FARAABIN_PrintfToFunction_("\nDelay Start...");
	fFaraabin_FlushBuffer();
	
	fChrono_DelayUs(value);
	
	FARAABIN_PrintfToFunction_("End");
	fFaraabin_FlushBuffer();

	FARAABIN_FUNCTION_END_();
}

/**
  * @brief
  * @note
  * @param
  * @retval
*/
FARAABIN_FUNCTION_(ChronoFn, DelayMs,
"type cmd help here") {
	
	unsigned long value;
	FARAABIN_Function_GetParam_(1, "%lu", &value);

	FARAABIN_PrintfToFunction_("\nDelay Start...");
	fFaraabin_FlushBuffer();
	
	fChrono_DelayMs(value);
	
	FARAABIN_PrintfToFunction_("End");
	fFaraabin_FlushBuffer();
	
	FARAABIN_FUNCTION_END_();
}

/**
  * @brief
  * @note
  * @param
  * @retval
*/
FARAABIN_FUNCTION_(ChronoFn, DelayS,
"type cmd help here") {
	
	unsigned long value;
	
	FARAABIN_Function_GetParam_(1, "%lu", &value);

	FARAABIN_PrintfToFunction_("\nDelay Start...");
	fFaraabin_FlushBuffer();
	
	fChrono_DelayS(value);
	
	FARAABIN_PrintfToFunction_("End");
	fFaraabin_FlushBuffer();
	
	FARAABIN_FUNCTION_END_();
}

/*
===============================================================================
                       ##### Exported Function #####
===============================================================================*/
/**
  * @brief
  * @note  
  * @param  
  * @retval
*/
FARAABIN_FUNCTION_GROUP_(ChronoFn,
"type cmd help here") {
  
	FARAABIN_FUNCTION_GROUP_ADD_(ChronoFn, Tick);
  
	FARAABIN_FUNCTION_GROUP_ADD_(ChronoFn, DelayUs);
	FARAABIN_FUNCTION_GROUP_ADD_(ChronoFn, DelayMs);
	FARAABIN_FUNCTION_GROUP_ADD_(ChronoFn, DelayS);
}

#endif

/**End of Group_Name
  * @}
  */

/************************ (C) COPYRIGHT MicroEmbeds **********END OF FILE******/
