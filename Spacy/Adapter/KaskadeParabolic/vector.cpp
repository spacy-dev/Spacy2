#include <vector>

#include "vector.hh"


namespace Spacy
{
  namespace KaskadeParabolic
  {

    Vector::Vector(const VectorSpace& space): ::Spacy::ProductSpace::Vector(space),transferMe(*this)
    {
      std::cout<<"IN TDS CONST asd"<<std::endl;
      if(::Spacy::is<VectorCreator>(space.get_nonconst_creator()))
      {
        std::cout<<"its a TDS VC"<<std::endl;
        VectorCreator& tdvc = ::Spacy::cast_ref<VectorCreator>(space.get_nonconst_creator());

        // c = tdvc.S_.connect([this](unsigned index,VectorSpace& VS) { std::cout<<this->numberOfVariables()<<std::endl;
        //                                                               return this->refine(index,VS); });
        this->c =  tdvc.S_.connect(transferMe);
      }
      std::cout<<"Returning from construc"<<std::endl;
    }
    Vector::Vector(const Vector& v): ::Spacy::ProductSpace::Vector(v), transferMe(*this)
    {

      std::cout<<"in Copy Constructor of TDVector"<<std::endl;
      auto& tdcv = ::Spacy::cast_ref<VectorCreator> (v.space().creator());
      //         this->c = tdcv.S_.connect([this](unsigned index,VectorSpace& VS) -> void { return this->refine(index,VS);});
      this->c = tdcv.S_.connect(transferMe);
    }
    Vector& Vector::operator=(const Vector& v)
    {
      std::cout<<"in Copy assigment of Vector"<<std::endl;
      ::Spacy::ProductSpace::Vector::operator=(v);
      //           transferMe(*this);
      auto& tdcv = ::Spacy::cast_ref<VectorCreator> (v.space().creator());
      //    this->c = tdcv.S_.connect([this](unsigned index,VectorSpace& VS) -> void { return this->refine(index,VS);});
      this->c = tdcv.S_.connect(transferMe);
      return *this;
    }

    Vector& Vector::operator=(Vector&& v)
    {
      std::cout<<"in Copy assigment of Vector"<<std::endl;
      ::Spacy::ProductSpace::Vector::operator=(std::move(v));
      //           transferMe(*this);
      auto& tdcv = ::Spacy::cast_ref<VectorCreator> (this->space().creator());
      //    this->c = tdcv.S_.connect([this](unsigned index,VectorSpace& VS) -> void { return this->refine(index,VS);});
      this->c = tdcv.S_.connect(transferMe);
      return *this;
    }

    void Vector::refine(unsigned index,VectorSpace& insertedVS)
    {
      std::cout<<"####in Vector refinement Function"<<index<<std::endl;
      auto toinsert =  zero(insertedVS);

      std::cout<<"...inserting..."<<std::endl;
      std::cout<<components_.size()<<std::endl;

      components_.insert(this->components_.begin()+index, toinsert);

      //components_.resize(components_.size()+1);
      std::cout<<"...done inserting"<<index<<std::endl;
      //            auto comp = this->components_.at(2);
      //            std::cout<<"hallo"<<std::endl;
      //.space();
      //           std::cout<<comp.space().index()<<std::endl;
      //           this->space().index();

      //            for(auto& ele:this->components_)
      //                std::cout<<ele.space().index()<<std::endl;

      std::cout<<"done refining"<<std::endl;
    }

    Vector::~Vector()
    {
      std::cout<<"CALLING DESTRUC"<<std::endl;
      c.disconnect();
    }
  }
}
