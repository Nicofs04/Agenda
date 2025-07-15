#ifndef FUNC_H
#define FUNC_H
#include "cola.h"

int comprobarFecha(int hora, int dia, int mes, int anho);

int calcularPrio(int hora, int dia, int mes, int anho);

int leerArchivo(char* nombreArchivo, TCOLA cola);

int guardarArchivo(char* nombreArchivo, TCOLA cola);

int comprobarNombre(char* nombre, TCOLA cola);

#endif //FUNC_H