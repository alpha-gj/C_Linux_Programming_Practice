#include<stdlib.h>
#include<stdio.h>

#define PAN_LIMIT_ANGEL 360
#define TILT_LIMIT_ANGEL 90

typedef enum {
	PAN_CIRCLE_ANGEL = 0,
	PAN_FIXED_ANGEL = PAN_LIMIT_ANGEL,
} PAN_ANGEL_CAPBILITY;

typedef enum {
	TILT_CIRCLE_ANGEL = 0,
	TILT_FIXED_ANGEL = TILT_LIMIT_ANGEL,
} TILT_ANGEL_CAPBILITY;

int main(void)
{
	PAN_ANGEL_CAPBILITY pan_angel_capbility = PAN_CIRCLE_ANGEL;
	TILT_ANGEL_CAPBILITY tilt_angel_capbility = TILT_FIXED_ANGEL;
	
	printf("pan is %d\n", (int)pan_angel_capbility);
	printf("tilt is %d\n", (int)tilt_angel_capbility);

	return 0;
}
