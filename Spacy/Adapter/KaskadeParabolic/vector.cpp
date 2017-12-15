#include <vector>

#include "vector.hh"
#include "vectorSpace.hh"


namespace Spacy
{
  namespace KaskadeParabolic
  {

    Vector::Vector(const VectorSpace& space)
      : VectorBase(space)
    {
      auto c = creator< VectorCreator<GridManDesc,SingleDesc> >(space);

      variableSet_.reserve(c.numberOfCreators());
      description_.reserve(c.numberOfCreators());
      v_.reserve(c.numberOfCreators());
      for(auto i = 0u; i< c.numberOfCreators(); i++)
      {
        variableSet_.emplace_back( (creator< VectorCreator<GridManDesc,SingleDesc> >(space)).getSubCreator(i).get() );
        description_.emplace_back( std::make_shared<SingleDesc>( creator< VectorCreator<GridManDesc,SingleDesc> >(space).getSubCreator(i).get()) );
        v_.emplace_back( SingleDesc::template CoefficientVectorRepresentation<>::init( variableSet_.at(i).descriptions.spaces ));
      }

      //connect to Signal of Creator
      auto& creator_ = creator< VectorCreator<GridManDesc,SingleDesc> >(space);
      this->c = creator_.S_->connect([this](unsigned index) { return this->refine(index); });
    }

    Vector::Vector(const Vector& v): VectorBase(v), variableSet_(v.variableSet_), description_(v.description_), v_(v.v_)
    {
//        std::cout<<"in Copy Constructor of Vector"<<std::endl;
      auto& creator_ = creator< VectorCreator<GridManDesc,SingleDesc> >(v.space());
      this->c = creator_.S_->connect([this](unsigned index) -> void { return this->refine(index);});
    }

    Vector& Vector::operator=(const Vector& v)
    {
//        std::cout<<"in Copy Assignment of Vector"<<std::endl;
      VectorBase::operator=(v);
      this->variableSet_ = v.variableSet_;
      this->description_ = v.description_;
      this->v_ = v.v_;

      auto& creator_ = creator< VectorCreator<GridManDesc,SingleDesc> >(v.space());
      this->c = creator_.S_->connect([this](unsigned index) -> void { return this->refine(index);});

    }

    Vector& Vector::operator=(Vector&& v)
    {
//        std::cout<<"in rvalue Copy Assigment of Vector"<<std::endl;
      VectorBase::operator=(std::move(v));
      this->variableSet_ = std::move(v.variableSet_);
      this->description_ = std::move(v.description_);
      this->v_ = std::move(v.v_);

      auto& creator_ = creator< VectorCreator<GridManDesc,SingleDesc> >(v.space());
      this->c = creator_.S_->connect([this](unsigned index) -> void { return this->refine(index);});
    }

    Vector::~Vector()
    {
//        std::cout<<"In Destructor of Vector"<<std::endl;
      c.disconnect();
    }

    void Vector::refine(unsigned k)
    {
//        std::cout<<"hallo ich bin der Vector refiner"<<std::endl;
//        std::cout<<this->space().index()<<std::endl;

      ::Spacy::KaskadeParabolic::VectorCreator<GridManDesc,SingleDesc>  vc = ::Spacy::creator<VectorCreator<GridManDesc,SingleDesc> >(this->space());
      /*::Spacy::Kaskade::VectorCreator<SingleDesc>*/ auto vc_k = vc.getSubCreator(k);

//        variableSet_.insert(variableSet_.begin()+k, VariableSet(vc_k.get()));
      variableSet_.insert(variableSet_.begin()+k, VariableSet(::Spacy::creator<VectorCreator<GridManDesc,SingleDesc> >(this->space()).getSubCreator(k).get()));
      description_.insert(description_.begin()+k, std::make_shared<SingleDesc>(vc_k.get()));
      v_.insert(v_.begin()+k, VectorImpl(SingleDesc::template CoefficientVectorRepresentation<>::init( variableSet_.at(k).descriptions.spaces )));
//        std::cout<<"done refining"<<std::endl;
    }

    ::Spacy::Real Vector::operator()(const Vector& y) const
    {
      assert(this->variableSet_.size() == y.variableSet_.size());
      Real result{0.};
      auto cy = creator< VectorCreator<GridManDesc,SingleDesc> >(y.space());
      auto cthis = creator< VectorCreator<GridManDesc,SingleDesc> >(this->space());


      for(auto i = 0;i<this->variableSet_.size();i++)
      {
        VectorImpl w( SingleDesc::template CoefficientVectorRepresentation<>::init(cy.getSpace(i)));
        VectorImpl v( SingleDesc::template CoefficientVectorRepresentation<>::init(cthis.getSpace(i)));

        variableSetToCoefficients(y.variableSet_.at(i),w);
        variableSetToCoefficients(this->variableSet_.at(i),v);

        result += v*w;

      }

      return result;
    }

  }
}
