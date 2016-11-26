# Install Spacy
git clone https://github.com/spacy-dev/Spacy.git
cd Spacy && mkdir -p build && cd build && cmake .. -DDolfin=ON && make -j2 && sudo make install
cd ../..

# Generate problem definition from .ufl file
ffc -l dolfin LagrangeFunctional.ufl
ffc -l dolfin NormalStepFunctional.ufl

# Compilation:
mkdir -p build && cd build && cmake .. && make
 
# Run example:
./run
cd ..
