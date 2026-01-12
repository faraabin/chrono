/**
  ******************************************************************************
  * @file           : test_chrono.c
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
#include "test_chrono.h"
#include "chrono_config.h"

#include "..\chrono.h"

#include "unity_fixture.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)
#define fChrono_Init_()   fChrono_Init(&TickValue)
#else
#define fChrono_Init_()   fChrono_Init(fGetTick)
#endif

#define fChrono_DeInit_() fChrono_Init(NULL)

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile tick_t TickValue;
static sChrono TestChrono;

static uint32_t UsToTick;
static uint32_t MsToTick;
static uint32_t SToTick;
static double TickToUs;
static double TickToMs;
static double TickToS;

TEST_GROUP(Init);
TEST_GROUP(IsTickAvailable);
TEST_GROUP(GetTick);
TEST_GROUP(GetTimeSpan);
TEST_GROUP(Delay);
TEST_GROUP(Elapsed);
TEST_GROUP(Left);
TEST_GROUP(Timeout);
TEST_GROUP(Interval);

/* Private function prototypes -----------------------------------------------*/
static void fRunTests(void);
static void fOneTimeSetup(void);
static void fOneTimeTearDown(void);

#if (CHRONO_TICK_TYPE == TICK_TYPE_FUNCTION)
static tick_t fGetTick(void) {
  return TickValue;
}
#endif

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
int fTestChrono_Run(int argc, const char* argv[]) {
  
  int ret = 0;
  
  fOneTimeSetup();
  
  ret = UnityMain(argc, argv, fRunTests);
  
  fOneTimeTearDown();
  
  return ret;
}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                              ##### Unit Tests #####                              ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief Unit test one time setup.
 * 
 * @note This setup resets the value of TickValue.
 */
static void fOneTimeSetup(void) {
  
  TickValue = 0U;

  UsToTick = 1000U / CHRONO_TICK_TO_NANOSECOND_COEF;
  MsToTick = 1000000U / CHRONO_TICK_TO_NANOSECOND_COEF;
  SToTick = 1000000000U / CHRONO_TICK_TO_NANOSECOND_COEF;

  TickToUs = 1.0 / (double)(UsToTick);
  TickToMs = 1.0 / (double)(MsToTick);
  TickToS = 1.0 / (double)(SToTick);

}

/**
 * @brief Unit test one time teardown.
 * 
 * @note Teardown makes chrono to be deinitialized.
 * User must reinitialize chrono module in its application with proper chrono_config.h definitions.
 * 
 */
static void fOneTimeTearDown(void) {

  fChrono_DeInit_();
}

/**
 * @brief Run all tests in test group
 * 
 */
static void fRunTests(void) {

  RUN_TEST_GROUP(Init);
  RUN_TEST_GROUP(IsTickAvailable);
  RUN_TEST_GROUP(GetTick);
  RUN_TEST_GROUP(GetTimeSpan);
  RUN_TEST_GROUP(Delay);
  RUN_TEST_GROUP(Elapsed);
  RUN_TEST_GROUP(Left);
  RUN_TEST_GROUP(Timeout);
  RUN_TEST_GROUP(Interval);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                                 ##### Init #####                                 ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(Init) {

}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(Init) {

}

/**
 * @brief 
 * 
 */
TEST(Init, Chrono_CallInitWithNullPointer_ReturnsError) {
  
  TEST_ASSERT_EQUAL(CHRONO_ERROR_TICK_PTR_ERROR, fChrono_DeInit_());
}

/**
 * @brief 
 * 
 */
TEST(Init, Chrono_CallInitWithPointer_ReturnsOk) {
  
  TEST_ASSERT_EQUAL(CHRONO_OK, fChrono_Init_());
}

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(Init) {

  RUN_TEST_CASE(Init, Chrono_CallInitWithNullPointer_ReturnsError);
  RUN_TEST_CASE(Init, Chrono_CallInitWithPointer_ReturnsOk);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                           ##### IsTickAvailable #####                            ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(IsTickAvailable) {
  fChrono_DeInit_();
}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(IsTickAvailable) {

}

/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoNotInitializedCallIsTickUsAvailable_ReturnsFalse) {

  TEST_ASSERT_FALSE(fChrono_IsTickUsAvailable());
}

#if ((1000U / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoInitializedCallIsTickUsAvailable_ReturnsCorrectValue) {

  fChrono_Init_();
  TEST_ASSERT_TRUE(fChrono_IsTickUsAvailable());
}
#else
/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoInitializedCallIsTickUsAvailable_ReturnsCorrectValue) {

  fChrono_Init_();
  TEST_ASSERT_FALSE(fChrono_IsTickUsAvailable());
}
#endif

/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoNotInitializedCallIsTickMsAvailable_ReturnsFalse) {

  TEST_ASSERT_FALSE(fChrono_IsTickMsAvailable());
}

#if ((1000000U / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoInitializedCallIsTickMsAvailable_ReturnsCorrectValue) {

  fChrono_Init_();
  TEST_ASSERT_TRUE(fChrono_IsTickMsAvailable());
}
#else
/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoInitializedCallIsTickMsAvailable_ReturnsCorrectValue) {

  fChrono_Init_();
  TEST_ASSERT_FALSE(fChrono_IsTickMsAvailable());
}
#endif

/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoNotInitializedCallIsTickSAvailable_ReturnsFalse) {

  TEST_ASSERT_FALSE(fChrono_IsTickSAvailable());
}

#if ((1000000000U / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoInitializedCallIsTickSAvailable_ReturnsCorrectValue) {

  fChrono_Init_();
  TEST_ASSERT_TRUE(fChrono_IsTickSAvailable());
}
#else
/**
 * @brief 
 * 
 */
TEST(IsTickAvailable, Chrono_ChronoInitializedCallIsTickSAvailable_ReturnsCorrectValue) {

  fChrono_Init_();
  TEST_ASSERT_FALSE(fChrono_IsTickSAvailable());
}
#endif

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(IsTickAvailable) {

  RUN_TEST_CASE(IsTickAvailable, Chrono_ChronoNotInitializedCallIsTickUsAvailable_ReturnsFalse);
  RUN_TEST_CASE(IsTickAvailable, Chrono_ChronoInitializedCallIsTickUsAvailable_ReturnsCorrectValue);
  RUN_TEST_CASE(IsTickAvailable, Chrono_ChronoNotInitializedCallIsTickMsAvailable_ReturnsFalse);
  RUN_TEST_CASE(IsTickAvailable, Chrono_ChronoInitializedCallIsTickMsAvailable_ReturnsCorrectValue);
  RUN_TEST_CASE(IsTickAvailable, Chrono_ChronoNotInitializedCallIsTickSAvailable_ReturnsFalse);
  RUN_TEST_CASE(IsTickAvailable, Chrono_ChronoInitializedCallIsTickSAvailable_ReturnsCorrectValue);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                               ##### GetTick #####                                ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief Test setup
 * 
 */
static TEST_SETUP(GetTick) {

  TickValue = 0U;

}

/**
 * @brief Test teardown
 * 
 */
static TEST_TEAR_DOWN(GetTick) {

}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetTick_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_HEX(0x0U, fChrono_GetTick());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetTick_ReturnsTickValue) {

  fChrono_Init_();

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetTick());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE, fChrono_GetTick());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE - 1, fChrono_GetTick());
  
  TickValue = 0x01;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_GetTick());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetTick());
  
  TickValue = 0x0U;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE, fChrono_GetTick());
  
  TickValue = 0x01;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE - 1, fChrono_GetTick());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_GetTick());
#endif

}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTick_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_HEX(0x0U, fChrono_GetContinuousTick());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetContnuousTick_ReturnsContinuousTickValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_HEX(0x00U, fChrono_GetContinuousTick());
  
  TickValue = 0x01;
  TEST_ASSERT_EQUAL_HEX(0x01, fChrono_GetContinuousTick());
  
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0x00U, fChrono_GetContinuousTick());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX(0x01, fChrono_GetContinuousTick());
  
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickIsOverflowedCallGetContnuousTick_ReturnsContinuousTickValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_HEX(0x00U, fChrono_GetContinuousTick());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE, fChrono_GetContinuousTick());
  
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_HEX((1 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());
  
  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_HEX((1 * CHRONO_TICK_TOP_VALUE) + 2, fChrono_GetContinuousTick());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_HEX((2 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());

  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_HEX((2 * CHRONO_TICK_TOP_VALUE) + 2, fChrono_GetContinuousTick());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_HEX((3 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0x00U, fChrono_GetContinuousTick());
  
  TickValue = 0x00;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE, fChrono_GetContinuousTick());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((1 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX((1 * CHRONO_TICK_TOP_VALUE) + 2, fChrono_GetContinuousTick());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((2 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX((2 * CHRONO_TICK_TOP_VALUE) + 2, fChrono_GetContinuousTick());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((3 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTick_ReturnsContinuousTickValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_HEX((3 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((3 * CHRONO_TICK_TOP_VALUE) + 1, fChrono_GetContinuousTick());
#endif
  
}

#if ((1000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickUs_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_UINT64(0x0UL, fChrono_GetContinuousTickUs());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetContinuousTickUs_ReturnsContinuousTickUsValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickUs());
  
  TickValue = 1 * UsToTick;
  TEST_ASSERT_EQUAL_UINT64(1, fChrono_GetContinuousTickUs());
  
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_EQUAL_UINT64(1, fChrono_GetContinuousTickUs());
  
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickIsOverflowedCallGetContnuousTickUs_ReturnsContinuousTickUsValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_GetContinuousTickUs());
  
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());
  
  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 2) * TickToUs, fChrono_GetContinuousTickUs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());

  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 2) * TickToUs, fChrono_GetContinuousTickUs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickUs());
  
  TickValue = 0x00;
  TEST_ASSERT_EQUAL_UINT64(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 2) * TickToUs, fChrono_GetContinuousTickUs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 2) * TickToUs, fChrono_GetContinuousTickUs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTickUs_ReturnsContinuousTickUsValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToUs, fChrono_GetContinuousTickUs());
#endif
  
}

#else

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickUs_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_UINT64(0x0UL, fChrono_GetContinuousTickUs());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickUsCoefIsZeroCallGetContinuousTickUs_ReturnsZero) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = 0x01;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickUsCoefIsZeroAndTickOverflowedCallGetContinuousTickUs_ReturnsZero) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());

  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = 0x00;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickUs());
#endif
  
}

#endif

#if ((1000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickMs_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_UINT64(0x0UL, fChrono_GetContinuousTickMs());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetContinuousTickMs_ReturnsContinuousTickMsValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  TEST_ASSERT_EQUAL_UINT64(0x00U * TickToMs, fChrono_GetContinuousTickMs());
  
  TickValue = 1 * MsToTick;
  TEST_ASSERT_EQUAL_UINT64(1, fChrono_GetContinuousTickMs());
  
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_EQUAL_UINT64(1, fChrono_GetContinuousTickMs());
  
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickIsOverflowedCallGetContnuousTickMs_ReturnsContinuousTickMsValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(CHRONO_TICK_TOP_VALUE * TickToMs, fChrono_GetContinuousTickMs());
  
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());
  
  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 2) * TickToMs, fChrono_GetContinuousTickMs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());

  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 2) * TickToMs, fChrono_GetContinuousTickMs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U * TickToMs, fChrono_GetContinuousTickMs());
  
  TickValue = 0x00;
  TEST_ASSERT_EQUAL_UINT64(CHRONO_TICK_TOP_VALUE * TickToMs, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 2) * TickToMs, fChrono_GetContinuousTickMs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 2) * TickToMs, fChrono_GetContinuousTickMs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTickMs_ReturnsContinuousTickMsValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToMs, fChrono_GetContinuousTickMs());
#endif
  
}

#else

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickMs_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_UINT64(0x0UL, fChrono_GetContinuousTickMs());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickMsCoefIsZeroCallGetContinuousTickMs_ReturnsZero) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = 0x01;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickMsCoefIsZeroAndTickOverflowedCallGetContinuousTickMs_ReturnsZero) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());

  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = 0x00;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickMs());
#endif
  
}

#endif

#if ((1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickS_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_UINT64(0x0UL, fChrono_GetContinuousTickS());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetContinuousTickS_ReturnsContinuousTickSValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickS());
  
  TickValue = 1 * SToTick;
  TEST_ASSERT_EQUAL_UINT64(1, fChrono_GetContinuousTickS());
  
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_EQUAL_UINT64(1, fChrono_GetContinuousTickS());
  
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickIsOverflowedCallGetContnuousTickS_ReturnsContinuousTickSValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(CHRONO_TICK_TOP_VALUE * TickToS, fChrono_GetContinuousTickS());
  
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());
  
  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 2) * TickToS, fChrono_GetContinuousTickS());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());

  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 2) * TickToS, fChrono_GetContinuousTickS());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0, fChrono_GetContinuousTickS());
  
  TickValue = 0x00;
  TEST_ASSERT_EQUAL_UINT64(CHRONO_TICK_TOP_VALUE * TickToS, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(((1 * CHRONO_TICK_TOP_VALUE) + 2) * TickToS, fChrono_GetContinuousTickS());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(((2 * CHRONO_TICK_TOP_VALUE) + 2) * TickToS, fChrono_GetContinuousTickS());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTickS_ReturnsContinuousTickSValue) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x01U;
  fChrono_ContinuousMaker_Run();
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_ContinuousMaker_Run();
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(((3 * CHRONO_TICK_TOP_VALUE) + 1) * TickToS, fChrono_GetContinuousTickS());
#endif
  
}

#else

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickS_ReturnsZero) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_EQUAL_UINT64(0x0UL, fChrono_GetContinuousTickS());
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickSCoefIsZeroCallGetContinuousTickS_ReturnsZero) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = 0x01;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
#endif
  
}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_TickSCoefIsZeroAndTickOverflowedCallGetContinuousTickS_ReturnsZero) {

  fChrono_Init_();

  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());

  TickValue = 0x01U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());

  TickValue = 0x00U;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = 0x00;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
  
  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_UINT64(0x00U, fChrono_GetContinuousTickS());
#endif
  
}

#endif

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetTickTopValue_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0U, fChrono_GetTickTopValue());

}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetTickTopValue_ReturnsTickTopValue) {

  fChrono_Init_();

  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE, fChrono_GetTickTopValue());

}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetTickToNsCoef_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0U, fChrono_GetTickToNsCoef());

}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetTickToNsCoef_ReturnsTickToNsCoef) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TO_NANOSECOND_COEF, fChrono_GetTickToNsCoef());

}

/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoNotInitializedCallGetTickPointer_ReturnsNULL) {
  
  fChrono_DeInit_();
  
  TEST_ASSERT_NULL(fChrono_GetTickPointer());
  
}

#if (CHRONO_TICK_TYPE == TICK_TYPE_VARIABLE)
/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetTickPointer_ReturnsPointer) {

  fChrono_Init_();

  TEST_ASSERT_EQUAL_PTR(&TickValue, fChrono_GetTickPointer());

}
#else
/**
 * @brief 
 * 
 */
TEST(GetTick, Chrono_ChronoInitializedCallGetTickPointer_ReturnsPointer) {

  fChrono_Init_();

  TEST_ASSERT_EQUAL_PTR(fGetTick, fChrono_GetTickPointer());

}
#endif

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(GetTick) {

  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetTick_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetTick_ReturnsTickValue);
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTick_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetContnuousTick_ReturnsContinuousTickValue);
  RUN_TEST_CASE(GetTick, Chrono_TickIsOverflowedCallGetContnuousTick_ReturnsContinuousTickValue);
  RUN_TEST_CASE(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTick_ReturnsContinuousTickValue);
#if ((1000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickUs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetContinuousTickUs_ReturnsContinuousTickUsValue);
  RUN_TEST_CASE(GetTick, Chrono_TickIsOverflowedCallGetContnuousTickUs_ReturnsContinuousTickUsValue);
  RUN_TEST_CASE(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTickUs_ReturnsContinuousTickUsValue);
#else
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickUs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickUsCoefIsZeroCallGetContinuousTickUs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickUsCoefIsZeroAndTickOverflowedCallGetContinuousTickUs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickUsCoefIsZeroAndTickOverflowedCallGetContinuousTickUs_ReturnsZero);
#endif
#if ((1000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickMs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetContinuousTickMs_ReturnsContinuousTickMsValue);
  RUN_TEST_CASE(GetTick, Chrono_TickIsOverflowedCallGetContnuousTickMs_ReturnsContinuousTickMsValue);
  RUN_TEST_CASE(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTickMs_ReturnsContinuousTickMsValue);
#else
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickMs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickMsCoefIsZeroCallGetContinuousTickMs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickMsCoefIsZeroAndTickOverflowedCallGetContinuousTickMs_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickMsCoefIsZeroAndTickOverflowedCallGetContinuousTickMs_ReturnsZero);
#endif
#if ((1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickS_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetContinuousTickS_ReturnsContinuousTickSValue);
  RUN_TEST_CASE(GetTick, Chrono_TickIsOverflowedCallGetContnuousTickS_ReturnsContinuousTickSValue);
  RUN_TEST_CASE(GetTick, Chrono_RunContinuousMakerAfterTickOverflowThenCallGetContinuousTickS_ReturnsContinuousTickSValue);
#else
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetContinuousTickS_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickSCoefIsZeroCallGetContinuousTickS_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickSCoefIsZeroAndTickOverflowedCallGetContinuousTickS_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_TickSCoefIsZeroAndTickOverflowedCallGetContinuousTickS_ReturnsZero);
#endif
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetTickTopValue_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetTickTopValue_ReturnsTickTopValue);
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetTickToNsCoef_ReturnsZero);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetTickToNsCoef_ReturnsTickToNsCoef);
  RUN_TEST_CASE(GetTick, Chrono_ChronoNotInitializedCallGetTickPointer_ReturnsNULL);
  RUN_TEST_CASE(GetTick, Chrono_ChronoInitializedCallGetTickPointer_ReturnsPointer);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                             ##### GetTimeSpan #####                              ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(GetTimeSpan) {

}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(GetTimeSpan) {

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoNotInitializedCallGetMaxMeasurableTimeUs_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetMaxMeasurableTimeUs());

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoInitializedCallGetMaxMeasurableTimeUs_ReturnsMaxMeasurableTimeUs) {

  fChrono_Init_();

#if ((1000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE / UsToTick, fChrono_GetMaxMeasurableTimeUs());
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetMaxMeasurableTimeUs());
#endif

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoNotInitializedCallGetMaxMeasurableTimeMs_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetMaxMeasurableTimeMs());

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoInitializedCallGetMaxMeasurableTimeMs_ReturnsMaxMeasurableTimeMs) {

  fChrono_Init_();

#if ((1000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE / MsToTick, fChrono_GetMaxMeasurableTimeMs());
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetMaxMeasurableTimeMs());
#endif

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoNotInitializedCallGetMaxMeasurableTimeS_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetMaxMeasurableTimeS());

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoInitializedCallGetMaxMeasurableTimeS_ReturnsMaxMeasurableTimeS) {

  fChrono_Init_();

#if ((1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE / SToTick, fChrono_GetMaxMeasurableTimeS());
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_GetMaxMeasurableTimeS());
#endif

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanTick_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_TimeSpanTick(0, CHRONO_TICK_TOP_VALUE));

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanTick_ReturnsTimeSpanInTicks) {

  fChrono_Init_();

  TEST_ASSERT_EQUAL_HEX(0x00, fChrono_TimeSpanTick(0x00, 0x00));
  TEST_ASSERT_EQUAL_HEX(0x01, fChrono_TimeSpanTick(0x00, 0x01));
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE, fChrono_TimeSpanTick(0x00, CHRONO_TICK_TOP_VALUE));
  TEST_ASSERT_EQUAL_HEX(0x01, fChrono_TimeSpanTick(CHRONO_TICK_TOP_VALUE, 0x00));

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanUs_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_TimeSpanUs(0, CHRONO_TICK_TOP_VALUE));

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanUs_ReturnsTimeSpanInUs) {

  fChrono_Init_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_TimeSpanUs(0, 0));
  TEST_ASSERT_EQUAL_HEX(1, fChrono_TimeSpanUs(0, (1 * UsToTick)));
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_TimeSpanUs(0x00, CHRONO_TICK_TOP_VALUE));
  TEST_ASSERT_EQUAL_HEX(1 * TickToUs, fChrono_TimeSpanUs(CHRONO_TICK_TOP_VALUE, 0x00));

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanMs_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_TimeSpanMs(0, CHRONO_TICK_TOP_VALUE));

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanMs_ReturnsTimeSpanInMs) {

  fChrono_Init_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_TimeSpanMs(0, 0));
  TEST_ASSERT_EQUAL_HEX(1, fChrono_TimeSpanMs(0, (1 * UsToTick)));
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_TimeSpanMs(0x00, CHRONO_TICK_TOP_VALUE));
  TEST_ASSERT_EQUAL_HEX(1 * TickToUs, fChrono_TimeSpanMs(CHRONO_TICK_TOP_VALUE, 0x00));

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanS_ReturnsZero) {

  fChrono_DeInit_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_TimeSpanS(0, CHRONO_TICK_TOP_VALUE));

}

/**
 * @brief 
 * 
 */
TEST(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanS_ReturnsTimeSpanInS) {

  fChrono_Init_();

  TEST_ASSERT_EQUAL_HEX(0, fChrono_TimeSpanS(0, 0));
  TEST_ASSERT_EQUAL_HEX(1, fChrono_TimeSpanS(0, (1 * UsToTick)));
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_TimeSpanS(0x00, CHRONO_TICK_TOP_VALUE));
  TEST_ASSERT_EQUAL_HEX(1 * TickToUs, fChrono_TimeSpanS(CHRONO_TICK_TOP_VALUE, 0x00));

}

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(GetTimeSpan) {

  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoNotInitializedCallGetMaxMeasurableTimeUs_ReturnsZero);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoInitializedCallGetMaxMeasurableTimeUs_ReturnsMaxMeasurableTimeUs);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoNotInitializedCallGetMaxMeasurableTimeMs_ReturnsZero);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoInitializedCallGetMaxMeasurableTimeMs_ReturnsMaxMeasurableTimeMs);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoNotInitializedCallGetMaxMeasurableTimeS_ReturnsZero);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoInitializedCallGetMaxMeasurableTimeS_ReturnsMaxMeasurableTimeS);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanTick_ReturnsZero);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanTick_ReturnsTimeSpanInTicks);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanUs_ReturnsZero);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanUs_ReturnsTimeSpanInUs);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanMs_ReturnsZero);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanMs_ReturnsTimeSpanInMs);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoNotInitializedCallTimeSpanS_ReturnsZero);
  RUN_TEST_CASE(GetTimeSpan, Chrono_ChronoInitializedCallTimeSpanS_ReturnsTimeSpanInS);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                                ##### Delay #####                                 ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(Delay) {

}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(Delay) {

}

/**
 * @brief 
 * 
 */
TEST(Delay, Chrono_ChronoNotInitializedCallDelayUs_DoesNothing) {

  fChrono_DeInit_();

  fChrono_DelayUs(0xFFFFFFFF);

}

/**
 * @brief 
 * 
 */
TEST(Delay, Chrono_ChronoNotInitializedCallDelayMs_DoesNothing) {

  fChrono_DeInit_();

  fChrono_DelayMs(0xFFFFFFFF);

}

/**
 * @brief 
 * 
 */
TEST(Delay, Chrono_ChronoNotInitializedCallDelayS_DoesNothing) {

  fChrono_DeInit_();

  fChrono_DelayS(0xFFFFFFFF);

}

/**
 * @brief 
 * 
 */
TEST(Delay, Chrono_ChronoInitializedCallDelayUs_CannotTest) {
  TEST_PASS_MESSAGE("Delay blocks the code. Test cannot change the tick value! Test it manually.");
}

/**
 * @brief 
 * 
 */
TEST(Delay, Chrono_ChronoInitializedCallDelayMs_CannotTest) {
  TEST_PASS_MESSAGE("Delay blocks the code. Test cannot change the tick value! Test it manually.");
}

/**
 * @brief 
 * 
 */
TEST(Delay, Chrono_ChronoInitializedCallDelayS_CannotTest) {
  TEST_PASS_MESSAGE("Delay blocks the code. Test cannot change the tick value! Test it manually.");
}

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(Delay) {

  RUN_TEST_CASE(Delay, Chrono_ChronoNotInitializedCallDelayUs_DoesNothing);
  RUN_TEST_CASE(Delay, Chrono_ChronoNotInitializedCallDelayMs_DoesNothing);
  RUN_TEST_CASE(Delay, Chrono_ChronoNotInitializedCallDelayS_DoesNothing);
  RUN_TEST_CASE(Delay, Chrono_ChronoInitializedCallDelayUs_CannotTest);
  RUN_TEST_CASE(Delay, Chrono_ChronoInitializedCallDelayMs_CannotTest);
  RUN_TEST_CASE(Delay, Chrono_ChronoInitializedCallDelayS_CannotTest);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                               ##### Elapsed #####                                ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(Elapsed) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif

}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(Elapsed) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif
}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_ChronoNotInitializedCallElapsedUs_ReturnsZero) {
  
  fChrono_DeInit_();
  fChrono_Start(&TestChrono);

  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedUs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_ChronoInitializedCallElapsedUs_ReturnsElapsedTime) {
  
  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_Start(&TestChrono);
  
  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedUs(&TestChrono));

  TickValue = 1 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedUs(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_ElapsedUs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_Start(&TestChrono);

  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedUs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedUs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_ElapsedUs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_StartChronoAndCallElapsedUs_ReturnsElapsedTime) {
  
  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_Start(&TestChrono);
  TickValue = 1 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedUs(&TestChrono));

  TickValue = 1;
  fChrono_Start(&TestChrono);
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_ElapsedUs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedUs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToUs, fChrono_ElapsedUs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_ChronoNotInitializedCallElapsedMs_ReturnsZero) {
  
  fChrono_DeInit_();
  fChrono_Start(&TestChrono);

  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedMs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_ChronoInitializedCallElapsedMs_ReturnsElapsedTime) {
  
  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_Start(&TestChrono);
  
  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedMs(&TestChrono));

  TickValue = 1 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedMs(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToMs, fChrono_ElapsedMs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_Start(&TestChrono);

  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedMs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedMs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToMs, fChrono_ElapsedMs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_StartChronoAndCallElapsedMs_ReturnsElapsedTime) {
  
  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_Start(&TestChrono);
  TickValue = 1 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedMs(&TestChrono));

  TickValue = 1;
  fChrono_Start(&TestChrono);
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToMs, fChrono_ElapsedMs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedMs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToMs, fChrono_ElapsedMs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_ChronoNotInitializedCallElapsedS_ReturnsZero) {
  
  fChrono_DeInit_();
  fChrono_Start(&TestChrono);

  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedS(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_ChronoInitializedCallElapsedS_ReturnsElapsedTime) {
  
  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_Start(&TestChrono);
  
  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedS(&TestChrono));

  TickValue = 1 * SToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedS(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToS, fChrono_ElapsedS(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_Start(&TestChrono);

  TEST_ASSERT_EQUAL_HEX(0, fChrono_ElapsedS(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedS(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToS, fChrono_ElapsedS(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Elapsed, Chrono_StartChronoAndCallElapsedS_ReturnsElapsedTime) {
  
  #if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_Start(&TestChrono);
  TickValue = 1 * SToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedS(&TestChrono));

  TickValue = 1;
  fChrono_Start(&TestChrono);
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToS, fChrono_ElapsedS(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_ElapsedS(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE * TickToS, fChrono_ElapsedS(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(Elapsed) {

  RUN_TEST_CASE(Elapsed, Chrono_ChronoNotInitializedCallElapsedUs_ReturnsZero);
  RUN_TEST_CASE(Elapsed, Chrono_ChronoInitializedCallElapsedUs_ReturnsElapsedTime);
  RUN_TEST_CASE(Elapsed, Chrono_StartChronoAndCallElapsedUs_ReturnsElapsedTime);
  RUN_TEST_CASE(Elapsed, Chrono_ChronoNotInitializedCallElapsedMs_ReturnsZero);
  RUN_TEST_CASE(Elapsed, Chrono_ChronoInitializedCallElapsedMs_ReturnsElapsedTime);
  RUN_TEST_CASE(Elapsed, Chrono_StartChronoAndCallElapsedMs_ReturnsElapsedTime);
  RUN_TEST_CASE(Elapsed, Chrono_ChronoNotInitializedCallElapsedS_ReturnsZero);
  RUN_TEST_CASE(Elapsed, Chrono_ChronoInitializedCallElapsedS_ReturnsElapsedTime);
  RUN_TEST_CASE(Elapsed, Chrono_StartChronoAndCallElapsedS_ReturnsElapsedTime);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                                 ##### Left #####                                 ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(Left) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif
}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(Left) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif
}

/**
 * @brief 
 * 
 */
TEST(Left, Chrono_ChronoNotInitializedCallLeftUs_ReturnsZero) {

  fChrono_DeInit_();

  fChrono_StartTimeoutUs(&TestChrono, 0xFFFFFFFF);
  
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Left, Chrono_StartTimeoutAndGetLeftUs_ReturnsZero) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_StartTimeoutUs(&TestChrono, 5 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(5 * UsToTick, fChrono_LeftUs(&TestChrono));

  TickValue = 1 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(4 * UsToTick, fChrono_LeftUs(&TestChrono));

  TickValue = 5 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));
  
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));

  TickValue = 1;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_StartTimeoutUs(&TestChrono, 5 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(5 * UsToTick, fChrono_LeftUs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(4 * UsToTick, fChrono_LeftUs(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftUs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Left, Chrono_ChronoNotInitializedCallLeftMs_ReturnsZero) {

  fChrono_DeInit_();

  fChrono_StartTimeoutMs(&TestChrono, 0xFFFFFFFF);
  
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Left, Chrono_StartTimeoutAndGetLeftMs_ReturnsZero) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_StartTimeoutMs(&TestChrono, 5 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(5 * MsToTick, fChrono_LeftMs(&TestChrono));

  TickValue = 1 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(4 * MsToTick, fChrono_LeftMs(&TestChrono));

  TickValue = 5 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));
  
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));

  TickValue = 1;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_StartTimeoutMs(&TestChrono, 5 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(5 * MsToTick, fChrono_LeftMs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(4 * MsToTick, fChrono_LeftMs(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftMs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Left, Chrono_ChronoNotInitializedCallLeftS_ReturnsZero) {

  fChrono_DeInit_();

  fChrono_StartTimeoutS(&TestChrono, 0xFFFFFFFF);
  
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Left, Chrono_StartTimeoutAndGetLeftS_ReturnsZero) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_StartTimeoutS(&TestChrono, 5 * SToTick);
  TEST_ASSERT_EQUAL_HEX(CHRONO_TICK_TOP_VALUE, fChrono_LeftS(&TestChrono));

  TickValue = 1 * SToTick;
  TEST_ASSERT_EQUAL_HEX(4 * SToTick, fChrono_LeftS(&TestChrono));

  TickValue = 5 * SToTick;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));
  
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));

  TickValue = 1;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_StartTimeoutS(&TestChrono, CHRONO_TICK_TOP_VALUE);
  TEST_ASSERT_EQUAL_HEX(5 * SToTick, fChrono_LeftS(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_EQUAL_HEX(4 * SToTick - 1000, fChrono_LeftS(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));
  
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));

  TickValue = CHRONO_TICK_TOP_VALUE - 1;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));

  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX(0, fChrono_LeftS(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(Left) {

  RUN_TEST_CASE(Left, Chrono_ChronoNotInitializedCallLeftUs_ReturnsZero);
  RUN_TEST_CASE(Left, Chrono_StartTimeoutAndGetLeftUs_ReturnsZero);
  RUN_TEST_CASE(Left, Chrono_ChronoNotInitializedCallLeftMs_ReturnsZero);
  RUN_TEST_CASE(Left, Chrono_StartTimeoutAndGetLeftMs_ReturnsZero);
  RUN_TEST_CASE(Left, Chrono_ChronoNotInitializedCallLeftS_ReturnsZero);
  RUN_TEST_CASE(Left, Chrono_StartTimeoutAndGetLeftS_ReturnsZero);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                               ##### Timeout #####                                ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(Timeout) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif
}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(Timeout) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif
}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_ChronoNotInitializedStartTimeUsCheckIsTimeout_ReturnsFalse) {

  fChrono_DeInit_();

  fChrono_StartTimeoutUs(&TestChrono, 0x0);
  
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_CallStartTimeUsCheckIsTimeoutWithNullInput_ReturnsFalse) {

  fChrono_StartTimeoutUs(NULL, 0x0);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(NULL));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutUsAndStopChronoAndCheckIsTimeout_ReturnsFalse) {

  fChrono_StartTimeoutUs(&TestChrono, 10);
  fChrono_Stop(&TestChrono);

  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutUsAndCheckIsTimeout_ReturnsTrueWhenTimedout) {
  
#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_StartTimeoutUs(&TestChrono, 10);
# if ((1000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = 1 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 9 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 10 * UsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 11 * UsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = 1 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 9 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 10 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 11 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_StartTimeoutUs(&TestChrono, 10);
# if ((1000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (9 * UsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (10 * UsToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (11 * UsToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (9 * UsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (10 * UsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (11 * UsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#endif

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_TickOverflowAfterStartTimeoutUsAndCheckIsTimeout_ReturnsTrueWhenTimedout) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = (CHRONO_TICK_TOP_VALUE - 5 * UsToTick);
  fChrono_StartTimeoutUs(&TestChrono, 10);
# if ((1000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 4 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 5 * UsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 6 * UsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 4 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 5 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 6 * UsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#else
# if ((1000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = 5 * UsToTick;
  fChrono_StartTimeoutUs(&TestChrono, 10);
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (CHRONO_TICK_TOP_VALUE - (5 * UsToTick));
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = (CHRONO_TICK_TOP_VALUE - (6 * UsToTick));
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = 5 * UsToTick;
  fChrono_StartTimeoutUs(&TestChrono, 10);
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (CHRONO_TICK_TOP_VALUE - (5 * UsToTick));
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (CHRONO_TICK_TOP_VALUE - (6 * UsToTick));
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#endif

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutUsThenStopCheckIsTimeout_ReturnsFalse) {

  fChrono_StartTimeoutUs(&TestChrono, 10);
  fChrono_Stop(&TestChrono);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_ChronoNotInitializedStartTimeMsCheckIsTimeout_ReturnsFalse) {

  fChrono_DeInit_();

  fChrono_StartTimeoutMs(&TestChrono, 0x0);
  
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_CallStartTimeMsCheckIsTimeoutWithNullInput_ReturnsFalse) {

  fChrono_StartTimeoutMs(NULL, 0x0);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(NULL));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutMsAndStopChronoAndCheckIsTimeout_ReturnsFalse) {

  fChrono_StartTimeoutMs(&TestChrono, 10);
  fChrono_Stop(&TestChrono);

  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutMsAndCheckIsTimeout_ReturnsTrueWhenTimedout) {
  
#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
  fChrono_StartTimeoutMs(&TestChrono, 10);
# if ((1000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = 1 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 9 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 10 * MsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 11 * MsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE * MsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0 * MsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = 1 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 9 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 10 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 11 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_StartTimeoutMs(&TestChrono, 10);
# if ((1000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (9 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (10 * MsToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (11 * MsToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (9 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (10 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (11 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#endif

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_TickOverflowAfterStartTimeoutMsAndCheckIsTimeout_ReturnsTrueWhenTimedout) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = CHRONO_TICK_TOP_VALUE - (5 * MsToTick);
  fChrono_StartTimeoutMs(&TestChrono, 10);
# if ((1000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 4 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 5 * MsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 6 * MsToTick;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 4 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 5 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 6 * MsToTick;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#else
# if ((1000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = 5 * MsToTick;
  fChrono_StartTimeoutMs(&TestChrono, 10);
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (5 * MsToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (6 * MsToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = 5 * MsToTick;
  fChrono_StartTimeoutMs(&TestChrono, 10);
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (5 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (6 * MsToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#endif

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutMsThenStopCheckIsTimeout_ReturnsFalse) {

  fChrono_StartTimeoutMs(&TestChrono, 10);
  fChrono_Stop(&TestChrono);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_ChronoNotInitializedStartTimeSCheckIsTimeout_ReturnsFalse) {

  fChrono_DeInit_();

  fChrono_StartTimeoutS(&TestChrono, 0x0);
  
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_CallStartTimeSCheckIsTimeoutWithNullInput_ReturnsFalse) {

  fChrono_StartTimeoutS(NULL, 0x0);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(NULL));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutSAndStopChronoAndCheckIsTimeout_ReturnsFalse) {

  fChrono_StartTimeoutS(&TestChrono, 10);
  fChrono_Stop(&TestChrono);

  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutSAndCheckIsTimeout_ReturnsTrueWhenTimedout) {
  
#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0 * (CHRONO_TICK_TO_NANOSECOND_COEF * 1000000000);
  fChrono_StartTimeoutS(&TestChrono, 10);
# if ((1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = (1 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (9 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (10 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = (11 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = (1 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (9 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (10 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (11 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
  fChrono_StartTimeoutS(&TestChrono, 10);
# if ((1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (9 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (10 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (11 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (9 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (10 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (11 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#endif

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_TickOverflowAfterStartTimeoutSAndCheckIsTimeout_ReturnsTrueWhenTimedout) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = CHRONO_TICK_TOP_VALUE - (5 * SToTick);
  fChrono_StartTimeoutS(&TestChrono, 10);
# if ((1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (4 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (5 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = (6 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (4 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (5 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = (6 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#else
# if ((1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF) != 0)
  TickValue = (5 * SToTick);
  fChrono_StartTimeoutS(&TestChrono, 10);
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (4 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (5 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (6 * SToTick);
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_TRUE(fChrono_IsTimeout(&TestChrono));
# else
  TickValue = (5 * SToTick);
  fChrono_StartTimeoutS(&TestChrono, 10);
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (4 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (5 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (6 * SToTick);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));
# endif
#endif

}

/**
 * @brief 
 * 
 */
TEST(Timeout, Chrono_StartTimeoutSThenStopCheckIsTimeout_ReturnsFalse) {

  fChrono_StartTimeoutS(&TestChrono, 10);
  fChrono_Stop(&TestChrono);
  TEST_ASSERT_FALSE(fChrono_IsTimeout(&TestChrono));

}

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(Timeout) {

  RUN_TEST_CASE(Timeout, Chrono_ChronoNotInitializedStartTimeUsCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_CallStartTimeUsCheckIsTimeoutWithNullInput_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutUsAndStopChronoAndCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutUsAndCheckIsTimeout_ReturnsTrueWhenTimedout);
  RUN_TEST_CASE(Timeout, Chrono_TickOverflowAfterStartTimeoutUsAndCheckIsTimeout_ReturnsTrueWhenTimedout);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutUsThenStopCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_ChronoNotInitializedStartTimeMsCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_CallStartTimeMsCheckIsTimeoutWithNullInput_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutMsAndStopChronoAndCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutMsAndCheckIsTimeout_ReturnsTrueWhenTimedout);
  RUN_TEST_CASE(Timeout, Chrono_TickOverflowAfterStartTimeoutMsAndCheckIsTimeout_ReturnsTrueWhenTimedout);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutMsThenStopCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_ChronoNotInitializedStartTimeSCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_CallStartTimeSCheckIsTimeoutWithNullInput_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutSAndStopChronoAndCheckIsTimeout_ReturnsFalse);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutSAndCheckIsTimeout_ReturnsTrueWhenTimedout);
  RUN_TEST_CASE(Timeout, Chrono_TickOverflowAfterStartTimeoutSAndCheckIsTimeout_ReturnsTrueWhenTimedout);
  RUN_TEST_CASE(Timeout, Chrono_StartTimeoutSThenStopCheckIsTimeout_ReturnsFalse);

}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                              ##### Interval #####                                ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
static TEST_SETUP(Interval) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif
}

/**
 * @brief 
 * 
 */
static TEST_TEAR_DOWN(Interval) {

  fChrono_Init_();
  fChrono_Stop(&TestChrono);

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 0;
#else
  TickValue = CHRONO_TICK_TOP_VALUE;
#endif
}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_ChronoNotInitializedCallIntervalUs_ReturnsZero) {

  fChrono_DeInit_();
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalUs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_CallIntervalUsWithNullInput_ReturnsZero) {

  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalUs(NULL));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StopChronoAndCallIntervalUs_ReturnsZero) {

  fChrono_Stop(&TestChrono);
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalUs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StartChronoAndCallIntervalUs_ReturnsInterval) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalUs(&TestChrono));
  TickValue = 1 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  TickValue = 2 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(2, fChrono_IntervalUs(&TestChrono));
  TickValue = 20 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(20, fChrono_IntervalUs(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToUs, fChrono_IntervalUs(&TestChrono));
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalUs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (2 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(2, fChrono_IntervalUs(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 0) * TickToUs, fChrono_IntervalUs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToUs, fChrono_IntervalUs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StartChronoEachTimeAndCallIntervalUs_ReturnsIntervalFromStart) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalUs(&TestChrono));
  TickValue = 1 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = 2 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = 3 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalUs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (2 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (3 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_CallIntervalUsWhileTickIsOverflowed_ReturnsInterval) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 1 * UsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToUs, fChrono_IntervalUs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * UsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalUs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToUs, fChrono_IntervalUs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_ChronoNotInitializedCallIntervalMs_ReturnsZero) {

  fChrono_DeInit_();
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalMs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_CallIntervalMsWithNullInput_ReturnsZero) {

  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalMs(NULL));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StopChronoAndCallIntervalMs_ReturnsZero) {

  fChrono_Stop(&TestChrono);
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalMs(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StartChronoAndCallIntervalMs_ReturnsInterval) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalMs(&TestChrono));
  TickValue = 1 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  TickValue = 2 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(2, fChrono_IntervalMs(&TestChrono));
  TickValue = 20 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(20, fChrono_IntervalMs(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToMs, fChrono_IntervalMs(&TestChrono));
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalMs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (2 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(2, fChrono_IntervalMs(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 0) * TickToMs, fChrono_IntervalMs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToMs, fChrono_IntervalMs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StartChronoEachTimeAndCallIntervalMs_ReturnsIntervalFromStart) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalMs(&TestChrono));
  TickValue = 1 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = 2 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = 3 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalMs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (2 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (3 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_CallIntervalMsWhileTickIsOverflowed_ReturnsInterval) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 1 * MsToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToMs, fChrono_IntervalMs(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * MsToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalMs(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToMs, fChrono_IntervalMs(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_ChronoNotInitializedCallIntervalS_ReturnsZero) {

  fChrono_DeInit_();
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalS(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_CallIntervalSWithNullInput_ReturnsZero) {

  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalS(NULL));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StopChronoAndCallIntervalS_ReturnsZero) {

  fChrono_Stop(&TestChrono);
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalS(&TestChrono));

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StartChronoAndCallIntervalS_ReturnsInterval) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalS(&TestChrono));
  TickValue = 1 * SToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  TickValue = 2 * SToTick;
  TEST_ASSERT_EQUAL_HEX(2, fChrono_IntervalS(&TestChrono));
  TickValue = 20 * SToTick;
  TEST_ASSERT_EQUAL_HEX(20, fChrono_IntervalS(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToS, fChrono_IntervalS(&TestChrono));
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalS(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (2 * SToTick);
  TEST_ASSERT_EQUAL_HEX(2, fChrono_IntervalS(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 0) * TickToS, fChrono_IntervalS(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToS, fChrono_IntervalS(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_StartChronoEachTimeAndCallIntervalS_ReturnsIntervalFromStart) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalS(&TestChrono));
  TickValue = 1 * SToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = 2 * SToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = 3 * SToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
#else
  TEST_ASSERT_EQUAL_HEX(0, fChrono_IntervalS(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (2 * SToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  fChrono_Start(&TestChrono);
  TickValue = CHRONO_TICK_TOP_VALUE - (3 * SToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
TEST(Interval, Chrono_CallIntervalSWhileTickIsOverflowed_ReturnsInterval) {

#if (CHRONO_TICK_COUNTERMODE == TICK_COUNTERMODE_UP)
  TickValue = 1 * SToTick;
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  TickValue = 0;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToS, fChrono_IntervalS(&TestChrono));
#else
  TickValue = CHRONO_TICK_TOP_VALUE - (1 * SToTick);
  TEST_ASSERT_EQUAL_HEX(1, fChrono_IntervalS(&TestChrono));
  TickValue = CHRONO_TICK_TOP_VALUE;
  TEST_ASSERT_EQUAL_HEX((CHRONO_TICK_TOP_VALUE + 1) * TickToS, fChrono_IntervalS(&TestChrono));
#endif

}

/**
 * @brief 
 * 
 */
static TEST_GROUP_RUNNER(Interval) {

  RUN_TEST_CASE(Interval, Chrono_ChronoNotInitializedCallIntervalUs_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_CallIntervalUsWithNullInput_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_StopChronoAndCallIntervalUs_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_StartChronoAndCallIntervalUs_ReturnsInterval);
  RUN_TEST_CASE(Interval, Chrono_StartChronoEachTimeAndCallIntervalUs_ReturnsIntervalFromStart);
  RUN_TEST_CASE(Interval, Chrono_CallIntervalUsWhileTickIsOverflowed_ReturnsInterval);
  RUN_TEST_CASE(Interval, Chrono_ChronoNotInitializedCallIntervalMs_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_CallIntervalMsWithNullInput_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_StopChronoAndCallIntervalMs_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_StartChronoAndCallIntervalMs_ReturnsInterval);
  RUN_TEST_CASE(Interval, Chrono_StartChronoEachTimeAndCallIntervalMs_ReturnsIntervalFromStart);
  RUN_TEST_CASE(Interval, Chrono_CallIntervalMsWhileTickIsOverflowed_ReturnsInterval);
  RUN_TEST_CASE(Interval, Chrono_ChronoNotInitializedCallIntervalS_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_CallIntervalSWithNullInput_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_StopChronoAndCallIntervalS_ReturnsZero);
  RUN_TEST_CASE(Interval, Chrono_StartChronoAndCallIntervalS_ReturnsInterval);
  RUN_TEST_CASE(Interval, Chrono_StartChronoEachTimeAndCallIntervalS_ReturnsIntervalFromStart);
  RUN_TEST_CASE(Interval, Chrono_CallIntervalSWhileTickIsOverflowed_ReturnsInterval);

}

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
