#include "quadratic.hh"

#include "iostream"

namespace Spacy
{
  namespace Functions_1D
  {
    Quadratic::Quadratic(Real a, Real b, Real c) noexcept
      : a_(a), b_(b), c_(c)
    {
//      std::cout << "constructor (quadratic): " << a_ << ", " << b_ << ", " << c_ << std::endl;
    }

    Real Quadratic::operator()(Real t) const noexcept
    {
//      std::cout << "eval (quadratic): " << a_ << ", " << b_ << ", " << c_ << ", " << t << std::endl;
//      std::cout << (a_ + t*b_ + t*t*c_) << std::endl;
      return a_ + t*b_ + t*t*c_;
    }
  }
}
