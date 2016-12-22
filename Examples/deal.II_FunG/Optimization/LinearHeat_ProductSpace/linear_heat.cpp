#include <Spacy/spacy.h>
#include <Spacy/Adapter/dealII.hh>
#include <Spacy/Algorithm/ACR/acr.hh>

#include <deal.II/base/tensor.h>
#include <deal.II/grid/tria.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/lac/vector.h>

#include <fung/fung.hh>
#include <fung/examples/nonlinear_heat.hh>

#include <iostream>

#define DIMENSION 2
#define VARIABLE_DIM 2
#define NCOMPONENTS 2

namespace FunG
{
    namespace LinearAlgebra
    {
        template < class T, int n, class MatrixConceptCheck>
        struct NumberOfRows< dealii::Tensor<2,n,T>, MatrixConceptCheck >    : std::integral_constant<int, n> {};

        template < class T, int n, class MatrixConceptCheck>
        struct NumberOfRows< dealii::Tensor<1,n,T>, MatrixConceptCheck >    : std::integral_constant<int, n> {};

        template < class T, int n, class MatrixConceptCheck>
        struct NumberOfColumns< dealii::Tensor<2,n,T>, MatrixConceptCheck > : std::integral_constant<int, n> {};

        template < class T, int n, class MatrixConceptCheck>
        struct NumberOfColumns< dealii::Tensor<1,n,T>, MatrixConceptCheck > : std::integral_constant<int,-1> {};
    }
}


const constexpr auto variable_dim = VARIABLE_DIM;
const constexpr auto dim = DIMENSION;

template <class Gradient>
auto linear_heat_model(double y0, const Gradient& dy0)
{
    using namespace FunG;
    using namespace FunG::LinearAlgebra;
    auto y = variable<0>(y0);
    auto dy = variable<0>(dy0);

    auto g = 1;
    auto f = 0.5*dy*dy - g*y;
    return finalize(f);
}

template <class Gradient>
auto linear_heat_model(double y0, double z0, const Gradient& dy0, const Gradient& dz0)
{
    using namespace FunG;
    using namespace FunG::LinearAlgebra;
    auto y = variable<0>(y0);
    auto dy = variable<0>(dy0);
    auto z = variable<1>(z0);
    auto dz = variable<1>(dz0);

    auto g = 1;
    auto f = 0.5*(dy*dy + dz*dz) - g*(y+z);
    return finalize(f);
}

template <class Gradient>
auto linear_heat_model(const dealii::Tensor<1,variable_dim>& y0, const Gradient& dy0)
{
    using namespace FunG;
    using namespace FunG::LinearAlgebra;
    auto y = variable<0>(y0);
    auto dy = variable<0>(dy0);

    auto g = y0;
    g[0] = 1;
    auto f = 0.5*trace(dy*dy) - g * y;
    return finalize(f);
}

template <class Gradient>
auto linear_heat_model(const dealii::Tensor<1,variable_dim>& y0,
                       const dealii::Tensor<1,variable_dim>& z0,
                       const Gradient& dy0,
                       const Gradient& dz0)
{
    using namespace FunG;
    using namespace FunG::LinearAlgebra;
    auto y = variable<0>(y0);
    auto dy = variable<0>(dy0);
    auto z = variable<0>(z0);
    auto dz = variable<0>(dz0);

    auto g = y0;
    g[0] = 1;
    auto f = 0.5*(trace(dy*dy) + trace(dz*dz)) - g * (y + z);
    return finalize(f);
}

int main()
{
    using VariableDims = Spacy::dealII::VariableDim<variable_dim>;

    dealii::Triangulation<dim> triangulation;
    dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
    triangulation.refine_global(5);

    const auto fe_order = 1;
    auto V = Spacy::dealII::makeHilbertSpace<variable_dim>(triangulation, fe_order);

#if VARIABLE_DIM==1
    double y0=0;
#else
    dealii::Tensor<1,variable_dim> y0;
#endif

    dealii::Tensor<variable_dim,dim> dy0;
#if NCOMPONENTS==1
    auto integrand = linear_heat_model(y0, dy0);
#else
    auto integrand = linear_heat_model(y0, y0, dy0, dy0);
#endif
    auto A = Spacy::dealII::C2FunGFunctional<decltype(integrand), dim, VariableDims>(std::move(integrand), V);

//    V.setScalarProduct(Spacy::InducedScalarProduct(A.hessian(zero(V))));
    Spacy::ACR::ACRSolver cubic_regularization_solver(A);
    auto x = cubic_regularization_solver();
    const auto& x_ = Spacy::cast_ref<Spacy::dealII::Vector>(x);
    Spacy::dealII::writeVTK<dim,VariableDims>(x_, "linear_heat");
}
