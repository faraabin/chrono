/**
	******************************************************************************
	* @file           : chrono_test.c
	* @brief          : Chrono module unit tests.
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
	*/

/* Includes ------------------------------------------------------------------*/
#include "chrono_test.h"

#include "unity.h"
#include "../chrono.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ArraySize_(arr_)  (sizeof(arr_) / sizeof(arr_[0]))

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint32_t tickVal;

/* Private function prototypes -----------------------------------------------*/
static void Chrono_InitChronoWithNullPointer_InitReturnFalse(void);
static void Chrono_GetTickIsCalledWhileChronoIsNotInit_ReturnZero(void);
static void Chrono_GetTickIsCalled_ReturnCurrentTick(void);
static void Chrono_GetTickMsIsCalledWhileChronoIsNotInit_ReturnZero(void);
static void Chrono_GetTickMsIsCalled_ReturnCumulativeTime(void);
static void Chrono_GetTickTopValueIsCalledWhileChronoIsNotInit_ReturnZero(void);
static void Chrono_GetTickTopValueIsCalledWhenChronoIsInit_ReturnExpectedValues(void);
static void Chrono_GetTickToNsCoefIsCalledWhileChronoIsNotInit_ReturnZero(void);
static void Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues(void);
static void Chrono_GetTickMaxTimeMsIsCalledWhileChronoIsNotInit_ReturnZero(void);
static void Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues(void);
static void Chrono_TimeSpanSIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_TimeSpanSIsCalledWhenChronoIsInit_ReturnTimeLength(void);
static void Chrono_TimeSpanMsIsCalledWhenChronoIsInit_ReturnTimeLength(void);
static void Chrono_TimeSpanUsIsCalledWhenChronoIsInit_ReturnTimeLength(void);
static void Chrono_ElapsedSIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_ElapsedSIsCalledWhenChronoIsInit_ReturnElapsed(void);
static void Chrono_ElapsedMsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_ElapsedMsIsCalledWhenChronoIsInit_ReturnElapsed(void);
static void Chrono_ElapsedUsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_ElapsedUsIsCalledWhenChronoIsInit_ReturnElapsed(void);
static void Chrono_LeftSIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_LeftSIsCalledWhenNotInRun_ReturnZero(void);
static void Chrono_LeftSIsCalledWhenChronoIsInit_ReturnLeft(void);
static void Chrono_LeftMsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_LeftMsIsCalledWhenNotInRun_ReturnZero(void);
static void Chrono_LeftMsIsCalledWhenChronoIsInit_ReturnLeft(void);
static void Chrono_LeftUsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_LeftUsIsCalledWhenNotInRun_ReturnZero(void);
static void Chrono_LeftUsIsCalledWhenChronoIsInit_ReturnLeft(void);
static void Chrono_IsTimedoutSIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_IsTimedoutSIsCalledWhenNotInRun_ReturnZero(void);
static void Chrono_IsTimedoutSIsCalledWhenChronoIsInit_ReturnTimeoutStatus(void);
static void Chrono_IsTimedoutMsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_IsTimedoutMsIsCalledWhenNotInRun_ReturnZero(void);
static void Chrono_IsTimedoutMsIsCalledWhenChronoIsInit_ReturnTimeoutStatus(void);
static void Chrono_IsTimedoutUsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_IsTimedoutUsIsCalledWhenNotInRun_ReturnZero(void);
static void Chrono_IsTimedoutUsIsCalledWhenChronoIsInit_ReturnTimeoutStatus(void);
static void Chrono_IntervalSIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_IntervalSIsCalledWhenChronoIsNotRun_ReturnZero(void);
static void Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval(void);
static void Chrono_IntervalMsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_IntervalMsIsCalledWhenChronoIsNotRun_ReturnZero(void);
static void Chrono_IntervalMsIsCalledWhenChronoIsInit_ReturnInterval(void);
static void Chrono_IntervalUsIsCalledWhenChronoIsNotInit_ReturnZero(void);
static void Chrono_IntervalUsIsCalledWhenChronoIsNotRun_ReturnZero(void);
static void Chrono_IntervalUsIsCalledWhenChronoIsInit_ReturnInterval(void);
void setUp(void);
void tearDown(void);

/* Variables -----------------------------------------------------------------*/

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                          ##### Exported Functions #####                          ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief Function that runs all unit tests and returns the result.
 * 
 * @return int 
 */
int ChronoUnitTestsRun(void) {
  UNITY_BEGIN();

  RUN_TEST(Chrono_InitChronoWithNullPointer_InitReturnFalse);
  RUN_TEST(Chrono_GetTickIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_GetTickIsCalled_ReturnCurrentTick);
  RUN_TEST(Chrono_GetTickMsIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_GetTickMsIsCalled_ReturnCumulativeTime);
  RUN_TEST(Chrono_GetTickTopValueIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_GetTickTopValueIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST(Chrono_GetTickToNsCoefIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST(Chrono_GetTickMaxTimeMsIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST(Chrono_TimeSpanSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_TimeSpanSIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST(Chrono_TimeSpanMsIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST(Chrono_TimeSpanUsIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST(Chrono_ElapsedSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_ElapsedSIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST(Chrono_ElapsedMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_ElapsedMsIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST(Chrono_ElapsedUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_ElapsedUsIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST(Chrono_LeftSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_LeftSIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST(Chrono_LeftSIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST(Chrono_LeftMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_LeftMsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST(Chrono_LeftMsIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST(Chrono_LeftUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_LeftUsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST(Chrono_LeftUsIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST(Chrono_IsTimedoutSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_IsTimedoutSIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST(Chrono_IsTimedoutSIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST(Chrono_IsTimedoutMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_IsTimedoutMsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST(Chrono_IsTimedoutMsIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST(Chrono_IsTimedoutUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_IsTimedoutUsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST(Chrono_IsTimedoutUsIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
	RUN_TEST(Chrono_IntervalSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_IntervalSIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST(Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval);
	RUN_TEST(Chrono_IntervalMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_IntervalMsIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST(Chrono_IntervalMsIsCalledWhenChronoIsInit_ReturnInterval);
  RUN_TEST(Chrono_IntervalUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST(Chrono_IntervalUsIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST(Chrono_IntervalUsIsCalledWhenChronoIsInit_ReturnInterval);

  return UNITY_END();
}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief Unit test setup.
 * 
 */
void setUp(void) {
	
}

/**
 * @brief Unit test teardown.
 * 
 */
void tearDown(void) {
	
}

/**
 * @brief fChrono_Init() must return 1 if user pass null pointer for tickValue.
 * 
 */
static void Chrono_InitChronoWithNullPointer_InitReturnFalse(void) {
  uint8_t ret = 0;

  ret = fChrono_Init(0, 0, NULL);

  TEST_ASSERT_EQUAL(1, ret);
}

/**
 * @brief fChrono_GetTick() must return 0 when module is not initialized properly.
 * 
 */
static void Chrono_GetTickIsCalledWhileChronoIsNotInit_ReturnZero(void) {
  uint32_t ret = 0;

  fChrono_Init(0, 0, NULL);
  
  ret = fChrono_GetTick();

  TEST_ASSERT_EQUAL_UINT32(0, ret);
}

/**
 * @brief fChrono_GetTick() must return current tick
 * 
 */
static void Chrono_GetTickIsCalled_ReturnCurrentTick(void) {

  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);
	
	tickVal = 0;
	TEST_ASSERT_EQUAL_UINT32(tickVal, fChrono_GetTick());
	
	tickVal = 0xFFFFFFFF;
	TEST_ASSERT_EQUAL_UINT32(tickVal, fChrono_GetTick());
	
	tickVal = 0xFFFFFFFE;
	TEST_ASSERT_EQUAL_UINT32(tickVal, fChrono_GetTick());
	
	tickVal = 0x01;
	TEST_ASSERT_EQUAL_UINT32(tickVal, fChrono_GetTick());
}

/**
 * @brief fChrono_GetContinuousTickMs() must return 0 when module is not initialized properly.
 * 
 */
static void Chrono_GetTickMsIsCalledWhileChronoIsNotInit_ReturnZero(void) {
  
	uint64_t ret = 0xFFFFFFFFFFFFFFFF;

  fChrono_Init(0, 0, NULL);
  
  ret = fChrono_GetContinuousTickMs();

  TEST_ASSERT_EQUAL(0, ret);
}

/**
 * @brief fChrono_GetContinuousTickMs() must return cumulative time length.
 * 
 */
static void Chrono_GetTickMsIsCalled_ReturnCumulativeTime(void) {
  tickVal = 0;

  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);

  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickMs());

  tickVal = 1000;
  TEST_ASSERT_EQUAL_UINT64(1000, fChrono_GetContinuousTickMs());

  tickVal = 0xFFFFFFFF;
  TEST_ASSERT_EQUAL_UINT64(0xFFFFFFFF, fChrono_GetContinuousTickMs());

  tickVal = 1;
  TEST_ASSERT_EQUAL_UINT64(0xFFFFFFFF + 1, fChrono_GetContinuousTickMs());

  tickVal = 1000;
  TEST_ASSERT_EQUAL_UINT64(0xFFFFFFFF + 1000, fChrono_GetContinuousTickMs());

  tickVal = 0xFFFFFFFF;
  TEST_ASSERT_EQUAL_UINT64(0xFFFFFFFF * 2, fChrono_GetContinuousTickMs());

}

/**
 * @brief fChrono_GetTickTopValue() must return 0 when module is not initialized properly.
 * 
 */
static void Chrono_GetTickTopValueIsCalledWhileChronoIsNotInit_ReturnZero(void) {

  uint32_t topValues[] = {0, 0xFFFF0000, 0xFFFFFFFE, 0xFFFFFFFF, 0x10000000};
  uint32_t retValues[] = {0, 0, 0, 0, 0};
  uint32_t expectedValues[] = {0, 0, 0, 0, 0};

  for(int i = 0; i < ArraySize_(topValues); i++) {
		
		fChrono_Init(topValues[i], 0, NULL);
    retValues[i] = fChrono_GetTickTopValue();
  }

  TEST_ASSERT_EQUAL_UINT32_ARRAY(expectedValues, retValues, ArraySize_(retValues));
}

/**
 * @brief fChrono_GetTickTopValue() must return top value that user passed to init function.
 * 
 */
static void Chrono_GetTickTopValueIsCalledWhenChronoIsInit_ReturnExpectedValues(void) {

  uint32_t topValues[] = {0, 0xFFFF0000, 0xFFFFFFFE, 0xFFFFFFFF, 0x10000000};
  uint32_t retValues[] = {0, 0, 0, 0, 0};

  for(int i = 0; i < ArraySize_(topValues); i++) {

    fChrono_Init(topValues[i], 0, &tickVal);
  
    retValues[i] = fChrono_GetTickTopValue();
  }

  TEST_ASSERT_EQUAL_UINT32_ARRAY(topValues, retValues, ArraySize_(topValues));
}

/**
 * @brief fChrono_GetTickToNsCoef() must return 0 when module is not initialized properly.
 * 
 */
static void Chrono_GetTickToNsCoefIsCalledWhileChronoIsNotInit_ReturnZero(void) {

  uint32_t coefValues[] = {0, 1, 1000, 0xFFFFFFFF, 0x10000000};
  uint32_t retValues[] = {0, 0, 0, 0, 0};
  uint32_t expectedValues[] = {0, 0, 0, 0, 0};

  for(int i = 0; i < ArraySize_(coefValues); i++) {

    fChrono_Init(0, coefValues[i], NULL);
  
    retValues[i] = fChrono_GetTickToNsCoef();
  }

  TEST_ASSERT_EQUAL_UINT32_ARRAY(expectedValues, retValues, ArraySize_(retValues));
}

/**
 * @brief fChrono_GetTickToNsCoef() must return coefficient that user passed to init function.
 * 
 */
static void Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues(void) {

  uint32_t coefValues[] = {0, 1, 1000, 0xFFFFFFFF, 0x10000000};
  uint32_t retValues[] = {0, 0, 0, 0, 0};

  for(int i = 0; i < ArraySize_(coefValues); i++) {

    fChrono_Init(0, coefValues[i], &tickVal);
  
    retValues[i] = fChrono_GetTickToNsCoef();
  }

  TEST_ASSERT_EQUAL_UINT32_ARRAY(coefValues, retValues, ArraySize_(coefValues));
}

/**
 * @brief fChrono_GetMaxMeasurableTimeMs() must return 0 when module is not initialized properly.
 * 
 */
static void Chrono_GetTickMaxTimeMsIsCalledWhileChronoIsNotInit_ReturnZero(void) {

  uint32_t topValues[] = {0, 0xFFFF0000, 0xFFFFFFFE, 0xFFFFFFFF, 0x10000000};
  uint32_t coefValues[] = {0, 1, 1000, 0xFFFFFFFF, 0x10000000};

  for(int i = 0; i < ArraySize_(topValues); i++) {

    fChrono_Init(topValues[i], coefValues[i], NULL);
		
		uint64_t ret = fChrono_GetMaxMeasurableTimeMs();
  
    TEST_ASSERT_EQUAL(0, ret);
  }
}

/**
 * @brief fChrono_GetMaxMeasurableTimeMs() must return maximum measurable time length.
 * 
 */
static void Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues(void) {

  uint32_t topValues[] = {0, 0xFFFF0000, 0xFFFFFFFE, 0xFFFFFFFF, 0x10000000, 0xFFFFFFFF};
  uint32_t coefValues[] = {0, 1, 1000, 0xFFFFFFFF, 0x10000000, 0xFFFFFFFF};
  uint64_t retValues[] = {0, 0, 0, 0, 0, 0};
  uint64_t expectedValues[] = {0, 0, 0, 0, 0, 0};

  for(int i = 0; i < ArraySize_(topValues); i++) {

    fChrono_Init(topValues[i], coefValues[i], &tickVal);
  
    retValues[i] = fChrono_GetMaxMeasurableTimeMs();
    expectedValues[i] = (uint64_t)(topValues[i] * ((float)coefValues[i] / 1000000.0f));
  }
	
  TEST_ASSERT_EQUAL_UINT64_ARRAY(expectedValues, retValues, ArraySize_(retValues));
}

/**
 * @brief fChrono_TimeSpanS() must return zero when module is not initialized.
 * 
 */
static void Chrono_TimeSpanSIsCalledWhenChronoIsNotInit_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000, NULL);

  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0, 0));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0, 0xFFFFFFFE));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0xFFFFFFFE, 0));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0x10000000, 0xFFFFFFFE));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0xFFFFFFFE, 0x10000000));

}

/**
 * @brief fChrono_TimeSpanS() must return maximum measurable time length.
 * 
 */
static void Chrono_TimeSpanSIsCalledWhenChronoIsInit_ReturnTimeLength(void) {

	float ret = 0.0f;
  fChrono_Init(0xFFFFFFFF, 1000000000, &tickVal);

	ret = fChrono_TimeSpanS(0, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanS(0x10000000, 0x10000000);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanS(0xFFFFFFFE, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanS(0, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xFFFFFFFE), ret);
	
	ret = fChrono_TimeSpanS(0xFFFFFFFE, 0);
  TEST_ASSERT_EQUAL_FLOAT((float)(0x1), ret);
	
	ret = fChrono_TimeSpanS(0x10000000, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xEFFFFFFE), ret);
	
	ret = fChrono_TimeSpanS(0x10000000, 0x80000000);
  TEST_ASSERT_EQUAL_FLOAT((float)(0x70000000), ret);
	
	ret = fChrono_TimeSpanS(0xFFFFFFFE, 0x10000000);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xFFFFFFFF - 0xFFFFFFFE + 0x10000000), ret);

}

/**
 * @brief fChrono_TimeSpanMs() must return maximum measurable time length.
 * 
 */
static void Chrono_TimeSpanMsIsCalledWhenChronoIsInit_ReturnTimeLength(void) {

	float ret = 0.0f;
  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);
	
	ret = fChrono_TimeSpanS(0, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanMs(0x10000000, 0x10000000);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanMs(0xFFFFFFFE, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanMs(0, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xFFFFFFFE), ret);
	
	ret = fChrono_TimeSpanMs(0xFFFFFFFE, 0);
  TEST_ASSERT_EQUAL_FLOAT((float)(0x1), ret);
	
	ret = fChrono_TimeSpanMs(0x10000000, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xEFFFFFFE), ret);
	
	ret = fChrono_TimeSpanMs(0x10000000, 0x80000000);
  TEST_ASSERT_EQUAL_FLOAT((float)(0x70000000), ret);
	
	ret = fChrono_TimeSpanMs(0xFFFFFFFE, 0x10000000);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xFFFFFFFF - 0xFFFFFFFE + 0x10000000), ret);

}

/**
 * @brief fChrono_TimeSpanUs() must return maximum measurable time length.
 * 
 */
static void Chrono_TimeSpanUsIsCalledWhenChronoIsInit_ReturnTimeLength(void) {

	float ret = 0.0f;
  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);
	
	ret = fChrono_TimeSpanS(0, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanUs(0x10000000, 0x10000000);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanUs(0xFFFFFFFE, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT(0, ret);
	
	ret = fChrono_TimeSpanUs(0, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xFFFFFFFE), ret);
	
	ret = fChrono_TimeSpanUs(0xFFFFFFFE, 0);
  TEST_ASSERT_EQUAL_FLOAT((float)(0x1), ret);
	
	ret = fChrono_TimeSpanUs(0x10000000, 0xFFFFFFFE);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xEFFFFFFE), ret);
	
	ret = fChrono_TimeSpanUs(0x10000000, 0x80000000);
  TEST_ASSERT_EQUAL_FLOAT((float)(0x70000000), ret);
	
	ret = fChrono_TimeSpanUs(0xFFFFFFFE, 0x10000000);
  TEST_ASSERT_EQUAL_FLOAT((float)(0xFFFFFFFF - 0xFFFFFFFE + 0x10000000), ret);

}

/**
 * @brief fChrono_ElapsedS() must return zero when module is not initialized.
 * 
 */
static void Chrono_ElapsedSIsCalledWhenChronoIsNotInit_ReturnZero(void) {
  
  fChrono_Init(0xFFFFFFFF, 1000000000, NULL);

  sChrono testChrono;
  fChrono_Start(&testChrono);
	
	uint32_t elapsed = fChrono_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

}

/**
 * @brief fChrono_ElapsedS() must return elapsed time.
 * 
 */
static void Chrono_ElapsedSIsCalledWhenChronoIsInit_ReturnElapsed(void) {
	
	uint32_t elapsed = 0;

  fChrono_Init(0xFFFFFFFF, 1000000000, &tickVal);

  sChrono testChrono;

  tickVal = 0;
  fChrono_Start(&testChrono);
	elapsed = fChrono_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

  tickVal = 0;
  fChrono_Start(&testChrono);
  tickVal = 1000;
	elapsed = fChrono_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1000, elapsed);
	
	tickVal = 0;
  fChrono_Start(&testChrono);
  tickVal = 0xFFFFFFFF;
	elapsed = fChrono_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFF, elapsed);

  tickVal = 1000;
  fChrono_Start(&testChrono);
  tickVal = 500;
	elapsed = fChrono_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32((0xFFFFFE0B), elapsed);

}

/**
 * @brief fChrono_ElapsedMs() must return zero when module is not initialized.
 * 
 */
static void Chrono_ElapsedMsIsCalledWhenChronoIsNotInit_ReturnZero(void) {
  
  fChrono_Init(0xFFFFFFFF, 1000000, NULL);

  sChrono testChrono;
  fChrono_Start(&testChrono);
	
	uint32_t elapsed = fChrono_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

}

/**
 * @brief fChrono_ElapsedMs() must return elapsed time.
 * 
 */
static void Chrono_ElapsedMsIsCalledWhenChronoIsInit_ReturnElapsed(void) {

  uint32_t elapsed = 0;

  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);

  sChrono testChrono;

  tickVal = 0;
  fChrono_Start(&testChrono);
	elapsed = fChrono_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

  tickVal = 0;
  fChrono_Start(&testChrono);
  tickVal = 1000;
	elapsed = fChrono_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1000, elapsed);
	
	tickVal = 0;
  fChrono_Start(&testChrono);
  tickVal = 0xFFFFFFFF;
	elapsed = fChrono_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFF, elapsed);

  tickVal = 1000;
  fChrono_Start(&testChrono);
  tickVal = 500;
	elapsed = fChrono_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32((0xFFFFFE0B), elapsed);

}

/**
 * @brief fChrono_ElapsedUs() must return zero when module is not initialized.
 * 
 */
static void Chrono_ElapsedUsIsCalledWhenChronoIsNotInit_ReturnZero(void) {
  
  fChrono_Init(0xFFFFFFFF, 1000, NULL);

  sChrono testChrono;
  fChrono_Start(&testChrono);
	
	uint32_t elapsed = fChrono_ElapsedUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

}

/**
 * @brief fChrono_ElapsedUs() must return elapsed time.
 * 
 */
static void Chrono_ElapsedUsIsCalledWhenChronoIsInit_ReturnElapsed(void) {

  uint32_t elapsed = 0;

  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);

  sChrono testChrono;

  tickVal = 0;
  fChrono_Start(&testChrono);
	elapsed = fChrono_ElapsedUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

  tickVal = 0;
  fChrono_Start(&testChrono);
  tickVal = 1000;
	elapsed = fChrono_ElapsedUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1000, elapsed);
	
	tickVal = 0;
  fChrono_Start(&testChrono);
  tickVal = 0xFFFFFFFF;
	elapsed = fChrono_ElapsedUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFF, elapsed);

  tickVal = 1000;
  fChrono_Start(&testChrono);
  tickVal = 500;
	elapsed = fChrono_ElapsedUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32((0xFFFFFE0B), elapsed);

}

/**
 * @brief fChrono_LeftS() returns zero if module is not initialized.
 * 
 */
static void Chrono_LeftSIsCalledWhenChronoIsNotInit_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000000, NULL);

  sChrono testChrono;
  fChrono_StartTimeoutS(&testChrono, 1000);
	
	uint32_t left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftS() returns zero if module is not running.
 * 
 */
static void Chrono_LeftSIsCalledWhenNotInRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000000, NULL);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
  
	uint32_t left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftS() returns time left until timeout.
 * 
 */
static void Chrono_LeftSIsCalledWhenChronoIsInit_ReturnLeft(void) {
	
	uint32_t left = 0;
  fChrono_Init(0xFFFFFFFF, 1000000000, &tickVal);

  sChrono testChrono;

  tickVal = 0;
  fChrono_StartTimeoutS(&testChrono, 1000);
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1000, left);

  tickVal = 500;
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(500, left);

  tickVal = 999;
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1, left);

  tickVal = 1000;
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

  tickVal = 1001;
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

  tickVal = 5000;
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);
	
	tickVal = 0;
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);
	
	tickVal = 500;
	left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftMs() returns zero if module is not initialized.
 * 
 */
static void Chrono_LeftMsIsCalledWhenChronoIsNotInit_ReturnZero(void) {
  
  fChrono_Init(0xFFFFFFFF, 1000000, NULL);

  sChrono testChrono;
  fChrono_StartTimeoutMs(&testChrono, 1000);
	
	uint32_t left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftMs() returns zero if module is not running.
 * 
 */
static void Chrono_LeftMsIsCalledWhenNotInRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000, NULL);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
	
	uint32_t left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftMs() returns time left until timeout.
 * 
 */
static void Chrono_LeftMsIsCalledWhenChronoIsInit_ReturnLeft(void) {

  uint32_t left = 0;
  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);

  sChrono testChrono;

  tickVal = 0;
  fChrono_StartTimeoutMs(&testChrono, 1000);
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1000, left);

  tickVal = 500;
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(500, left);

  tickVal = 999;
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1, left);

  tickVal = 1000;
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

  tickVal = 1001;
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

  tickVal = 5000;
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);
	
	tickVal = 0;
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);
	
	tickVal = 500;
	left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);
}

/**
 * @brief fChrono_LeftUs() returns zero if module is not initialized.
 * 
 */
static void Chrono_LeftUsIsCalledWhenChronoIsNotInit_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000, NULL);

  sChrono testChrono;
  fChrono_StartTimeoutUs(&testChrono, 1000);
	
	uint32_t left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftS() returns zero if module is not running.
 * 
 */
static void Chrono_LeftUsIsCalledWhenNotInRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000, NULL);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
	
	uint32_t left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftUs() returns time left until timeout.
 * 
 */
static void Chrono_LeftUsIsCalledWhenChronoIsInit_ReturnLeft(void) {

  uint32_t left = 0;
  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);

  sChrono testChrono;

  tickVal = 0;
  fChrono_StartTimeoutUs(&testChrono, 1000);
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1000, left);

  tickVal = 500;
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(500, left);

  tickVal = 999;
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1, left);

  tickVal = 1000;
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

  tickVal = 1001;
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

  tickVal = 5000;
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);
	
	tickVal = 0;
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);
	
	tickVal = 500;
	left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_IsTimeout() returns false when module is not initialized.
 * 
 */
static void Chrono_IsTimedoutSIsCalledWhenChronoIsNotInit_ReturnZero(void) {
  
  fChrono_Init(0xFFFFFFFF, 1000000000, NULL);

  sChrono testChrono;
  fChrono_StartTimeoutS(&testChrono, 1000);
	
	bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

}

/**
 * @brief fChrono_IsTimeoutS() returns false when module is not running.
 * 
 */
static void Chrono_IsTimedoutSIsCalledWhenNotInRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000000, &tickVal);

  sChrono testChrono;
	fChrono_Stop(&testChrono);
	
	bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);
	
}

/**
 * @brief fChrono_IsTimeoutS() returns timeout status.
 * 
 */
static void Chrono_IsTimedoutSIsCalledWhenChronoIsInit_ReturnTimeoutStatus(void) {
	
	bool isTimeout = false;
  fChrono_Init(0xFFFFFFFF, 1000000000, &tickVal);

  sChrono testChrono;
  tickVal = 0;

  fChrono_StartTimeoutS(&testChrono, 1000);
	
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 500;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 999;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 1000;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);

  tickVal = 1001;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);

  tickVal = 5000;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
	
	tickVal = 0;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
	
	tickVal = 500;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);

}

/**
 * @brief fChrono_IsTimeout() returns false when module is not initialized.
 * 
 */
static void Chrono_IsTimedoutMsIsCalledWhenChronoIsNotInit_ReturnZero(void) {
  
  fChrono_Init(0xFFFFFFFF, 1000000, NULL);

  sChrono testChrono;
  fChrono_StartTimeoutMs(&testChrono, 1000);
	
	bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

}

/**
 * @brief fChrono_IsTimeout() returns false when module is not running.
 * 
 */
static void Chrono_IsTimedoutMsIsCalledWhenNotInRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);

  sChrono testChrono;
	fChrono_Stop(&testChrono);
	
	bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);
	
}

/**
 * @brief fChrono_IsTimeout() returns timeout status.
 * 
 */
static void Chrono_IsTimedoutMsIsCalledWhenChronoIsInit_ReturnTimeoutStatus(void) {

  bool isTimeout = false;
  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);

  sChrono testChrono;
  tickVal = 0;

  fChrono_StartTimeoutMs(&testChrono, 1000);
	
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 500;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 999;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 1000;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);

  tickVal = 1001;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);

  tickVal = 5000;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
	
	tickVal = 0;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
	
	tickVal = 500;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
  
}

/**
 * @brief fChrono_IsTimeout() returns false when module is not initialized.
 * 
 */
static void Chrono_IsTimedoutUsIsCalledWhenChronoIsNotInit_ReturnZero(void) {
  
  fChrono_Init(0xFFFFFFFF, 1000, NULL);

  sChrono testChrono;
  fChrono_StartTimeoutUs(&testChrono, 1000);
	
	bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

}

/**
 * @brief fChrono_IsTimeout() returns false when module is not running.
 * 
 */
static void Chrono_IsTimedoutUsIsCalledWhenNotInRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);

  sChrono testChrono;
	fChrono_Stop(&testChrono);
	
	bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);
	
}

/**
 * @brief fChrono_IsTimeout() returns timeout status.
 * 
 */
static void Chrono_IsTimedoutUsIsCalledWhenChronoIsInit_ReturnTimeoutStatus(void) {

  bool isTimeout = false;
  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);

  sChrono testChrono;
  tickVal = 0;

  fChrono_StartTimeoutUs(&testChrono, 1000);
	
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 500;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 999;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

  tickVal = 1000;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);

  tickVal = 1001;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);

  tickVal = 5000;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
	
	tickVal = 0;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
	
	tickVal = 500;
	isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(true, isTimeout);
  
}

/**
 * @brief fChrono_fIntervalS() returns zero when module is not initialized.
 * 
 */
static void Chrono_IntervalSIsCalledWhenChronoIsNotInit_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000000, NULL);

  sChrono testChrono;
  tickVal = 0;

  fChrono_Start(&testChrono);
	
	float interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

}

/**
 * @brief fChrono_fIntervalS() returns zero when module is not running.
 * 
 */
static void Chrono_IntervalSIsCalledWhenChronoIsNotRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000000, &tickVal);

  sChrono testChrono;
  tickVal = 0;
	fChrono_Stop(&testChrono);

	float interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

}

/**
 * @brief fChrono_fIntervalS() returns interval.
 * 
 */
static void Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval(void) {
	
	float interval = 0.0f;
  fChrono_Init(0xFFFFFFFF, 1000000000, &tickVal);

  sChrono testChrono;
  tickVal = 0;

  fChrono_Start(&testChrono);
	interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 1000;
	interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

  tickVal = 10000;
	interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(10000, interval);

  tickVal = 20000;
  fChrono_Start(&testChrono);
	interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 21000;
	interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

}

/**
 * @brief fChrono_IntervalMs() returns zero when module is not initialized.
 * 
 */
static void Chrono_IntervalMsIsCalledWhenChronoIsNotInit_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000, NULL);

  sChrono testChrono;
  tickVal = 0;

  fChrono_Start(&testChrono);
	
	float interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

}

/**
 * @brief fChrono_IntervalMs() returns zero when module is not running.
 * 
 */
static void Chrono_IntervalMsIsCalledWhenChronoIsNotRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);

  sChrono testChrono;
  tickVal = 0;
	fChrono_Stop(&testChrono);

	float interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

}

/**
 * @brief fChrono_IntervalMs() returns interval.
 * 
 */
static void Chrono_IntervalMsIsCalledWhenChronoIsInit_ReturnInterval(void) {
	
	float interval = 0.0f;
  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);

  sChrono testChrono;
  tickVal = 0;

  fChrono_Start(&testChrono);
	interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 1000;
	interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

  tickVal = 10000;
	interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(10000, interval);

  tickVal = 20000;
  fChrono_Start(&testChrono);
	interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 21000;
	interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

}

/**
 * @brief fChrono_IntervalUs() returns zero when module is not initialized.
 * 
 */
static void Chrono_IntervalUsIsCalledWhenChronoIsNotInit_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000, NULL);

  sChrono testChrono;
  tickVal = 0;

  fChrono_Start(&testChrono);
	
	float interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

}

/**
 * @brief fChrono_IntervalUs() returns zero when module is not running.
 * 
 */
static void Chrono_IntervalUsIsCalledWhenChronoIsNotRun_ReturnZero(void) {

  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);

  sChrono testChrono;
  tickVal = 0;
	fChrono_Stop(&testChrono);

	float interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

}

/**
 * @brief fChrono_IntervalUs() returns interval.
 * 
 */
static void Chrono_IntervalUsIsCalledWhenChronoIsInit_ReturnInterval(void) {
	
	float interval = 0.0f;
  fChrono_Init(0xFFFFFFFF, 1000, &tickVal);

  sChrono testChrono;
  tickVal = 0;

  fChrono_Start(&testChrono);
	interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 1000;
	interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

  tickVal = 10000;
	interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(10000, interval);

  tickVal = 20000;
  fChrono_Start(&testChrono);
	interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 21000;
	interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

}

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
