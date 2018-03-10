[![Build Status](https://travis-ci.org/spacy-dev/Spacy.svg?branch=master)](https://travis-ci.org/spacy-dev/Spacy/builds)
[![Coverity Status](https://scan.coverity.com/projects/10277/badge.svg)](https://scan.coverity.com/projects/spacy)
[![Coverage Status](https://coveralls.io/repos/github/spacy-dev/Spacy/badge.svg?branch=master)](https://coveralls.io/github/spacy-dev/Spacy?branch=master)
[![Gitter chat](https://badges.gitter.im/spacy-dev/spacy.png)](https://gitter.im/spacy-dev/Spacy)
[![Stories in Ready](https://badge.waffle.io/spacy-dev/Spacy.svg?label=ready&title=Ready)](http://waffle.io/spacy-dev/Spacy)

# Spacy - Vector Space Algorithms

### Download

Download from gerrithub via

    git clone https://review.gerrithub.io/spacy-dev/Spacy \
    && (cd Spacy \
    && curl -kLo `git rev-parse --git-dir`/hooks/commit-msg \
    https://review.gerrithub.io/tools/hooks/commit-msg; \
    chmod +x `git rev-parse --git-dir`/hooks/commit-msg)

Push to gerrithub via (with my-branch=master,...)

    git push origin HEAD:refs/for/my-branch

### Dependencies
The core of Spacy does not require external dependencies.

Depending on your use-case the following dependencies may be required:
  - [googletest](https://github.com/google/googletest) for compiling the tests (enable with `cmake .. -DBuildTest=ON`),
  - [Eigen](http://eigen.tuxfamily.org/) for enabling finite-dimensional problem descriptions `using namespace Spacy::Rn` (automatically enabled if cmake finds Eigen),
  - [FunG](https://lubkoll.github.io/FunG) an automatic-differentiation library that can be used for the definition of integrands for [Kaskade 7](https://www.zib.de/projects/kaskade-7-finite-element-toolbox), [deal.II](http://dealii.org) and for the generation of finite-dimensional problems with [Eigen](http://eigen.tuxfamily.org/),
  - [Kaskade 7](https://www.zib.de/projects/kaskade-7-finite-element-toolbox) for developing algorithms with Kaskade 7 (enable with `cmake .. -DKaskade=ON`),
  - [FEniCS](https://fenicsproject.org) for developing algorithms with FEniCS (enable with `cmake .. -DDolfin=ON`).
  - [deal.II](http://dealii.org) for developing algorithms with deal.II (enable with `cmake .. -DdealII=ON`). 

Currently there are integration tests for
  - [Eigen](http://eigen.tuxfamily.org/) and [FunG](https://lubkoll.github.io/FunG)

    [![Build Status](https://travis-ci.org/spacy-dev/Spacy-Integration-Tests-Eigen.svg?branch=master)](https://travis-ci.org/spacy-dev/Spacy-Integration-Tests-Eigen/builds)
  - [FEniCS](https://fenicsproject.org) 

    [![Build Status](https://travis-ci.org/spacy-dev/Spacy-Integration-Tests-FEniCS.svg?branch=master)](https://travis-ci.org/spacy-dev/Spacy-Integration-Tests-FEniCS/builds)
  - [deal.II](http://dealii.org)

    [![Build Status](https://travis-ci.org/spacy-dev/Spacy-Integration-Tests-deal.II.svg?branch=master)](https://travis-ci.org/spacy-dev/Spacy-Integration-Tests-deal.II/builds)

### Installation
Go to download folder, i.e.

    cd Spacy
  
and type

    mkdir build
    cd build
    cmake .. (-DBuildTest=ON -DDolfin=ON -DKaskade=ON -DCoverage=ON)
    make (-j n_threads)
    make install

### Examples
Currently there are examples for
  - simple usage without external libraries (currently a scalar example),
  - [Kaskade 7](https://www.zib.de/projects/kaskade-7-finite-element-toolbox) using [FunG](https://lubkoll.github.io/FunG),
  - [Eigen](http://eigen.tuxfamily.org/) using [FunG](https://lubkoll.github.io/FunG),
  - [FEniCS](https://fenicsproject.org) with and without [FunG](https://lubkoll.github.io/FunG),
  - [deal.II](http://dealii.org) with and without [FunG](https://lubkoll.github.io/FunG)

These are located in
 - `Spacy/Examples/Kaskade`,
 - `Spacy/Examples/FEniCS`.

Paths to Kaskade may need to be adjusted according to your setup.



