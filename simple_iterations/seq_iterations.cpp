#include "seq_iterations.h"
#include <stdexcept>
#include <random>

using dbl = double;

static inline dbl phi(dbl x, dbl r)
{
   return r*x*(1 - x);
}

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

static dbl get_null_point(dbl r)
{
   dbl lp = lower_endpoint(r);
   dbl up = upper_endpoint(r);
   if (r <= 0.) throw std::runtime_error("negative constatn r is not allowed");
   return get_point_from_interval(lp, up);
}

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
