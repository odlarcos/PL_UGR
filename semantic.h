#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TS 500
#define YYSTYPE attributes

/**********************************************************************
 *                          ESTRUCTURAS DE DATOS                      *
 **********************************************************************/

typedef struct {
   char *EtiquetaEntrada ;
   char *EtiquetaSalida ;
   char *EtiquetaElse ;
   char *NombreVarControl ;
} DescriptorDeInstrControl ;

typedef enum {
    mark,             /* marca comienzo bloque */
    function,         /* si es subprograma */
    variable,         /* si es variable */
    formal_parameter, /* si es parametro formal */
    condition,        /* si es parametro de control de alguna estructura
                         tipo if-else, bucle, etc. */
    condition_used    /* si el parametro de control ya ha sido usado */
} inType ;

typedef enum {
    integer,
    real,
    character,
    boolean,
    list,
    string,
    unknow,
    not_assigned
} dtype ;

typedef struct {
    inType                   entrada ;
    char                     *nombre ;
    dtype                    tipoDato ;
    dtype                    tipoDato2 ;
    unsigned int             parametros ;
    DescriptorDeInstrControl descriptor ;
} inTS ;

typedef struct {
    int   atrib ;   /* Atributo del símbolo (si tiene) */
    char  *lexema ; /* Nombre del lexema */
    dtype tipo ;    /* Tipo del símbolo */
    dtype tipo2 ;
    char  *variable ;
} attributes ;


/**********************************************************************
 *                           VARIABLES GLOBALES                       *
 **********************************************************************/

/* Fichero "generated.c" */
FILE *file ;
FILE *dec_fun ;
int ARCHIVO;
FILE* ficheros[100];

int MAX_archivo;

/* Pila de la Tabla de Símbolos */
inTS TS[MAX_TS] ;
/* Gestión de la Tabla de Símbolos */
int TOPE ;
bool inMain;

/* Gestión de los parámetros */
int nParam ;
dtype typeParams[10] ;
dtype typeParams2[10] ;

/* Tipos globales */
dtype globalType ;
dtype globalType2 ;

extern int line ;

int SubProg ;

int pos_currentFunction ;

int etiq ;
int temp ;

/**********************************************************************
 *                               FUNCIONES                            *
 **********************************************************************/

/* SETTERS */
void setType (attributes a) ;
void setTypeList (attributes a) ;
void setSubProg (int s) ;

/* MANEJO DE LA PILA DE LA TS */
void TS_InsertMark () ;                   /* Añade una marca de tope */
void TS_CleanIn () ;                      /* Limpia las entradas hasta el 
                                             primer inicio de bloque */
void TS_AddIn (inTS in) ;                 /* Inserta una entrada */
void TS_InsertVAR (attributes a) ;     /* Inserta identificador */
void TS_InsertPARAMF (attributes a) ;  /* Inserta parametros formales */
void TS_InsertSUBPROG (attributes a) ; /* Inserta sub-programa */

/* CHECK TIPO DATO */
bool isInteger (attributes a) ;
bool isReal (attributes a) ;
bool isCharacter (attributes a) ;
bool isBool (attributes a) ;
bool isList (attributes a) ;
bool isUnknow (attributes a) ;
bool isNotAssigned (attributes a) ;

/* GETTER */
dtype listType (attributes a) ;

/* FUNCIONES CON EXPRESIONES */
attributes orF (attributes a1, attributes a2) ;
attributes andF (attributes a1, attributes a2) ;
attributes unaryF (attributes a, attributes op) ;
attributes equalityF (attributes a1, attributes a2, attributes op) ;
attributes relationF (attributes a1, attributes a2, attributes op) ;
attributes addsubF (attributes a1, attributes a2, attributes op) ;
attributes multoperF (attributes a1, attributes a2, attributes op) ;
attributes incrementF (attributes a) ;
attributes decrementF1 (attributes a) ;
attributes decrementF2 (attributes a1, attributes a2) ;
attributes atF(attributes a1, attributes a2) ;
attributes attF(attributes a1, attributes a2, attributes a3) ;

/* LISTAS */
attributes listleftrightF (attributes a, attributes op) ;
attributes dollarF (attributes a, attributes op) ;

/* FUNCION PARA COPIAR ATRIBUTOS A OTRO OBJETO */
void assig (attributes * a1, attributes a2) ;

// Funcion para comprobar el return de una funcion
void checkReturn (attributes a) ;

int posFunction (attributes a, int mg) ;
int posVar (attributes a, int mg) ;
attributes tipoFunc (attributes a) ;
attributes tipoVariable (attributes a) ;
bool cmpTypes (attributes a1, attributes a2) ;

void UpdateParamaters (attributes a, int params) ;
void checkNParameters (attributes a, int params) ;
void checkTypeParameters (int pos, int params) ;
bool checkBool (attributes a) ;
bool checkExpSwitch (attributes a) ;

// Print de la pila
void print () ;

void openFile () ;
void closeFile () ;
void defineVar (attributes a) ;
void defineDefaultVar (attributes a) ;
void defineVarList (attributes a) ;

char * temporal () ;
char * etiqueta () ;

void UnaryOp (attributes * atb, attributes a, char * op) ;
void BinaryOp (attributes * atb, attributes a, attributes b, char * op) ;

void asignar (attributes a1, attributes a2) ;

/* ESTRUCTURA IF-THEN-ELSE */
void insert_ifthenelse () ;
void assertGotoElse (char * name) ;
void assertGotoEndAndElse () ;
void assertEnd () ;
char* searchElse () ;
char* searchEnd (int del) ;

/* ESTRUCTURA WHILE */
void insert_while () ;
void assertGotoOutWhile (char * name) ;
void assertInWhile () ;
void assertJumpIn () ;
DescriptorDeInstrControl searchInOutWhile (int del) ;

/* ESTRUCTURA CASES */
void insert_switch (char * name) ;
void insert_case () ;
void assertCase (char * valor, int cmp) ;
void assertGotoEnd();
void assertEndSwitch();
void destroyLastCase();
DescriptorDeInstrControl searchSwitch (int del) ;
char * searchInCase();
char * searchNextInCase();

void printInput (attributes a) ;
void printOutput (attributes a) ;

char* printFunc(attributes e);
char* printVariables(attributes a, attributes b);

void printReturn (attributes a) ;

/* SUBPROGRAMAS */
void newFunction (attributes a) ;
void addParam (attributes a, int first) ;
void addParentEnd () ;
void closeFiles (int i) ;