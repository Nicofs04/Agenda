#include <stdio.h>
#include <stdlib.h>
#include "cola.h"
#include <time.h>
#include <string.h>

int comprobarFecha(int hora, int dia, int mes, int anho){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    //se comprueba que la fecha no sea pasada, las sumas se deben a que en la struct tm, los años se cuentan desde 1900 y los meses de 0 a 11
    if((anho < (tm.tm_year + 1900)) || (anho == (tm.tm_year + 1900) && mes < (tm.tm_mon + 1))
    || (anho == (tm.tm_year + 1900) && mes == (tm.tm_mon + 1) && dia < tm.tm_mday)
    || (anho == (tm.tm_year + 1900) && mes == (tm.tm_mon + 1) && dia == tm.tm_mday && hora < tm.tm_hour)){
        printf("Error, datos incorrectos\n");
        return 1;
    }

    //se comprueba que los días y meses sean correctos
    if(mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12){
        if(dia < 0 || dia > 31){
            printf("Error, datos incorrectos\n");
            return 1;
        }
    }else if(mes == 4 || mes == 6 || mes == 9 || mes == 11){
        if(dia < 0 || dia > 30){
            printf("Error, datos incorrectos\n");
            return 1;
        }
    }else if(mes == 2){
        if((anho % 4 == 0 && anho % 100 != 0) || (anho % 400 == 0)){
            if(dia < 0 || dia > 29){
                printf("Error, datos incorrectos\n");
                return 1;
            }
        }else{
            if(dia < 0 || dia > 28){
                printf("Error, datos incorrectos\n");
                return 1;
            }
        }
    }else{
        printf("Error, datos incorrectos\n");
        return 1;
    }

    //se comprueba que la hora sea correcta
    if(hora < 0 || hora > 24){
        printf("Error, datos incorrectos\n");
        return 1;
    }

    return 0;
}

//menos de 3 días -> prio 1
//más de 3 días y menos de 1 semana -> prio 2
//más de 1 semana -> prio 3
int calcularPrio(int hora, int dia, int mes, int anho){
    int prio = 0;
    
    struct tm fechaObjetivo = {0};
    fechaObjetivo.tm_year = anho - 1900;
    fechaObjetivo.tm_mon = mes - 1;
    fechaObjetivo.tm_mday = dia;
    fechaObjetivo.tm_hour = hora;

    time_t tiempoObjetivo = mktime(&fechaObjetivo);
    if(tiempoObjetivo == -1){
        printf("Error, no se ha podido convertir a hora\n");
        return prio;
    }

    time_t t = time(NULL);
    if(t == -1){
        printf("Error a la hora de obtener la hora actual\n");
        return prio;
    }

    double diferenciaSegundos = tiempoObjetivo - t;
    double diferenciaHoras = diferenciaSegundos / 3600.0;

    if(diferenciaHoras < 72){
        prio = 1;
    }else if(diferenciaHoras >= 72 && diferenciaHoras < 168){
        prio = 2;
    }else{
        prio = 3;
    }

    return prio;
}

int leerArchivo(char* nombreArchivo, TCOLA cola){
    FILE* archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL){
        printf("Error, no se ha podido abrir el archivo\n");
        return EXIT_FAILURE;
    }

    char* cadenaAux = (char*) malloc(50 * sizeof(char)); //reserva memoria para 50 caracteres
    char* nombreTareaAux = (char*) malloc(50 * sizeof(char)); //reserva memoria para 50 caracteres
    int prioAux;
    TIPOELEMENTOCOLA e;

    while(fgets(cadenaAux, 50, archivo) != NULL){
        sscanf(cadenaAux, "%[^,], %d", nombreTareaAux, &prioAux);
        strcpy(e.tarea, nombreTareaAux);
        e.prioridad = prioAux;

        anadirElementoCola(&cola, e);
    }

    fclose(archivo);
    return EXIT_SUCCESS;
}

int guardarArchivo(char* nombreArchivo, TCOLA cola){
    FILE* archivo = fopen(nombreArchivo, "w");
    if(archivo == NULL){
        printf("Error, no se ha podido abrir el archivo\n");
        return EXIT_FAILURE;
    }

    TIPOELEMENTOCOLA e;
    while(!esColaVacia(cola)){
        consultarPrimerElementoCola(cola, &e);
        suprimirElementoCola(&cola);

        fprintf(archivo, "%s, %d\n", e.tarea, e.prioridad);
    }

    fclose(archivo);
    return EXIT_SUCCESS;
}

int comprobarNombre(char* nombre, TCOLA cola){
    int resul = 0;
    TCOLA aux;
    TIPOELEMENTOCOLA e;

    crearCola(&aux);

    while(!esColaVacia(cola)){ //se trasladan las tareas de una cola a otra hasta que la cola inicial esté vacía o se encuentre una tarea con un
                               //nombre repetido
        consultarPrimerElementoCola(cola, &e);
        suprimirElementoCola(&cola);
        anadirElementoCola(&aux, e);

        if(strcmp(nombre, e.tarea) == 0){
            resul = 1;
            break;
        }
    }

    while(!esColaVacia(aux)){ //si no se encuentra un nombre de una tarea repetido o si la cola inicial se vacía, se devuelven las tareas a
                              //la cola inicial
        consultarPrimerElementoCola(aux, &e);
        suprimirElementoCola(&aux);
        anadirElementoCola(&cola, e);
    }
    destruirCola(&aux);

    if(resul != 0){ //si resul es distinto de 0, ya existe una tarea con dicho nombre. Si no, no existe y se devuelve EXIT_SUCCESS para permitir
                    //añadir la tarea a la cola
        printf("No se ha podido añadir la tarea, ya existe una tarea con dicho nombre\n");
        return EXIT_FAILURE;
    }else{
        return EXIT_SUCCESS;
    }
}