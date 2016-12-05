#include <Spacy/spacy.h>
#include <Spacy/Adapter/dealII.hh>

#include <deal.II/base/tensor.h>
#include <deal.II/grid/tria.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/grid/grid_generator.h>

#include <iostream>

const constexpr auto dim = 2;

template <int dim, int n_components=1>
struct LaplaceOperator
{
    decltype(auto) operator()(const dealii::Tensor<n_components,dim>& Dx) const
    {
        return Dx;
    }

    decltype(auto) d1(const dealii::Tensor<n_components,dim>&, const dealii::Tensor<n_components,dim>& dDx) const
    {
        return dDx;
    }
};

int main()
{
    dealii::Triangulation<dim> triangulation;
    dealii::GridGenerator::hyper_cube (triangulation, -1, 1);
    triangulation.refine_global (8);

    const auto fe_order = 1;
    auto V = Spacy::dealII::makeHilbertSpace(triangulation, fe_order);
    auto A = Spacy::dealII::C1Operator<LaplaceOperator<dim>, dim>(LaplaceOperator<dim>(), V, V.dualSpace());

    auto p = Spacy::Newton::Parameter{};
    p.setRelativeAccuracy(1e-12);
    p.setMaxSteps(10);

    auto x = Spacy::localNewton(A, p);
    const auto& x_ = Spacy::cast_ref<Spacy::dealII::Vector>(x);
    Spacy::dealII::writeVTK<dim>(x_, "laplace");
}
