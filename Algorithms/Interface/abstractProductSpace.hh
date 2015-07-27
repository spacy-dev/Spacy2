//#ifndef ALGORITHM_INTERFACE_ABSTRACT_PRODUCT_SPACE_HH
//#define ALGORITHM_INTERFACE_ABSTRACT_PRODUCT_SPACE_HH

//#include <vector>

//#include "abstractBanachSpace.hh"

//namespace Algorithm
//{
//  namespace Interface
//  {
//    class AbstractProductSpace : public AbstractBanachSpace
//    {
//    public:
//      explicit AbstractProductSpace(const std::vector<unsigned>& primalSubSpaceIds, const std::vector<unsigned> dualSubSpaceIds = {});

//      const std::vector<unsigned> primalSubSpaceIds() const;

//      const std::vector<unsigned> dualSubSpaceIds() const;

//      virtual std::unique_ptr<AbstractBanachSpace> subSpace(unsigned i) const = 0;

//      virtual std::unique_ptr<AbstractProductSpace> primalSubSpace() const = 0;

//      virtual std::unique_ptr<AbstractProductSpace> dualSubSpace() const = 0;

//    private:
//      std::vector<unsigned> primalSubSpaceIds_, dualSubSpaceIds_;
//    };
//  }
//}

//#endif // ALGORITHM_INTERFACE_ABSTRACT_PRODUCT_SPACE_HH

