// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_TEST_ADAPTER_EIGEN_SETUP_HH
#define SPACY_TEST_ADAPTER_EIGEN_SETUP_HH

#include <Eigen/Dense>
#include "Spacy/Adapter/Eigen/c1Operator.hh"
#include "Spacy/Adapter/Eigen/c2Functional.hh"

inline auto testDim()
{
  return 2u;
}

inline auto testXValue()
{
  return 1;
}

inline auto testYValue()
{
  return 2;
}

inline auto testVector()
{
  Eigen::VectorXd vec(testDim());
  vec << testXValue() , testYValue();
  return vec;
}

inline auto testMatrix()
{
  Eigen::MatrixXd mat(testDim(),testDim());
  mat << 1, 0, 1, 2;
  return mat;
}

inline auto testOperator(const Spacy::VectorSpace& domain, const Spacy::VectorSpace& range)
{
  return Spacy::Rn::C1Operator( [](const auto& x){ return 1.1*x; } ,
                                [](const auto& x){ return 1.1*::Eigen::MatrixXd::Identity(x.size(),x.size()); },
                                domain, range);
}

inline auto testFunctional(const Spacy::VectorSpace& domain)
{
  return Spacy::Rn::C2Functional( [](const ::Eigen::VectorXd& x){ return x.dot(x); } ,
                                  [](const auto& x){ return 2*x; },
                                  [](const auto& x){ return 2*Eigen::MatrixXd::Identity(x.size(),x.size()); },
                                  domain);
}


#endif // SPACY_TEST_ADAPTER_EIGEN_SETUP_HH

