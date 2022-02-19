/****************************************************************/
/*                                                              */
/*      Program 2-2:  Pascal Source Tokenizer                   */
/*                                                              */
/*      Recognize Pascal tokens.                                */
/*                                                              */
/*      FILE:       token.c                                    */
/*                                                              */
/*      REQUIRES:   Modules error, scanner                      */
/*                                                              */
/*      USAGE:      token2 sourcefile                           */
/*                                                              */
/*          sourcefile      name of source file to tokenize     */
/*                                                              */
/*      Copyright (c) 1991 by Ronald Mak                        */
/*      For instructional purposes only.  No warranties.        */
/*  Updates                                                     */
/*   Gary Dohmeier: 01/02/22:                                   */
/*        update to run on mac osx / linus w/gcc                */
/*                                                              */
/****************************************************************/

#include <stdio.h>
#include "common.h"
#include "error.h"
#include "scanner.h"

/*--------------------------------------------------------------*/
/*  Token name strings                                          */
/*--------------------------------------------------------------*/

char *symbol_strings[] = {
    "<no token>", "<IDENTIFIER>", "<NUMBER>", "<STRING>",
    "^", "*", "(", ")", "-", "+", "=", "[", "]", ":", ";",
    "<", ">", ",", ".", "/", ":=", "<=", ">=", "<>", "..",
    "<END OF FILE>", "<ERROR>",
    "AND", "ARRAY", "BEGIN", "CASE", "CONST", "DIV", "DO", "DOWNTO",
    "ELSE", "END", "FILE", "FOR", "FUNCTION", "GOTO", "IF", "IN",
    "LABEL", "MOD", "NIL", "NOT", "OF", "OR", "PACKED", "PROCEDURE",
    "PROGRAM", "RECORD", "REPEAT", "SET", "THEN", "TO", "TYPE",
    "UNTIL", "VAR", "WHILE", "WITH",
};

/*--------------------------------------------------------------*/
/*  Externals                                                   */
/*--------------------------------------------------------------*/

extern TOKEN_CODE token;
extern char       token_string[];
extern LITERAL    literal;


void print_token(void);
void init_scanner(char *name);
void quit_scanner(void);
void get_token(void);



/*--------------------------------------------------------------*/
/*  main                Loop to tokenize source file.           */
/*--------------------------------------------------------------*/

int main(int argc, char *argv[])
{
 
    // debug
    //fprintf (stderr, "debug: argc='%i' argv[1]='%s'.\n", argc,argv[1]);

    /*
    --  check parametres and open filep.
    */
    if (argc <= 1) { 
        fprintf (stderr, "syntax: list <path><filename>");
        return -1;
    } else {
        init_scanner(argv[1]);
    }

    /*
    --  Repeatedly fetch tokens until a period
    --  or the end of file.
    */
    do {
        token = NO_TOKEN;
        get_token();
        if (token == END_OF_FILE) {
            error(UNEXPECTED_END_OF_FILE);
            break;
	    }
	    print_token();
    } while (token != PERIOD);

    quit_scanner();
}

/*--------------------------------------------------------------*/
/*  print_token         Print a line describing the current     */
/*                      token.                                  */
/*--------------------------------------------------------------*/

void print_token(void)
{
    char line[MAX_SOURCE_LINE_LENGTH + 32];
    char *symbol_string = symbol_strings[token];

    switch (token) {

	case NUMBER:
	    if (literal.type == INTEGER_LIT) {
		    sprintf(line, "     >> %-16s %d (integer)\n",
            symbol_string, literal.value.integer);
        } else {
            sprintf(line, "     >> %-16s %g (real)\n",
            symbol_string, literal.value.real);
        }
        break;

	case STRING:
	    sprintf(line, "     >> %-16s '%-s'\n",
        symbol_string, literal.value.string);
	    break;

	default:
	    sprintf(line, "     >> %-16s %-s\n",
        symbol_string, token_string);
	    break;
    }
    print_line(line);
}
