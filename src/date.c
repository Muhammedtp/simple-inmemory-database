#include"date.h"

datetype packdate(int day, int month, int year){
    if(year<0 || year > ((1<<yearbits)-1) || month<1 || month>12 || day >31 || day <1){
        printf("Error: Invalid date - Year: %d, Month: %d, Day: %d\n", year, month, day);
        return 0;
    }

    if (!is_valid_date(day, month, year)) {
        printf("Error: Invalid date - %d/%d/%d doesn't exist\n", day, month, year);
        return 0;
    }

    return (year<<monthbits+daybits) | (month << daybits) | day;
}



void unpackdate(datetype date, int* day, int *month,int *year){
    if (year != NULL) {
        *year = (date >> (monthbits + daybits)) & ((1 << yearbits) - 1);
    }
    if (month != NULL) {
        *month = (date >> daybits) & ((1 << monthbits) - 1);
    }
    if (day != NULL) {
        *day = date & ((1 << daybits) - 1);
    }
}

bool is_valid_date(int day, int month, int year) {
    if (month < 1 || month > 12 || day < 1) {
        return false;
    }
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2) {
        bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (is_leap) {
            days_in_month[1] = 29;
        }
    }
    
    return day <= days_in_month[month - 1];
}

void print_date(datetype date) {
    int year, month, day;
    unpackdate(date, &year, &month, &day);
    printf("%02d-%02d-%04d", year, month, day);
}

int compare_dates(datetype date1, datetype date2) {
    if (date1 < date2) return -1;
    if (date1 > date2) return 1;
    return 0;
}


datetype parse_date_string(const char* date_str) {
    if (!date_str) {
        printf("Error: Null date string\n");
        return 0;
    }
    
    int day, month, year;
    int parsed = 0;

    // Format 1: DD/MM/YYYY 
    if (sscanf(date_str, "%d/%d/%d", &day, &month, &year) == 3) {
        parsed = 1;
    }

    // Format 2: DD-MM-YYYY
    else if (sscanf(date_str, "%d-%d-%d", &day, &month, &year) == 3) {
        parsed = 1;
    }
    
    
    if (!parsed) {
        printf("Error: Invalid date format '%s'. Supported formats: DD/MM/YYYY and DD-MM-YYYY\n", date_str);
        return 0;
    }
    
    return packdate(day, month, year);
}
