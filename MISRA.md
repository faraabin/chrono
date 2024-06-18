# MISRA Compliance

The chrono library files conform to the [MISRA C:2012](https://www.misra.org.uk/misra-c)
guidelines, with the deviations listed below.

#### Rule 15.5

_Ref 15.5_

- This rule states that a function should have no more than one *return* statement.
  In this library functions return in the middle of the body whenever proper conditions for
  executing the code are not satisfied. This can be dealt with lots of if else statements
  inside the function body.

#### Rule 1.3

_Ref 1.3_

- This rule states that there shall be no occurence of undefined or critical unspecified behaviour.
  In some functions, pointer to a local variable is passed to another function. This is not a violation
  of Rule 1.3 since taking the address of a local variable is not C undefined behaviour.
