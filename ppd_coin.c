/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_coin.h"
#include "ppd_main.h"

/**
 * @file ppd_coin.c implement functions here for managing coins in the
 * "cash register" contained in the @ref ppd_system struct.
 **/

/**
 * initialises the coin struct to a safe working state
 **/
void coin_init(struct coin * coin)
{
    coin->denom = FIVE_CENTS;
    coin->count = 0;
}

/**
 * loads a denomination into a coin struct
 **/
int coin_load(struct coin * coin, int denom, int qty)
{
    /* do not load if quantity is negative */
    if (qty < 0)
    {
        return FALSE;
    }
    coin->count = (unsigned)qty;
    switch(denom)
    {
        case 5:
            coin->denom = FIVE_CENTS;
            break;
        case 10:
            coin->denom = TEN_CENTS;
            break;
        case 20:
            coin->denom = TWENTY_CENTS;
            break;
        case 50:
            coin->denom = FIFTY_CENTS;
            break;
        case 100:
            coin->denom = ONE_DOLLAR;
            break;
        case 200:
            coin->denom = TWO_DOLLARS;
            break;
        case 500:
            coin->denom = FIVE_DOLLARS;
            break;
        case 1000:
            coin->denom = TEN_DOLLARS;
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/**
 * returns the cent value given a coin
 **/
int get_denomination(struct coin * coin)
{
    int coin_denomination = 0;
    switch(coin->denom)
    {
        case FIVE_CENTS:
            coin_denomination = 5;
            break;
        case TEN_CENTS:
            coin_denomination = 10;
            break;
        case TWENTY_CENTS:
            coin_denomination = 20;
            break;
        case FIFTY_CENTS:
            coin_denomination = 50;
            break;
        case ONE_DOLLAR:
            coin_denomination = 100;
            break;
        case TWO_DOLLARS:
            coin_denomination = 200;
            break;
        case FIVE_DOLLARS:
            coin_denomination = 500;
            break;
        case TEN_DOLLARS:
            coin_denomination = 1000;
            break;
    }
    return coin_denomination;
}

/**
 * prints the value name of a coin (i.e. "$1")
 **/
void print_coin(struct coin * coin)
{
    switch(coin->denom)
    {
        case FIVE_CENTS:
            printf("5c");
            break;
        case TEN_CENTS:
            printf("10c");
            break;
        case TWENTY_CENTS:
            printf("20c");
            break;
        case FIFTY_CENTS:
            printf("50c");
            break;
        case ONE_DOLLAR:
            printf("$1");
            break;
        case TWO_DOLLARS:
            printf("$2");
            break;
        case FIVE_DOLLARS:
            printf("$5");
            break;
        case TEN_DOLLARS:
            printf("$10");
            break;
    }
}

/**
 * places the english name of a coin into a destination string
 * (i.e. "1 dollar")
 **/
void get_coin_name(struct coin * coin, char * coin_name)
{
    switch(coin->denom)
    {
      case FIVE_CENTS:
          strcpy(coin_name, "5 cents");
          break;
      case TEN_CENTS:
          strcpy(coin_name, "10 cents");
          break;
      case TWENTY_CENTS:
          strcpy(coin_name, "20 cents");
          break;
      case FIFTY_CENTS:
          strcpy(coin_name, "50 cents");
          break;
      case ONE_DOLLAR:
          strcpy(coin_name, "1 dollar");
          break;
      case TWO_DOLLARS:
          strcpy(coin_name, "2 dollars");
          break;
      case FIVE_DOLLARS:
          strcpy(coin_name, "5 dollars");
          break;
      case TEN_DOLLARS:
          strcpy(coin_name, "20 dollars");
          break;
    }
}
