#include <stdio.h>
#include <stdlib.h>
#include "opciones.h"


int main(int argc, char* argv[]){
    char opcion;

    if(argc != 2){
        printf("Error, modo de uso: ./'nombre_programa' 'nombre_archivo'\n");
        return EXIT_FAILURE;
    }

    TCOLA cola;
    crearCola(&cola);

    leerArchivo(argv[1], cola);

    do{
        printf("----------GESTOR DE TAREAS----------\n"); //QUE PASARÍA SI PONGO UNA FECHA ANTERIOR AL DÍA ACTUAL
        printf("a. Añadir tarea\n");
        printf("b. Ver tareas pendientes\n");
        printf("c. Eliminar tarea\n");
        printf("s. Salir\n");
        printf("------------------------------------\n");
        scanf(" %c", &opcion);
        
        switch(opcion){
            case 'a':
                anhadirTarea(cola);
            break;
            case 'b':
                verTareas(cola);
            break;
            case 'c':
                eliminarTarea(cola);
            break;
        }
    }while(opcion != 's');

    imprimir(&cola);
    
    guardarArchivo(argv[1], cola);

    destruirCola(&cola);

    return EXIT_SUCCESS;
}