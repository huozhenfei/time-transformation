// 而日历时间（Calendar Time）是通过time_t数据类型来表示的，
// 用time_t表示的时间（日历时间）是从一个时间点（例如：1970年1月1日0时0分0秒）
// 到此时的秒数
// 此程序将日历时间转化为日期 (年月日时分秒) (不考虑能被100整除且不能被400整除的年份为平年)
// 可以由日历时间算出日历时间对应的天数%7+初始星期即可算出
// common year ----leap year
// from 1970.1.1 0:0:0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DAY_SECOND (24*60*60)
#define COMMON_YEAR_SECOND (365*DAY_SECOND)
#define LEAP_YEAR_SECOND (366*DAY_SECOND)
#define YEAR_CIRCLE_SECOND (COMMON_YEAR_SECOND*3 + LEAP_YEAR_SECOND*1)
char* gmtime2(unsigned long second0);
int* Find_date_from_second(int year_second_left, int leap_common);
int main(int argc, char const *argv[])
{
	time_t time0 = YEAR_CIRCLE_SECOND*12 + 3600*24*1320 + 3666;
	struct tm *tm0 =  gmtime(&time0);
	int *date = gmtime2(time0);
	printf("%d-%d-%d %d:%d:%d\n", tm0->tm_year+1900, tm0->tm_mon+1, tm0->tm_mday,
		tm0->tm_hour, tm0->tm_min, tm0->tm_sec);
	free(date);
	return 0;
}

char* gmtime2(unsigned long second0)
{
	char *date2 = (char*)malloc(25*sizeof(char));
	int *date_in_year;
	int year_circle, circle_second_left, year_second_left;
	int date[6];
	year_circle = second0 / YEAR_CIRCLE_SECOND;
	circle_second_left = second0 % YEAR_CIRCLE_SECOND;
	date[0] = 1970 + year_circle * 4;
	if (circle_second_left >= COMMON_YEAR_SECOND*2 + LEAP_YEAR_SECOND*1) //common year
	{
		year_second_left = circle_second_left - (COMMON_YEAR_SECOND*2 + LEAP_YEAR_SECOND*1);
		date[0] += 3;
		date_in_year = Find_date_from_second(year_second_left, 0);
	}
	else if (circle_second_left >= COMMON_YEAR_SECOND*2) //leap year
	{
		year_second_left = circle_second_left - COMMON_YEAR_SECOND*2;
		date[0] += 2;
		date_in_year = Find_date_from_second(year_second_left, 1);
	}
	else if (circle_second_left >= COMMON_YEAR_SECOND) //common year
	{
		year_second_left = circle_second_left - COMMON_YEAR_SECOND;
		date[0] += 1;
		date_in_year = Find_date_from_second(year_second_left, 0);
	}
	else //common year
	{
		year_second_left = circle_second_left;
		date_in_year = Find_date_from_second(year_second_left, 0);
	}

	memcpy(&date[1], date_in_year, 5 * sizeof(int));
	free(date_in_year);
	sprintf(date2, "%d-%d-%d %d:%d:%d", date[0], date[1], date[2], date[3], date[4], date[5]);
	printf("%s\n", date2);
	return date2;
}

int* Find_date_from_second(int year_second_left, int leap_common) //leap_common:leap=1,common=0
{
	int month_common_day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int month_leap_day[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int i, j;
	int *date = (int*)calloc(1, 5 * sizeof(int));
	for (i = 11; i > 0; --i) //初始化月份含天数表（变为累加和形式）
	{
		month_common_day[i] = 0;
		for (j = i-1; j >= 0; --j)
		{
			month_common_day[i] += month_common_day[j] * DAY_SECOND;
		}
		month_leap_day[i] = 0;
		for (j = i-1; j >= 0; --j)
		{
			month_leap_day[i] += month_leap_day[j] * DAY_SECOND;
		}
	}
	month_common_day[0] = month_leap_day[0] = 0;
	if (year_second_left == 0)
	{
		date[0] = 1;
		date[1] = 1;
		return date;
	}
	else
	{
		if (leap_common == 1) //leap year
		{
			for (i = 11; i >= 0; --i) //month
			{
				if (year_second_left > month_leap_day[i])
				{
					date[0] = i+1;
					break;
				}
			}
			year_second_left -= month_leap_day[i];
		}
		else //common year
		{
			for (i = 11; i >= 0; --i) //month
			{
				if (year_second_left > month_common_day[i])
				{
					date[0] = i+1;
					break;
				}
			}
			year_second_left -= month_common_day[i];
		}
		date[1] = year_second_left / DAY_SECOND; //day
		year_second_left -= date[1] * DAY_SECOND;
		date[1] += 1;
		date[2] = year_second_left / (60 * 60); //hour
		year_second_left -= date[2] * (60 * 60);
		date[3] = year_second_left / 60;	//min
		date[4] = year_second_left % 60;	//sec
	}
	return date;
}
