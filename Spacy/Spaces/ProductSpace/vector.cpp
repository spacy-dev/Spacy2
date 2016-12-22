#include "vector.hh"

#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/cast.hh>

#include "vectorSpace.hh"

#include <algorithm>
#include <cassert>

namespace Spacy
{
    namespace ProductSpace
    {
        Vector::Vector(const VectorSpace& space)
            : VectorBase(space)
        {
            const auto& spaces = Spacy::creator<VectorCreator>(space).subSpaces();
            components_.reserve(spaces.size());
            for (auto i=0u; i<spaces.size(); ++i)
            {
                components_.emplace_back( zero(*spaces[i]) );
                assert(components_.back());
            }
            assert(components_.size() == spaces.size());
        }

//        Vector::Vector(const Vector& other)
//            : VectorBase(other.space()),
//              components_(other.components_)
//        {
//            for(auto i=0u; i<numberOfVariables(); ++i)
//            {
//                assert(other.components_[i]);
//                assert(components_[i]);
//            }
//            std::cout << "Size1: " << components_.size() << std::endl;
//            std::cout << "Size1_other: " << other.components_.size() << std::endl;
//            id_ = vector_id_counter++;
//            std::cout << "Id: " << id_ << ", space id: " << space().index() <<  std::endl;
//            std::cout << "copy from " << other.id_ << ", space id: " << other.space().index() << std::endl;
//            assert(id_ != other.id_);
//        }

//        Vector::Vector(Vector&& other)
//            : VectorBase(other.space()),
//              components_(std::move(other.components_))
//        {
//            assert(other.components_.empty());
//            for(auto i=0u; i<numberOfVariables(); ++i)
//                assert(components_[i]);
//            std::cout << "Size2: " << components_.size() << std::endl;
//            std::cout << "Size2_other: " << other.components_.size() << std::endl;
//            id_ = vector_id_counter++;
//            std::cout << "Id: " << id_ << ", space id: " << space().index() <<  std::endl;
//            std::cout << "move from " << other.id_ << ", space id: " << other.space().index() << std::endl;
//        }

        Vector& Vector::operator+=(const Vector& y)
        {
            checkSpaceCompatibility(this->space(),y.space());

            for(auto i=0u; i<components_.size(); ++i)
                component(i) += y.component(i);
            return *this;
        }

        //  AbstractVector& ProductSpaceElement::axpy(double a, const AbstractVector& y)
        //  {
        //    const auto& y_ = castTo<ProductSpaceElement>(y);

        //    if( !isPrimalDual() )
        //    {
        //      for(auto i=0u; i<variables_.size(); ++i)
        //        variable(i).axpy(a,y_.variable(i));
        //      return *this;
        //    }

        //    if( isPrimalEnabled() && y_.isPrimalEnabled() )
        //      primalComponent().axpy(a,y_.primalComponent());

        //    if( isDualEnabled() && y_.isDualEnabled() )
        //      dualComponent().axpy(a,y_.dualComponent());

        //    reset(y_);
        //    return *this;
        //  }


        Vector& Vector::operator-=(const Vector& y)
        {
            checkSpaceCompatibility(this->space(),y.space());

            for(auto i=0u; i<components_.size(); ++i)
                component(i) -= y.component(i);
            return *this;
        }

        Vector& Vector::operator*=(double a)
        {
            for(auto i=0u; i<components_.size(); ++i)
                component(i) *= a;
            return *this;
        }

        Vector Vector::operator- () const
        {
            return Vector(*this) *= -1;
        }

        bool Vector::operator==(const Vector& y) const
        {
            checkSpaceCompatibility(this->space(),y.space());

            for(auto i=0u; i<components_.size(); ++i)
                if( !(component(i) == y.component(i)) )
                    return false;

            return true;
        }

        unsigned Vector::numberOfVariables() const
        {
            return components_.size();
        }

        ::Spacy::Vector& Vector::component(unsigned k)
        {
            assert(components_[k]);
            return components_[k];
        }

        const ::Spacy::Vector& Vector::component(unsigned k) const
        {
            assert(components_[k]);
            return components_[k];
        }

        const VectorCreator& Vector::creator() const
        {
            return Spacy::creator<VectorCreator>(space());
        }


        Real Vector::operator()(const Vector& y) const
        {
            checkDualPairing(*this,y);
            assert( components_.size() == y.components_.size() );

            auto result = Real{0.};
            for(auto i=0u; i<components_.size(); ++i)
                result += component(i)( y.component(i) );
            return result;
        }

        Vector::iterator Vector::begin()
        {
            return components_.begin();
        }

        Vector::iterator Vector::end()
        {
            return components_.end();
        }

        Vector::const_iterator Vector::cbegin() const
        {
            return components_.cbegin();
        }

        Vector::const_iterator Vector::cend() const
        {
            return components_.cend();
        }
    }

    ::Spacy::Vector& primalComponent(::Spacy::Vector& v)
    {
        return cast_ref<ProductSpace::Vector>(v).component(PRIMAL);
    }

    const ::Spacy::Vector& primalComponent(const ::Spacy::Vector& v)
    {
        return cast_ref<ProductSpace::Vector>(v).component(PRIMAL);
    }

    ::Spacy::Vector& dualComponent(::Spacy::Vector& v)
    {
        return cast_ref<ProductSpace::Vector>(v).component(DUAL);
    }

    const ::Spacy::Vector& dualComponent(const ::Spacy::Vector& v)
    {
        return cast_ref<ProductSpace::Vector>(v).component(DUAL);
    }
}
