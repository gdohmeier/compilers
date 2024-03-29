/****************************************************************/
/*                                                              */
/*      Program 3-3:  Pascal Source Uncruncher			*/
/*                                                              */
/*      Uncrunch a crunched Pascal source file.                 */
/*                                                              */
/*      FILE:       uncrunch.c                                  */
/*                                                              */
/*      USAGE:      uncrunch crunchfile                         */
/*                                                              */
/*          crunchfile      file to uncrunch, as created by	*/
/*                          the cruncher utility                */
/*                                                              */
/*      Copyright (c) 1991 by Ronald Mak                        */
/*      For instructional purposes only.  No warranties.        */
/*                                                              */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"
#include "../common/scanner.h"

#define MAX_OUTPUT_RECORD_LENGTH	80

/*--------------------------------------------------------------*/
/*  Token classes						*/
/*--------------------------------------------------------------*/

typedef enum {
    DELIMITER, NONDELIMITER,
} TOKEN_CLASS;

/*--------------------------------------------------------------*/
/*  Globals							*/
/*--------------------------------------------------------------*/

FILE *crunch_file;
char token_string[MAX_TOKEN_STRING_LENGTH];
char output_record[MAX_OUTPUT_RECORD_LENGTH];

TOKEN_CODE ctoken;              /* current token from crunch file */
int        record_length;       /* length of output record */
char       *recp;               /* pointer into output record */
char       **symtab_strings;    /* array of symtab strings */

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

TOKEN_CLASS token_class();

//
// prototypes
//

//void print_token(void);
void init_scanner(char *);
void quit_scanner(void);
void get_token(void);
void read_crunched_symtab(void);
void get_ctoken(void);
void append_blank(void);
void append_token(void);
void flush_output_record(void);
TOKEN_CLASS token_class(void);



/*--------------------------------------------------------------*/
/*  Main program	Uncrunch a source file.			*/
/*--------------------------------------------------------------*/

int main(int argc, char* argv[])
//    int  argc;
//    char *argv[];
{
    TOKEN_CLASS class1;		/* current token class */
    TOKEN_CLASS prev_class;	/* previous token class */
    
    /*
    --  Open the crunch file.
    */
    crunch_file = fopen(argv[1], "rb");

    if (crunch_file == NULL) {
	    printf("*** Error: Failed to open crunch file-out.\n");
	    printf("*** SYNTAX: uncrunch <crunchfile im>.\n");
	    exit(-2);
    }

    /*
    --  Initialize the uncruncher.
    */
    prev_class = DELIMITER;
    recp  = output_record;
    *recp = '\0';
    record_length = 0;

    /*
    --  Read the crunched symbol table.
    */
    read_crunched_symtab();

    /*
    --  Repeatedly process tokens until a period
    --  or the end of file.
    */
    do {
	    get_ctoken();
	    if (ctoken == END_OF_FILE) break;
	    class1 = token_class();

        /*
	    --  Append a blank only if two adjacent nondelimiters.
	    --  Then append the token string.
        */
	    if ((prev_class == NONDELIMITER) && (class1 == NONDELIMITER))
	        append_blank();

	    append_token();
	    prev_class = class1;
    } while (ctoken != PERIOD);

    /*
    --  Flush the last output record if it is partially filled.
    */
    if (record_length > 0) flush_output_record();
}

/*--------------------------------------------------------------*/
/*  read_crunched_symtab        Read the crunched symbol table  */
/*                              and build an array of its name  */
/*                              strings.                        */
/*--------------------------------------------------------------*/
void read_crunched_symtab(void)
{
    short count;	/* number of symtab entries */
    short index;	/* symtab entry index */
    char  length;	/* length of name string, incl. '\0' */

    /*
    --  Read the count of symbol table entries and
    --  allocate that many elements for the array.
    */
    fread(&count, sizeof(short), 1, crunch_file);
    symtab_strings = (char **) alloc_bytes(count*sizeof(char *));

    /*
    --  Read each symbol table entry (array index, string length,
    --  and string).  Set the array element.
    */
    do {
	fread(&index, sizeof(short), 1, crunch_file);
	fread(&length, sizeof(char), 1, crunch_file);

	symtab_strings[index] = alloc_bytes(length);
	fread(symtab_strings[index], length, 1, crunch_file);
    } while (--count > 0);
}

/*--------------------------------------------------------------*/
/*  get_ctoken          Read the next token code from the       */
/*                      crunch file.  Uncrunch the token into   */
/*                      token _string with a lookup in the      */
/*                      symbol table name strings array or in   */
/*                      the symbol strings array.               */
/*--------------------------------------------------------------*/
void get_ctoken(void)
{
    /*
    --  Read the crunched token code.
    */
    fread(&ctoken, sizeof(char), 1, crunch_file);
    
    /*
    --  Identifier, number, and string tokens:  Look up in the
    --  symbol table name strings array.  All other tokens:  Look
    --  up in the symbol strings array.
    */
    switch (ctoken) {

	case IDENTIFIER:
	case NUMBER:
	case STRING:  {
	    short index;	/* symtab strings index */

	    fread(&index, sizeof(short), 1, crunch_file);
	    strcpy(token_string, symtab_strings[index]);
	    break;
	}

	default:
	    strcpy(token_string, symbol_strings[ctoken]);
	    break;
    }
}

/*--------------------------------------------------------------*/
/*  token_class		Return the class of the current token.	*/
/*--------------------------------------------------------------*/
TOKEN_CLASS token_class(void)
{
    /*
    --  Nondelimiters:  identifiers, numbers, and reserved words
    --  Delimiters:     strings and special symbols
    */
    switch (ctoken) {

	case IDENTIFIER:
	case NUMBER:
	    return(NONDELIMITER);

	default:
	    return(ctoken < AND ? DELIMITER : NONDELIMITER);
    }
}

/*--------------------------------------------------------------*/
/*  append_blank	Append a blank to the output record,	*/
/*			or flush the record if it is full.	*/
/*--------------------------------------------------------------*/
void append_blank(void)
{
    if (++record_length == MAX_OUTPUT_RECORD_LENGTH - 1)
	flush_output_record();
    else strcat(output_record, " ");
}

/*--------------------------------------------------------------*/
/*  append_token	Append the token string to the output	*/
/*			record if it fits.  If not, flush the	*/
/*			current record and append the string	*/
/*			to append to the new record.		*/
/*--------------------------------------------------------------*/
void append_token(void)
{
    int token_length;           /* length of token string */
    
    token_length = strlen(token_string);
    if (record_length + token_length
				>= MAX_OUTPUT_RECORD_LENGTH - 1)
	flush_output_record();

    strcat(output_record, token_string);
    record_length += token_length;
}

/*--------------------------------------------------------------*/
/*  flush_output_record		Flush the current output	*/
/*				record.				*/
/*--------------------------------------------------------------*/
void flush_output_record(void)
{
    printf("%s\n", output_record);
    recp  = output_record;
    *recp = '\0';
    record_length = 0;
}
