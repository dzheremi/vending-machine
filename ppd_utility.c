/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * reads an integer from keyboard, into an integer when passed it's pointer
 **/
BOOLEAN read_integer(char * prompt, int * destination, int size, int min_value, int max_value)
{

    /* local variables */
    int result;
    char input_string[size + EXTRA_CHARS];
    char * end = NULL;
    BOOLEAN valid = FALSE;

    /* iterate until an integer is read, or if the user aborts */
    do
    {

        /* print the supplied prompt */
        printf("%s: ", prompt);

        /* if user aborts with CTRL-D, clear buffer and end */
        if (fgets(input_string, size + EXTRA_CHARS, stdin) == NULL)
        {
            read_rest_of_line();
            return FALSE;

        /* user has not aborted */
        } else {

            /* ensure the read string is not to long, if it is, clear the input buffer */
            if (input_string[strlen(input_string) - 1] != '\n')
            {
                printf("Error: input is too long.\n");
                read_rest_of_line();

            /* remove the carnage return from the end of the string */
            } else {
                input_string[strlen(input_string) - 1] = NULL_CHAR;

                /* convert the string input to integer */
                result = (int)(strtol(input_string, &end, 10));

                /* if conversion was unsuccessful, re-iterate */
                if (strcmp(end, "") != 0)
                {
                    printf("Error: input is not numeric.\n");

                /* if input is not within the specified range, re-iterate */
                } else if (result < min_value || result > max_value) {
                    printf("Error: input is not within range.\n");

                /* if all conditions met, validate as true and break loop */
                } else {
                    valid = TRUE;
                }
            }
        }
    } while (valid == FALSE);

    /* save the integer into supplied variable via pointer */
    *destination = result;

    return TRUE;
}

/**
 * reads a currency from keyboard, into a price struct when passed it's pointer
 **/
BOOLEAN read_currency(char * prompt, int size, struct price * price)
{

    /* local variables */
    char input_string[size + EXTRA_CHARS];
    char * end = NULL;
    int dollars = 0;
    int cents = 0;
    BOOLEAN valid = FALSE;

    /* iterate until a valid currency is read, or if the user aborts */
    do
    {

        /* print prompt */
        printf("%s: ", prompt);

        /* if user aborts with CTRL-D, clear buffer and end */
        if (fgets(input_string, size + EXTRA_CHARS, stdin) == NULL)
        {
            read_rest_of_line();
            return FALSE;

        /* user has not aborted */
        } else {

            /* ensure the read string is not to long, if it is, clear the input buffer */
            if (input_string[strlen(input_string) - 1] != '\n')
            {
                printf("Error: input is too long.\n");
                read_rest_of_line();
            } else {

                /* remove the carnage return from the end of the string */
                input_string[strlen(input_string) - 1] = '\0';

                /* attempt to load the string into a price struct using strtol */
                dollars = (int)(strtol(input_string, &end, 10));
                cents = (int)(strtol((end + 1), &end, 10));
                if (&input_string[strlen(input_string)] == end)
                {
                    price->dollars = dollars;
                    price->cents = cents;
                    valid = TRUE;

                /* string could not be loaded into price struct, re-iterate loop */
                } else {
                    printf("Error: invalid currency entered.\n");
                }
            }
        }
    } while(valid == FALSE);

    return TRUE;

}

/**
 * reads a string from keyboard, into char array when passed it's pointer
 */
BOOLEAN read_string(char * prompt, int size, char * string)
{

    /* local variables */
    char input_string[size + EXTRA_CHARS];
    BOOLEAN valid = FALSE;

    /* iterate until a valid string is read, or if the user aborts */
    do
    {

        /* display prompt */
        printf("%s: ", prompt);

        /* if user aborts with CTRL-D, clear buffer and end */
        if (fgets(input_string, size + EXTRA_CHARS, stdin) == NULL)
        {
            read_rest_of_line();
            return FALSE;

        /* user has not aborted */
        } else {

            /* ensure the read string is not to long, if it is, clear the input buffer */
            if (input_string[strlen(input_string) - 1] != '\n')
            {
                printf("Error: input is too long.\n");
                read_rest_of_line();

            /* valid input */
            } else {
                valid = TRUE;
            }
        }
    } while (valid == FALSE);

    /* remove the carriage return from the end of the string */
    input_string[strlen(input_string) - 1] = '\0';

     /* save the string into the destination string via pointer */
    strcpy(string, input_string);

    /* ensure the destination matches the input */
    if(strcmp(input_string, string) == 0)
    {
        return TRUE;
    } else {
        return FALSE;
    }

}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name, const char * stock_name)
{
    /* save the names of the file names */
    system->coin_file_name = coins_name;
    system->stock_file_name = stock_name;

    /* load the coins file */
    if (!parse_coin_file(system, coins_name))
    {
        return FALSE;
    }

    /* load the stock file */
    if (!parse_stock_file(system, stock_name))
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * function which parses a coin file into an array of coin structs. Input file
 * must be comma seperated, with a total of two columns per line.
 * First column must contain a valid denomination expressed in cents, second
 * column must contain a valid quantity.
 **/
BOOLEAN parse_coin_file(struct ppd_system * system, const char * coin_file)
{
    /* local variables */
    FILE * fp;
    char line[LINE_SIZE + 1];
    char * token = NULL;
    char * end = NULL;
    int counter = 0;
    int denom = 0;
    int qty = 0;

    /* attempt to open the input file, error if not available */
    fp = fopen(coin_file, "r");
    if (fp == NULL)
    {
        printf("Failed to open coins file: No such file or directory\n\n");
        return FALSE;
    }

    /* parse the input file line by line */
    while (fgets(line, sizeof(line), fp) != NULL)
    {

        /* parse the line columns using comma seperators */
        token = strtok(line, ",");

        /* proceed if denomination column is present */
        if (token)
        {

            /* attempt parse the first column to long - end pointer must point to the
             * last byte of the token string, otherwise column is invalid. */
            strtol(token, &end, 10);
            if (end != &token[strlen(token)])
            {
                printf("Error in coins file: invalid denomination detected.\n");
                printf("\nDenomination column must contain only digits.\n\n");
                return FALSE;
            }

            /* parse the denomination to integer */
            denom = (int)(strtol(token, &end, 10));

            /* read the next column - quantity */
            token = strtok(NULL, COIN_DELIM);

            /* proceed if the quantity column is present */
            if (token)
            {

                /* remove the CR from the token, as should be the last in line */
                token[strlen(token) - 1] = NULL_CHAR;

                /* attempt to phase the second column to long - see comment above */
                strtol(token, &end, 10);
                if (end != &token[strlen(token)])
                {
                    printf("Error in coins file: invalid quantity detected.\n");
                    printf("\nQuantity column must contain only digits.\n\n");
                    return FALSE;
                }

                /* parse the quantity to integer */
                qty = (int)(strtol(token, &end, 10));

                /* attempt to load the coin struct */
                if (!coin_load(&system->cash_register[counter], denom, qty))
                {
                    printf("Error in coins file: invalid denomination or quantity detected.\n");
                    printf("\nDenomination must be valid and present, and quantity must be 0 or greater.\n\n");
                    return FALSE;
                }

                counter++;
            }
        }
    }

    /* ensure there are the exact number of denominations in the coins file */
    if (counter != NUM_DENOMS)
    {
        printf("Error in coins file: invalid number of lines in file.\n");
        printf("\nThere must be exactely %i lines in your coin file, you have %i.\n\n", NUM_DENOMS, counter);
        return FALSE;
    }

    /* close the file */
    fclose(fp);

    return TRUE;
}

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
BOOLEAN parse_stock_file(struct ppd_system * system, const char * stock_file)
{
    /* local variables */
    FILE * fp;
    char line[LINE_SIZE + 1];
    char * token = NULL;
    char * end = NULL;
    int counter = 0;
    struct ppd_stock * stock;

    /* attempt to open the input file, error if not available */
    fp = fopen(stock_file, "r");
    if (fp == NULL)
    {
        printf("Failed to open stock file: No such file or directory\n\n");
        return FALSE;
    }

    /* parse the input file line by line */
    while (fgets(line, sizeof(line), fp) != NULL)
    {

        /* counter is used to track the current column of the stock file being processed */
        counter = 0;

        /* attempt to allocate memory to a new stock item struct */
        stock = malloc(sizeof(struct ppd_stock));
        if (stock == NULL)
        {
            printf("Error: could not allocate memory to program.\n\n");
            return FALSE;
        }

        /* initialise the stock struct to safe values */
        stock_init(stock);

        /* parse the line columns using pipe separators */
        token = strtok(line, STOCK_DELIM);

        /* process each column whilst a token is present */
        while (token)
        {

            /* determine which column is being processed by column number */
            switch (counter)
            {

                /* item ID */
                case ITEM_ID:
                    strcpy(stock->id, token);
                    break;

                /* item name */
                case ITEM_NAME:
                    strcpy(stock->name, token);
                    break;

                /* item description */
                case ITEM_DESC:
                    strcpy(stock->desc, token);
                    break;

                /* item price */
                case ITEM_PRICE:

                    /* attempt to parse the price to a double - this is not the final
                     * value being saved, it is only being used to determine if the
                     * price field is valid - end pointer must point to the last byte
                     * of the token string, otherwise the column is invalid. */
                    strtod(token, &end);
                    if (end != &token[strlen(token)])
                    {
                        printf("Error in stock file: invalid price detected.\n");
                        printf("\nPrice column must contain decimal seperated dollars and cents.\n\n");
                        return FALSE;
                    }

                    /* now parse the token to two integer values (as required by the
                     * price struct). First conversion should leave the end pointer
                     * pointing to the value after the decimal point. */
                    stock->price.dollars = (unsigned)(strtol(token, &end, 10));
                    stock->price.cents = (unsigned)(strtol((end + 1), &end, 10));
                    break;

                /* item quantity */
                case ITEM_QTY:

                    /* remove the CR from the token, as this should be the last line */
                    token[strlen(token) - 1] = NULL_CHAR;

                    /* attempt to phase the quantity column to long - see comment above */
                    strtol(token, &end, 10);
                    if (end != &token[strlen(token)])
                    {
                        printf("Error in stock file: invalid quantity detected.\n");
                        printf("\nQuantity column must contain only digits.\n\n");
                        return FALSE;
                    }

                    /* parse the quantity to integer */
                    stock->on_hand = (unsigned)(strtol(token, &end, 10));
                    break;
            }

            /* increment the column counter, and read the next token */
            counter++;
            token = strtok(NULL, "|");
        }

        /* check for a valid number of columns in the row */
        if (counter != 5)
        {
            printf("Error in stock file: invalid number of tokens in line.\n");
            printf("Each stock item must have exactely 5 columns.\n\n");
            free(stock);
            return FALSE;

        } else {

            /* attempt to add the new stock item to the linked list */
            if (list_add(system->item_list, stock) == FALSE)
            {
                printf("Could not allocate memory to stock item!\n\n");
                free(stock);
                return FALSE;
            }
        }
    }

    /* close the file */
    fclose(fp);

    return TRUE;
}

/**
 * saves the the stock linked list to pipe separated file.
 **/
BOOLEAN save_stock_file(struct ppd_system * system)
{

    /* local variables */
    FILE * fp;
    struct ppd_stock * stock;

    /* open the file for writing - overwrite existing contents */
    fp = fopen(system->stock_file_name, "w");

    /* if file cannot be opened for writing, abort */
    if (fp == NULL)
    {
        printf("Error: could not save stock data.\n\n");
        return FALSE;
    }

    /* traverse the item list, writing each item to a new line in the file */
    stock = traverse_list(system->item_list);
    while(stock)
    {
        fprintf(fp, "%s%s%s%s%s%s%i.%i%s%i\n", stock->id, STOCK_DELIM, stock->name, STOCK_DELIM, stock->desc,
                STOCK_DELIM, stock->price.dollars, stock->price.cents, STOCK_DELIM, stock->on_hand);
        stock = traverse_list(system->item_list);
    }

    /* close the file */
    fclose(fp);

    return TRUE;

}

BOOLEAN save_coin_file(struct ppd_system * system)
{

    /* local variables */
    FILE * fp;
    int denomination = 0;
    int i = 0;

    /* open the file for writing - overwrite existing contents */
    fp = fopen(system->coin_file_name, "w");

    /* if file cannot be opened for writing, abort */
    if (fp == NULL)
    {
        printf("Error: could not save coin data.\n\n");
        return FALSE;
    }

    /**
     * traverse the cash register, write each denomination to a new line
     * in the file
     **/
    for (i = 0; i < NUM_DENOMS; i++)
    {
        denomination = get_denomination(&system->cash_register[i]);
        fprintf(fp, "%i%s%i\n", denomination, COIN_DELIM, system->cash_register[i].count);
    }

    /* close the file */
    fclose(fp);

    return TRUE;

}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system, char * stock_file_name, char * coin_file_name)
{
    int i = 0;

    /* save the coin and stock file names */
    system->stock_file_name = stock_file_name;
    system->coin_file_name = coin_file_name;

    /* initialise the cash register */
    for(i = 0; i < NUM_DENOMS; i++)
    {
        coin_init(&system->cash_register[i]);
    }

    /* allocate memory for the item list */
    system->item_list = malloc(sizeof(struct ppd_list));
    if (system->item_list == NULL) {
        printf("Error: could not allocate memory to program.\n\n");
        return FALSE;
    }

    /* initialise the item list */
    list_init(system->item_list);

    return TRUE;
}

/**
 * locates the cash register index given a coin
 **/
int find_coin_index(struct ppd_system * system, struct coin coin)
{
    int i = 0;

    for (i=0; i < NUM_DENOMS; i++)
    {
        if (system->cash_register[i].denom == coin.denom)
        {
            return i;
        }
    }
    return NULL;
}

/**
 * calculates the largest denomination that can be issued as change,
 * given a value in cents. this function is designed to be called in
 * a loop until the cents value is zero.
 */
int calculate_change(struct ppd_system * system, struct coin * coin, int cents)
{

    /* local variables */
    int i = 0;
    int coin_denomination = 0;
    int largest_denomination = 0;

    /* initialise the coin struct */
    coin_init(coin);

    /* loop through each denomination in the cash register */
    for (i=0; i < NUM_DENOMS; i++)
    {

        /* ensure that the cash register has this denomination */
        if (system->cash_register[i].count > 0)
        {

            /* get the integer value of the coin */
            coin_denomination = get_denomination(&system->cash_register[i]);

            /* if the value of the coin is less or equal to the change required */
            if (cents >= coin_denomination)
            {

                /* if this coin denomination is larger than any other found so far */
                if (coin_denomination >= largest_denomination)
                {
                    largest_denomination = coin_denomination;
                }
            }
        }
    }

    /* if a valid denomination is not found */
    if (largest_denomination == 0)
    {
        return 0;
    } else {
        coin_load(coin, largest_denomination, 1);
        return 1;
    }
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
    free_list(system->item_list);
    free(system->item_list);
}
