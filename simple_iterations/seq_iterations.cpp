#include "seq_iterations.h"
#include <stdexcept>
#include <random>

using dbl = double;

/* function phi from equation x = phi(x), where phi(x) = r*x*(1 - x) */
static inline dbl phi(dbl x, dbl r)
{
   return r*x*(1 - x);
}

/* returns random point from interval (a, b) */
static dbl get_point_from_interval(dbl a, dbl b)
{
   static std::random_device rd;
   static std::mt19937 gen(rd());
   std::uniform_real_distribution<> dis(a, b);
   return dis(gen);
}

/* returns the lower point of the interval at which "good" point x0 must be */
static dbl lower_endpoint(dbl r)
{
   return (r - 1) / (2 * r);
}

/* returns the upper point of the interval at which "good" point x0 must be */
static dbl upper_endpoint(dbl r)
{
   return (r + 1) / (2 * r);
}

/* returns the x(0) from the sequnce x(n) */
static dbl get_null_point(dbl r)
{
   if (r <= 0.) throw std::runtime_error("negative constatn r is not allowed");
   dbl lp = lower_endpoint(r);
   dbl up = upper_endpoint(r);
   return get_point_from_interval(lp, up);
}

/* returns vector of points that will be used for visualization */
std::vector<dbl> get_seq_iteration_points(dbl r)
{
   int ITERATIONS = get_amount_of_iterations(r); //TODO understand amount of iterations

   std::vector<dbl> ans;
   dbl x = get_null_point(r);
   ans.push_back(get_next_point(r, true));
   for (int i = 0; i < ITERATIONS; i++)
   {
      ans.push_back(get_next_point(r, false));
   }
   return ans;
}

double get_next_point(double r, bool restart)
{
   static dbl x;
   if (restart)
   {
      x = get_null_point(r);
      return x;
   }
   x = phi(x, r);
   return x;
}

size_t get_amount_of_iterations(double r)
{
   return 10;
}
