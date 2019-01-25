%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantic.h"

void yyerror(const char *msg) ;
int yylex(void) ;

%}

%error-verbose

%left 	AT
%left	OR
%left	AND
%left	EQUALITYOPER
%left 	RELATIONOPER
%left	ADDSUB
%left 	MULTOPER
%right 	UNARYOPER
%left 	DECREMENT
%left 	INCREMENT
%left 	ATT
%left	LISTLEFTRIGHT
%left	DOLLAR

%token 	DATATYPE
%token	LISTNAME
%token	CONDITIONIF
%token	CONDITIONELSE
%token	LOOP
%token	OPTION
%token	CASE
%token	PARENTSTART
%token	PARENTEND
%token	SQBRASTART
%token	SQBRAEND
%token	INPUT
%token	OUTPUT
%token	ENDCASE
%token	DEFAULTCASE
%token	SEMICOLON
%token	COLON
%token 	ASSIGNATION
%token 	DECLARE
%token	MAINP
%token TWOPOINTS
%token RETURN
%token MULTOPER
%token	LOGICVALUE
%token	INTVALUE
%token	DOUBLEVALUE
%token	CHARVALUE
%token	STRINGVALUE
%token	IDENT

%start Program

%%

Program: { openFile(); temp = 0; etiq = 0; ARCHIVO = 0; inMain = true; MAX_archivo = 0;} Program_head block { closeFile(); fclose(dec_fun)};

Program_head : DATATYPE MAINP PARENTSTART PARENTEND
	| error ;

block :	Block_init { TS_InsertMark(); setSubProg(0);} Local_var_declaration Subprogs_declaration Sentences Block_end { TS_CleanIn(); };

Block_init : SQBRASTART;

Local_var_declaration :	init_mark_var_declaration local_variables end_mark_var_declaration
	| ;

Subprogs_declaration : Subprogs_declaration Subprog_declaration
	| ;

Sentences :	Sentences Sentence
	| ;

Block_end :	SQBRAEND
	| error ;

init_mark_var_declaration :	DECLARE SQBRASTART ;

local_variables : local_variables Var_body_declaration
	| Var_body_declaration ;

end_mark_var_declaration: SQBRAEND { if(inMain){ fputs("int main(){\n", file); inMain=false;} } ;

Subprog_declaration : Subprog_head {setSubProg(1);} block { fputs("}\n", ficheros[ARCHIVO-1]); ARCHIVO--;};

Sentence : block
	| assignment_sentence
	| if_sentence
	| while_sentence
	| switch_sentence
	| input_sentence
	| output_sentence
	| return_sentence
	| list_operations 
	;

Var_body_declaration : types list_of_ident SEMICOLON
	| error ;

Subprog_head : types IDENT {TS_InsertSUBPROG($2); newFunction($2); } PARENTSTART {nParam = 0;} arg_func {UpdateParamaters($2, nParam); addParentEnd();} PARENTEND;

arg_func : arg_func COLON types IDENT {TS_InsertPARAMF($4); nParam++; addParam($4,0);}
	| types IDENT {TS_InsertPARAMF($2); nParam++; addParam($2,1);}
	| error
	| ;

assignment_sentence : IDENT ASSIGNATION expression SEMICOLON {if(cmpTypes($1, $3)) asignar($1, $3);} ;

if_sentence: CONDITIONIF PARENTSTART expressionIF PARENTEND sentencia_if {assertEnd();}
	| CONDITIONIF PARENTSTART expressionIF PARENTEND sentencia_if CONDITIONELSE Sentence {assertEnd();};

expressionIF : expression { insert_ifthenelse(); if(checkBool($1)) assertGotoElse($1.variable);};

sentencia_if : Sentence { assertGotoEndAndElse(); };

while_sentence : LOOP parentstart_while expression {if(checkBool($3)) assertGotoOutWhile($3.variable);} PARENTEND Sentence {assertJumpIn();};

parentstart_while: PARENTSTART {insert_while(); assertInWhile();}

switch_sentence : OPTION PARENTSTART expression {if(checkExpSwitch($3)) insert_switch($3.variable);} PARENTEND SQBRASTART cases {destroyLastCase(); assertEndSwitch();} SQBRAEND ;

input_sentence : INPUT PARENTSTART IDENT {posVar($3,1); printInput($3);} PARENTEND SEMICOLON ;

output_sentence : OUTPUT PARENTSTART expression PARENTEND SEMICOLON {printOutput($3);};

return_sentence : RETURN expression SEMICOLON {checkReturn($2); printReturn($2); } ;

list_operations : IDENT LISTLEFTRIGHT SEMICOLON
	| DOLLAR IDENT SEMICOLON ;

expression : UNARYOPER expression { assig(&$$, unaryF($2,$1));}
	| INCREMENT expression {assig(&$$, incrementF($2)) ;}
	| DECREMENT expression {assig(&$$, decrementF1($2)) ;}
	| ADDSUB expression %prec UNARYOPER {assig(&$$, $2) ;}
	| expression ADDSUB expression {assig(&$$, addsubF($1,$3,$2)) ;}
	| expression OR expression {assig(&$$, orF($1,$3)) ;}
	| expression AND expression {assig(&$$, andF($1,$3)) ;}
	| expression EQUALITYOPER expression {assig(&$$, equalityF($1,$3,$2)) ;}
	| expression RELATIONOPER expression {assig(&$$, relationF($1,$3,$2)) ;}
	| expression DECREMENT expression {assig(&$$, decrementF2($1,$3)) ;}
	| expression INCREMENT expression ATT expression {assig (&$$, attF($1,$3,$5)) ;}
	| expression AT expression {assig (&$$, atF($1, $3)) ;}
	| expression MULTOPER expression {assig (&$$, multoperF($1, $3, $2)) ;}
	| PARENTSTART expression PARENTEND { $2.lexema = $2.variable; assig (&$$, $2) ;}
	| IDENT {assig(&$$, tipoVariable($1));}
	| constants {assig(&$$, $1) ;}
	| function {$1.lexema = $1.variable; assig(&$$, $1); }
	| error ;

cases :	caseType TWOPOINTS Sentences ENDCASE {assertGotoEnd();} SEMICOLON cases
	| caseType TWOPOINTS Sentences cases
	| ;

caseType: CASE cnst {insert_case(); assertCase($2.lexema, 1);}
	| DEFAULTCASE {insert_case(); assertCase("", 0);} ;

cnst : INTVALUE {$$.tipo = integer; $$.lexema = $1.lexema;}
	| CHARVALUE {$$.tipo = character; $$.lexema = $1.lexema;};

function : IDENT PARENTSTART {nParam = 0;} expression_list {checkNParameters($1, nParam);} PARENTEND { assig(&$$,tipoFunc($1)); $$.variable = strcat($1.lexema, printFunc($4));}
	| IDENT PARENTSTART PARENTEND { assig(&$$,tipoFunc($1)); $$.variable = strcat($1.lexema, "()");};

expression_list : expression_list COLON expression {nParam++; typeParams[nParam-1] = $3.tipo; typeParams2[nParam-1] = $3.tipo2; $$.variable = printVariables($1,$3);}
	| expression {nParam++; typeParams[nParam-1] = $1.tipo;  typeParams2[nParam-1] = $1.tipo2; $$.variable = $1.variable;} ;

types:	DATATYPE { setType($1); }
	| LISTNAME DATATYPE {setTypeList($2); } ;

list_of_ident:	list_of_ident COLON IDENT {TS_InsertVAR($3); defineVar($3);}
	| IDENT { TS_InsertVAR($1); defineVar($1);}
	| error ;

constants:	INTVALUE {$$.tipo = integer; $$.tipo2 = not_assigned;}
	| DOUBLEVALUE {$$.tipo = real; $$.tipo2 = not_assigned;}
	| LOGICVALUE {$$.tipo = boolean; $$.tipo2 = not_assigned;}
	| CHARVALUE {$$.tipo = character; $$.tipo2 = not_assigned;}
	| STRINGVALUE {$$.tipo = string;} // TODO: no tipo
	| c_lists {$$.tipo = list; $$.tipo2 = $1.tipo;} ; // TODO: tipo lista

c_lists:	c_int_list {$$.tipo = integer;}
	| c_double_list {$$.tipo = real;}
	| c_boolean_list {$$.tipo = boolean;}
	| c_character_list {$$.tipo = character;} ;

c_int_list : SQBRASTART int_list SQBRAEND ;

c_double_list :	SQBRASTART double_list SQBRAEND ;

c_boolean_list : SQBRASTART boolean_list SQBRAEND ;

c_character_list : SQBRASTART character_list SQBRAEND ;

int_list : int_list COLON INTVALUE
	| INTVALUE ;

double_list:	double_list COLON DOUBLEVALUE
	| DOUBLEVALUE ;

boolean_list:	boolean_list COLON LOGICVALUE
	| LOGICVALUE ;

character_list:	character_list COLON CHARVALUE
	| CHARVALUE ;

%%

#include "lex.yy.c"

void yyerror (const char *msg) {
	fprintf(stderr, "Linea %d: %s \n", line, msg);
}
