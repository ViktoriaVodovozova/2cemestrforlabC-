#ifndef GET_POINTS_H
#define GET_POINTS_H

#include "classes/Dataset.h"

// объявления функций
double randomDouble(double min, double max);
Dataset get_points(int first_cl_size, int second_cl_size, float k, float b);

#endif // GET_POINTS_H