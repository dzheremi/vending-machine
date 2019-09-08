/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include <stdio.h>
#include <string.h>
/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#ifndef PPD_COIN
#define PPD_COIN

/* delimiter used in reading and writing the stock file */
#define COIN_DELIM ","
/* the minimum denomination in cents */
#define MIN_COIN 5
/* the maximum denomination value in cents */
#define MAX_COIN 1000
/**
 * the maximum number of coins the system can be tendered in
 * one transaction
 **/
#define MAX_TENDER 20
/**
 * the maximum number of characters allowable to read a
 * coin in cents - #### = 4 (i.e. 1000 = $1)
 **/
#define COIN_SIZE 4
/**
 * maximum number of characters allowable to describe a currency
 * in plain english (i.e. "5 cents")
 **/
#define COIN_NAME_LEN 20

/**
 * defines the ppd_system struct, from ppd_stock.h, to allow it to
 * be used in ppd_coin files
 */
struct ppd_system;

/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

/**
 * initialises the coin struct to a safe working state
 **/
void coin_init(struct coin *);

/**
 * loads a denomination into a coin struct
 **/
int coin_load(struct coin *, int, int);

/**
 * returns the cent value given a coin
 **/
int get_denomination(struct coin *);

/**
 * prints the value name of a coin (i.e. "$1")
 **/
void print_coin(struct coin *);

/**
 * places the english name of a coin into a destination string
 * (i.e. "1 dollar")
 **/
void get_coin_name(struct coin *, char *);

#endif
