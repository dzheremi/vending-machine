/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ppd_main.h"

/* maximum line size when importing from file */
#define LINE_SIZE 400

/* extra characters appended to the end of a string */
#define EXTRA_CHARS 2

/* null character */
#define NULL_CHAR '\0'

/* column numbers in stock file */
#define ITEM_ID 0
#define ITEM_NAME 1
#define ITEM_DESC 2
#define ITEM_PRICE 3
#define ITEM_QTY 4

#ifndef PPD_UTILITY
#define PPD_UTILITY
/**
 * @file ppd_utility.h defines some helper functions for the rest of your
 * program. In particular, it declares three of the most important 
 * functions for your application: @ref load_data which loads the data
 * in from files, @ref system_init which initialises the system to a 
 * known safe state and @ref system_free which frees all memory when you
 * are going to quit the application.
 **/

/**
 * the function to call for buffer clearing. This was discussed extensively
 * for assignment 1
 **/
void read_rest_of_line(void);

/**
 * reads an integer from keyboard, into an integer when passed it's pointer
 **/
BOOLEAN read_integer(char *, int *, int, int, int);

/**
 * reads a currency from keyboard, into a price struct when passed it's pointer
 **/
BOOLEAN read_currency(char *, int, struct price *);

/**
 * reads a string from keyboard, into char array when passed it's pointer
 */
BOOLEAN read_string(char *, int, char *);

/**
 * function which is called to load the coin and stock data into the 
 * system. You should ideally break this function up into smaller parts
 * to load the stock and the coins. You will also need to assign the 
 * char pointers to the stock and coin file names here so that the same
 * names will be used for save. A key part of this function is input
 * validation. A substantial number of marks are allocated to this function
 * - half are allocated to getting this function working and the other
 * half for rejecting invalid files. Please don't see file validation
 * as an optional extra but it is a central component of this assignment.
 **/
BOOLEAN load_data(struct ppd_system * , const char * , const char * );

/**
 * function which parses a coin file into an array of coin structs. Input file
 * must be comma seperated, with a total of two columns per line.
 * First column must contain a valid denomination expressed in cents, second
 * column must contain a valid quantity.
 **/
BOOLEAN parse_coin_file(struct ppd_system *, const char *);

/**
 * this function parses a stock file into the system's linked list
 * (ppd_system -> item_list). Input file must be pipe separated, with
 * a total of five columns per line.
 *      line 1: id
 *      line 2: name
 *      line 3: description
 *      line 4: price
 *      line 5: quantity
 **/
BOOLEAN parse_stock_file(struct ppd_system *, const char *);

/**
 * saves the the stock linked list to pipe separated file.
 **/
BOOLEAN save_stock_file(struct ppd_system *);

/**
 * save the cash register to comma separated file.
 **/
BOOLEAN save_coin_file(struct ppd_system *);

/**
 * Initialise the system to a known safe state. Before you post on the
 * discussion board about this, have a look at the structures as defined
 * in ppd_stock.h, ppd_coin.h and ppd_main.h. Given that nothing is 
 * initialized by the system if it
 * is a local variable, what would be good starting values for each of 
 * these. You already have some experience with this from assignment 
 * 1 to help you.
 **/
BOOLEAN system_init(struct ppd_system *, char *, char *);

/**
 * locates the cash register index given a coin
 **/
int find_coin_index(struct ppd_system *, struct coin);

/**
 * calculates the largest denomination that can be issued as change,
 * given a value in cents. this function is designed to be called in
 * a loop until the cents value is zero.
 */
int calculate_change(struct ppd_system *, struct coin *, int);

/**
 * free all memory that has been allocated. If you are struggling to
 * find all your memory leaks, compile your program with the -g flag
 * and run it through valgrind. An important thing to think about here:
 * as malloc() returns a memory address to the first byte allocated, you
 * must pass each of these memory addresses to free, and no other 
 * memory addresses.
 **/
void system_free(struct ppd_system *);

#endif
