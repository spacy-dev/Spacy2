#pragma once

#include <deal.II/numerics/data_out.h>

#include <Spacy/zeroVectorCreator.hh>

#include "vector.hh"
#include "vectorSpace.hh"

#include <ostream>
#include <fstream>
#include <string>

namespace Spacy
{
    namespace dealII
    {
        template <int dim>
        void writeVTK(const Vector& x, const std::string& fileName)
        {
            const auto& dealIICreator = creator< VectorCreator<dim> >(x.space());
            dealii::DataOut<dim> data_out;
            data_out.attach_dof_handler(dealIICreator.dofHandler());
            data_out.add_data_vector(get(x), fileName);
            data_out.build_patches();

            std::ofstream stream(fileName + ".vtk");
            data_out.write_vtk(stream);
        }
    }
}
