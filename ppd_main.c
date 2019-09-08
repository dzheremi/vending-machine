/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{

    /* local variables */
    BOOLEAN error = FALSE;
    int i = 0;
    struct menu_item main_menu[NUM_MAIN_MENU];
    struct menu_item admin_menu[NUM_ADMIN_MENU];

    /* represents the data structures to manage the system */
    struct ppd_system system;

    /* setup main menu options and functions */
    MENU_ITEMS main_options[NUM_MAIN_MENU] =
    {
        "Display Items",
        "Purchase Item",
        "Save and Exit"
    };
    MENU_FUNCTIONS main_functions[NUM_MAIN_MENU] =
    {
        display_items,
        purchase_item,
        save_system
    };

    /* setup admin menu options and function */
    MENU_ITEMS admin_options[NUM_ADMIN_MENU] =
    {
        "Add Item",
        "Remove Item",
        "Display Coins",
        "Reset Stock",
        "Reset Coins",
        "Abort Program"
    };
    MENU_FUNCTIONS admin_functions[NUM_ADMIN_MENU] =
    {
        add_item,
        remove_item,
        display_coins,
        reset_stock,
        reset_coins,
        abort_system
    };

    /* validate command line arguments */
    if (argc != NUM_ARGS)
    {
        printf("Error: invalid arguments passed in.\n\n");
        error = TRUE;
    }

    /* init the system */
    if (!error)
    {
        if (system_init(&system, argv[1], argv[2]) == FALSE)
        {
            printf("Error: could not initialise system.\n\n");
            error = TRUE;
        }
    }

    /* load data into the system struct from data files */
    if (!error)
    {
        if (load_data(&system, argv[2], argv[1]) == FALSE)
        {
            error = TRUE;
        }
    }

    /* test if everything has been initialised correctly */

    /* do not continue if an error has occurred */
    if (error)
    {
        printf("Correct arguments are:\n");
        printf("\t./ppd <stockfile> <coinfile>\n");
        printf("Where <stockfile> and <coinfile> are two valid files in the expected format.\n");
        return EXIT_FAILURE;
    }

    /* initialise the menu system */
    for (i=0; i < NUM_MAIN_MENU; i++)
    {
        init_menu(&main_menu[i]);
        strcpy(main_menu[i].name, main_options[i]);
        main_menu[i].function = main_functions[i];
    }
    for (i=0; i < NUM_ADMIN_MENU; i++)
    {
        init_menu(&admin_menu[i]);
        strcpy(admin_menu[i].name, admin_options[i]);
        admin_menu[i].function = admin_functions[i];
    }

    /* loop, asking for options from the menu */
    run_menu(&system, main_menu, admin_menu);

    return EXIT_SUCCESS;
}
