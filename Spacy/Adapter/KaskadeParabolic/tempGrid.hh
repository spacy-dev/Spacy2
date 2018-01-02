#pragma once

#include <vector>
#include <iostream>
#include <Spacy/Spaces/ScalarSpace/Real.hh>

namespace Spacy
{
  namespace KaskadeParabolic
  {

    Real exp(const Real x)
    {
      return Real{std::exp(get(x))};
    }
    Real log(const Real x)
    {
      return Real{std::log(get(x))};
    }

    class TempGrid
    {
    public:

      TempGrid(Real t_start,Real t_end,unsigned N):
        ts_(t_start),te_(t_end),N_(N)
      {
        vertexVec_.reserve(N);
        dtVec_.reserve(N);
        dtVec_.push_back(0);
        vertexVec_.push_back(ts_);

        for(auto i = 1u;i<N;i++)
        {
          dtVec_.push_back( Real{(te_-ts_) /(double) (N-1)});
          vertexVec_.push_back(vertexVec_.at(i-1)+dtVec_.at(i));
        }
      }

      TempGrid(Real t_start, Real t_end, unsigned N, Real lambda):
        ts_(t_start),te_(t_end),N_(N)
      {
        vertexVec_.reserve(N);
        dtVec_.push_back(0);
        vertexVec_.push_back(ts_);

        Real c = 1./lambda*1./(N-1)*(exp(lambda*te_)-exp(lambda*ts_));

        std::cout<< "c is "<<c<<std::endl;
        for (int i = 0 ; i < N-1; i++) {
          std::cout<<i<<std::endl;
          vertexVec_.push_back(1./lambda*log(lambda*c + exp(lambda*vertexVec_[i])));
          std::cout << vertexVec_[i+1] << std::endl;

          dtVec_.push_back(vertexVec_[i+1] - vertexVec_[i]);
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

      const std::vector<Real>& getVertexVec() const
      {
        return vertexVec_;
      }

      const std::vector<Real>& getDtVec() const
      {
        return dtVec_;
      }

      const unsigned getInverval(Real t)
      {
        auto i = 0u;
        for( ;i< vertexVec_.size(); i++)
        {
          if(vertexVec_.at(i) + 1e-10 >= t)
            return i;
        }

        std::cout<<"did not find time "<<t<< "as vertex is "<<vertexVec_.at(i-1)<<std::endl;
        return 0;
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
      std::vector<Real> vertexVec_{};
      std::vector<Real> dtVec_{};
      Real ts_,te_;
      unsigned N_;
    };



  }
}
