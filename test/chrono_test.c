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
#include "chrono_config.h"

#include "..\chrono.h"
#include "..\ex\chrono_long.h"

#include "unity_fixture.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ArraySize_(arr_)  (sizeof(arr_) / sizeof(arr_[0]))

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint32_t tickVal;
static volatile tick_t *pTick;

/* Private function prototypes -----------------------------------------------*/
static void RunTests(void);
static void OneTimeSetup(void);
static void OneTimeTearDown(void);

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
int fChrono_UnitTests_Run(int argc, const char* argv[]) {
  
  int ret = 0;
  
  OneTimeSetup();
  
#if (CHRONO_TEST_STATUS == CHRONO_TESTS_ENABLED)
  ret = UnityMain(argc, argv, RunTests);
#endif
  
  OneTimeTearDown();
  
  return ret;
}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                              ##### Unit Tests #####                              ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/
#if(CHRONO_TEST_GROUP == TEST_GROUP_1)
/** @defgroup TEST_GROUP_1
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_1);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_1) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_1) {

}

/**
 * @brief fChrono_Init() must return 1 if user pass null pointer for tickValue.
 * 
 */
TEST(GROUP_1, Chrono_InitChronoWithNullPointer_InitReturnFalse) {
  uint8_t ret = 0;

  ret = fChrono_Init(NULL);

  TEST_ASSERT_EQUAL(CHRONO_ERROR_TICK_PTR_ERROR, ret);
}

/**
 * @brief fChrono_GetTick() must return 0 when module is not initialized properly.
 * 
 */
TEST(GROUP_1, Chrono_GetTickIsCalledWhileChronoIsNotInit_ReturnZero) {
  uint32_t ret = 0;

  fChrono_Init(NULL);
  
  ret = fChrono_GetTick();

  TEST_ASSERT_EQUAL_UINT32(0, ret);
}

/**
 * @brief fChrono_GetTick() must return current tick
 * 
 */
TEST(GROUP_1, Chrono_GetTickIsCalled_ReturnCurrentTick) {

  fChrono_Init(&tickVal);
  
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
TEST(GROUP_1, Chrono_GetTickMsIsCalledWhileChronoIsNotInit_ReturnZero) {
  
  uint64_t ret = 0xFFFFFFFFFFFFFFFF;

  fChrono_Init(NULL);
  
  ret = fChrono_GetContinuousTickMs();

  TEST_ASSERT_EQUAL(0, ret);
}

/**
 * @brief fChrono_GetContinuousTickMs() must return cumulative time length.
 * 
 */
TEST(GROUP_1, Chrono_GetTickMsIsCalled_ReturnCumulativeTime) {
  
  tickVal = 0;
  
  fChrono_Init(&tickVal);
  
  uint64_t contTick = 0;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(0, contTick);

  tickVal = 1000;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(1, contTick);

  tickVal = 0xFFFFFFFF;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(0x418937, contTick);

  tickVal = 1;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(0x418937, contTick);

  tickVal = 1000;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(0x418937, contTick);

  tickVal = 0xFFFFFFFF;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(0x83126D, contTick);

}

/**
 * @brief fChrono_GetTickTopValue() must return 0 when module is not initialized properly.
 * 
 */
TEST(GROUP_1, Chrono_GetTickTopValueIsCalledWhileChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

  uint32_t ret = fChrono_GetTickTopValue();

  TEST_ASSERT_EQUAL_UINT32(0U, ret);

}

/**
 * @brief fChrono_GetTickTopValue() must return top value that user passed to init function.
 * 
 */
TEST(GROUP_1, Chrono_GetTickTopValueIsCalledWhenChronoIsInit_ReturnExpectedValues) {

  fChrono_Init(&tickVal);

  uint32_t ret = fChrono_GetTickTopValue();

  TEST_ASSERT_EQUAL_UINT32(CHRONO_TICK_TOP_VALUE, ret);

}

/**
 * @brief fChrono_GetTickToNsCoef() must return 0 when module is not initialized properly.
 * 
 */
TEST(GROUP_1, Chrono_GetTickToNsCoefIsCalledWhileChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

  uint32_t ret = fChrono_GetTickToNsCoef();

  TEST_ASSERT_EQUAL_UINT32(0U, ret);

}

/**
 * @brief fChrono_GetTickToNsCoef() must return coefficient that user passed to init function.
 * 
 */
TEST(GROUP_1, Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues) {

  fChrono_Init(&tickVal);

  uint32_t ret = fChrono_GetTickToNsCoef();

  TEST_ASSERT_EQUAL_UINT32(CHRONO_TICK_TO_NANOSECOND_COEF, ret);

}

/**
 * @brief fChrono_GetMaxMeasurableTimeMs() must return 0 when module is not initialized properly.
 * 
 */
TEST(GROUP_1, Chrono_GetTickMaxTimeMsIsCalledWhileChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

  uint64_t ret = fChrono_GetMaxMeasurableTimeMs();

  TEST_ASSERT_EQUAL(0, ret);

}

/**
 * @brief fChrono_GetMaxMeasurableTimeMs() must return maximum measurable time length.
 * 
 */
TEST(GROUP_1, Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues) {

  fChrono_Init(&tickVal);

  uint64_t ret = fChrono_GetMaxMeasurableTimeMs();

  TEST_ASSERT_EQUAL(fChrono_GetTickTopValue() / (1000000U / CHRONO_TICK_TO_NANOSECOND_COEF), ret);

}

/**
 * @brief fChrono_TimeSpanS() must return zero when module is not initialized.
 * 
 */
TEST(GROUP_1, Chrono_TimeSpanSIsCalledWhenChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0, 0));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0, 0xFFFFFFFE));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0xFFFFFFFE, 0));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0x10000000, 0xFFFFFFFE));
  TEST_ASSERT_EQUAL_FLOAT(0, fChrono_TimeSpanS(0xFFFFFFFE, 0x10000000));

}

/**
 * @brief fChrono_TimeSpanUs() must return maximum measurable time length.
 * 
 */
TEST(GROUP_1, Chrono_TimeSpanUsIsCalledWhenChronoIsInit_ReturnTimeLength) {

  float ret = 0.0f;
  fChrono_Init(&tickVal);
  
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
 * @brief fChrono_ElapsedUs() must return zero when module is not initialized.
 * 
 */
TEST(GROUP_1, Chrono_ElapsedUsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_Start(&testChrono);
  
  uint32_t elapsed = fChrono_ElapsedUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

}

/**
 * @brief fChrono_ElapsedUs() must return elapsed time.
 * 
 */
TEST(GROUP_1, Chrono_ElapsedUsIsCalledWhenChronoIsInit_ReturnElapsed) {

  uint32_t elapsed = 0;

  fChrono_Init(&tickVal);

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
 * @brief fChrono_LeftUs() returns zero if module is not initialized.
 * 
 */
TEST(GROUP_1, Chrono_LeftUsIsCalledWhenChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_StartTimeoutUs(&testChrono, 1000);
  
  uint32_t left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftS() returns zero if module is not running.
 * 
 */
TEST(GROUP_1, Chrono_LeftUsIsCalledWhenNotInRun_ReturnZero) {

  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
  
  uint32_t left = fChrono_LeftUs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftUs() returns time left until timeout.
 * 
 */
TEST(GROUP_1, Chrono_LeftUsIsCalledWhenChronoIsInit_ReturnLeft) {

  uint32_t left = 0;
  fChrono_Init(&tickVal);

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
TEST(GROUP_1, Chrono_IsTimedoutUsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_StartTimeoutUs(&testChrono, 1000);
  
  bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

}

/**
 * @brief fChrono_IsTimeout() returns false when module is not running.
 * 
 */
TEST(GROUP_1, Chrono_IsTimedoutUsIsCalledWhenNotInRun_ReturnZero) {

  fChrono_Init(&tickVal);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
  
  bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);
  
}

/**
 * @brief fChrono_IsTimeout() returns timeout status.
 * 
 */
TEST(GROUP_1, Chrono_IsTimedoutUsIsCalledWhenChronoIsInit_ReturnTimeoutStatus) {

  bool isTimeout = false;
  fChrono_Init(&tickVal);

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
 * @brief fChrono_IntervalUs() returns zero when module is not initialized.
 * 
 */
TEST(GROUP_1, Chrono_IntervalUsIsCalledWhenChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

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
TEST(GROUP_1, Chrono_IntervalUsIsCalledWhenChronoIsNotRun_ReturnZero) {

  fChrono_Init(&tickVal);

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
TEST(GROUP_1, Chrono_IntervalUsIsCalledWhenChronoIsInit_ReturnInterval) {
  
  float interval = 0.0f;
  fChrono_Init(&tickVal);

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
  TEST_ASSERT_EQUAL_FLOAT(9000, interval);

  tickVal = 20000;
  fChrono_Start(&testChrono);
  interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 21000;
  interval = fChrono_IntervalUs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_1) {

  RUN_TEST_CASE(GROUP_1, Chrono_InitChronoWithNullPointer_InitReturnFalse);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickIsCalled_ReturnCurrentTick);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickMsIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickMsIsCalled_ReturnCumulativeTime);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickTopValueIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickTopValueIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickToNsCoefIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickMaxTimeMsIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST_CASE(GROUP_1, Chrono_TimeSpanSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_TimeSpanUsIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST_CASE(GROUP_1, Chrono_ElapsedUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_ElapsedUsIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST_CASE(GROUP_1, Chrono_LeftUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_LeftUsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_LeftUsIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST_CASE(GROUP_1, Chrono_IsTimedoutUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_IsTimedoutUsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_IsTimedoutUsIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST_CASE(GROUP_1, Chrono_IntervalUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_IntervalUsIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST_CASE(GROUP_1, Chrono_IntervalUsIsCalledWhenChronoIsInit_ReturnInterval);

}

/** @} */ //End of TEST_GROUP_1
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_2)
/** @defgroup TEST_GROUP_2
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_2);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_2) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_2) {

}

/**
 * @brief fChrono_GetTickToNsCoef() must return coefficient that user passed to init function.
 * 
 */
TEST(GROUP_2, Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues) {

  fChrono_Init(&tickVal);

  uint32_t ret = fChrono_GetTickToNsCoef();

  TEST_ASSERT_EQUAL_UINT32(CHRONO_TICK_TO_NANOSECOND_COEF, ret);

}

/**
 * @brief fChrono_GetMaxMeasurableTimeMs() must return maximum measurable time length.
 * 
 */
TEST(GROUP_2, Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues) {

  fChrono_Init(&tickVal);

  uint64_t ret = fChrono_GetMaxMeasurableTimeMs();

  TEST_ASSERT_EQUAL(fChrono_GetTickTopValue() / (1000000U / CHRONO_TICK_TO_NANOSECOND_COEF), ret);

}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_2) {

  RUN_TEST_CASE(GROUP_2, Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST_CASE(GROUP_2, Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues);

}

/** @} */ //End of TEST_GROUP_2
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_3)
/** @defgroup TEST_GROUP_3
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_3);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_3) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_3) {

}

/**
 * @brief fChrono_Init() must return 1 if user pass zero as tickTopValue.
 * 
 */
TEST(GROUP_3, Chrono_InitChronoWithZeroTickTopValue_InitReturnFalse) {
  uint8_t ret = 0;

  ret = fChrono_Init(&tickVal);

  TEST_ASSERT_EQUAL(CHRONO_ERROR_TICK_TOP_ZERO, ret);
}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_3) {

  RUN_TEST_CASE(GROUP_3, Chrono_InitChronoWithZeroTickTopValue_InitReturnFalse);

}

/** @} */ //End of TEST_GROUP_3
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_4)
/** @defgroup TEST_GROUP_4
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_4);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_4) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_4) {

}

/**
 * @brief fChrono_Init() must return 1 if user pass zero as tickToNsCoef.
 * 
 */
TEST(GROUP_4, Chrono_InitChronoWithZeroTickToNsCoef_InitReturnFalse) {
  uint8_t ret = 0;

  ret = fChrono_Init(&tickVal);

  TEST_ASSERT_EQUAL(CHRONO_ERROR_TICK_TO_NS_ZERO, ret);
}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_4) {

  RUN_TEST_CASE(GROUP_4, Chrono_InitChronoWithZeroTickToNsCoef_InitReturnFalse);

}

/** @} */ //End of TEST_GROUP_4
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_5)
/** @defgroup TEST_GROUP_5
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_5);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_5) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_5) {

}

/**
 * @brief fChrono_TimeSpanS() must return maximum measurable time length.
 * 
 */
TEST(GROUP_5, Chrono_TimeSpanSIsCalledWhenChronoIsInit_ReturnTimeLength) {

  float ret = 0.0f;
  fChrono_Init(&tickVal);

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
 * @brief fChrono_ElapsedS() must return zero when module is not initialized.
 * 
 */
TEST(GROUP_5, Chrono_ElapsedSIsCalledWhenChronoIsNotInit_ReturnZero) {
  
  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_Start(&testChrono);
  
  uint32_t elapsed = fChrono_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

}

/**
 * @brief fChrono_ElapsedS() must return elapsed time.
 * 
 */
TEST(GROUP_5, Chrono_ElapsedSIsCalledWhenChronoIsInit_ReturnElapsed) {
  
  uint32_t elapsed = 0;

  fChrono_Init(&tickVal);

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
 * @brief fChrono_LeftS() returns zero if module is not initialized.
 * 
 */
TEST(GROUP_5, Chrono_LeftSIsCalledWhenChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_StartTimeoutS(&testChrono, 1000);
  
  uint32_t left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftS() returns zero if module is not running.
 * 
 */
TEST(GROUP_5, Chrono_LeftSIsCalledWhenNotInRun_ReturnZero) {

  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
  
  uint32_t left = fChrono_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftS() returns time left until timeout.
 * 
 */
TEST(GROUP_5, Chrono_LeftSIsCalledWhenChronoIsInit_ReturnLeft) {
  
  uint32_t left = 0;
  fChrono_Init(&tickVal);

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
 * @brief fChrono_IsTimeout() returns false when module is not initialized.
 * 
 */
TEST(GROUP_5, Chrono_IsTimedoutSIsCalledWhenChronoIsNotInit_ReturnZero) {
  
  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_StartTimeoutS(&testChrono, 1000);
  
  bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

}

/**
 * @brief fChrono_IsTimeoutS() returns false when module is not running.
 * 
 */
TEST(GROUP_5, Chrono_IsTimedoutSIsCalledWhenNotInRun_ReturnZero) {

  fChrono_Init( &tickVal);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
  
  bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);
  
}

/**
 * @brief fChrono_IsTimeoutS() returns timeout status.
 * 
 */
TEST(GROUP_5, Chrono_IsTimedoutSIsCalledWhenChronoIsInit_ReturnTimeoutStatus) {
  
  bool isTimeout = false;
  fChrono_Init(&tickVal);

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
 * @brief fChrono_fIntervalS() returns zero when module is not initialized.
 * 
 */
TEST(GROUP_5, Chrono_IntervalSIsCalledWhenChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

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
TEST(GROUP_5, Chrono_IntervalSIsCalledWhenChronoIsNotRun_ReturnZero) {

  fChrono_Init(&tickVal);

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
TEST(GROUP_5, Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval) {
  
  float interval = 0.0f;
  fChrono_Init(&tickVal);

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
  TEST_ASSERT_EQUAL_FLOAT(9000, interval);

  tickVal = 20000;
  fChrono_Start(&testChrono);
  interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 21000;
  interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_5) {

  RUN_TEST_CASE(GROUP_5, Chrono_TimeSpanSIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST_CASE(GROUP_5, Chrono_ElapsedSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_5, Chrono_ElapsedSIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST_CASE(GROUP_5, Chrono_LeftSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_5, Chrono_LeftSIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(GROUP_5, Chrono_LeftSIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST_CASE(GROUP_5, Chrono_IsTimedoutSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_5, Chrono_IsTimedoutSIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(GROUP_5, Chrono_IsTimedoutSIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST_CASE(GROUP_5, Chrono_IntervalSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_5, Chrono_IntervalSIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST_CASE(GROUP_5, Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval);

}

/** @} */ //End of TEST_GROUP_5
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_6)
/** @defgroup TEST_GROUP_6
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_6);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_6) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_6) {

}

/**
 * @brief fChrono_TimeSpanMs() must return maximum measurable time length.
 * 
 */
TEST(GROUP_6, Chrono_TimeSpanMsIsCalledWhenChronoIsInit_ReturnTimeLength) {

  float ret = 0.0f;
  fChrono_Init(&tickVal);
  
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
 * @brief fChrono_ElapsedMs() must return zero when module is not initialized.
 * 
 */
TEST(GROUP_6, Chrono_ElapsedMsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_Start(&testChrono);
  
  uint32_t elapsed = fChrono_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

}

/**
 * @brief fChrono_ElapsedMs() must return elapsed time.
 * 
 */
TEST(GROUP_6, Chrono_ElapsedMsIsCalledWhenChronoIsInit_ReturnElapsed) {

  uint32_t elapsed = 0;

  fChrono_Init(&tickVal);

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
 * @brief fChrono_LeftMs() returns zero if module is not initialized.
 * 
 */
TEST(GROUP_6, Chrono_LeftMsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_StartTimeoutMs(&testChrono, 1000);
  
  uint32_t left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftMs() returns zero if module is not running.
 * 
 */
TEST(GROUP_6, Chrono_LeftMsIsCalledWhenNotInRun_ReturnZero) {

  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
  
  uint32_t left = fChrono_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChrono_LeftMs() returns time left until timeout.
 * 
 */
TEST(GROUP_6, Chrono_LeftMsIsCalledWhenChronoIsInit_ReturnLeft) {

  uint32_t left = 0;
  fChrono_Init(&tickVal);

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
 * @brief fChrono_IsTimeout() returns false when module is not initialized.
 * 
 */
TEST(GROUP_6, Chrono_IsTimedoutMsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
  fChrono_Init(NULL);

  sChrono testChrono;
  fChrono_StartTimeoutMs(&testChrono, 1000);
  
  bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);

}

/**
 * @brief fChrono_IsTimeout() returns false when module is not running.
 * 
 */
TEST(GROUP_6, Chrono_IsTimedoutMsIsCalledWhenNotInRun_ReturnZero) {

  fChrono_Init(&tickVal);

  sChrono testChrono;
  fChrono_Stop(&testChrono);
  
  bool isTimeout = fChrono_IsTimeout(&testChrono);
  TEST_ASSERT_EQUAL(false, isTimeout);
  
}

/**
 * @brief fChrono_IsTimeout() returns timeout status.
 * 
 */
TEST(GROUP_6, Chrono_IsTimedoutMsIsCalledWhenChronoIsInit_ReturnTimeoutStatus) {

  bool isTimeout = false;
  fChrono_Init(&tickVal);

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
 * @brief fChrono_IntervalMs() returns zero when module is not initialized.
 * 
 */
TEST(GROUP_6, Chrono_IntervalMsIsCalledWhenChronoIsNotInit_ReturnZero) {

  fChrono_Init(NULL);

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
TEST(GROUP_6, Chrono_IntervalMsIsCalledWhenChronoIsNotRun_ReturnZero) {

  fChrono_Init(&tickVal);

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
TEST(GROUP_6, Chrono_IntervalMsIsCalledWhenChronoIsInit_ReturnInterval) {
  
  float interval = 0.0f;
  fChrono_Init(&tickVal);

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
  TEST_ASSERT_EQUAL_FLOAT(9000, interval);

  tickVal = 20000;
  fChrono_Start(&testChrono);
  interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 21000;
  interval = fChrono_IntervalMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_6) {

  RUN_TEST_CASE(GROUP_6, Chrono_TimeSpanMsIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST_CASE(GROUP_6, Chrono_ElapsedMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_6, Chrono_ElapsedMsIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST_CASE(GROUP_6, Chrono_LeftMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_6, Chrono_LeftMsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(GROUP_6, Chrono_LeftMsIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST_CASE(GROUP_6, Chrono_IsTimedoutMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_6, Chrono_IsTimedoutMsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(GROUP_6, Chrono_IsTimedoutMsIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST_CASE(GROUP_6, Chrono_IntervalMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(GROUP_6, Chrono_IntervalMsIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST_CASE(GROUP_6, Chrono_IntervalMsIsCalledWhenChronoIsInit_ReturnInterval);

}

/** @} */ //End of TEST_GROUP_6
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_7)
/** @defgroup TEST_GROUP_7
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_7);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_7) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_7) {

}

/**
 * @brief fChronoLong_ElapsedS() returns correct elapsed time when tick is overflowed multiple times.
 * 
 */
TEST(GROUP_7, ChronoLong_ElapsedSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  uint32_t elapsed = 0;
  sChronoLong testChrono = {0x00};
  tickVal = 0;

  fChrono_Init(&tickVal); // It takes 4.294967296 seconds to overflow the tick generator.
  
  tickVal = 0;
  fChronoLong_Start(&testChrono);
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

  tickVal = 1100000000;
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1, elapsed);
  
  tickVal = 2100000000;
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(2, elapsed);

  tickVal = 3100000000;
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(3, elapsed);

  tickVal = 4100000000;
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(4, elapsed);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(4, elapsed);

  tickVal = 806000000;
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(5, elapsed);

  tickVal = 1806000001;
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(6, elapsed);

  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(8, elapsed);

  tickVal = 420000000;
  elapsed = fChronoLong_ElapsedS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(9, elapsed);

}

/**
 * @brief fChronoLong_ElapsedMs() returns correct elapsed time when tick is overflowed multiple times.
 * 
 */
TEST(GROUP_7, ChronoLong_ElapsedMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  timeMs_t elapsed = 0;
  sChronoLong testChrono = {0x00};

  fChrono_Init(&tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_Start(&testChrono);
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, elapsed);

  tickVal = 1000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1000, elapsed);
  
  tickVal = 2000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(2000, elapsed);

  tickVal = 3000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(3000, elapsed);

  tickVal = 4000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(4000, elapsed);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(4294, elapsed);

  tickVal = 706000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(5000, elapsed);

  tickVal = 1706000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(6000, elapsed);

  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(8588, elapsed);

  tickVal = 420000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(9008, elapsed);

}

/**
 * @brief fChronoLong_LeftS() returns correct left time when tick is overflowed multiple times.
 * 
 */
TEST(GROUP_7, ChronoLong_LeftSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  uint32_t left = 0;
  sChronoLong testChrono = {0x00};

  fChrono_Init(&tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_StartTimeoutS(&testChrono, 10);
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(10, left);

  tickVal = 0xFFFFFFFF;
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(6, left);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(6, left);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_LeftS(&testChrono);

  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(2, left);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_LeftS(&testChrono);

  tickVal = 0; // This is the third overflow. It equals 12.884901888 seconds.
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(0, left);

}

/**
 * @brief fChronoLong_LeftMs() returns correct left time when tick is overflowed multiple times.
 * 
 */
TEST(GROUP_7, ChronoLong_LeftMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  timeMs_t left = 0;
  sChronoLong testChrono = {0x00};

  fChrono_Init(&tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_StartTimeoutMs(&testChrono, 10000);
  left = fChronoLong_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(10000, left);

  tickVal = 0xFFFFFFFF;
  left = fChronoLong_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(5706, left);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  left = fChronoLong_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(5706, left);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_LeftS(&testChrono);

  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  left = fChronoLong_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1412, left);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_LeftS(&testChrono);

  tickVal = 0; // This is the third overflow. It equals 12.884901888 seconds.
  left = fChronoLong_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, left);

}

/**
 * @brief fChronoLong_IsTimeout() returns correct result for a verly long timout. (Second)
 * 
 */
TEST(GROUP_7, ChronoLong_TimeoutSWithVeryLongDuration_IsTimeoutReturnCorrectly) {

  bool isTimeout = false;
  sChronoLong testChrono = {0x00};

  fChrono_Init(&tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_StartTimeoutS(&testChrono, 10);
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_FALSE(isTimeout);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_IsTimeout(&testChrono);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_FALSE(isTimeout);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_IsTimeout(&testChrono);
  
  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_FALSE(isTimeout);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_IsTimeout(&testChrono);
  
  tickVal = 0; // This is the third overflow. It equals 12.884901888 seconds.
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_TRUE(isTimeout);

}

/**
 * @brief fChronoLong_IsTimeout() returns correct result for a verly long timout. (Millisecond)
 * 
 */
TEST(GROUP_7, ChronoLong_TimeoutMsWithVeryLongDuration_IsTimeoutReturnCorrectly) {

  bool isTimeout = false;
  sChronoLong testChrono = {0x00};

  fChrono_Init(&tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_StartTimeoutMs(&testChrono, 10000);
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_FALSE(isTimeout);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_IsTimeout(&testChrono);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_FALSE(isTimeout);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_IsTimeout(&testChrono);

  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_FALSE(isTimeout);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_IsTimeout(&testChrono);

  tickVal = 0; // This is the third overflow. It equals 12.884901888 seconds.
  isTimeout = fChronoLong_IsTimeout(&testChrono);
  TEST_ASSERT_TRUE(isTimeout);

}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_7) {

  RUN_TEST_CASE(GROUP_7, ChronoLong_ElapsedSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(GROUP_7, ChronoLong_ElapsedMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(GROUP_7, ChronoLong_LeftSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(GROUP_7, ChronoLong_LeftMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(GROUP_7, ChronoLong_TimeoutSWithVeryLongDuration_IsTimeoutReturnCorrectly);
  RUN_TEST_CASE(GROUP_7, ChronoLong_TimeoutMsWithVeryLongDuration_IsTimeoutReturnCorrectly);
  RUN_TEST_CASE(GROUP_7, ChronoLong_TimeoutMsWithVeryLongDuration_IsTimeoutReturnCorrectly);

}

/** @} */ //End of TEST_GROUP_7
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_8)
/** @defgroup TEST_GROUP_8
 *  @{
 */

/**
 * @brief Test group object.
 * 
 */
TEST_GROUP(GROUP_8);

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(GROUP_8) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(GROUP_8) {

}

/**
 * @brief fChrono_fIntervalS() returns interval.
 * 
 */
TEST(GROUP_8, Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval) {
  
  float interval = 0.0f;
  fChrono_Init(&tickVal);

  sChrono testChrono;
  tickVal = 0xFFFFFFFFU;

  fChrono_Start(&testChrono);
  interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 0xFFFFFC17U;
  interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

  tickVal = 0xFFFFD8EFU;
  interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(9000, interval);

  tickVal = 0xFFFFB1DFU;
  fChrono_Start(&testChrono);
  interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0, interval);

  tickVal = 0xFFFFADF7U;
  interval = fChrono_IntervalS(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1000, interval);

}

/**
 * @brief Test group runner.
 * 
 */
TEST_GROUP_RUNNER(GROUP_8) {

  RUN_TEST_CASE(GROUP_8, Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval);

}

/** @} */ //End of TEST_GROUP_8
#endif

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief Run all tests in test group.
 * 
 */
static void RunTests(void) {
  
#if(CHRONO_TEST_STATUS == CHRONO_TESTS_DISABLED)
  (void)(RunTests);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_1)
  RUN_TEST_GROUP(GROUP_1);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_2)
  RUN_TEST_GROUP(GROUP_2);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_3)
  RUN_TEST_GROUP(GROUP_3);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_4)
  RUN_TEST_GROUP(GROUP_4);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_5)
  RUN_TEST_GROUP(GROUP_5);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_6)
  RUN_TEST_GROUP(GROUP_6);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_7)
  RUN_TEST_GROUP(GROUP_7);
#endif

#if(CHRONO_TEST_GROUP == TEST_GROUP_8)
  RUN_TEST_GROUP(GROUP_8);
#endif

}

/**
 * @brief Unit test one time setup.
 * 
 */
static void OneTimeSetup(void) {
  
  pTick = fChrono_GetTickPointer();
  
}

/**
 * @brief Unit test one time teardown.
 * 
 */
static void OneTimeTearDown(void) {

  fChrono_Init(pTick);
  
}

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
