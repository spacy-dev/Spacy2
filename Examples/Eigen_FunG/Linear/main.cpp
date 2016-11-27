#include <Eigen/Dense>
#include <Spacy/spacy.h>
#include <Spacy/Adapter/eigen.hh>
#include <fung/fung.hh>

#include <iostream>

const constexpr int dim = 2;
using Vector = Eigen::VectorXd;
using Matrix = Eigen::MatrixXd;

auto get_fung_operator()
{
    Matrix A(dim,dim);
    A.fill(0);
    A(0,0) = 1;
    A(1,1) = 2;

    Vector b(dim);
    b(0) = -10;
    b(1) = -20;

    // use FunG to generate operator
    auto v = FunG::variable<0>(Vector(dim));
    return A*v + b;
}

int main()
{
    std::cout << std::endl;
    using namespace Spacy;

    auto A_ = get_fung_operator();
    auto V = Rn::makeHilbertSpace(dim);
    auto A = Rn::getFunGC1Operator(A_, V, V.dualSpace());
    V.setScalarProduct( InducedScalarProduct( A.linearization(zero(V)) ) );

    auto x = localNewton(A);
    std::cout << "\nsolution:\n" << get(cast_ref<Rn::Vector>(x)) << '\n' << std::endl;
}
