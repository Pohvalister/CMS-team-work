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
   dbl lp = lower_endpoint(r);
   dbl up = upper_endpoint(r);
   if (r <= 0.) throw std::runtime_error("negative constatn r is not allowed");
   return get_point_from_interval(lp, up);
}

/* returns vector of points that will be used for visualization */
std::vector<dbl> get_seq_iteration_points(dbl r)
{
   constexpr int ITERATIONS = 10; //TODO understand amount of iterations

   std::vector<dbl> ans;
   dbl x = get_null_point(r);
   ans.push_back(x);
   for (int i = 0; i < ITERATIONS; i++)
   {
      dbl y = phi(x, r);
      ans.push_back(y);
      x = y;
   }
   return ans;
}
