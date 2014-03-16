/***************************************
 * Anemometer.h
 * created by Taiki Watai, 21 Nov 2013. 
 ***************************************/

#ifndef Anemometer_h
#define Anemometer_h
#include <stdint.h>
#define ANEMO 2

class Anemometer
{
public:
	void setup(void);
	void get_anemo (unsigned int *);
private:
};
#endif
