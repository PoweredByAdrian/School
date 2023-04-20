// eratosthenes.c
// Riešenie IJC-DU1, príklad b), 24.3.2021
// Autor: Adrián Horváth, FIT
// Preložene: gcc 10.2.1
// Hlavičkový súbor na výpis chybových hlášok


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void warning_msg(const char *fmt, ...){
    va_list ap;
    fprintf(stderr,"CHYBA: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr,"\n");
}

void error_exit(const char *fmt, ...){
    va_list ap;
    fprintf(stderr,"CHYBA: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr,"\n");
    exit(1);
}