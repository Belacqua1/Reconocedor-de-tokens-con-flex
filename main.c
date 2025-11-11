#include <stdio.h>

extern int yylex();
extern FILE *yyin;

int main() {
    char seleccion_usuario;
    char buffer_linea[512];

    while (1) {
        // menu de opciones
        printf("\n*** Herramienta de Analisis Lexico ***\n");
        printf(" A - Ejecutar analisis desde el archivo: %s\n", NOMBRE_ARCHIVO_ENTRADA);
        printf(" B - Iniciar modo de entrada en linea (interactivo)\n");
        printf(" C - Terminar el programa\n");
        printf("**************************************\n");
        printf("Ingrese su seleccion: ");

        // Lectura de la opción
        if (scanf(" %c", &seleccion_usuario) != 1) {
            // Manejo de error de scanf
            while (getchar() != '\n'); // Limpiar buffer en caso de error
            continue;a
        }
        // Asegurar que el buffer se limpie despues de la lectura del char
        while (getchar() != '\n');

        // --- Manejo de la opción 'A' (Análisis de archivo) ---
        if (seleccion_usuario == 'A' || seleccion_usuario == 'a') {
            yyin = fopen(NOMBRE_ARCHIVO_ENTRADA, "r");
            if (yyin == NULL) {
                perror("FALLO al abrir el archivo de comandos " NOMBRE_ARCHIVO_ENTRADA);
                continue;
            }
            printf("\n--- Iniciando el procesamiento de %s ---\n", NOMBRE_ARCHIVO_ENTRADA);
            yylex();
            fclose(yyin);
            printf("--- El procesamiento del archivo ha finalizado ---\n");
        }
        // --- Manejo de la opción 'B' (Modo interactivo) ---
        else if (seleccion_usuario == 'B' || seleccion_usuario == 'b') {
            printf("\n>> MODO INTERACTIVO ACTIVO <<\n");
            printf("Ingrese sentencias (deje una linea vacia y presione Enter para volver al menu):\n");
            
            // Abrir el archivo de entrada principal en modo 'append' para guardar las líneas ingresadas
            FILE *archivo_log = fopen(NOMBRE_ARCHIVO_ENTRADA, "a"); 
            if (archivo_log == NULL) {
                perror("No se pudo acceder al log " NOMBRE_ARCHIVO_ENTRADA " para escritura");
                continue;
            }

            while (1) {
                printf("CMD > ");
                // Usar fgets para leer la línea de entrada
                if (fgets(buffer_linea, sizeof(buffer_linea), stdin) == NULL || buffer_linea[0] == '\n') {
                    // Si fgets falla o se ingresa una línea vacía
                    break;
                }

                // 1. Guardar en el archivo de log (commands.sql)
                fprintf(archivo_log, "%s", buffer_linea);
                fflush(archivo_log); // Forzar la escritura inmediata

                // 2. Analizar la línea de manera inmediata
                
                // Escribir la línea en un archivo temporal para que Flex la pueda leer
                FILE *flujo_temp = fopen(NOMBRE_ARCHIVO_TEMP, "w");
                if (flujo_temp == NULL) {
                    perror("Error: No se puede crear el archivo temporal " NOMBRE_ARCHIVO_TEMP);
                    // No hacemos break, pero el analisis de esta linea se omite
                    continue; 
                }
                fprintf(flujo_temp, "%s", buffer_linea);
                fclose(flujo_temp);

                // Configurar yyin para leer del archivo temporal y ejecutar el análisis
                yyin = fopen(NOMBRE_ARCHIVO_TEMP, "r");
                if (yyin != NULL) {
                    yylex();
                    fclose(yyin);
                } else {
                    perror("Error: Fallo la lectura del archivo temporal para analisis");
                }
                
                // Opcional: Eliminar el archivo temporal
                // remove(NOMBRE_ARCHIVO_TEMP); 
            }

            fclose(archivo_log);
            printf(">> Retornando al Menu Principal <<\n");
        }
        // --- Manejo de la opción 'C' (Salir) ---
        else if (seleccion_usuario == 'C' || seleccion_usuario == 'c') {
            printf("Cerrando la herramienta de analisis. ¡Hasta pronto!\n");
            break;
        }
        // --- Manejo de opción inválida ---
        else {
            printf("ERROR: '%c' no es una opcion valida. Por favor, seleccione A, B o C.\n", seleccion_usuario);
        }
    }

    return 0;
}