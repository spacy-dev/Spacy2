include /home/ignaz/lib/Kaskade7.3/Makefile.Local
include /home/ignaz/lib/Kaskade7.3/Makefile.Rules

INCLUDE = $(DUNEINC) $(UGINC) $(BOOSTINC) $(KASKADEINC) $(MUMPSINC) $(SPACYINC)
FLAGS = $(WFLAGS) $(OPTFLAGS)

nonlinear_heat: nonlinear_heat.o Makefile $(KASKADE7)/libs/libkaskade.a 
	$(CXX) $(FLAGS) $< $(KASKADE7)/libs/mumps_solve.o $(KASKADELIB) \
 $(DUNELIB) \
 $(UGLIB) \
 $(BOOSTLIB) \
 $(MUMPSLIB) \
 $(BLASLIB) $(FTNLIB) $(NUMALIB) $(SPACYLIB) -o $@

clean:
	rm -f gccerr.txt nonlinear_heat nonlinear_heat.o temperature.vtu *.bak

depend:
	cp Makefile.gen Makefile; /home/ignaz/lib/Kaskade7.3/gccmakedep -f Makefile $(INCLUDE) $(MAKEDEPENDFLAGS)  nonlinear_heat.cpp; /home/ignaz/lib/Kaskade7.3/remove_install_deps Makefile

# DO NOT DELETE
nonlinear_heat.o: nonlinear_heat.cpp  \
 /home/ignaz/C++/Spacy/Spacy/Adapter/kaskade.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/directSolver.hh \
$(KASKADE7)/linalg/direct.hh \
$(KASKADE7)/fem/istlinterface.hh \
$(KASKADE7)/fem/fixdune.hh \
$(KASKADE7)/fem/functionspace.hh \
$(KASKADE7)/fem/gridmanager.hh \
$(KASKADE7)/utilities/threading.hh \
$(KASKADE7)/utilities/kalloc.hh \
$(KASKADE7)/fem/shapefunctioncache.hh \
$(KASKADE7)/fem/fetransfer.hh \
$(KASKADE7)/fem/mllgeometry.hh \
$(KASKADE7)/linalg/dynamicMatrix.hh \
$(KASKADE7)/fem/pshapefunctions.hh \
$(KASKADE7)/fem/barycentric.hh \
$(KASKADE7)/utilities/detailed_exception.hh \
$(KASKADE7)/linalg/triplet.hh \
$(KASKADE7)/linalg/crsutil.hh \
$(KASKADE7)/linalg/umfpack_tools.hh \
$(KASKADE7)/linalg/factorization.hh \
$(KASKADE7)/utilities/enums.hh \
$(KASKADE7)/linalg/factory.hh \
$(KASKADE7)/linalg/symmetricOperators.hh \
$(KASKADE7)/linalg/threadedMatrix.hh \
$(KASKADE7)/utilities/duneInterface.hh \
$(KASKADE7)/utilities/is_function.hh \
$(KASKADE7)/utilities/member_variable_macro.hh \
 /home/ignaz/C++/Spacy/Spacy/vector.hh \
 /home/ignaz/C++/Spacy/Spacy/vectorSpace.hh \
 /home/ignaz/C++/Spacy/Spacy/norm.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/normConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/conceptBase.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/vectorConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/singleConcepts.hh \
 /home/ignaz/C++/Spacy/Spacy/scalarProduct.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/scalarProductConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/vectorConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/vectorCreatorConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/impl.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/voider.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Exceptions/incompatibleSpaceException.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Exceptions/genericException.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/cast.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Exceptions/invalidArgumentException.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Base/operatorBase.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/util.hh \
$(KASKADE7)/fem/variables.hh \
$(KASKADE7)/fem/linearspace.hh \
 /home/ignaz/C++/Spacy/Spacy/Spaces/productSpace.hh \
 /home/ignaz/C++/Spacy/Spacy/Spaces/ProductSpace/vector.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Base/vectorBase.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/eps.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/mixinConnection.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/primalDualSwitch.hh \
 /home/ignaz/C++/Spacy/Spacy/Spaces/ProductSpace/vectorSpace.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/l2Product.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/vector.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/c2Functional.hh \
$(KASKADE7)/fem/assemble.hh \
$(KASKADE7)/fem/firstless.hh \
$(KASKADE7)/fem/fixfusion.hh \
$(KASKADE7)/fem/functional_aux.hh \
$(KASKADE7)/fem/quadrature.hh \
 /home/ignaz/C++/Spacy/Spacy/operator.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/operatorConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/linearSolverConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/vectorConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Exceptions/invalidArgumentException.hh \
 /home/ignaz/C++/Spacy/Spacy/vectorSpace.hh \
 /home/ignaz/C++/Spacy/Spacy/hessian.hh \
 /home/ignaz/C++/Spacy/Spacy/functional.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/functionalConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/operatorConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/linearSolver.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/linearSolverConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/numberOfThreads.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Base/functionalBase.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/directSolver.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/vectorSpace.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/l2Product.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/lagrangeCGSolver.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/CG/linearSolver.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/CG/cg.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/iterativeRefinements.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/verbosity.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/CG/terminationCriteria.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/mixins.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/absoluteAccuracy.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/adjointIndex.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/contractionRate.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/controlIndex.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/dampingAccuracy.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/decreaseCondition.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/eps.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/impl.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/iterativeRefinements.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/maxSteps.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/minimalAccuracy.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/primalDualSwitch.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/regularityTest.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/relativeAccuracy.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/stateIndex.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/timer.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/verbosity.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/CG/terminationCriterionConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/conceptBase.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/CG/triangularStateConstraintPreconditioner.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/adjointIndex.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/controlIndex.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/stateIndex.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/linearOperator.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/linearOperator.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/operator.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/c1Operator.hh \
 /home/ignaz/C++/Spacy/Spacy/linearizedOperator.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/vector.hh \
 /home/ignaz/C++/Spacy/Spacy/Adapter/Kaskade/vectorSpace.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/Newton/newton.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/Newton/dampingStrategies.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/dampingFactor.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Mixins/regularityTest.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/Newton/dampingStrategyConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/linearSolverConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/Newton/terminationCriteria.hh \
 /home/ignaz/C++/Spacy/Spacy/Util/Concepts/Newton/terminationCriterionConcept.hh \
 /home/ignaz/C++/Spacy/Spacy/Algorithm/Newton/parameter.hh \
 /home/ignaz/C++/Spacy/Spacy/inducedScalarProduct.hh \
$(KASKADE7)/fem/lagrangespace.hh \
$(KASKADE7)/fem/converter.hh \
$(KASKADE7)/fem/gridcombinatorics.hh \
$(KASKADE7)/fem/lagrangeshapefunctions.hh \
$(KASKADE7)/fem/combinatorics.hh \
$(KASKADE7)/utilities/power.hh \
$(KASKADE7)/fem/scalarspace.hh \
$(KASKADE7)/fem/views.hh \
$(KASKADE7)/io/vtk.hh \
$(KASKADE7)/io/vtkwriter.hh \
$(KASKADE7)/io/iobase.hh \
$(KASKADE7)/utilities/gridGeneration.hh \
$(KASKADE7)/utilities/GridGeneration/cube.hh \
$(KASKADE7)/utilities/vectorTools.hh nonlinear_heat.hh \
$(KASKADE7)/utilities/linalg/scalarproducts.hh
