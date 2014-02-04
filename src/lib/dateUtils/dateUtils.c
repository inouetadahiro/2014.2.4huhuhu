
#include "dateUtils.h"

static const unsigned char MONTH_DAYS[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}		//うるう年
};

bool isLeapYear(unsigned char year)//int year)
{
	return (year % 4 == 0);		//今のところ0 <= year <= 99
	//return (year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
}

bool isValidDate(unsigned char year, unsigned char month, unsigned char day)//int
{
	return (year >= 0 && year <= 99//9999
			&& month >= 1 && month <= 12
			&& day >= 1 && day <= MONTH_DAYS[(unsigned char) isLeapYear(year)][month]);
}

TDate encodeDate(unsigned char year, unsigned char month, unsigned char day)
{
	TDate result;
	int i;

	result = (unsigned short) year * 365 + (year + 3) / 4 + day;
	if (isLeapYear(year))
		for (i = 1; i < month; i++)
			result += MONTH_DAYS[1][i];
	else
		for (i = 1; i < month; i++)
			result += MONTH_DAYS[0][i];

	return result;
}

void decodeDate(unsigned char * res, TDate date)
{
/*	unsigned char temp;

	res[0] = 0;
	while (true)
	{
		temp = (isLeapYear(res[0])) ? 365 + 1 : 365;
		if (date > temp)
		{
			res[0]++;
			date -= temp;
		}
		else
			break;
	}
	if (isLeapYear(res[0]))
		for (temp = 1; date > MONTH_DAYS[1][temp]; date -= MONTH_DAYS[1][temp], temp++);
	else
		for (temp = 1; date > MONTH_DAYS[0][temp]; date -= MONTH_DAYS[0][temp], temp++);
	res[1] = temp;
	res[2] = (unsigned char) date;*/
	unsigned char temp;

	for (temp = 0; date > (365 * 4 + 1); temp += 4, date -= (365 * 4 + 1));
	if (date > (365 + 1))
	{
		temp++;
		date -= (365 + 1);
		for (; date > 365; temp++, date -= 365);
	}
	res[0] = temp;
	if (isLeapYear(res[0]))
		for (temp = 1; date > MONTH_DAYS[1][temp]; date -= MONTH_DAYS[1][temp], temp++);
	else
		for (temp = 1; date > MONTH_DAYS[0][temp]; date -= MONTH_DAYS[0][temp], temp++);
	res[1] = temp;
	res[2] = (unsigned char) date;
}

TDate recodeDate(TDate date, int year, int month, int day)
{
//負で指定無し、0以上ならありえない数でも設定
	unsigned char temp[3];

	decodeDate(temp, date);
	if (year >= 0)
		temp[0] = year;
	if (month >= 0)
		temp[1] = month;
	if (day >= 0)
		temp[2] = day;
	return encodeDate(temp[0], temp[1], temp[2]);
}

unsigned char yearOf(TDate date)		//効率悪いからあまり使いたくはないけど
{
	unsigned char temp[3];

	decodeDate(temp, date);
	return temp[0];
}

unsigned char monthOf(TDate date)
{
	unsigned char temp[3];

	decodeDate(temp, date);
	return temp[1];
}

unsigned char dayOf(TDate date)
{
	unsigned char temp[3];

	decodeDate(temp, date);
	return temp[2];
}

unsigned char dayOfTheWeek(TDate date)
{
	return (unsigned char) ((date + 4) % 7 + 1);
}

bool isSameDay(TDate date1, TDate date2)		//ﾏｸﾛにするべき
{
	return (date1 == date2);
}

int daysBetween(TDate date1, TDate date2)		//ﾏｸﾛにするべく
{
	return (int) (date1 - date2);
}

bool isInLeapYear(TDate date)		//ﾏｸﾛにするべけ
{
	return isLeapYear(yearOf(date));
}

