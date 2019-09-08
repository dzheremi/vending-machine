/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{

    /* pointer to ppd_stock used to traverse the linked-list */
    struct ppd_stock * item;

    /* traverse the linked list */
    item = traverse_list(system->item_list);

    /* print the table header */
    printf("\n%-*s | %-*s | %-*s | %-*s\n", 5, "ID", 40, "Name", 8, "Available", 6, "Price");
    printf("----------------------------------------------------------------------\n");

    /* while the list is still traversing, display each item as a row */
    while(item)
    {
        printf("%-*s | %-*s | %-*i | $%*i.%.2i\n", 5, item->id, 40, item->name, 9, item->on_hand, 2,
               item->price.dollars, item->price.cents);

        /* load next item in the list */
        item = traverse_list(system->item_list);
    }
    printf("\n");

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{

    /* local variables */
    char stock_id[IDLEN];
    char prompt[PROMPT_SIZE];
    struct ppd_stock * item;
    struct coin change[MAX_TENDER];
    struct coin tendered[MAX_TENDER];
    struct coin coin;
    struct price price;
    int balance = 0;
    int tender = 0;
    int coin_index = 0;
    int change_count = 0;
    int tendered_count = 0;
    int i = 0;
    BOOLEAN exact_change_given = TRUE;
    BOOLEAN no_change = FALSE;
    BOOLEAN canceled = FALSE;

    /**
     * initialise the coin struct defined above to test if tendered amounts
     * are valid
     **/
    coin_init(&coin);

    /* print module header */
    printf("\nPurchase Item\n");
    printf("-------------\n");

    /* ask for id until item is found, or if user aborts */
    do
    {
        sprintf(prompt, "Please enter the ID of the item you wish to purchase");

        /* read item it, abort if CTRL-D is detected */
        if (!read_string(prompt, IDLEN, stock_id))
        {
            return FALSE;
        }

        /* attempt to load stock item by entered id */
        item = find_stock_by_id(system->item_list, stock_id);

        /* prompt if id is invalid */
        if (!item)
        {
            printf("Invalid selection, please try again.\n");
        }
    } while(!item);

    /* abort if item is out of stock */
    if(item->on_hand <= 0)
    {
        printf("We are out of stock of %s.\n\n", item->name);
        return FALSE;
    }

    /* print user selection */
    printf("You have selected \"%s - %s\". ", item->name, item->desc);
    printf("This will cost you $%i.%.2i.\n", item->price.dollars, item->price.cents);

    /* convert the price of the item to cents */
    balance = price_to_cents(item->price);

    printf("Please hand over the money - type in the value of each note / coin in cents.\n");
    printf("Press enter or ctrl-d on a new line to cancel this purchase:\n");

    /* ask user for money until balance equals or exceeds item cost */
    while (balance > 0)
    {

        /* convert the balance in cents to a price struct (avoids floating imprecision) */
        price = cents_to_price(balance);

        /* prompts user for remaining balance */
        sprintf(prompt, "You still need to give us $%i.%.2i: ", price.dollars, price.cents);

        /* if user does not abort, attempt to validate their input */
        if (read_integer(prompt, &tender, COIN_SIZE, MIN_COIN, MAX_COIN))
        {

            /**
             * if the tendered amount cannot be loaded into a coin struct, it is invalid
             * prompt user and re-read input
             **/
            while (coin_load(&coin, tender, 1) == FALSE)
            {

                /* convert the tendered amount to a price struct and display error message */
                price = cents_to_price(tender);
                printf("Error: $%i.%.2i is not a valid denomination of money.\n", price.dollars, price.cents);

                /* if user aborts, break out of this loop and note the cancellation */
                if (!read_integer(prompt, &tender, COIN_SIZE, MIN_COIN, MAX_COIN))
                {
                    canceled = TRUE;
                    break;
                }
            }

            /* if user has not aborted */
            if (!canceled) {

                /* subtract tendered amount from outstanding balance */
                balance -= tender;

                /* record the coin that has been tendered, in case of cancellation */
                tendered[tendered_count] = coin;
                tendered_count++;

                /* update the system cash register */
                coin_index = find_coin_index(system, coin);
                system->cash_register[coin_index].count++;
            }

        /* if user has aborted, note the cancellation and break out of this loop */
        } else {
            canceled = TRUE;
            break;
        }
    }

    /**
     * if the balance is less than zero, change needs to be given
     * only proceed if user has not aborted
     **/
    if (balance < 0 && !canceled)
    {

        /* convert balance to a positive integer */
        balance = balance * -1;

        /* convert the balance to a price struct */
        price = cents_to_price(balance);

        /* loop until change has been calculated */
        while (balance > 0)
        {

            /**
             * determine if the cash register is able to give change
             * for the current balance
             **/
            if(calculate_change(system, &coin, balance))
            {

                /* tell the system that exact change was not given by the user */
                exact_change_given = FALSE;

                /* record the the coin that has been given in change */
                change[change_count] = coin;
                change_count++;

                /* subtract the coin value from the change balance */
                balance -= get_denomination(&coin);

                /* update the system cash register */
                coin_index = find_coin_index(system, coin);
                system->cash_register[coin_index].count--;

            /**
             * if the cash register does not have sufficient coins to
             * issue change, the transaction must be cancelled and the
             * tendered coins must be refunded
             */
            } else {
                no_change = TRUE;
                break;
            }
        }

    /**
     * if the user has not aborted at this stage, and given correct
     * change, update the stock count */
    } else if (!canceled) {
        printf("Thank you. Here is your %s", item->name);
        item->on_hand--;
    }

    /* process a cancellation due to user abort or no change */
    if (no_change || canceled) {

        /* prompt the user as to the reason of the cancellation */
        if (no_change)
        {
            printf("Error: cannot tender change, correct change is required.\n");
        } else {
            printf("\nTransaction canceled!\n");
        }
        printf("Refunding your coins: ");

        /* loop through each tendered coin, and refund it to the user */
        for (i = 0; i < tendered_count; i++)
        {

            /* update the system cash register */
            coin_index = find_coin_index(system, tendered[i]);
            system->cash_register[coin_index].count--;

            /* print the coin value to screen */
            printf(" ");
            print_coin(&tendered[i]);

        }

        /**
         * loop through each coin set aside as change, and return them to
         * the cash register
         **/
        for (i = 0; i < change_count; i++)
        {
            coin_index = find_coin_index(system, change[i]);
            system->cash_register[coin_index].count++;
        }

    /**
     * if change must be given, update the stock count and print the
     * change to screen
     **/
    } else if (!exact_change_given) {
        printf("Thank you. Here is your %s", item->name);
        item->on_hand--;
        printf(" and your change of $%i.%.2i:", price.dollars, price.cents);

        /* loop through each coin to issue as change and print to screen */
        for (i = 0; i < change_count; i++) {
            printf(" ");
            print_coin(&change[i]);
        }
    }

    printf("\n\n");
    return TRUE;

}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{

    /* save stock file */
    save_stock_file(system);

    /* save coin file */
    save_coin_file(system);

    /* free malloced memory and exit */
    abort_system(system);

    return TRUE;

}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{

    /* local variables */
    char prompt[PROMPT_SIZE];
    struct ppd_stock * item;
    struct price price;
    BOOLEAN canceled = TRUE;

    /* allocate memory to the new stock item */
    item = malloc(sizeof(struct ppd_stock));
    if (item==NULL)
    {
        printf("Error: could not allocate memory to program.\n\n");
        return FALSE;
    }

    /* get the next stock id */
    get_next_stock_id(system->item_list, item->id);

    /* get the stock name from user */
    printf("\nThis new meal item will have item id of %s.\n", item->id);
    sprintf(prompt, "Enter the item name");
    if (read_string(prompt, NAMELEN, item->name))
    {

        /* get the stock description from user */
        sprintf(prompt, "Enter the item description");
        if (read_string(prompt, DESCLEN, item->desc))
        {

            /* get the price of the stock item from user */
            sprintf(prompt, "Enter the price for this item");
            if (read_currency(prompt, PRICE_SIZE, &price))
            {

                /**
                 * if the user has not cancelled at any of the inputs above
                 * turn the canceled flag off (initially set to on)
                 **/
                canceled = FALSE;

            }
        }
    }

    /* proceed only if the user has not aborted */
    if (!canceled)
    {

        /**
         * ensure that the entered amount it divisible by the lowest
         * coin denomination
         **/
        while (price.cents % MIN_COIN != 0) {

            printf("Error: $%i.%.2i is not a valid price, please round to the nearest 5c.\n", price.dollars,
                   price.cents);

            /* re-read the item price, break from loop if user aborts */
            if (!read_currency(prompt, PRICE_SIZE, &price))
            {
                canceled = TRUE;
                break;
            }
        }

        /* proceed only if the user has not aborted */
        if (!canceled)
        {

            /* assign price and default storck level */
            item->price = price;
            item->on_hand = DEFAULT_STOCK_LEVEL;

            /* attempt to add the item to the linked list */
            if (list_add(system->item_list, item)) {
                printf("This item \"%s - %s\" has now been added to the menu.\n\n", item->name, item->desc);
                return TRUE;
            } else {

                /* item could not be added to the list, free memory */
                free(item);
                printf("\nError: item could not be added to the menu.\n\n");
                return FALSE;
            }
        }
    }

    /**
     * if this block is reached, the user has aborted, free
     * memory allocated to the item and return false *
     **/
    free(item);
    return FALSE;

}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{

    /* local variables */
    struct ppd_stock * item;
    char prompt[PROMPT_SIZE];
    char item_id[IDLEN];

    printf("\n");

    /* prompt user and read the stock id to remove */
    sprintf(prompt, "Enter the item id of the item to remove from the menu");
    if (read_string(prompt, IDLEN, item_id)) {

        /* attempt to locate the stock item */
        item = find_stock_by_id(system->item_list, item_id);

        /* if no items are found, return false */
        if (item == NULL) {
            printf("Error: no item was found with that id.\n\n");
            return FALSE;

        /**
         * tell the user which item has been removed, although we have not
         * actually removed it yet - if we attempt to print after list_remove()
         * the memory allocated to the item is freed.
         **/
        } else {
            printf("\"%s - %s - %s\" has been removed from the system.\n\n", item->id, item->name, item->desc);
        }

        /* remote the item, and free any used memory */
        if (list_remove(system->item_list, item)) {
            return TRUE;
        } else {
            return FALSE;
        }

    } else {
        return FALSE;
    }
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
    struct ppd_stock * stock;

    /**
     * traverse the stock list and set each stock level to
     * the default value
     **/
    stock = traverse_list(system->item_list);
    while (stock)
    {
        stock->on_hand = DEFAULT_STOCK_LEVEL;
        stock = traverse_list(system->item_list);
    }
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    int i = 0;

    /**
     * iterate over the cash register and set each coin count
     * to the default value
     **/
    for (i=0; i < NUM_DENOMS; i++)
    {
        system->cash_register[i].count = DEFAULT_COIN_COUNT;
    }
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{

    /* local variables */
    int i = 0;
    char coin_name[COIN_NAME_LEN];

    /* print table header */
    printf("\nCoins Summary\n");
    printf("---------------\n");
    printf("%-*s | %-*s\n", 15, "Denomination", 10, "Count");
    printf("-----------------------\n");

    /* iterate each coin, and print name and count */
    for (i=0; i < NUM_DENOMS; i++)
    {
      get_coin_name(&system->cash_register[i], coin_name);
      printf("%-*s | %-*i\n", 15, coin_name, 10, system->cash_register[i].count);
    }

    printf("\n");
    return TRUE;
}

/**
 * This option aborts the system - without saving to files.
 * All malloced memory is freed before exiting.
 */
BOOLEAN abort_system(struct ppd_system * system)
{
    /* free all malloced memory */
    system_free(system);

    /* exit program */
    printf("\n\nGoodbye!\n\n");
    exit(EXIT_SUCCESS);

}
