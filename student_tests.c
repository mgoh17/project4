#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

/*Use as comp_func*/
static int comp_minutes(const void *ptr1, const void *ptr2) {
    return ((Event *)ptr1)->duration_minutes -
           ((Event *)ptr2)->duration_minutes;
}

/* Description here: This test checks the print_calendar function*/
static int test1() {
    int days = 7;
    Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            return destroy_calendar(calendar);
        }
    }

    return FAILURE;
}

/* This test checks the add_event function*/
static int test2() {
    int days = 4;
    Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "coffee date", 1200, 60, NULL, 2) == SUCCESS &&
            add_event(calendar, "study date", 900, 45, NULL, 3) == SUCCESS &&
            add_event(calendar, NULL, 500, 35, NULL, 3) == FAILURE &&
            add_event(calendar, "Stalk Dictionary's Date", 1200, 0, NULL, 2) == FAILURE &&
            add_event(calendar, "Morgy Poo", -10, 10, NULL, 2) == FAILURE &&
            add_event(calendar, "study date", 900, 45, NULL, 3) == FAILURE) {
            return destroy_calendar(calendar);
        }
    }

    return FAILURE;
}

/* This test checks the find_event function*/
static int test3() {
    int days = 6;
    Calendar *calendar;
    Event *event;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "coffee date", 1200, 60, NULL, 5) == SUCCESS &&
            add_event(calendar, "study date", 900, 45, NULL, 4) == SUCCESS) {
            if (find_event(calendar, "coffee date", &event) == SUCCESS) {
                return destroy_calendar(calendar);
            }
        }
    }

    return FAILURE;
}

/* This test checks the find_event_in_day function*/
static int test4() {
    int days = 9;
    Calendar *calendar;
    Event *event;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "coffee date", 1200, 60, NULL, 6) == SUCCESS &&
            add_event(calendar, "study date", 900, 45, NULL, 7) == SUCCESS) {
            if (find_event_in_day(calendar, "coffee date", 6, &event) ==
                SUCCESS) {
                return destroy_calendar(calendar);
            }
        }
    }

    return FAILURE;
}

/* This test checks the remove_event function*/
static int test5() {
    int days = 13;
    Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "coffee date", 1200, 60, NULL, 6) == SUCCESS &&
            add_event(calendar, "study date", 900, 45, NULL, 7) == SUCCESS) {
            if (remove_event(calendar, "coffee date") == SUCCESS) {
               return destroy_calendar(calendar);
            }
        }
    }

    return FAILURE;
}

/* This test checks the get_event_info function*/
static int test6() {
    int days = 18;
    Calendar *calendar;
    char info = 'm';

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "coffee date", 1200, 60, &info, 6) == SUCCESS &&
            add_event(calendar, "study date", 900, 45, NULL, 7) == SUCCESS) {
            if (get_event_info(calendar, "coffee date") == &info) {
                return destroy_calendar(calendar);
            }
        }
    }

    return FAILURE;
}

/* This test checks the clear_calendar function*/
static int test7() {
    int days = 18;
    Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "coffee date", 1200, 60, NULL, 6) == SUCCESS &&
            add_event(calendar, "study date", 900, 45, NULL, 7) == SUCCESS) {
            if (clear_calendar(calendar) == SUCCESS) {
                return destroy_calendar(calendar);
            }
        }
    }

    return FAILURE;
}

/* This test checks the clear_day function*/
static int test8() {
    int days = 18;
    Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (add_event(calendar, "coffee date", 1200, 60, NULL, 6) == SUCCESS &&
            add_event(calendar, "study date", 900, 45, NULL, 7) == SUCCESS) {
            if (clear_day(calendar, 5) == SUCCESS) {
                return destroy_calendar(calendar);
            }
        }
    }

    return FAILURE;
}


int main() {
    int result = SUCCESS;

    if (test1() == FAILURE)
        result = FAILURE;
    if (test2() == FAILURE)
        result = FAILURE;
    if (test3() == FAILURE)
        result = FAILURE;
    if (test4() == FAILURE)
        result = FAILURE;
    if (test5() == FAILURE)
        result = FAILURE;
    if (test6() == FAILURE)
        result = FAILURE;
    if (test7() == FAILURE)
        result = FAILURE;
    if (test8() == FAILURE)
        result = FAILURE;

    if (result == FAILURE) {
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
