/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ppd_stock.h"
#include "ppd_coin.h"
#ifndef PPD_MAIN
#define PPD_MAIN
/**
 * @file ppd_main.h this file holds the rest of the application together.
 * From these header files, main() can call all functions refered to 
 * in the header files included. You might also want to insert here
 * any functions for handling and validation of command line arguments
 **/

/**
 * datatype to represent a boolean value within the system
 **/
typedef enum truefalse
{
    /**
     * the constant for false
     **/
    FALSE, 
    /**
     * the constant for true
     **/
    TRUE
} BOOLEAN;

/**
 * typedef for menu functions - all return boolean values
 **/
typedef BOOLEAN (*MENU_FUNCTIONS)(struct ppd_system *);

/**
 * typedef string to menu item names
 **/
typedef char * MENU_ITEMS;

/* the number of arguments this program requires - including program name */
#define NUM_ARGS 3
/* number of main menu items */
#define NUM_MAIN_MENU 3
/* number of admin menu items */
#define NUM_ADMIN_MENU 6
/**
 * the maximum size of a prompt used by read_integer, read_currency and
 * read_string.
 **/
#define PROMPT_SIZE 200
#endif
