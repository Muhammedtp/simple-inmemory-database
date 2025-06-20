#ifndef DATE_H
#define DATE_H

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>

//0 - 4096 ACE
#define yearbits 12
#define monthbits 4
#define daybits 5

typedef uint32_t datetype;

datetype packdate(int day, int month, int year);
void unpackdate(datetype date, int* day, int *month,int *year);
bool is_valid_date(int day, int month, int year);
void print_date(datetype date);
int compare_dates(datetype date1, datetype date2);
datetype parse_date_string(const char* date_str);


#endif