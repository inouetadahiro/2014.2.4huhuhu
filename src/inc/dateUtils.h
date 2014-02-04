
#ifndef dateUtilsH
#define dateUtilsH

#ifndef bool
//#define bool int
typedef int bool;
#endif

typedef unsigned short TDate;

#define Monday		0x1
#define Tuesday		0x2
#define Wednesday	0x3
#define Thursday	0x4
#define Friday		0x5
#define Saturday	0x6
#define Sunday		0x7

#define LIMIT_DATE	36160

bool isValidDate(unsigned char year, unsigned char month, unsigned char day);
TDate encodeDate(unsigned char year, unsigned char month, unsigned char day);
void decodeDate(unsigned char * res, TDate date);
TDate recodeDate(TDate date, int year, int month, int day);
unsigned char yearOf(TDate date);
unsigned char monthOf(TDate date);
unsigned char dayOf(TDate date);
unsigned char dayOfTheWeek(TDate date);
bool isSameDay(TDate date1, TDate date2);
int daysBetween(TDate date1, TDate date2);
bool isInLeapYear(TDate date);

#endif
