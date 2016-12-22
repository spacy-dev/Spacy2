#pragma once

#include <deal.II/fe/fe_system.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/numerics/data_out.h>

#include <Spacy/zeroVectorCreator.hh>

#include "init.hh"
#include "util.hh"
#include "vector.hh"
#include "vectorSpace.hh"

#include <ostream>
#include <fstream>
#include <string>

namespace Spacy
{
    namespace dealII
    {
        template <int dim, class VariableDims = VariableDim<1> >
        void writeVTK(const Vector& x, const std::string& fileName)
        {
            auto fe_system = get_finite_element_system<dim,VariableDims>(x.space());
            dealii::DoFHandler<dim> dof_handler(creator< VectorCreator<dim,GetDim<0,VariableDims>::value> >(extractSubSpace(x.space(),0)).triangulation());
            dof_handler.distribute_dofs(fe_system);

            dealii::DataOut<dim> data_out;
            data_out.attach_dof_handler(dof_handler);
            data_out.add_data_vector(get(x), fileName);
            data_out.build_patches();

            std::ofstream stream(fileName + ".vtk");
            data_out.write_vtk(stream);
        }
    }
}
