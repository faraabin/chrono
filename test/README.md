Some of the chrono configurations depend on the user definitions in chrono_config.h header file.
For running tests in all cases:

1- Consider both TICK_TYPE_VARIABLE and TICK_TYPE_FUNCTION tick types.
2- Consider both TICK_COUNTERMODE_UP and TICK_COUNTERMODE_DOWN counter modes.
3- Choose a CHRONO_TICK_TO_NANOSECOND_COEF so that following divisions are zero in one case and non zero in other cases:
  - (1000 / CHRONO_TICK_TO_NANOSECOND_COEF)
  - (1000000 / CHRONO_TICK_TO_NANOSECOND_COEF)
  - (1000000000 / CHRONO_TICK_TO_NANOSECOND_COEF)
4- Tests depend on assertions, so use CHRONO_USE_FULL_ASSERT in all tests.