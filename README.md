# FF
Class to handle formfactor objects for use in condensed matter system solvers.
Also checkout our [paper](https://journals.aps.org/prb/abstract/10.1103/PhysRevB.98.075143)

# General Purpose
In a condensed matter system one desires to compute general quantities (eq. magnetizations, charge densities or field theoretic quantities such as generalized Green's functions) as functions of quasi-momenta in the Brillouin-Zone.
It is then clear how to obtain quantities as functions of the real lattice index - one performs a discrete Fourier transform using complex exponential functions.

However, it is often desireble to use instead basis-functions that transform like the point group of the underlying lattice.
This facilitates the identification of symmteries and also the identification of suitable approximations.

This class is supposed to give easy access to such a complete and orthonormal set of basis functions for the 2D square lattice.

The functions are completely precomputed and saved in a 'std::vector<double>' (they can of course be chosen to be all real due to linearity of the transformations) such that basis transformations can easily and efficiently be computed using GEMM.

# Installation
No installation needed.
Simply include the header FF.h and statically link to libFF.a in /lib.

# Usage
See also uni tests on how to use the class.
## Initialization
The constructor takes a `std::string` as an argument. 
The string should contain the filename to a file of #lattice-sites booleans determining which basisfunctions to use.
For this it is important to provide the booleans in the correct order - the numbring of the basis functions will be discussed later.

The second argument to the constructor is the lattice size.
### Nubering of basis functions:
To Do

## Usable Output
The constructor automatically determines the number of Formfactors that are used.
The number can be obtained by calling `get_Nl()`

The basis functions are stored in a `std::vector<double>`.
One can get a single value via `get_FF(int l, int kx, int ky)` where kx and ky are quasi-momentum arguments in a finite Brillouin-zone and l indexes the basis function.
For numbering of the basis-function - see above.

A pointer to the data (to use for example in DGEMM) one can use `GetFFPtr(void)` (see also unit test).


# Testing
The functionality of the class can be tested using Google's googletest suite.
The currently implemented test computes a full basis and checks for it's orthonormality using DGEMM.
This is a pretty strong check given, that if it should be false positive, one still has an orthonormal basis - just with the wrong basis functions (which is of course still undesireble).

One could therefore comprehend this test by plotting the functions - but this of course needs human intervention.
One can easily implement new tests that will be automatically registered with Google's testsuite.
The test program can be built using cmake.
