# MISRA Compliance

The chrono library files conform to the [MISRA C:2012](https://www.misra.org.uk/misra-c)
guidelines, with the deviations listed below.

#### Rule 3.1

- This rule states that the character sequences /* an d // shall not be used within a comment.
  In the comment section of the chrono.c file, there are some explanations and code snippets for users
  to get started. This rule is violated to be able to generate a user manual within the file.

#### Rule 15.5

- This rule states that a function should have no more than one *return* statement.
  In this library functions return in the middle of the body whenever proper conditions for
  executing the code are not satisfied. This deviation from the standard rule is sanctioned in
  our coding style to enhance readability.

