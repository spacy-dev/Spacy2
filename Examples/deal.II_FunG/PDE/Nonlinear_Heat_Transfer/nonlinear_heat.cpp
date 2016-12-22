#include <Spacy/spacy.h>
#include <Spacy/Adapter/dealII.hh>

#include <deal.II/base/tensor.h>
#include <deal.II/grid/tria.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/grid/grid_generator.h>

#include <fung/fung.hh>
#include <fung/examples/nonlinear_heat.hh>

#include <iostream>

const constexpr int variable_dim = 1;
const constexpr auto dim = 2;

int main()
{
    using VariableDims = Spacy::dealII::VariableDim<variable_dim>;

    dealii::Triangulation<dim> triangulation;
    dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
    triangulation.refine_global(5);

    const auto fe_order = 1;
    auto V = Spacy::dealII::makeHilbertSpace<variable_dim>(triangulation, fe_order);


    double c = 1, d = 1e-1;
    double u0 = 0;
    dealii::Tensor<variable_dim,dim> du0;
    auto integrand = FunG::heatModel(c, d, u0, du0);
    auto A = Spacy::dealII::C1FunGOperator<decltype(integrand), dim, VariableDims>(std::move(integrand), V, V.dualSpace());

    auto p = Spacy::Newton::Parameter{};
    p.setRelativeAccuracy(1e-12);
    p.setMaxSteps(10);

    auto x = Spacy::localNewton(A, p);
    const auto& x_ = Spacy::cast_ref<Spacy::dealII::Vector>(x);
    Spacy::dealII::writeVTK<dim,VariableDims>(x_, "nonlinear_heat");
}
