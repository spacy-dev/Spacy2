#include <tuple>

#include <Eigen/Dense>
#include <Spacy/spacy.h>
#include <Spacy/Adapter/eigen.hh>
#include <fung/fung.hh>

const constexpr int dim = 2;
using Vector = Eigen::VectorXd;
using Matrix = Eigen::MatrixXd;

auto get_fung_operator()
{
    Matrix A_(dim,dim);
    A_.fill(0);
    A_(0,0) = 1;
    A_(1,1) = 2;

    Vector v_(dim), b(dim);
    v_.fill(0);
    b(0) = -1;
    b(1) = -16;

    auto v = FunG::variable<0>(v_);
    return A_*v + b;
}

int main()
{
    std::cout << std::endl;
    using namespace Spacy;

    auto A_ = get_fung_operator();
    auto V = Rn::makeHilbertSpace(dim);
    auto A = Rn::getFunGC1Operator(A_, V, V.dualSpace());

    auto x = localNewton(A);
    std::cout << "\nsolution:\n" << get(cast_ref<Rn::Vector>(x)) << '\n' << std::endl;
}
