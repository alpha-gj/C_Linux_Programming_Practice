/**
* @file 定義traffic_parameter.h 的cpp內容
*
* 這個檔案定義下列的traffic model所需的參數
*
*
* @author jhanggj
*/

#include "stdafx.h"
#pragma once

#include "Traffic_Model_function.h"
#include <cmath> 
#include <cstdlib>
#include <random>

double ibm_random() {
	long ix = rand();
	long k = ix / 127773;
	ix = 16807 * (ix - k * 127773) - k * 2836;
	if (ix < 0) ix += 2147483647;

	return(ix * 4.656612875e-10);
}
int ibm_rand(int max, int min)
{
	max -= min;
	return (int)max * ibm_random() + min;
}
float pareto_on(float t) { //t=Alpha 
	double x;
	x = ibm_random();
	return((float)BETA_on*pow((1.0 - x), -1.0 / t));
}

float pareto_off(float t) { //t=Alpha 
	double x;
	x = ibm_random();
	
	return((float)BETA_off*pow((1.0 - x), -1.0 / t));
}

float expo(float t) {
	double x;
	x = ibm_random();

	return((float)-log(x) / t);

}

int get_random_int(int down, int up)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long int> dis(down, up);
	int value;
	

	value = dis(gen);
	
	return value;
}

