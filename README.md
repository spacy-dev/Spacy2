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
  - git clone https://review.gerrithub.io/spacy-dev/Spacy

Push to gerrithub via (with my-branch=master,...)
  - git push origin HEAD:refs/for/my-branch

### Installation
Go to download folder, i.e.
  - cd Spacy
  
and type
  - mkdir build
  - cd build
  - cmake ..
  - make

### Examples
Currently there are examples for
  - simple usage without external libraries (currently a scalar example)
  - [Kaskade 7](https://www.zib.de/projects/kaskade-7-finite-element-toolbox)
  - [FEniCS](https://fenicsproject.org).

These are located in
 - Spacy/Examples/Kaskade,
 - Spacy/Examples/FEniCS.

Paths to Kaskade, resp. FEniCs may need to be adjusted according to your setup.



