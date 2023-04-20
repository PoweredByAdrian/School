// eratosthenes.c
// Riešenie IJC-DU1, príklad b), 24.3.2021
// Autor: Adrián Horváth, FIT
// Preložene: gcc 10.2.1
// Hlavičkový súbor na výpis chybových hlášok


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...);

void error_exit(const char *fmt, ...);