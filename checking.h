#include "FF.h"
#include <assert.h>
#include <fstream>
#include <iostream>

//introduce new namespace for auxillary checking functions
namespace check {

//function intended to check the orthogonality of the formfactors
//this should be done via more efficient tensor contractions
void check_ortho(const FF& to_check);

//function to output FF-array for plotting to actually look at it
void output_FF(const FF& to_check);
}
