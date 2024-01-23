#include <stdio.h>

#define TRUE  1
#define FALSE 0

int is_leapyear(int year)
{
    if(year%100 == 0) {
        if(year%400 == 0) {
            return 1;
        } else {
            return 0;
        }
    }
    if(year%4 == 0) {
        return 1;
    }
    return 0;
}

int day_of_the_week(int day, int month, int year) 
/* following the method on the lab assignment */
{
    int result = 0;
    int extract = year%100;
    int month_keys[] = {1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
    int year_keys[] = {4, 2, 0, 6};
    result = extract/4;
    result += day;
    result += month_keys[month-1];
    if (month == 1 || month == 2) {
        if(is_leapyear(year) == 1) {
            result -=1;
        }
    }
    while(year>2099) {
        year -= 400;
    }
    result += year_keys[year/100-17];
    result += extract;
    result = (result-1)%7;
    return result+1;
}

void print_calendar(int month, int year)
{
    int i;
    int count = 1;    
    int day = 1;
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  
    char *months[] = {"January",   "February", "March",    "April", 
                        "May",       "June",     "July",     "August",
                        "September", "October",  "November", "December"};

        /* Print calendar header */
    printf("%s, %i\n", months[month - 1], year);
    printf("Su Mo Tu We Th Fr Sa\n");
    printf("---------------------\n");
    for(i=1;i<day_of_the_week(1, month, year);++i) {
        printf("   ");
        ++count;
    }
    while(day <= days_in_month[month-1]) { /* printing each day 1 by 1 */ 
        printf("%2d ", day);
        ++count;
        ++day;
        if(count > 7 && day <= days_in_month[month-1]) {
            count = 1;
            printf("\n");
        }
    }
    printf("\n");

}