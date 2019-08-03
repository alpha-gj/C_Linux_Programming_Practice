
#pragma once

/*********************/
/* Traffic parameter */
/********************/
#define Lambda_RT_ON1 5
#define Lambda_RT_ON2 5
#define Lambda_NRT_ON 35
#define Lambda_OFF 1

#define ALPHA_ON1	1.0	// preto // modify~~//越大量越大
#define ALPHA_ON2	1.2// preto // modify~~//越大量越大
#define ALPHA_ON3	0	// preto // modify~~//越大量越大
#define ALPHA_ON4	1.9	// preto // modify~~//越大量越大

#define ALPHA_OFF1	1.9		// preto
#define ALPHA_OFF2	1.6	// preto
#define ALPHA_OFF3	0		// preto
#define ALPHA_OFF4	1.4		// preto
#define BETA_on 1
#define BETA_off 1

/*********************/
/* Traffic function */
/********************/
double ibm_random();
int ibm_rand(int max, int min);
float pareto_on(float t);	//t=Alpha
float pareto_off(float t);	//t=Alpha
float expo(float t);
int get_random_int(int down, int up);




