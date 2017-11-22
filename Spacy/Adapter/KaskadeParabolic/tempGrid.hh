#pragma once

#include <vector>
#include <iostream>

namespace Spacy
{
  namespace KaskadeParabolic
  {
    class TempGrid
    {
    public:

      TempGrid(double t_start,double t_end,unsigned N):
        ts_(t_start),te_(t_end),N_(N)
      {
        vertexVec_.reserve(N);
        dtVec_.reserve(N);
        dtVec_.push_back(0);
        vertexVec_.push_back(ts_);

        for(auto i = 1u;i<N;i++)
        {
          dtVec_.push_back( (te_-ts_) /(double) (N-1));
          vertexVec_.push_back(vertexVec_.at(i-1)+dtVec_.at(i));
        }
      }

      void refine(const unsigned index)
      {
        assert(index > 0);
        vertexVec_.insert(vertexVec_.begin()+index, (vertexVec_.at(index-1)+vertexVec_.at(index))/2. );
        dtVec_.insert(dtVec_.begin()+index, (vertexVec_.at(index)-vertexVec_.at(index-1)) );
        dtVec_.at(index+1) = (vertexVec_.at(index+1)-vertexVec_.at(index));
        return;
      }

      const std::vector<double>& getVertexVec() const
      {
        return vertexVec_;
      }

      const std::vector<double>& getDtVec() const
      {
        return dtVec_;
      }

      void print() const
      {
        std::cout<<"Vertices  :";
        for(auto ele :vertexVec_) std::cout<<ele<<" ";
        std::cout<<std::endl;

        std::cout<<"Stepsizes :";
        for(auto ele :dtVec_) std::cout<<ele<<" ";
        std::cout<<std::endl;

        return;
      }

    private:
      std::vector<double> vertexVec_{};
      std::vector<double> dtVec_{};
      double ts_,te_;
      unsigned N_;
    };



  }
}
