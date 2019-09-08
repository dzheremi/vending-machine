/***********************************************************************
 * Advanced Programming Techniques
 * Semester 2 2015 
 * Jeremy John Mercer
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_menu.h"
#include "ppd_utility.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
    strcpy(menu->name, "");
    menu->function = NULL;
}

/**
 * runs the main menu, in an infinite - accepts 2 arrays of menu_item struct,
 * one being the main menu, the other being the admin menu.
 */
void run_menu(struct ppd_system * system, struct menu_item main_menu[NUM_MAIN_MENU],
              struct menu_item admin_menu[NUM_ADMIN_MENU])
{

    /* local variables */
    int selection = 0;

    /* infinite loop to keep the main menu running */
    while(TRUE)
    {
        /* display menus and accept user selection */
        selection = display_menu(main_menu, admin_menu);

        /**
         * if selection is valid, run the associated function using a function
         * pointer
         **/
        if (selection > 0)
        {
            if (selection <= NUM_MAIN_MENU) {
                main_menu[selection - 1].function(system);
            } else {
                admin_menu[selection - NUM_MAIN_MENU - 1].function(system);
            }
        }
    }
}

/**
 * prints menu options to screen
 **/
int display_menu(struct menu_item main_menu[NUM_MAIN_MENU], struct menu_item admin_menu[NUM_ADMIN_MENU])
{

    /* local variables */
    int selection = 0;
    char prompt[PROMPT_SIZE];

    /* print main menu */
    printf("\nMain Menu:\n");
    display_main_menu(main_menu);

    /* print admin menu */
    printf("Administrator-Only Menu:\n");
    display_admin_menu(admin_menu);
    printf("\n");

    /* read user selection from keyboard */
    sprintf(prompt, "Select you option (%i-%i)", 1, (NUM_MAIN_MENU + NUM_ADMIN_MENU));
    read_integer(prompt, &selection ,1, 1, (NUM_MAIN_MENU + NUM_ADMIN_MENU));

    return selection;
}

/**
 * prints the main menu options to screen
 */
void display_main_menu(struct menu_item main_menu[NUM_MAIN_MENU])
{
    int i = 0;
    for (i = 0; i < NUM_MAIN_MENU; i++)
    {
        printf("\t%i. %s\n", (i+1), main_menu[i].name);
    }
}

/**
 * prints the admin menu options to screen
 */
void display_admin_menu(struct menu_item admin_menu[NUM_ADMIN_MENU])
{
    int i = 0;
    for (i = 0; i < NUM_ADMIN_MENU; i++)
    {
        printf("\t%i. %s\n", (i + NUM_MAIN_MENU + 1), admin_menu[i].name);
    }
}
