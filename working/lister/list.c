/****************************************************************/
/*                                                              */
/*      Program 1-1:  Source File Lister                        */
/*                                                              */
/*      Print the contents of a source file		            	*/
/*      with line numbers and page headings.                    */
/*                                                              */
/*      FILE:       list.c                                      */
/*                                                              */
/*      USAGE:      list sourcefile                             */
/*								                                */
/*	    sourcefile	    name of source file to list		        */
/*                                                              */
/*                                                              */
/*      Copyright (c) 1991 by Ronald Mak                        */
/*      For instructional purposes only.  No warranties.        */
/*                                                              */
/*  Updates                                                     */
/*   Gary Dohmeier: 01/02/22:                                   */
/*        update to run on mac osx / linus w/gcc                */
/*        test input parm is file or error                      */
/*        add error check for open file                         */
/*                                                              */
/*                                                              */
/****************************************************************/

#include <stdio.h>
#include <sys/_types.h>
#include <sys/time.h>
#include <string.h>


#define FORM_FEED_CHAR		'\f'

#define MAX_FILE_NAME_LENGTH    256
#define MAX_SOURCE_LINE_LENGTH  256
#define MAX_PRINT_LINE_LENGTH   80
#define MAX_LINES_PER_PAGE      50
#define DATE_STRING_LENGTH      26

typedef enum {
    FALSE, TRUE,
} BOOLEAN;

/*--------------------------------------------------------------*/
/*  Globals                                                     */
/*--------------------------------------------------------------*/

int line_number = 0;                    /* current line number */
int page_number = 0;                    /* current page number */
int level       = 0;                    /* current nesting level */
int line_count  = MAX_LINES_PER_PAGE;   /* no. lines on current pg */

char source_buffer[MAX_SOURCE_LINE_LENGTH]; /* source file buffer */

char source_name[MAX_FILE_NAME_LENGTH]; /* name of source file */
char date[DATE_STRING_LENGTH];          /* current date and time */

FILE *source_file;

/*--------------------------------------------------------------*/
/*  main		Contains the main loop that drives	*/
/*			the lister.				*/
/*--------------------------------------------------------------*/



/*--------------------------------------------------------------*/
/*  init_lister         Initialize the lister globals.          */
/*--------------------------------------------------------------*/

void init_lister(char *name)    			/* name of source file */
{
    time_t timer;

    /*
    --  Copy the source file name and open the source file.
    */
    strcpy(source_name, name);
    //source_file = fopen(source_name, "r");

    /*
    --  Set the current date and time in the date string.
    */
    time( (long*)&timer );
    strcpy(date, asctime(localtime(&timer)));
}

/*--------------------------------------------------------------*/
/*  print_page_header	Print the page header at the top of	*/
/*			the next page.				*/
/*--------------------------------------------------------------*/

void print_page_header()
{
    putchar(FORM_FEED_CHAR);
    printf("Page %d   %s   %s\n\n", ++page_number, source_name, date);
}


/*--------------------------------------------------------------*/
/*  print_line		Print out a line.  Start a new page if  */
/*                      the current page is full.               */
/*--------------------------------------------------------------*/

void print_line(char line[])  /* line to be printed */     
{
    char save_ch;
    char *save_chp = NULL;

    if (++line_count > MAX_LINES_PER_PAGE) {
	print_page_header();
	line_count = 1;
    };

    if (strlen(line) > MAX_PRINT_LINE_LENGTH)
	save_chp = &line[MAX_PRINT_LINE_LENGTH];

    if (save_chp) {
	save_ch   = *save_chp;
	*save_chp = '\0';
    }

    printf("%s", line);

    if (save_chp) *save_chp = save_ch;
}


/*--------------------------------------------------------------*/
/*  get_source_line	Read the next line from the source	*/
/*			file.  If there was one, print it out	*/
/*			and return TRUE.  Else at end of file,	*/
/*			so return FALSE.			*/
/*--------------------------------------------------------------*/

BOOLEAN get_source_line()
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];

    if ((fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, source_file)) != NULL) {
	++line_number;

	sprintf(print_buffer, "%4d %d: %s", line_number, level, source_buffer);
	print_line(print_buffer);

	return(TRUE);
    }
    else return(FALSE);
}


int main(int argc, char *argv[])
{
    //fprintf (stderr, "debug: argc='%i' argv[1]='%s'.\n", argc,argv[1]);

    if (argc <= 1) { 
        fprintf (stderr, "syntax: list <path><filename>");
        return 1;
    } else {
        source_file = fopen (argv[1], "r");
    }

    if (!source_file) {  /* validate file open for reading */
        fprintf (stderr, "error: file open failed '%s'.\n", argv[1]);
        return 2;
    }

    /* do your stuff here */
    //get_source_line();

    init_lister(argv[1]);
    /*
    --  Repeatedly call get_source_line to read and print
    --  the next source line until the end of file.
    */
    while (get_source_line());

    if (source_file) {
        fclose (source_file);   /* close file if not stdin */
        //fprintf (stderr, "debug: close file.\n");
    }
 
    return 0;
}