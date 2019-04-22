# FF
Class to handle formfactor objects for use in condensed matter system solvers.

# Installation
No installation needed.
Simply include the header FF.h and link to libFF.a in /lib.

#Usage
To Do

# Testing
The functionality of the class can be tested using Google's googletest suite.
The currently implemented test computes a full basis and checks for it's orthogonality using DGEMM.
This is a pretty strong check given, that if it should be false positive, one still has an orthogonal basis - just with the wrong basis functions (which is of course still undesireble).
One could therefore comprehend this test by plotting the functions - but this of course needs human intervention.
One can easily implement new tests that will be automatically registered with Google's testsuite.
The test program can be built using cmake
