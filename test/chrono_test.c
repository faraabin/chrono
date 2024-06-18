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

#include "..\chrono.h"
#include "..\ex\chrono_long.h"

#include "unity_fixture.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ArraySize_(arr_)  (sizeof(arr_) / sizeof(arr_[0]))

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint32_t tickVal;
static uint32_t currentTickTop;
static uint32_t tickToNs;
static volatile tick_t *pTick;
TEST_GROUP(chronoUnitTests);
TEST_GROUP(chronoLongUnitTests);

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
  
  OneTimeSetup();
  
  int ret = UnityMain(argc, argv, RunTests);
  
  OneTimeTearDown();
  
  return ret;
}

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
  
  RUN_TEST_GROUP(chronoUnitTests);
  RUN_TEST_GROUP(chronoLongUnitTests);
  
}

/**
 * @brief Unit test one time setup.
 * 
 */
static void OneTimeSetup(void) {
  
  currentTickTop = fChrono_GetTickTopValue();
  tickToNs = fChrono_GetTickToNsCoef();
  pTick = fChrono_GetTickPointer();
  
}

/**
 * @brief Unit test one time teardown.
 * 
 */
static void OneTimeTearDown(void) {

  fChrono_Init(currentTickTop, tickToNs, pTick);
  
}

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(chronoUnitTests) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(chronoUnitTests) {

}

/**
 * @brief Unit test setup.
 * 
 */
TEST_SETUP(chronoLongUnitTests) {
  
}

/**
 * @brief Unit test teardown.
 * 
 */
TEST_TEAR_DOWN(chronoLongUnitTests) {

}

/**
 * @brief fChrono_Init() must return 1 if user pass null pointer for tickValue.
 * 
 */
TEST(chronoUnitTests, Chrono_InitChronoWithNullPointer_InitReturnFalse) {
  uint8_t ret = 0;

  ret = fChrono_Init(0, 0, NULL);

  TEST_ASSERT_EQUAL(1, ret);
}

/**
 * @brief fChrono_GetTick() must return 0 when module is not initialized properly.
 * 
 */
TEST(chronoUnitTests, Chrono_GetTickIsCalledWhileChronoIsNotInit_ReturnZero) {
  uint32_t ret = 0;

  fChrono_Init(0, 0, NULL);
  
  ret = fChrono_GetTick();

  TEST_ASSERT_EQUAL_UINT32(0, ret);
}

/**
 * @brief fChrono_GetTick() must return current tick
 * 
 */
TEST(chronoUnitTests, Chrono_GetTickIsCalled_ReturnCurrentTick) {

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
TEST(chronoUnitTests, Chrono_GetTickMsIsCalledWhileChronoIsNotInit_ReturnZero) {
  
  uint64_t ret = 0xFFFFFFFFFFFFFFFF;

  fChrono_Init(0, 0, NULL);
  
  ret = fChrono_GetContinuousTickMs();

  TEST_ASSERT_EQUAL(0, ret);
}

/**
 * @brief fChrono_GetContinuousTickMs() must return cumulative time length.
 * 
 */
TEST(chronoUnitTests, Chrono_GetTickMsIsCalled_ReturnCumulativeTime) {
  
  tickVal = 0;
  
  fChrono_Init(0xFFFFFFFF, 1000000, &tickVal);
  
  uint64_t contTick = 0;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(0, contTick);

  tickVal = 1000;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(1000, contTick);

  tickVal = 0xFFFFFFFF;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64(0xFFFFFFFF, contTick);

  tickVal = 1;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64((uint64_t)(0xFFFFFFFF) + 1, contTick);

  tickVal = 1000;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64((uint64_t)(0xFFFFFFFF) + 1000, contTick);

  tickVal = 0xFFFFFFFF;
  contTick = fChrono_GetContinuousTickMs();
  TEST_ASSERT_EQUAL_UINT64((uint64_t)(0xFFFFFFFF) * 2, contTick);

}

/**
 * @brief fChrono_GetTickTopValue() must return 0 when module is not initialized properly.
 * 
 */
TEST(chronoUnitTests, Chrono_GetTickTopValueIsCalledWhileChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_GetTickTopValueIsCalledWhenChronoIsInit_ReturnExpectedValues) {

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
TEST(chronoUnitTests, Chrono_GetTickToNsCoefIsCalledWhileChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues) {

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
TEST(chronoUnitTests, Chrono_GetTickMaxTimeMsIsCalledWhileChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues) {

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
TEST(chronoUnitTests, Chrono_TimeSpanSIsCalledWhenChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_TimeSpanSIsCalledWhenChronoIsInit_ReturnTimeLength) {

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
TEST(chronoUnitTests, Chrono_TimeSpanMsIsCalledWhenChronoIsInit_ReturnTimeLength) {

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
TEST(chronoUnitTests, Chrono_TimeSpanUsIsCalledWhenChronoIsInit_ReturnTimeLength) {

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
TEST(chronoUnitTests, Chrono_ElapsedSIsCalledWhenChronoIsNotInit_ReturnZero) {
  
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
TEST(chronoUnitTests, Chrono_ElapsedSIsCalledWhenChronoIsInit_ReturnElapsed) {
  
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
TEST(chronoUnitTests, Chrono_ElapsedMsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
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
TEST(chronoUnitTests, Chrono_ElapsedMsIsCalledWhenChronoIsInit_ReturnElapsed) {

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
TEST(chronoUnitTests, Chrono_ElapsedUsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
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
TEST(chronoUnitTests, Chrono_ElapsedUsIsCalledWhenChronoIsInit_ReturnElapsed) {

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
TEST(chronoUnitTests, Chrono_LeftSIsCalledWhenChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_LeftSIsCalledWhenNotInRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_LeftSIsCalledWhenChronoIsInit_ReturnLeft) {
  
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
TEST(chronoUnitTests, Chrono_LeftMsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
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
TEST(chronoUnitTests, Chrono_LeftMsIsCalledWhenNotInRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_LeftMsIsCalledWhenChronoIsInit_ReturnLeft) {

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
TEST(chronoUnitTests, Chrono_LeftUsIsCalledWhenChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_LeftUsIsCalledWhenNotInRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_LeftUsIsCalledWhenChronoIsInit_ReturnLeft) {

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
TEST(chronoUnitTests, Chrono_IsTimedoutSIsCalledWhenChronoIsNotInit_ReturnZero) {
  
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
TEST(chronoUnitTests, Chrono_IsTimedoutSIsCalledWhenNotInRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IsTimedoutSIsCalledWhenChronoIsInit_ReturnTimeoutStatus) {
  
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
TEST(chronoUnitTests, Chrono_IsTimedoutMsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
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
TEST(chronoUnitTests, Chrono_IsTimedoutMsIsCalledWhenNotInRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IsTimedoutMsIsCalledWhenChronoIsInit_ReturnTimeoutStatus) {

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
TEST(chronoUnitTests, Chrono_IsTimedoutUsIsCalledWhenChronoIsNotInit_ReturnZero) {
  
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
TEST(chronoUnitTests, Chrono_IsTimedoutUsIsCalledWhenNotInRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IsTimedoutUsIsCalledWhenChronoIsInit_ReturnTimeoutStatus) {

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
TEST(chronoUnitTests, Chrono_IntervalSIsCalledWhenChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IntervalSIsCalledWhenChronoIsNotRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval) {
  
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
TEST(chronoUnitTests, Chrono_IntervalMsIsCalledWhenChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IntervalMsIsCalledWhenChronoIsNotRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IntervalMsIsCalledWhenChronoIsInit_ReturnInterval) {
  
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
TEST(chronoUnitTests, Chrono_IntervalUsIsCalledWhenChronoIsNotInit_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IntervalUsIsCalledWhenChronoIsNotRun_ReturnZero) {

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
TEST(chronoUnitTests, Chrono_IntervalUsIsCalledWhenChronoIsInit_ReturnInterval) {
  
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

/**
 * @brief fChronoLong_ElapsedS() returns correct elapsed time when tick is overflowed multiple times.
 * 
 */
TEST(chronoLongUnitTests, ChronoLong_ElapsedSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  uint32_t elapsed = 0;
  sChrono testChrono = {0x00};

  fChrono_Init(0xFFFFFFFF, 1, &tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

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
TEST(chronoLongUnitTests, ChronoLong_ElapsedMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  timeMs_t elapsed = 0;
  sChrono testChrono = {0x00};

  fChrono_Init(0xFFFFFFFF, 1, &tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_Start(&testChrono);
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(0.0, elapsed);

  tickVal = 1000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(999.0, elapsed);
  
  tickVal = 2000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(1998.0, elapsed);

  tickVal = 3000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(2997.0, elapsed);

  tickVal = 4000000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(3996.0, elapsed);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(4290.0, elapsed);

  tickVal = 706000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(4995.0, elapsed);

  tickVal = 1706000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(5994, elapsed);

  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(8582.0, elapsed);

  tickVal = 420000000;
  elapsed = fChronoLong_ElapsedMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(9001.0, elapsed);

}

/**
 * @brief fChronoLong_LeftS() returns correct left time when tick is overflowed multiple times.
 * 
 */
TEST(chronoLongUnitTests, ChronoLong_LeftSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  uint32_t left = 0;
  sChrono testChrono = {0x00};

  fChrono_Init(0xFFFFFFFF, 1, &tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_StartTimeoutS(&testChrono, 10);
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(10, left);

  tickVal = 0xFFFFFFFF;
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(5, left);

  tickVal = 0; // This is the first overflow. It equals 4.294967296 seconds.
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(5, left);
  
  tickVal = 0xFFFFFFFF;
  fChronoLong_LeftS(&testChrono);

  tickVal = 0; // This is the second overflow. It equals 8.589934591 seconds.
  left = fChronoLong_LeftS(&testChrono);
  TEST_ASSERT_EQUAL_UINT32(1, left);
  
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
TEST(chronoLongUnitTests, ChronoLong_LeftMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime) {

  timeMs_t left = 0;
  sChrono testChrono = {0x00};

  fChrono_Init(0xFFFFFFFF, 1, &tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

  tickVal = 0;
  fChronoLong_StartTimeoutMs(&testChrono, 10000);
  left = fChronoLong_LeftMs(&testChrono);
  TEST_ASSERT_EQUAL_FLOAT(10000.0, left);

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
TEST(chronoLongUnitTests, ChronoLong_TimeoutSWithVeryLongDuration_IsTimeoutReturnCorrectly) {

  bool isTimeout = false;
  sChrono testChrono = {0x00};

  fChrono_Init(0xFFFFFFFF, 1, &tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

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
TEST(chronoLongUnitTests, ChronoLong_TimeoutMsWithVeryLongDuration_IsTimeoutReturnCorrectly) {

  bool isTimeout = false;
  sChrono testChrono = {0x00};

  fChrono_Init(0xFFFFFFFF, 1, &tickVal); // It takes 4.294967296 seconds to overflow the tick generator.

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
 * @brief Chrono test group runner.
 * 
 */
TEST_GROUP_RUNNER(chronoUnitTests) {

  RUN_TEST_CASE(chronoUnitTests, Chrono_InitChronoWithNullPointer_InitReturnFalse);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickIsCalled_ReturnCurrentTick);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickMsIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickMsIsCalled_ReturnCumulativeTime);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickTopValueIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickTopValueIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickToNsCoefIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickToNsCoefIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickMaxTimeMsIsCalledWhileChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_GetTickMaxTimeMsIsCalledWhenChronoIsInit_ReturnExpectedValues);
  RUN_TEST_CASE(chronoUnitTests, Chrono_TimeSpanSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_TimeSpanSIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST_CASE(chronoUnitTests, Chrono_TimeSpanMsIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST_CASE(chronoUnitTests, Chrono_TimeSpanUsIsCalledWhenChronoIsInit_ReturnTimeLength);
  RUN_TEST_CASE(chronoUnitTests, Chrono_ElapsedSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_ElapsedSIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST_CASE(chronoUnitTests, Chrono_ElapsedMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_ElapsedMsIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST_CASE(chronoUnitTests, Chrono_ElapsedUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_ElapsedUsIsCalledWhenChronoIsInit_ReturnElapsed);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftSIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftSIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftMsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftMsIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftUsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_LeftUsIsCalledWhenChronoIsInit_ReturnLeft);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutSIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutSIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutMsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutMsIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutUsIsCalledWhenNotInRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IsTimedoutUsIsCalledWhenChronoIsInit_ReturnTimeoutStatus);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalSIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalSIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalSIsCalledWhenChronoIsInit_ReturnInterval);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalMsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalMsIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalMsIsCalledWhenChronoIsInit_ReturnInterval);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalUsIsCalledWhenChronoIsNotInit_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalUsIsCalledWhenChronoIsNotRun_ReturnZero);
  RUN_TEST_CASE(chronoUnitTests, Chrono_IntervalUsIsCalledWhenChronoIsInit_ReturnInterval);

}

/**
 * @brief Chrono long test group runner.
 * 
 */
TEST_GROUP_RUNNER(chronoLongUnitTests) {

  RUN_TEST_CASE(chronoLongUnitTests, ChronoLong_ElapsedSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(chronoLongUnitTests, ChronoLong_ElapsedMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(chronoLongUnitTests, ChronoLong_LeftSIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(chronoLongUnitTests, ChronoLong_LeftMsIsCalledWhenTickIsOverflowedMultipleTimes_ReturnsCorrectElapsedTime);
  RUN_TEST_CASE(chronoLongUnitTests, ChronoLong_TimeoutSWithVeryLongDuration_IsTimeoutReturnCorrectly);
  RUN_TEST_CASE(chronoLongUnitTests, ChronoLong_TimeoutMsWithVeryLongDuration_IsTimeoutReturnCorrectly);

}

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
