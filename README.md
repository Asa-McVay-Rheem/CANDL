# CANDL

Steps to take if settings need to be changed in STM32CubeMX so that project stays in cpp:

 * Make the desired changes
 * Generate code. This will make a main.c file separate from the original main.cpp.
 * Copy over #include statements from main.cpp to main.c
 * Copy any code from the StartDataCollect() and StartInternetServices() functions.
 * Remove main.cpp from the project and delete.
 * Rename main.c to main.cpp.
 * Add main.cpp to the project.