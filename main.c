#include <stdio.h>

/* Declaraciones que vienen de Flex */
int yylex(void);
extern FILE *yyin;

int main(int argc, char **argv) {
    /* Si se pasa un archivo como argumento, lo abrimos y analizamos */
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("No se pudo abrir el archivo de entrada");
            return 1;
        }

        printf("Analizando archivo: %s\n\n", argv[1]);
        yylex();   /* Llama al analizador léxico generado por Flex */

        fclose(yyin);
    } else {
        /* Modo interactivo por stdin */
        printf("Analizador léxico SQL (modo interactivo)\n");
        printf("Escribe comandos SQL y presiona Ctrl+D (Linux/macOS) o Ctrl+Z y Enter (Windows) para terminar.\n\n");

        yyin = stdin;
        yylex();
    }

    return 0;
}
