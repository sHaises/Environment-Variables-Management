# Environment Variables Management in C  

This project implements basic functions for managing environment variables in C, including setting, retrieving, and clearing them. The implementation is inspired by how these functions are handled in standard libraries like `stdlib.h`.  

## Features  
- **CLEANENV**: Clears all environment variables.  
- **GETENV**: Retrieves the value of an environment variable.  
- **SETENV**: Sets an environment variable with a specified value.  
- **PUTENV**: Adds or modifies an environment variable.  

## How It Works  
- The `CLEANENV` function iterates through all environment variables and sets them to `NULL`, effectively removing them.  
- `GETENV` searches for a variable by name and returns its value if found, otherwise returns `NULL`.  
- `SETENV` allows creating or updating environment variables, ensuring proper memory management.  
- `PUTENV` parses a string in the format `NAME=VALUE` and adds the variable to the environment.  

## Compilation and Execution  
To compile and run the project, use:  

```sh
gcc -o app project.c
./app
```

## Expected Output  
```
TEST1 = Val1
TEST2 = Val2
TEST3 = Val3
AFTER CLEANENV:
TEST1 = (null)
TEST2 = (null)
TEST3 = (null)
```

## Challenges Faced  
The biggest challenge was understanding how `getenv`, `setenv`, `putenv`, and `cleanenv` work internally. Memory management issues led to several segmentation faults before optimizing memory allocation and handling.  

## References  
The implementation was inspired by standard C library functions. A useful reference was:  
- [Glibc getenv Implementation](https://github.com/lattera/glibc/blob/master/stdlib/getenv.c)  
