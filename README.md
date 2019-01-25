# PL_UGR
Design of a compiler for a C based language (Lexical, Syntax and Semantic Analysis + Intermediate Code Generation to C)

(YOU NEED: FLEX and BISON)

Especificación del lenguaje:

- Permite definición de procedimientos anidados (no permite funciones externas como C)

- Estructura: Cabecera programa + declaracion de variables (declare) + declaracion procedimientos + cuerpo programa

- Cuerpos programa: Estructuras if/else, while, switch como en C++, operaciones algebraicas C, print, scan

Observar los distintos ejemplos proporcionados en la carpeta "programas"

USO:
- make ~ Genera el ejecutable "analize"
- ./analysis programas/<name_file> ~ Informa de los errores lexico, sintactico, semantico y crea "generated.c" (Codigo intermedio)
- gcc generated.c ~ Compilar el codigo intermedio (Codigo C similar a emsamblador, para mayor simplicidad)
- ./a.out ~ Ejecutar
