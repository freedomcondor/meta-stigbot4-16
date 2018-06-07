#include <iostream>

extern int16_t targetLeftSpeed, targetRightSpeed;
extern int16_t fbLeftSpeed, fbRightSpeed;
extern int16_t fbLeftError, fbRightError;
extern float fbLeftErrorInt, fbRightErrorInt;
extern int16_t fbLeftErrorDev, fbRightErrorDev;
extern float targetKp, targetKi, targetKd;

/////////////////// functions //////////////////
int function_init();
int function_step();
int function_exit();
