/****************************************************************/
/*                                                              */
/*      S C A N N E R   (Header)                                */
/*                                                              */
/*      FILE:       scanner.h                                   */
/*                                                              */
/*      MODULE:     scanner                                     */
/*                                                              */
/*      Copyright (c) 1991 by Ronald Mak                        */
/*      For instructional purposes only.  No warranties.        */
/*  Updates                                                     */
/*   Gary Dohmeier: 01/02/22:                                   */
/*        update to run on mac osx / linus w/gcc                */
/*                                                              */
/****************************************************************/

#ifndef scanner_h
#define scanner_h
#include "common.h"

/*--------------------------------------------------------------*/
/*  Token codes							*/
/*--------------------------------------------------------------*/

typedef enum {
    NO_TOKEN, IDENTIFIER, NUMBER, STRING,
    UPARROW, STAR, LPAREN, RPAREN, MINUS, PLUS, EQUAL,
    LBRACKET, RBRACKET, COLON, SEMICOLON, LT, GT, COMMA, PERIOD,
    SLASH, COLONEQUAL, LE, GE, NE, DOTDOT, END_OF_FILE, ERROR,
    AND, ARRAY, BEGIN, CASE, CONST, DIV, DO, DOWNTO, ELSE, END,
    FFILE, FOR, FUNCTION, GOTO, IF, IN, LABEL, MOD, NIL, NOT,
    OF, OR, PACKED, PROCEDURE, PROGRAM, RECORD, REPEAT, SET,
    THEN, TO, TYPE, UNTIL, VAR, WHILE, WITH,
} TOKEN_CODE;

/*--------------------------------------------------------------*/
/*  Literal structure						*/
/*--------------------------------------------------------------*/

typedef enum {
    INTEGER_LIT, REAL_LIT, STRING_LIT,
} LITERAL_TYPE;

typedef struct {
    LITERAL_TYPE type;
    union {
	int   integer;
	float real;
	char  string[MAX_SOURCE_LINE_LENGTH];
    } value;
} LITERAL;


BOOLEAN open_source_file(char *);
void skip_comment(void);
void close_source_file(void);
void get_word(void);
void get_number(void);
void get_string(void);
void get_special(void);
void get_number(void);
void downshift_word(void);
void accumulate_value(float *valuep, ERROR_CODE error_code);
void error(ERROR_CODE code);
void print_page_header(void);


#endif  ///////////
