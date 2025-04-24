/**
 * @file time.c
 * @brief Function to control simulated time.
 */

#include "time.h"
#include "utils.h"

/**
  * @brief Updates the system's current simulated date.
  *
  * If the new date is invalid or earlier than the current date,
  * prints an error message.
  *
  * @param sys Pointer to the system.
  * @param in Input string containing the new date.
  */
void timeControl(Sys *sys, char *in){
    Date temp;
    if(sscanf(in, "%*s %2d-%2d-%d", &temp.day, &temp.month, &temp.year) < 1){
        printf("%.2d-%.2d-%d\n", sys->tcurr.day,
            sys->tcurr.month, sys->tcurr.year);
        return;
    }else if((!verifyDate(temp))||(compareDates(sys->tcurr, temp) > 0)){
        if(sys->state == PT){
            puts(PTEINVDATE);
        }else{
            puts(ENGEINVDATE);
        }
        return;
    }else{
        sys->tcurr = temp;
        printf("%.2d-%.2d-%d\n", sys->tcurr.day,
            sys->tcurr.month, sys->tcurr.year);
    }
}