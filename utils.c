/**
 * @file utils.c
 * @brief Utility functions for date handling.
 */

#include "utils.h"

/**
  * @brief Returns the number of days in a given month and year.
  *
  * @param month Month (1-12)
  * @param year Year
  * @return Number of days in the month.
  */
 int daysInM(int month, int year){
    int daysPerM[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(month == 2 && ((year % 4 == 0 && year % 100 != 0) ||
    (year % 400 == 0))){
        return 29;
    }
    return daysPerM[month - 1];
}
 
/**
  * @brief Checks if a date is valid.
  *
  * @param date The date to check.
  * @return 1 if valid, 0 otherwise.
  */
int verifyDate(Date date){
    if(1 > date.month || 12 < date.month){
        return 0;
    }else if(date.day > daysInM(date.month, date.year) || date.day < 1){
        return 0;
    }
    return 1;
 }
 
/**
  * @brief Compares two dates.
  *
  * @param d1 First date.
  * @param d2 Second date.
  * @return Negative if d1 is before d2, zero if equal, positive if after.
  */
int compareDates(Date d1, Date d2){
    if(d1.year != d2.year){
        return d1.year - d2.year;
    }
    if(d1.month != d2.month){
        return d1.month - d2.month;
    }
    return d1.day - d2.day;
}
