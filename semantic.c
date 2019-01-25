#include "semantic.h"

/*********************************************************************
 *                              SETTERS                              *
 *********************************************************************/

void setType (attributes a) {
  globalType = a.tipo ;
  globalType2 = not_assigned ;
}

void setTypeList (attributes a) {
  globalType = list ;
  globalType2 = a.tipo ;
}

void setSubProg (int s) {
  SubProg = s ;
}

/*********************************************************************
 *                    MANEJO DE LA PILA DE LA TS                     *
 *********************************************************************/

void TS_InsertMark () {
    inTS input ;
    input.entrada = mark ;
    input.nombre = "{" ;
    input.tipoDato = not_assigned ;
    input.tipoDato2 = not_assigned ;
    input.parametros = 0 ;
    TS_AddIn(input) ;
    if ( SubProg == 1 ) {
  		int j = TOPE - 2 ; // Saltamos la posicion vacia y la mark
  		while ( j > 0 && TS[j].entrada == formal_parameter ) { // Meto los formal_parameters como variable
  			inTS newIn ;
  			newIn.entrada = variable ;
  			newIn.nombre = TS[j].nombre ;
  			newIn.tipoDato = TS[j].tipoDato ;
        newIn.tipoDato2 = TS[j].tipoDato2 ;
  			newIn.parametros = TS[j].parametros ;
  			TS_AddIn(newIn) ;
  			j-- ;
		  }
	 }
}

void TS_CleanIn () {
  int mark_not_found = 0 ;
  TOPE-- ; // Lo coloco en la pos llena
  while(TOPE >= 0 && (mark_not_found == 0)) { // me salto la marca
    if(TS[TOPE].entrada == mark){
      mark_not_found = 1 ;
    }
    TOPE-- ;
  }
  if ( TOPE > 0 ) { // Estoy despues de la marca
    TOPE++ ; // para dejarlo en la pos vacia sin pisar function ni formal_parameter
  } else if ( TOPE < 0 ) TOPE = 0 ;
}

void TS_AddIn (inTS in) {
    if ( TOPE < MAX_TS ) {
      TS[TOPE].entrada = in.entrada ;
  		TS[TOPE].nombre = in.nombre ;
  		TS[TOPE].tipoDato = in.tipoDato ;
  		TS[TOPE].parametros = in.parametros ;
  		TS[TOPE].tipoDato2 = in.tipoDato2 ;
      TS[TOPE].descriptor = in.descriptor ;
      TOPE++;
    } else {
      printf("Semantic Error(%d): Stack overflow", line) ;
    }
}

void TS_InsertVAR (attributes a) {
  int j = TOPE - 1, found = 0 ;
  if ( j >= 0 ) {
    // Mientras sea distinto del inicio del bloque (dentro del ámbito)
    while ( (TS[j].entrada != mark) && (j >= 0) && found == 0 ) {
      if ( strcmp(TS[j].nombre, a.lexema) != 0 ) j-- ; // si encuentro otra cosa con el mismo nombre(sea funcion o variable imagino)
      else {
        found = 1 ;
        printf("Semantic Error(%d): Exist ident: %s\n", line, a.lexema) ;
      }
    }
    // Si no se ha encontrado significa que no existe, por lo que se añade
    // a la pila
    if ( found == 0 ) {
      inTS newIn ;
      newIn.entrada = variable ;
      newIn.nombre = a.lexema ;
      newIn.tipoDato = globalType ;
      newIn.parametros = 0 ;
      newIn.tipoDato2 = globalType2 ;
      TS_AddIn(newIn) ;
    }
  }
}

void TS_InsertPARAMF (attributes a) {
  int j = TOPE - 1, found = 0 ;
  // Va atrás en la pila hasta llegar a currentFunction
  while ( (j != pos_currentFunction)  && (!found) ) {
    if ( strcmp(TS[j].nombre, a.lexema) != 0 ) j-- ; // Si el parámetro ya estaba definido
    else {
      found = 1;
      printf("Semantic Error(%d): Exist param: %s\n", line, a.lexema) ; // error
    }
  }
  if ( ! found ) { // Si no estaba definido, lo insertamos
    inTS newIn ;
    newIn.entrada = formal_parameter ;
    newIn.nombre = a.lexema ;
    newIn.tipoDato = globalType ;
    newIn.tipoDato2 = globalType2 ;
    newIn.parametros = 0 ;
    TS_AddIn(newIn) ;
  }
}

void TS_InsertSUBPROG (attributes a) {
    inTS newIn ;
    newIn.entrada = function ;
    newIn.nombre = a.lexema ;
    newIn.tipoDato = globalType ; // TODO: tipoDato == globalType?? para asignarle tipo y comprobar con lo q devuelva
    newIn.tipoDato2 = globalType2 ;
    newIn.parametros = 0 ;
    pos_currentFunction = TOPE ;
    TS_AddIn(newIn) ;
}

/*********************************************************************
 *                          CHECK TIPO DATO                          *
 *********************************************************************/

bool isInteger (attributes a) {
  if ( a.tipo == integer ) return (true) ;
  if ( a.atrib == 0 ) return (true) ;
  int posvar = posVar(a,0) ;
  if (posvar != -1)
    if (TS[posvar].tipoDato == integer)
      return (TS[posvar].tipoDato2 == not_assigned) ;
  return (false) ;
}

bool isReal (attributes a) {
  if ( a.tipo == real ) return (true) ;
  if ( a.atrib == 3 )  return (true) ;
  int posvar = posVar(a,0) ;
  if ( posvar != -1 )
    if ( TS[posvar].tipoDato == real )
      return ( TS[posvar].tipoDato2 == not_assigned ) ;
  return (false) ;
}

bool isCharacter (attributes a) {
  if ( a.tipo == character ) return (true) ;
  if ( a.atrib == 1 ) return (true) ;
  int posvar = posVar(a,0) ;
  if ( posvar != -1 )
    if ( TS[posvar].tipoDato == character )
      return ( TS[posvar].tipoDato2 == not_assigned ) ;
  return (false) ;
}

bool isBool (attributes a) {
  if ( a.tipo == boolean ) return (true) ;
  if ( a.atrib == 4 )  return (true) ;
  int posvar = posVar(a,0) ;
  if ( posvar != -1 )
    if ( TS[posvar].tipoDato == boolean )
      return ( TS[posvar].tipoDato2 == not_assigned ) ;
  return (false) ;
}

bool isList (attributes a) {
  if ( a.tipo == list ) return (true) ;
  int posvar = posVar(a,0) ;
  if ( posvar != -1 )
    return ( TS[posvar].tipoDato == list ) ;
  return (false) ;
}

bool isUnknow (attributes a) {
  if ( a.tipo == unknow ) return (true);
  int posvar = posVar(a,0) ;
  if ( posvar != -1 )
    if ( TS[posvar].tipoDato == unknow )
      return ( TS[posvar].tipoDato2 == not_assigned ) ;
  return (false) ;
}

bool isNotAssigned (attributes a) {
  int posvar = posVar(a,0) ;
  if ( posvar != -1 )
    if ( TS[posvar].tipoDato == not_assigned )
      return ( TS[posvar].tipoDato2 == not_assigned ) ;
  return (false) ;
}

/*********************************************************************
 *                              GETTER                               *
 *********************************************************************/

dtype listType (attributes a) {
  int posvar = posVar(a,0) ;
  if ( posvar != -1 )
    return ( TS[posvar].tipoDato2 ) ;
  return (not_assigned) ;
}

/*********************************************************************
 *             FUNCIONES DE EXPRESIONES + (C. Intermedio)            *
 *********************************************************************/

attributes orF (attributes a1, attributes a2) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isBool(a1) && isBool(a2) ) atb.tipo = boolean ;
  BinaryOp(&atb, a1, a2, " || ");
  return (atb) ;
}

attributes andF (attributes a1, attributes a2) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isBool(a1) && isBool(a2) ) atb.tipo = boolean ;
  BinaryOp(&atb, a1, a2, " && ");
  return (atb) ;
}

attributes unaryF (attributes a, attributes op) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isBool(a) && op.atrib == 0 ) {
    atb.tipo = boolean ;
    UnaryOp(&atb, a, "!") ;
  }
  return (atb) ;
}

attributes equalityF (attributes a1, attributes a2, attributes op) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isInteger(a1) && isInteger(a2) ) atb.tipo = boolean ;
  if ( isReal(a1) && isReal(a2) ) atb.tipo = boolean ;
  if ( isCharacter(a1) && isCharacter(a2) ) atb.tipo = boolean ;
  if ( isBool(a1) && isBool(a2) ) atb.tipo = boolean ;
  if ( isList(a1) && isList(a2) ) atb.tipo = boolean ;
  if ( op.atrib == 0 ) BinaryOp(&atb, a1, a2, " == ") ;
  else if ( op.atrib == 1 ) BinaryOp(&atb, a1, a2, " != ") ;
  return (atb) ;
}

attributes relationF (attributes a1, attributes a2, attributes op) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isInteger(a1) && isInteger(a2) ) atb.tipo = boolean ;
  if ( isReal(a1) && isReal(a2) ) atb.tipo = boolean ;
  if ( isCharacter(a1) && isCharacter(a2) ) atb.tipo = boolean ;
  if ( op.atrib == 0) BinaryOp(&atb, a1, a2, " < ") ;
  else if ( op.atrib == 1 ) BinaryOp(&atb, a1, a2, " > ") ;
  else if ( op.atrib == 2 ) BinaryOp(&atb, a1, a2, " >= ") ;
  else if ( op.atrib == 3 ) BinaryOp(&atb, a1, a2, " <= ") ;
  return (atb) ;
}

attributes addsubF (attributes a1, attributes a2, attributes op) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isList(a1) && isInteger(a2) ) {
    atb.tipo = list ;
    atb.tipo2 = a1.tipo2 ;
  } else if ( isList(a2) && isInteger(a1) ) {
    atb.tipo = list ;
    atb.tipo2 = a1.tipo2 ;
  } else if ( !isList(a1) && !isList(a2) ) {
    if ( a1.tipo == a2.tipo ) atb.tipo = a1.tipo ;
    atb.tipo2 = not_assigned ;
  }
  if ( op.atrib == 0 ) BinaryOp(&atb, a1, a2, " + ") ;
  else if ( op.atrib == 1 ) BinaryOp(&atb, a1, a2, " - ") ;
  return (atb) ;
}

attributes multoperF (attributes a1, attributes a2, attributes op) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isList(a1) && isInteger(a2) ) {
    atb.tipo = list ;
    atb.tipo2 = a1.tipo2 ;
  } else if ( isList(a2) && isInteger(a1)){
    atb.tipo = list ;
    atb.tipo2 = a1.tipo2 ;
  } else if ( !isList(a1) && !isList(a2) ) {
    if ( a1.tipo == a2.tipo ) atb.tipo = a1.tipo ;
    atb.tipo2 = not_assigned ;
  }
  if ( op.atrib == 0 ) BinaryOp(&atb, a1, a2, " * ") ;
  else if ( op.atrib == 1 ) BinaryOp(&atb, a1, a2, " / ") ;
  return (atb) ;
}

attributes incrementF (attributes a) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isInteger(a) ) atb.tipo = integer ;
  UnaryOp(&atb, a, "++") ;
  return (atb) ;
}

attributes decrementF1 (attributes a) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isInteger(a) ) atb.tipo = integer ;
  UnaryOp(&atb, a, "--") ;
  return (atb) ;
}

attributes decrementF2 (attributes a1, attributes a2) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isList(a1) && isInteger(a2) ) {
    atb.tipo = list ;
    atb.tipo2 = a1.tipo2 ;
  }
  return (atb) ;
}

attributes atF (attributes a1, attributes a2) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isList(a1) && isInteger(a2) ) atb.tipo = a1.tipo2 ;
  return (atb) ;
}

attributes attF (attributes a1, attributes a2, attributes a3) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  if ( isList(a1) ) {
    if ( isInteger(a3) && a2.tipo == a1.tipo2 ) {
      atb.tipo = list ;
      atb.tipo2 = a1.tipo2 ;
    }
  }
  return (atb) ;
}

/*********************************************************************
 *            FUNCION PARA COPIAR ATRIBUTOS A OTRO OBJETO            *
 *********************************************************************/

void assig (attributes * a1, attributes a2) {
  (*a1).atrib = a2.atrib ;
  (*a1).tipo = a2.tipo ;
  (*a1).tipo2 = a2.tipo2 ;
  (*a1).variable = a2.lexema;
}

/*********************************************************************
 *            MAS COMPROBACIONES SEMANTICAS (srry 4 chaos)           *
 *********************************************************************/

void checkReturn (attributes a) {
  int i = TOPE - 1 ;
  do{
  while ( TS[i].entrada != mark && i > 0 ) i-- ; i-- ;
  while ( TS[i].entrada == formal_parameter && i > 0 ) i-- ;
  }while(TS[i].entrada != function);
  if ( i == -1) {
    if ( (a.tipo != integer) || (a.tipo2 != not_assigned) )
      printf("Semantic Error(%d): return dont match function main\n", line) ;
  } else if ( (TS[i].tipoDato != a.tipo) || (TS[i].tipoDato2 != a.tipo2) )
    printf("Semantic Error(%d): return dont match function %s\n", line, TS[i].nombre);
}

int posFunction (attributes a, int mg) {
  int i = TOPE - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( (strcmp(TS[i].nombre, a.lexema) == 0) )
      if ( TS[i].entrada == function ) pos = i ;
    i-- ;
  }
  if ( pos == -1 && mg ) 
    printf("Semantic Error(%d): Function %s not defined or out of scope\n", line, a.lexema) ;
  return pos ;
}

int posVar (attributes a, int mg) {
  int i = TOPE - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( (strcmp(TS[i].nombre, a.lexema) == 0) && TS[i].entrada == variable ) {
      pos = i ;
    }
    i-- ;
  }
  if ( pos == -1 && mg )
    printf("Semantic Error(%d): Var %s not defined or out of scope \n", line, a.lexema) ;
  return (pos) ;
}

attributes tipoFunc (attributes a) {
  attributes atb ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  int pos = posFunction(a,1) ;
  if ( pos != -1 ) {
    atb.tipo = TS[pos].tipoDato ;
    atb.tipo2 = TS[pos].tipoDato2 ;
  }
  return (atb) ;
}

attributes tipoVariable (attributes a) {
  attributes atb ;
  atb.lexema = a.lexema ;
  atb.tipo = not_assigned ;
  atb.tipo2 = not_assigned ;
  int pos = posVar(a,1) ;
  if ( pos != -1 ) {
    atb.tipo = TS[pos].tipoDato ;
    atb.tipo2 = TS[pos].tipoDato2 ;
  }
  return (atb) ;
}

bool cmpTypes(attributes a1, attributes a2) {
  attributes i = tipoVariable(a1) ;
  if ( (i.tipo != a2.tipo) || (i.tipo2 != a2.tipo2) )
    printf("Semantic Error(%d): Incompatible types %i and %d \n", line, i.tipo, a2.tipo) ;
  return ((i.tipo == a2.tipo) && (i.tipo2 == a2.tipo2)) ;
}

void UpdateParamaters (attributes a, int params) {
  TS[pos_currentFunction].parametros = params ;
}

void checkNParameters (attributes a, int params) {
  int pos = posFunction(a,0) ;
  if ( pos != -1 ) {
    if ( TS[pos].parametros != params ) {
      printf("Semantic Error(%d): Incorrect number of parameters in %s\n", line, a.lexema) ;
    } else {
      checkTypeParameters(pos, params) ;
    }
  }
}

void checkTypeParameters (int pos, int params) {
  for (int i = 0 ; i < params ; i++ ) {
    if ( (typeParams[i] != TS[pos+i+1].tipoDato) || (typeParams2[i] != TS[pos+i+1].tipoDato2) ) {
      printf("Semantic Error(%d): Incorrect parameter %i. Get %u, expected %u\n", line, i+1, typeParams[0], TS[pos+i+1].tipoDato) ;
    }
  }
}

bool checkBool (attributes a) {
  if ( a.tipo != boolean || a.tipo2 != not_assigned ) {
    printf("Semantic Error(%d): not boolean expression\n", line) ;
    return (false) ;
  }
  return (true) ;
}

bool checkExpSwitch (attributes a) {
  bool correcto = false ;
  if ( isInteger(a) ) correcto = true ;
  else if ( isCharacter(a) ) correcto = true ;
  if ( !correcto )
    printf("Semantic Error(%d): not integer or character expression\n", line) ;
  return (correcto) ;        
}

void print () {
  for ( int i = 0 ; i < TOPE ; i++ ) {
    printf("%d ", TS[i].entrada) ;
    printf("%s ", TS[i].nombre) ;
    printf("%d\n", TS[i].tipoDato) ;
  }
}

/************************************************************
*************************************************************
            GENERACIÓN DE CÓDIGO INTERMEDIO
*************************************************************
************************************************************/


char * temporal () {
  char * cadena ;
	cadena = (char *) malloc(20) ;
  sprintf(cadena, "temp%d", temp) ;
	temp++ ;
  return (cadena) ;
}

char * etiqueta () {
	char * cadena ;
	cadena = (char *) malloc(20) ;
	sprintf(cadena, "etiqueta_%d", etiq) ;
	etiq++ ;
	return (cadena) ;
}

void fichero(){

  FILE * fichero;
  char* nombre_fichero;
  nombre_fichero = (char *) malloc(20);
  sprintf(nombre_fichero, "aux_file_%d",ARCHIVO);
  strcat(nombre_fichero, ".txt");
  fichero = fopen(nombre_fichero, "w");
  ficheros[ARCHIVO] = fichero;
}

// Abrir fichero
void openFile () {
  file = fopen("generated.c","w") ;
	fputs("#include <stdio.h>\n",file) ;
  fputs("#include <stdlib.h>\n",file) ;
  fputs("#include <string.h>\n",file) ;
  fputs("#include <stdbool.h>\n",file) ;
  fputs("#include \"dec_fun.c\"\n",file) ;
  dec_fun = fopen("dec_fun.c","w") ;
  fputs("#include <stdio.h>\n",dec_fun) ;
  fputs("#include <stdlib.h>\n",dec_fun) ;
  fputs("#include <string.h>\n",dec_fun) ;
  fputs("#include <stdbool.h>\n",dec_fun) ;
}

// Cerrar fichero
void closeFile () {
  fputs("}\n",file) ;
  fclose(file) ;
  fclose(dec_fun);
  //printf("%i",MAX_archivo);
  for(int i = MAX_archivo-1; i>=0; i--)
    closeFiles (i) ;
}

void closeFiles(int i){

  char c;

  // Read contents from file 
  char* temporal;
  char* orden1;
  char* orden2;
  temporal = (char *) malloc(20);
  orden1 = (char *) malloc(50);
  orden2 = (char *) malloc(50);

  sprintf(temporal, "aux_file_%i", i);
  strcat(temporal, ".txt");

  //printf("%s", temporal);
/*
  while(1) {
      c = fgetc(aux);
      if( feof(aux) ) { 
         break ;
      }
      printf("%d", c);
      fputc(c, dec_fun);
   }
*/
  fclose(ficheros[i]);

  strcat(orden1, "cat ");
  strcat(orden1, temporal);
  strcat(orden1, " >> ");
  strcat(orden1, "dec_fun.c");
  //printf("%s",orden1);
  system(orden1);
  
  strcpy(orden2, "rm ");
  strcat(orden2, temporal);
  system(orden2) ;

  fclose(ficheros[i]);
}

// Define variable
void defineVar (attributes a) {
  if ( globalType != not_assigned ) {
    if ( globalType != list ) defineDefaultVar(a) ;
    else if ( globalType == list ) defineVarList(a) ;
  }
}

void defineDefaultVar (attributes a) {
  char * def ;
	def = (char *) malloc(15) ;
  if ( globalType == integer ) strcpy(def, "int ") ;
  else if ( globalType == real ) strcpy(def, "double ") ;
  else if ( globalType == boolean ) strcpy(def, "bool ") ;
  else if ( globalType == character ) strcpy(def, "char ") ;
  strcat(def, a.lexema) ;
  strcat(def, ";\n") ;
  if ( ARCHIVO == 0 ) fputs(def,file) ;
  else fputs(def, ficheros[ARCHIVO-1]) ;
}

void defineVarList (attributes a) {
  char * def ;
  def = (char *) malloc(40) ;
  strcpy(def, "list<") ;
  dtype type = listType(a) ;
  if ( type == integer ) strcat(def, "int") ;
  else if ( type == real ) strcat(def, "double") ;
  else if ( type == boolean ) strcat(def, "bool") ;
  else if ( type == character ) strcat(def, "char") ;
  strcat(def, "> ") ;
  strcat(def, a.lexema) ;
  strcat(def, ";\n") ;
  if ( ARCHIVO == 0 ) fputs(def,file) ;
  else fputs(def, ficheros[ARCHIVO-1]) ;
}

char* printFunc(attributes e){
  char* txt;
  txt = (char *) malloc (50);

  strcat(txt, "(");
  strcat(txt, e.variable);
  strcat(txt, ")");

  return txt;
}

char* printVariables(attributes a, attributes b){
  char* txt;
  txt = (char *) malloc(50);
  strcat(txt, a.variable);
  strcat(txt, ", ");
  strcat(txt, b.variable);

  return txt;
}

void UnaryOp (attributes * atb, attributes a, char * op) {
  (*atb).lexema = temporal() ;
  char * exp ;
  exp = (char *) malloc(15) ;
  if ( (*atb).tipo != not_assigned ) {
    globalType = (*atb).tipo ;
    defineVar(*atb) ;
    strcat(exp, (*atb).lexema) ;
    strcat(exp, " = ") ;
    strcat(exp, op) ;
    strcat(exp, a.variable) ;
    strcat(exp, ";\n") ;
    if ( ARCHIVO == 0 ) fputs(exp,file) ;
    else fputs(exp, ficheros[ARCHIVO-1]) ;
  }
}

void BinaryOp (attributes * atb, attributes a, attributes b, char * op ) {
  (*atb).lexema = temporal() ;
  char * exp ;
  exp = (char *) malloc(15) ;
  if ( (*atb).tipo != not_assigned ) {
    globalType = (*atb).tipo ;
    defineVar(*atb) ;
    strcat(exp, (*atb).lexema) ;
    strcat(exp, " = ") ;
    strcat(exp, a.variable) ;
    strcat(exp, op) ;
    strcat(exp, b.variable) ;
    strcat(exp, ";\n") ;
    if ( ARCHIVO == 0 ) fputs(exp,file) ;
  else fputs(exp, ficheros[ARCHIVO-1]) ;
  }
}

void asignar (attributes a1, attributes a2) {
  char * igualdad ;
  igualdad = (char *) malloc(15) ;
  strcat(igualdad, a1.lexema) ;
  strcat(igualdad, " = ");
  strcat(igualdad, a2.variable) ;
  strcat(igualdad, ";\n") ;
  if ( ARCHIVO == 0 ) fputs(igualdad,file) ;
  else fputs(igualdad, ficheros[ARCHIVO-1]) ;
}

/*********************************************************************
 *                      ESTRUCTURA IF-THEN-ELSE                      *
 *********************************************************************/

void insert_ifthenelse () {
  inTS newIn ;
  DescriptorDeInstrControl descrp ;
  descrp.EtiquetaSalida = etiqueta() ;
  descrp.EtiquetaElse = etiqueta() ;
  newIn.entrada = condition ;
  newIn.nombre = "ifthenelse" ;
  newIn.tipoDato = not_assigned ; // TODO: tipoDato == globalType?? para asignarle tipo y comprobar con lo q devuelva
  newIn.tipoDato2 = not_assigned ;
  newIn.parametros = 0 ;
  newIn.descriptor = descrp ;
  TS_AddIn(newIn) ;
}

void assertGotoElse (char * name) {
  char * ifsentence ;
  char * etiq_else ;
  ifsentence = (char *) malloc(30) ;
  etiq_else = searchElse() ;
  strcat(ifsentence, "if(! ") ;
  strcat(ifsentence, name) ;
  strcat(ifsentence, " ) goto ") ;
  strcat(ifsentence, etiq_else) ;
  strcat(ifsentence, ";\n") ;
  if ( ARCHIVO == 0 ) fputs(ifsentence,file) ;
  else fputs(ifsentence, ficheros[ARCHIVO-1]) ;
}

void assertGotoEndAndElse () {
  char * etiq ;
  char * els ;
  etiq = (char *) malloc(30) ;
  els = searchEnd(0) ;
  strcat(etiq, "goto ") ;
  strcat(etiq, els) ;
  strcat(etiq, ";\n") ;
  els = searchElse() ;
  strcat(etiq, els) ;
  strcat(etiq, ":;\n") ;
  if ( ARCHIVO == 0 ) fputs(etiq,file) ;
  else fputs(etiq, ficheros[ARCHIVO-1]) ;
}

void assertEnd () {
  char * txt ;
  char * etiq ;
  txt = (char *) malloc(30) ;
  etiq = searchEnd(1) ;
  strcat(txt, etiq) ;
  strcat(txt, ":;\n") ;
  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}

char * searchElse () {
  char * etiq ;
  int i = TOPE - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( TS[i].entrada == condition ) pos = i ;
    i-- ;
  }
  etiq = TS[pos].descriptor.EtiquetaElse ;
  return (etiq) ;
}

char * searchEnd (int del) {
  char * etiq ;
  int i = TOPE - 1;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( TS[i].entrada == condition ) {
      pos = i ;
      if ( del == 1 ) TS[i].entrada = condition_used ;
    }
    i-- ;
  }
  etiq = TS[pos].descriptor.EtiquetaSalida ;
  return (etiq) ;
}

/*********************************************************************
 *                           ESTRUCTURA WHILE                        *
 *********************************************************************/

void insert_while () {
  inTS newIn ;
  DescriptorDeInstrControl descrp ;
  descrp.EtiquetaSalida = etiqueta() ;
  descrp.EtiquetaEntrada = etiqueta() ;
  newIn.entrada = condition ;
  newIn.nombre = "while" ;
  newIn.tipoDato = not_assigned ; // TODO: tipoDato == globalType?? para asignarle tipo y comprobar con lo q devuelva
  newIn.tipoDato2 = not_assigned ;
  newIn.parametros = 0 ;
  newIn.descriptor = descrp ;
  TS_AddIn(newIn) ;
}

void assertGotoOutWhile (char * name) {
  char * whilesentece ;
  char * etiq_out ;
  char * etiq_in ;
  whilesentece = (char *) malloc(30) ;
  DescriptorDeInstrControl des = searchInOutWhile(0) ;
  etiq_in = des.EtiquetaEntrada ;
  etiq_out = des.EtiquetaSalida ;
  strcat(whilesentece, "if(! ") ;
  strcat(whilesentece, name) ;
  strcat(whilesentece, " ) goto ") ;
  strcat(whilesentece, etiq_out) ;
  strcat(whilesentece, ";\n") ;
  if ( ARCHIVO == 0 ) fputs(whilesentece,file) ;
  else fputs(whilesentece, ficheros[ARCHIVO-1]) ;
}

void assertInWhile () {
  char * whilesentece ;
  char * etiq_in ;
  whilesentece = (char *) malloc(30) ;
  DescriptorDeInstrControl des = searchInOutWhile(0) ;
  etiq_in = des.EtiquetaEntrada ;
  strcat(whilesentece, etiq_in) ;
  strcat(whilesentece, ":;\n") ;
  if ( ARCHIVO == 0 ) fputs(whilesentece,file) ;
  else fputs(whilesentece, ficheros[ARCHIVO-1]) ;
}

void assertJumpIn () {
  char * txt ;
  char * etiq_out ;
  char * etiq_in ;
  txt = (char *) malloc(40) ;
  DescriptorDeInstrControl des = searchInOutWhile(1) ;
  etiq_in = des.EtiquetaEntrada ;
  etiq_out = des.EtiquetaSalida ;
  strcat(txt, "goto ") ;
  strcat(txt, etiq_in) ;
  strcat(txt, ";\n") ;
  strcat(txt, etiq_out) ;
  strcat(txt, ":;\n") ;
  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}

DescriptorDeInstrControl searchInOutWhile (int del) {
  int i = TOPE - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( TS[i].entrada == condition ) {
      pos = i ;
      if ( del == 1 ) TS[i].entrada = condition_used ;
    }
    i-- ;
  }
  DescriptorDeInstrControl des = TS[pos].descriptor ;
  return (des) ;
}


/*********************************************************************
 *                           ESTRUCTURA CASES                        *
 *********************************************************************/

/*
Idea general: Se incluye una condition switch al entrar, que generará su etiqueta de salida y la almacenará

Cada case incluye condition y guardará como etiqueta de entrada, la entrada del siguiente case (porque sí) y 
como salida nada (ya lo buscará, será la misma de salida del switch)
*/

void insert_switch (char * name) {
  inTS newIn ;
  DescriptorDeInstrControl descrp ;
  descrp.EtiquetaSalida = etiqueta() ;
  descrp.EtiquetaEntrada = "#" ;
  descrp.NombreVarControl = name ;
  newIn.entrada = condition ;
  newIn.nombre = "switch" ;
  newIn.tipoDato = not_assigned ; 
  newIn.tipoDato2 = not_assigned ;
  newIn.parametros = 0 ;
  newIn.descriptor = descrp ;
  TS_AddIn(newIn) ;
}

void insert_case () {
  inTS newIn ;
  DescriptorDeInstrControl descrp ;
  descrp.EtiquetaEntrada = etiqueta() ; // La etiqueta del next case
  newIn.entrada = condition ;
  newIn.nombre = "case" ;
  newIn.tipoDato = not_assigned ; 
  newIn.tipoDato2 = not_assigned ;
  newIn.parametros = 0 ;
  newIn.descriptor = descrp ;
  TS_AddIn(newIn) ;
}

void assertCase (char * valor, int cmp) { // Incluye etiqueta: y condicion goto somewhere
  char * txt ;
  char * etiq_in ;
  char * etiq_next_in ;
  char * etiq_out ;
  char * var_control ;
  DescriptorDeInstrControl switch_des ;
  txt = (char *) malloc(40) ;
  etiq_in = searchInCase() ; // Busco en el superior
  etiq_next_in = searchNextInCase() ; // Busco en mi
  switch_des = searchSwitch(0) ;
  etiq_out = switch_des.EtiquetaSalida ;
  var_control = switch_des.NombreVarControl ;
  if ( !(strcmp(etiq_in, "#") == 0) ) { // Si es distinto de ""
    strcat(txt, etiq_in) ;
    strcat(txt, ":;\n") ;
  }
  if ( cmp == 1 ) {
    strcat(txt, "if( ") ;
    strcat(txt, var_control) ;
    strcat(txt, " != ") ;
    strcat(txt, valor) ;
    strcat(txt, ") goto ") ;
    strcat(txt, etiq_next_in) ;
    strcat(txt, ";\n") ;
  }
  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}

void assertGotoEnd () {
  char * txt ;
  DescriptorDeInstrControl switch_des ;
  txt = (char *) malloc(30) ;
  switch_des = searchSwitch(0) ;
  strcat(txt, "goto ") ;
  strcat(txt, switch_des.EtiquetaSalida) ;
  strcat(txt, ";\n") ;
  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}

void assertEndSwitch () {
  char * txt ;
  DescriptorDeInstrControl switch_des ;
  txt = (char *) malloc(20) ;
  switch_des = searchSwitch(1) ;
  strcat(txt, switch_des.EtiquetaSalida) ;
  strcat(txt, ":;\n") ;
  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}

void destroyLastCase () {
  int i = TOPE - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( TS[i].entrada == condition && (strcmp(TS[i].nombre, "case") == 0) ) {
      pos = i ;
      TS[i].entrada = condition_used ;
    }
    i-- ;
  }
}

DescriptorDeInstrControl searchSwitch (int del) { // Al encontrar el switch encima del case, obtengo varcontrol y Etiqueta salida
  int i = TOPE - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( (strcmp(TS[i].nombre, "switch") == 0) && TS[i].entrada == condition ) {
      pos = i ;
      if ( del == 1 ) TS[i].entrada = condition_used ;
    }
    i-- ;
  }
  DescriptorDeInstrControl etiq = TS[pos].descriptor ;
  return (etiq) ;
}

// Mi etiqueta de entrada está guardada en la Entrada del case superior
char * searchInCase () {
  int i = TOPE - 2 ; // Para no comprobarse con sí mismo, que acaba de añadirse
  int pos = -1 ;
  while ( i > 0 && pos == -1 ){ 
    if ( TS[i].entrada == condition ) {
      pos = i ;
      if ( strcmp(TS[i].nombre, "case") == 0 ) TS[i].entrada = condition_used ;
    }
    i-- ;
  }
  char * entrada ;
  entrada = TS[pos].descriptor.EtiquetaEntrada ;
  return (entrada) ; 
}

// Busco en mi mismo la entrada del next Case ;)
char * searchNextInCase () { 
  int i = TOPE - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( TS[i].entrada == condition && (strcmp(TS[i].nombre, "case") == 0) ) {
      pos = i ;    
    }
    i-- ;
  }
  char * entrada ;
  if ( pos != -1 ) entrada = TS[pos].descriptor.EtiquetaEntrada ;
  return (entrada) ;
}

void printInput (attributes a) {
  char * txt ;
  txt = (char *) malloc(30) ;
  strcat(txt, "scanf(") ;
  if(a.tipo == integer) {
    strcat(txt, "\"%i\", &") ;
    strcat(txt, a.lexema) ;
  }else if(a.tipo == character) {
    strcat(txt, "\"%s\", &") ;
    strcat(txt, a.lexema) ;
  }
  strcat(txt, ");\n") ;
  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}

void printOutput (attributes a) {
  char * txt ;
  txt = (char *) malloc(30) ;
  strcat(txt, "printf(") ;
  if ( a.tipo == string ) strcat(txt, a.lexema) ;
  else if ( a.tipo == integer ) {
    strcat(txt, "\"%i\",") ;
    strcat(txt, a.lexema) ;
  }
  strcat(txt, ");\n") ;
  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}

/*********************************************************************
 *                              SUBPROGRAMAS                         *
 *********************************************************************/

void newFunction (attributes a) {

  if(ARCHIVO == MAX_archivo){
    fichero();
    MAX_archivo++;
  }
  ARCHIVO++;

  char* txt;
  txt = (char *) malloc(20);

  if ( globalType == integer ) strcpy(txt, "int ") ;
  else if ( globalType == real ) strcpy(txt, "double ") ;
  else if ( globalType == boolean ) strcpy(txt, "bool ") ;
  else if ( globalType == character ) strcpy(txt, "char ") ;

  strcat(txt, a.lexema);
  strcat(txt, "(");

  fputs(txt, ficheros[ARCHIVO-1]);
}

// 0: necesita coma ; 1: no necesita coma
void addParam (attributes a, int first) {
  char* txt;
  txt = (char *) malloc(20);

  if ( first == 0 ) strcpy(txt, ", ") ;

  if ( globalType == integer ) strcat(txt, "int ") ;
  else if ( globalType == real ) strcat(txt, "double ") ;
  else if ( globalType == boolean ) strcat(txt, "bool ") ;
  else if ( globalType == character ) strcat(txt, "char ") ;

  strcat(txt, a.lexema);

  fputs(txt, ficheros[ARCHIVO-1]);
}

void addParentEnd(){
  char* txt;
  txt = (char *) malloc(10);

  strcpy(txt, "){\n");
  fputs(txt, ficheros[ARCHIVO-1]);
}

void printReturn (attributes a) {
  char* txt;
  txt = (char *) malloc(30);

  strcpy(txt, "return ");
  strcat(txt, a.variable);
  strcat(txt, ";\n");

  if ( ARCHIVO == 0 ) fputs(txt,file) ;
  else fputs(txt, ficheros[ARCHIVO-1]) ;
}