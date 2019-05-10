#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct timespec tt;
	clock_getres(CLOCK_REALTIME, &tt);
	printf("resolution: %ld(nano sec)\n", tt.tv_nsec);

	struct timespec tt1, tt2;
	int result;

    printf("using CLOCK_MONOTONIC\n");
	clock_gettime(CLOCK_MONOTONIC, &tt1);
	result = abs(-33);
	clock_gettime(CLOCK_MONOTONIC, &tt2);
	printf("abs() consumes %ld nanoseconds!\n(Result of abs: %d)\n", tt2.tv_nsec - tt1.tv_nsec, result);

	return 0;
}
