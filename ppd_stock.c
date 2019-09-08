/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_stock.h"

 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/

/**
 * initialise the list nodes to a safe working state
 **/
void list_init(struct ppd_list * list)
{
    list->head = NULL;
    list->count = 0;
}

/**
 * frees all memory used by a list
 */
void free_list(struct ppd_list * list)
{

    /* local variables */
    struct ppd_node * curr;
    struct ppd_node * next;

    /**
     * iterate over the list, and free memory allocated to data and
     * associated nodes
     **/
     curr = list->head;
     while (curr)
     {
         next = curr->next;
         free(curr->data);
         free(curr);
         curr = next;

     }
}

/**
 * add a stock item to a list
 **/
int list_add(struct ppd_list * list, struct ppd_stock * stock)
{

    /* local variables */
    struct ppd_node * curr;
    struct ppd_node * prev;

    /* create and allocate memory to a new list node */
    struct ppd_node * node;
    node = malloc(sizeof(struct ppd_node));
    if (node == NULL)
    {
        printf("Error: could not allocate memory to program.\n\n");
        return 0;
    }

    /* points are set to keep track of positioning in the list */
    node->data = stock;
    node->next = NULL;

    /* if this is the first item in the list */
    if (list->count == 0)
    {

        /* insert the item at the head of the list */
        node->next = list->head;
        list->head = node;
        list->count++;

    /* if the item is not the first item in the list */
    } else {

        /* iterate over the list */
        curr = list->head;
        prev = NULL;
        while (curr)
        {

            /* check the correct position in lexicographic order using item name */
            if (strcmp(stock->name, curr->data->name) <= 0)
            {

                /* insert the item node into the correct position */
                node->next = curr;
                if(prev)
                {
                    prev->next = node;
                } else {
                    list->head = node;
                }
                break;

            }

            /* item not inserted, move to next node in list */
            prev = curr;
            curr = curr->next;

        }

        /**
         * after iterating the entire list, if the item is still not inserted, it
         * must belong in the last position
         **/
        if (node->next == NULL)
        {
            prev->next = node;
        }

        /* increment the list counter */
        list->count++;
    
    }

    return 1;

}

/**
 * removes stock from a list
 */
int list_remove(struct ppd_list * list, struct ppd_stock * stock)
{

    /* local variables */
    struct ppd_node * curr;
    struct ppd_node * prev;

    /* if there is only one item in the list, remove the head */
    if (list->count == 1)
    {
        free(list->head);
        list->head = NULL;
        list->count = 0;
        return 1;

    /* if the list is already empty, do nothing */
    } else if (list->count == 0) {
        return 0;

    /* iterate over the list, looking for the item to remove */
    } else {

        /* points are set to keep track of positioning in the list */
        curr = list->head;
        prev = NULL;
        while (curr)
        {

            /* if the item is found, remove it from the list */
            if (strcmp(stock->id, curr->data->id) == 0)
            {
                if(prev)
                {
                    prev->next = curr->next;
                } else {
                    list->head = curr->next;
                }

                /* free allocated memory to node and item */
                free(curr->data);
                free(curr);

                /* de-increment the list counter */
                list->count--;
                return 1;
            }

            /* item not remove, move to next node in list */
            prev = curr;
            curr = curr->next;

        }

        return 0;
    }
}

/**
 * traverses the stock list
 **/
struct ppd_stock * traverse_list(struct ppd_list * list)
{

    /**
     * constant local variable used to keep track of the position in the list,
     * the last time this function was called
     **/
    static const struct ppd_node * position = NULL;

    /* if the position pointer is NULL, reset it back to the head of the list */
    if (!position)
    {
        position = list->head;

    /* move the position pointer forward one node */
    } else {
        position = position->next;
    }

    /* ensure the pointer is still pointing at something */
    if (position)
    {

        /* return the item the node is pointing to */
        return position->data;

    /* return NULL when the end of the list is reached */
    } else {
        return NULL;

    }
}

/**
 * finds a stock item by stock id
 **/
struct ppd_stock * find_stock_by_id(struct ppd_list * list, char * stock_id)
{

    /* iterate over the list */
    struct ppd_node * curr;
    curr = list->head;
    while (curr)
    {

        /* if stock id matches, return the item */
        if (strcmp(curr->data->id, stock_id) == 0)
        {
            return curr->data;

        /* if stock id does not match, move to next node */
        } else {
            curr = curr->next;

        }
    }

    /* if control reaches here, no item has been found */
    return NULL;

}

/**
 * get the next unallocated stock id
 **/
int get_next_stock_id(struct ppd_list * list, char * destination)
{

    /* local variables */
    struct ppd_stock * item;
    int this_id = 0;
    int last_id = 0;
    char * end = NULL;

    /* traverse the item list */
    item = traverse_list(list);
    while(item)
    {

        /**
         * call strtol to move our char * end to the first readable
         * integer in the item id
         **/
        strtol(item->id, &end, 10);

        /* read the item id integer component */
        this_id = (int)(strtol((end + 1), &end, 10));

        /* if the read item id integer is higher than any other found, save it */
        if(this_id > last_id)
        {
            last_id = this_id;
        }

        /* move to next item */
        item = traverse_list(list);

    }

    /* increment the highest found stock id by 1 */
    this_id = ++last_id;

    /* copy the new stock id string (re-insert the I) */
    sprintf(destination, "I%.4i", this_id);
    return 1;

}

/**
 * convert a price struct to its value in cents
 **/
int price_to_cents(struct price price)
{
    int cents = 0;
    cents = price.dollars * CENTS_IN_DOLLAR;
    cents += price.cents;
    return cents;
}

/**
 * convert a value in cents to a price struct
 **/
struct price cents_to_price(int cents)
{
    struct price price;
    price.dollars = (unsigned)cents / CENTS_IN_DOLLAR;
    price.cents = (unsigned)cents % CENTS_IN_DOLLAR;
    return price;
}

/**
 * initialise a stock item to a safe working state
 **/
void stock_init(struct ppd_stock * stock)
{
    strcpy(stock->id, "");
    strcpy(stock->name, "");
    strcpy(stock->desc, "");
    stock->price.dollars = 0;
    stock->price.cents = 0;
    stock->on_hand = 0;
}
