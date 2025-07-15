#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"
#include "func.h"

void anhadirTarea(TCOLA q){
    char* nombre = (char*) malloc(50 * sizeof(char)); //reserva memoria para 50 caracteres
    int hora, dia, mes, anho;

    printf("Escriba el nombre de la tarea: \n");
    scanf(" %[^\n]", nombre);
    printf("Escriba la fecha límite para la tarea (h, d, m, a): \n");
    scanf("%d %d %d %d", &hora, &dia, &mes, &anho);

    if(!comprobarFecha(hora, dia, mes, anho) && !comprobarNombre(nombre, q)){
        int prio = calcularPrio(hora, dia, mes, anho);

        if(prio != 0){
            TIPOELEMENTOCOLA e;
            strcpy(e.tarea, nombre);
            e.prioridad = prio;

            anadirElementoCola(&q, e);
            printf("Tarea añadida\n");
        }
    }
}

void verTareas(TCOLA q){
    
    if(esColaVacia(q)){
        printf("No hay tareas pendientes\n");
    }else{
        imprimir(&q);
    }
}

void eliminarTarea(TCOLA q){

    if(esColaVacia(q)){
        printf("Error, no hay tareas pendientes\n");
    }else{
        char* nombre = (char*)malloc (50 * sizeof(char));

        printf("Escriba el nombre de la tarea a eliminar: \n");
        scanf(" %[^\n]", nombre);

        TCOLA aux;
        crearCola(&aux);

        TIPOELEMENTOCOLA e;

        while(!esColaVacia(q)){ //mientras no se acaben de pasar de una cola a otra las tareas o mientras no se encuentre el nombre,
                                //seguir iterando por la cola
            consultarPrimerElementoCola(q, &e);
            suprimirElementoCola(&q);
            if(e.tarea != NULL && strcmp(nombre, e.tarea) == 0){
                break;
            }
            anadirElementoCola(&aux, e);
        }

        if(esColaVacia(q) && (strcmp(nombre, e.tarea) != 0)){ //si se vacía por completo y el último elemento no coincide con el nombre de
                                                              //la tarea a eliminar, se imprime un error
            printf("Error, no se ha encontrado la tarea a eliminar\n");
        }else{
            printf("Tarea eliminada\n");
        }

        while(!esColaVacia(aux)){ //se devuelven todas las tareas a la cola original
            consultarPrimerElementoCola(aux, &e);
            suprimirElementoCola(&aux);
            anadirElementoCola(&q, e);
        }
        destruirCola(&aux);
    }
}