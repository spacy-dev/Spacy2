//#ifndef ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH
//#define ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH

//#include <memory>
//#include <vector>

//#include "vector.hh"
//#include "Interface/Operator/abstractOperator.hh"
//#include "operator.hh"
//#include "linearOperator.hh"

//namespace Algorithm
//{

//  class JacobiPreconditioner : public Interface::AbstractOperator
//  {
//  public:
//    JacobiPreconditioner(const Operator& A);

//    JacobiPreconditioner(const LinearOperator& A);

//    Vector operator()(const Vector& x) const override;

//  private:
//    JacobiPreconditioner* cloneImpl() const;

//    std::vector<double> diag_;
//    Vector x_;
//  };

//  Operator jacobiPreconditioner(const Operator& A);

//  Operator jacobiPreconditioner(const LinearOperator& A);
//}
//#endif // JACOBIPRECONDITIONER_HH
