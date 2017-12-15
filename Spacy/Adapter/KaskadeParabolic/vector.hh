#pragma once

#include <vector>
#include <boost/signals2.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/Base/VectorBase.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Util/Mixins/Get.hh>
#include <Spacy/Adapter/KaskadeParabolic/util.hh>




namespace Spacy
{
  namespace KaskadeParabolic
  {

    template <class> class VectorCreator;
    template <class> class SubCreator;

    /**
         * @ingroup KaskadeGroup VectorSpaceGroup
         * @brief Coefficient vector implementation for %Kaskade 7 (single space).
         * @tparam Description %Kaskade::VariableSetDescription
         */
    template <class Description>
    class Vector :
        public VectorBase
    {
      using VectorImpl = typename Description::template CoefficientVectorRepresentation<>::type;
      using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Variables,0>::type> >;
      using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<typename Description::Spaces,Variable::spaceIndex>::type> >;
      using VariableSet = typename Description::VariableSet;

      using Spaces = typename Description::Spaces;


    public:

      /**
             * @brief Construct vector \f$x\f$ from underlying vector space.
             * @param space underlying vector space
             */
      Vector(const VectorSpace& space): VectorBase(space)
      {
        auto c = creator< VectorCreator<Description> >(space);

        variableSet_.reserve(c.numberOfCreators());
        description_.reserve(c.numberOfCreators());
        v_.reserve(c.numberOfCreators());
        for(auto i = 0u; i< c.numberOfCreators(); i++)
        {
          variableSet_.emplace_back( (creator< VectorCreator<Description> >(space)).getSubCreator(i).get() );
          description_.emplace_back( std::make_shared<Description>( creator< VectorCreator<Description> >(space).getSubCreator(i).get()) );
          v_.emplace_back( Description::template CoefficientVectorRepresentation<>::init( variableSet_.at(i).descriptions.spaces ));
        }

        //connect to Signal of Creator
        auto& creator_ = creator< VectorCreator<Description> >(space);
        this->c = creator_.S_->connect([this](unsigned index) { return this->refine(index); });
      }

      Vector(const Vector& v): VectorBase(v), variableSet_(v.variableSet_), description_(v.description_), v_(v.v_)
      {
  //        std::cout<<"in Copy Constructor of Vector"<<std::endl;
        auto& creator_ = creator< VectorCreator<Description> >(v.space());
        this->c = creator_.S_->connect([this](unsigned index) -> void { return this->refine(index);});
      }

      Vector& operator=(const Vector& v)
      {
  //        std::cout<<"in Copy Assignment of Vector"<<std::endl;
        VectorBase::operator=(v);
        this->variableSet_ = v.variableSet_;
        this->description_ = v.description_;
        this->v_ = v.v_;

        auto& creator_ = creator< VectorCreator<Description> >(v.space());
        this->c = creator_.S_->connect([this](unsigned index) -> void { return this->refine(index);});

      }

      Vector& operator=(Vector&& v)
      {
  //        std::cout<<"in rvalue Copy Assigment of Vector"<<std::endl;
        VectorBase::operator=(std::move(v));
        this->variableSet_ = std::move(v.variableSet_);
        this->description_ = std::move(v.description_);
        this->v_ = std::move(v.v_);

        auto& creator_ = creator< VectorCreator<Description> >(v.space());
        this->c = creator_.S_->connect([this](unsigned index) -> void { return this->refine(index);});
      }

      /**
             * @brief Apply as dual element.
             * @param y primal vector
             * @return \f$x(y)\f$
             */
      ::Spacy::Real operator()(const Vector& y) const
      {
        assert(this->variableSet_.size() == y.variableSet_.size());
        Real result{0.};
        auto cy = creator< VectorCreator<Description> >(y.space());
        auto cthis = creator< VectorCreator<Description> >(this->space());


        for(auto i = 0;i<this->variableSet_.size();i++)
        {
          VectorImpl w( Description::template CoefficientVectorRepresentation<>::init(cy.getSpace(i)));
          VectorImpl v( Description::template CoefficientVectorRepresentation<>::init(cthis.getSpace(i)));

          variableSetToCoefficients(y.variableSet_.at(i),w);
          variableSetToCoefficients(this->variableSet_.at(i),v);

          result += v*w;

        }

        return result;
      }

      ~Vector()
      {
  //        std::cout<<"In Destructor of Vector"<<std::endl;
        c.disconnect();
      }

      void refine(unsigned k)
      {
  //        std::cout<<"hallo ich bin der Vector refiner"<<std::endl;
  //        std::cout<<this->space().index()<<std::endl;

        ::Spacy::KaskadeParabolic::VectorCreator<Description>  vc = ::Spacy::creator<VectorCreator<Description> >(this->space());
        /*::Spacy::Kaskade::VectorCreator<SingleDesc>*/
        ::Spacy::KaskadeParabolic::SubCreator<Description> vc_k = vc.getSubCreator(k);

  //        variableSet_.insert(variableSet_.begin()+k, VariableSet(vc_k.get()));
        variableSet_.insert(variableSet_.begin()+k, VariableSet(::Spacy::creator<VectorCreator<Description> >(this->space()).getSubCreator(k).get()));
        description_.insert(description_.begin()+k, std::make_shared<Description>(vc_k.get()));
        v_.insert(v_.begin()+k, VectorImpl(Description::template CoefficientVectorRepresentation<>::init( variableSet_.at(k).descriptions.spaces )));
  //        std::cout<<"done refining"<<std::endl;
      }


      /// Access coefficient vector.
      const VariableSet& get(const unsigned i) const
      {
        if(i>=variableSet_.size()) std::cout<<"err in vec"<<variableSet_.size()<<std::endl;
        return variableSet_.at(i);
      }

      /// Access nonconst coefficient vector.
      VariableSet& get_nonconst(const unsigned i)
      {
        if(i>=variableSet_.size()) std::cout<<"err in vec"<<variableSet_.size()<<std::endl;
        return variableSet_.at(i);
      }

      const auto& getCoeffVec(const unsigned i) const
      {
        if(i>=variableSet_.size()) std::cout<<"err in vec"<<variableSet_.size()<<std::endl;
        return  ::boost::fusion::at_c<0>(variableSet_.at(i).data).coefficients();
      }

      auto& getCoeffVec_nonconst(const unsigned i)
      {
        if(i>=variableSet_.size()) std::cout<<"err in vec"<<variableSet_.size()<<std::endl;
        return  ::boost::fusion::at_c<0>(variableSet_.at(i).data).coefficients();
      }
      /**
           * @brief In-place summation \f$ x+=y\f$.
           * @param y vector to add to this vector
           * @return \f$ x+=y\f$.
           */
      Vector& operator+=(const Vector& y)
      {
        checkSpaceCompatibility(this->space(),y.space());
        assert(this->variableSet_.size() == y.variableSet_.size());
        for(auto i = 0u;i<this->variableSet_.size();i++)
          this->variableSet_.at(i) +=  y.variableSet_.at(i);
        return *this;
      }

      /**
           * @brief In-place subtraction \f$ x-=y\f$.
           * @param y vector to subtract from this vector
           * @return \f$ x-=y\f$.
           */
      Vector& operator-=(const Vector& y)
      {
        checkSpaceCompatibility(this->space(),y.space());
        assert(this->variableSet_.size() == y.variableSet_.size());
        for(auto i = 0u;i<this->variableSet_.size();i++)
          this->variableSet_.at(i) -=  y.variableSet_.at(i);
        return *this;
      }

      /**
           * @brief In-place multiplication \f$ x*=a\f$.
           * @param a scaling factor
           * @return \f$ x*=a\f$.
           */
      Vector& operator*=(double a)
      {
        for(auto i = 0u;i<this->variableSet_.size();i++)
          this->variableSet_.at(i) *=a;
        return *this;
      }

      /**
           * @brief Negation \f$ -x\f$.
           * @return \f$ -x \f$.
           */
      Vector operator-() const
      {
        Vector y = *this;
        for(auto i = 0u;i<y.variableSet_.size();i++)
          y.variableSet_.at(i) *=  -1;
        return y;
      }

      /**
           * @brief Comparison operator \f$ x==y\f$.
           * @param y vector to compare with this vector
           * @return \f$ x==y\f$.
           */
      bool operator==(const Vector& y) const
      {
        const auto& this_ = static_cast<const Vector&>(*this);
        checkSpaceCompatibility(this_.space(),y.space());
        assert(this->variableSet_.size() == y.variableSet_.size());
        auto max = std::max(::Spacy::Mixin::get(this_(this_)), ::Spacy::Mixin::get(y(y)));
        if( max == 0 )
          max = 1;
        auto dx = y;
        dx -= this_;
        return ::Spacy::Mixin::get(dx(dx)) < max*y.space().eps()*y.space().eps();
      }



    private:
      template <class Desc>
      friend void writeVTK(const Vector<Desc>& x, const char* fileName);

      std::vector<VariableSet> variableSet_{};
      std::vector< std::shared_ptr<Description> > description_{};
      mutable std::vector<VectorImpl> v_{};

      boost::signals2::connection c;

    };
  }
}
