#include "calendar.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Initializes a Calendar struct based on the provided parameter*/
int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *ptr1, const void *ptr2),
                  void (*free_info_func)(void *ptr), Calendar **calendar) {
    /*Check conditions*/
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    } else if (days < 1) {
        return FAILURE;
    } else {
        /*Allocates the various items*/
        *calendar = malloc(sizeof(Calendar));
        (*calendar)->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
        (*calendar)->events = calloc(days, sizeof(Event));

        /*Checks if memory allocation fails*/
        if ((*calendar) == NULL) {
            return FAILURE;
        }
        if ((*calendar)->name == NULL || (*calendar)->events == NULL) {
            return FAILURE;
        }

        /*Initalizes new Calendar struct*/
        strcpy((*calendar)->name, name);
        (*calendar)->days = days;
        (*calendar)->total_events = 0;
        (*calendar)->comp_func = comp_func;
        (*calendar)->free_info_func = free_info_func;

        return SUCCESS;
    }
}

/*The function prints to the designated output stream various members*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i;
    Event *event_ptr;

    /*Checks conditions*/
    if (calendar == NULL || output_stream == NULL) {
        return FAILURE;
    } else {
        /*Checks if print_all is true and if so then print calendar name, days,
         * and total events*/
        if (print_all) {
            fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
            fprintf(output_stream, "Days: %d\n", calendar->days);
            fprintf(output_stream, "Total Events: %d\n",
                    calendar->total_events);
        }

        /*Print header*/
        fprintf(output_stream, "\n**** Events ****\n");

        /*Print name, start time, and duration*/
        if (calendar->total_events > 0) {
            for (i = 0; i < calendar->days; i++) {
                fprintf(output_stream, "Day %d\n", (i + 1));

                event_ptr = calendar->events[i];

                while (event_ptr) {
                    fprintf(output_stream, "Event's Name: \"%s\", ",
                            event_ptr->name);
                    fprintf(output_stream, "Start_time: %d, ",
                            event_ptr->start_time);
                    fprintf(output_stream, "Duration: %d\n",
                            event_ptr->duration_minutes);

                    event_ptr = event_ptr->next;
                }
            }
        }
        return SUCCESS;
    }
}

/*This function adds the specified event to the list assciated with the day
 * paramter*/
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {

    Event *event_temp, *new_event_ptr, *event_ptr;
    Event *prev_ptr = NULL;

    /*Checks condition of the function*/
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    } else if (start_time < 0 || start_time > 2400 || duration_minutes <= 0 ||
               day < 1 || day > calendar->days ||
               find_event(calendar, name, &event_temp) == SUCCESS) {
        return FAILURE;
    } else {
        /*Allocates memory*/
        new_event_ptr = malloc(sizeof(Event));
        new_event_ptr->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));

        /*Checks if memory allocation fails*/
        if (new_event_ptr == NULL) {
            return FAILURE;
        }
        if (new_event_ptr->name == NULL) {
            return FAILURE;
        }

        /*Initializes new_event*/
        strcpy((new_event_ptr)->name, name);
        new_event_ptr->start_time = start_time;
        new_event_ptr->duration_minutes = duration_minutes;
        new_event_ptr->info = info;

        /*Create "head" of list*/
        event_ptr = calendar->events[day - 1];

        /*Checks if null*/
        if (event_ptr == NULL) {
            calendar->events[day - 1] = new_event_ptr;
            new_event_ptr->next = NULL;
        } else {
            /*Sort using comp_func*/
            if (calendar->comp_func(event_ptr, new_event_ptr) > 0) {
                calendar->events[day - 1] = new_event_ptr;
                new_event_ptr->next = event_ptr;
            } else {
                prev_ptr = event_ptr;
                event_ptr = event_ptr->next;

                /*Checks condition of if event already exists or not*/
                while (event_ptr &&
                       calendar->comp_func(event_ptr, new_event_ptr) <= 0) {
                    prev_ptr = event_ptr;
                    event_ptr = event_ptr->next;
                }

                new_event_ptr->next = event_ptr;
                prev_ptr->next = new_event_ptr;
            }
        }
        (calendar->total_events)++;
        return SUCCESS;
    }
    return FAILURE;
}

/*This function returns a pointer to the event with the specified name*/
int find_event(Calendar *calendar, const char *name, Event **event) {
    int i;

    /*Checks conditions*/
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    } else {
        /*Iterates through the days in the calendar*/
        for (i = 0; i < calendar->days; i++) {
            if (find_event_in_day(calendar, name, (i + 1), event) == SUCCESS) {
                return SUCCESS;
            }
        }
        return FAILURE;
    }
}

/*Function returns a pointer to the event with the specified name in the
 * specified day*/
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
    Event *event_ptr;

    /*Checks conditions*/
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    } else if (day < 1 || day > calendar->days) {
        return FAILURE;
    }
    
    event_ptr = calendar->events[day - 1];

    /*Checks if event already exists by searching if event name is
     * already included*/
    while (event_ptr && strcmp(event_ptr->name, name) != 0) {
        /*Checks if event is NULL or not*/
        event_ptr = event_ptr->next;
    }

    if (event && event_ptr) {
        *event = event_ptr;
        return SUCCESS;
    }
    if (event_ptr)
        return SUCCESS;

    return FAILURE;
}

/*Function removes the specified event from the calendar*/
int remove_event(Calendar *calendar, const char *name) {
    Event **event_temp = NULL, *event_ptr, *event_prev = NULL;
    int i;

    /*Checks conditions*/
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    } else if (find_event(calendar, name, event_temp) == FAILURE) {
        return FAILURE;
    } else {
        /*iterates through the days in the calendar*/
        for (i = 0; i < calendar->days; i++) {

            event_ptr = calendar->events[i];

            if (event_ptr == NULL) {
                continue;
            }
            event_prev = event_ptr;

            while (event_ptr && strcmp(event_ptr->name, name)) {
                event_prev = event_ptr;
                event_ptr = event_ptr->next;
            }
            if (event_ptr) {
                /*Check if it is the head */
                if (event_ptr &&
                    strcmp(event_ptr->name, event_prev->name) == 0) {
                    calendar->events[i] = event_ptr->next;

                    /*Frees allocated memory*/
                    free(event_ptr->name);

                    /*Checks if free_info_func exists*/
                    if (calendar->free_info_func != NULL &&
                        event_ptr->info != NULL) {

                        calendar->free_info_func(event_ptr->info);
                    }
                    free(event_ptr);
                    (calendar->total_events)--;

                    return SUCCESS;
                } else {
                    /*Then it is the middle or the end */
                    event_prev->next = event_ptr->next;

                    /*Frees allocated memory*/
                    free(event_ptr->name);

                    /*Checks if free_info_func and info exists*/
                    if (calendar->free_info_func != NULL &&
                        event_ptr->info != NULL) {

                        calendar->free_info_func(event_ptr->info);
                    }

                    free(event_ptr);
                    (calendar->total_events)--;

                    return SUCCESS;
                }
            }
        }
    }
    return FAILURE;
}

/*Function returns the info pointer associated with the specified event*/
void *get_event_info(Calendar *calendar, const char *name) {
    Event *event_temp;

    /*Checks if the event already exists*/
    if (find_event(calendar, name, &event_temp) == SUCCESS) {
        return event_temp->info;
    } else {
        return NULL;
    }
}

/*This function removes all the event lists associated with the calendar and set
 * them to empty*/
int clear_calendar(Calendar *calendar) {
    int i;

    /*Check condition*/
    if (calendar == NULL) {
        return FAILURE;
    }
    /*Iterates through the days in the calendar*/
    for (i = 0; i < calendar->days; i++) {
        /*Clears the designated events*/
        clear_day(calendar, (i + 1));
    }
    calendar->total_events = 0;
    return SUCCESS;
}

/*This function removes all the events for the specified day, setting the event
 * list to empty*/
int clear_day(Calendar *calendar, int day) {
    Event *temp, *event_ptr;

    /*checks conditions*/
    if (calendar == NULL) {
        return FAILURE;
    } else if (day < 0 || day > calendar->days) {
        return FAILURE;
    } else {
        if (calendar->total_events > 0) {
            event_ptr = calendar->events[day - 1];

            while (event_ptr) {
                /*assign the events of current to temp and assign the next day's
                 * event to current day's event*/
                temp = event_ptr;
                event_ptr = event_ptr->next;

                if (calendar->free_info_func && temp->info) {
                    calendar->free_info_func(temp->info);
                }
                free(temp->name);
                free(temp);
                (calendar->total_events)--;
            }
        }
        calendar->events[day - 1] = NULL;
        return SUCCESS;
    }
    return FAILURE;
}

/*This function frees memory that was dynamically allocated for the calendar*/
int destroy_calendar(Calendar *calendar) {
    /*Checks condition of function*/
    if (calendar == NULL) {
        return FAILURE;
    } else {
        clear_calendar(calendar);

        free(calendar->name);
        free(calendar->events);
        free(calendar);

        return SUCCESS;
    }
}