#ifndef SEQ_ITERATIONS_H
#define SEQ_ITERATIONS_H

#include <vector>
#include <stddef.h>

/* returns vector of points that will be used for visualization */
std::vector<double> get_seq_iteration_points(double r, size_t iterations);

/**
   Calculates the next point for iterations method

   @param r the constant at function
   @param restart to begin calulations for new function
   @return new point in iterations method
*/
double get_next_point(double r, bool restart);

//TODO calculate the right amount of iterations following the apriot/aposerior evaluation
//TODO for Ivan
size_t get_amount_of_iterations(double r, int it);

std::vector<double> get_sequence_of_x_n (double r, int it, double conv);


#endif // SEQ_ITERATIONS_H
