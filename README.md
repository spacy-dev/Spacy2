[![Build Status](https://travis-ci.org/spacy-dev/Spacy.svg?branch=master)](https://travis-ci.org/spacy-dev/Spacy/builds)
[![Coverity Status](https://scan.coverity.com/projects/10277/badge.svg)](https://scan.coverity.com/projects/spacy)
[![Coverage Status](https://coveralls.io/repos/github/spacy-dev/Spacy/badge.svg?branch=master)](https://coveralls.io/github/spacy-dev/Spacy?branch=master)
[![Gitter chat](https://badges.gitter.im/spacy-dev/spacy.png)](https://gitter.im/spacy-dev/Spacy)
[![Stories in Ready](https://badge.waffle.io/spacy-dev/Spacy.svg?label=ready&title=Ready)](http://waffle.io/spacy-dev/Spacy)

# Spacy - Vector Space Algorithms

### Download
Download via
  - git clone https://github.com/spacy-dev/Spacy.git

### gerrithub
Download from gerrithub via
  - git clone ssh://username@review.gerrithub.io:29418/spacy-dev/Spacy

Push to gerrithub via (with my-branch=master,...)
  - git push origin HEAD:refs/for/my-branch

### Dependencies
The core of Spacy does not require external dependencies.

Depending on your use-case the following dependencies may be required:
  - [googletest](https://github.com/google/googletest) for compiling the tests (enable with cmake .. -DBuildTest=ON),
  - [Eigen](http://eigen.tuxfamily.org/) for enabling finite-dimensional problem descriptions using namespace 'Spacy::Rn' (automatically enabled if cmake finds Eigen),
  - [FunG](https://lubkoll.github.io/FunG) an automatic-differentiation library that can be used for the definition of integrands for [Kaskade 7](https://www.zib.de/projects/kaskade-7-finite-element-toolbox), [deal.II](http://dealii.org) and for the generation of finite-dimensional problems with [Eigen](http://eigen.tuxfamily.org/),
  - [Kaskade 7](https://www.zib.de/projects/kaskade-7-finite-element-toolbox) for developing algorithms with Kaskade 7 (enable with cmake .. -DKaskade=ON),
  - [FEniCS](https://fenicsproject.org) for developing algorithms with FEniCS (enable with cmake .. -DDolfin=ON).
  - [deal.II](http://dealii.org) for developing algorithms with deal.II (enable with cmake .. -DdealII=ON). 

Currently there are integration tests for [Eigen](http://eigen.tuxfamily.org/), [FEniCS](https://fenicsproject.org) and 
[FunG](https://lubkoll.github.io/FunG), running in a separate CI:

[![Build Status](https://travis-ci.org/spacy-dev/Spacy-Integration-Tests.svg?branch=master)]
(https://travis-ci.org/spacy-dev/Spacy-Integration-Tests/builds)

### Installation
Go to download folder, i.e.
  - cd Spacy
  
and type
  - mkdir build
  - cd build
  - cmake .. (-DBuildTest=ON -DDolfin=ON -DKaskade=ON -DCoverage=ON)
  - make (-j n_threads)
  - make install

### Examples
Currently there are examples for
  - simple usage without external libraries (currently a scalar example),
  - [Kaskade 7](https://www.zib.de/projects/kaskade-7-finite-element-toolbox),
  - [FEniCS](https://fenicsproject.org).

These are located in
 - Spacy/Examples/Kaskade,
 - Spacy/Examples/FEniCS.

Paths to Kaskade, resp. FEniCs may need to be adjusted according to your setup.



