#include "checking.h"
#include "FF.h"
#include "globals.h"
#include "util.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

//introduce new namespace for auxillary checking functions
namespace check {

//function intended to check the orthogonality of the formfactors
//this should be done via more efficient tensor contractions
void check_ortho(const FF& to_check)
{

    assert(to_check.get_NL() > 0);

    std::vector<double> ortho(to_check.get_NL() * to_check.get_NL(), {});

    bool check = true;
    double tol = 1e-10;

    //perform the tensor contraction via for-loops for the moment
    for (auto l1 = 0ul; l1 < to_check.get_NL(); ++l1) {
        for (auto l2 = 0ul; l2 < to_check.get_NL(); ++l2) {

            for (auto kx = 0ul; kx < Nx; ++kx) {
                for (auto ky = 0ul; ky < Ny; ++ky) {
                    ortho[l1 * to_check.get_NL() + l2] += to_check.get_FF(l1, kx, ky) * to_check.get_FF(l2, kx, ky);
                }
            }
        }
    }

    //now check if resulting matrix is indeed del_l1_l2
    for (auto l1 = 0ul; l1 < to_check.get_NL(); ++l1) {
        for (auto l2 = 0ul; l2 < to_check.get_NL(); ++l2) {

            if (l1 == l2) {
                if (std::abs(ortho[l1 * to_check.get_NL() + l2] - 1.0) > tol)
                    check = false;
            } else {
                if (std::abs(ortho[l1 * to_check.get_NL() + l2]) > tol)
                    check = false;
            }
        }
    }

    if (check) {
        std::cout << "Orthogonality check --- SUCCESS! --- " << '\n';
        std::cout << "Tolerance : " << tol << '\n';
    } else {
        std::cout << "Orthogonality check --- FAIL! ---" << '\n';
        std::cout << "Tolerance : " << tol << '\n';
    }
}

// ------------------------------

//function to output FF-array for plotting to actually look at it
void output_FF(const FF& to_check)
{

    std::cout << '\n'
              << "outputting Formfactors" << '\n';

    for (auto l = 0ul; l < to_check.get_NL(); ++l) {

        std::ofstream output;
        std::stringstream ss;
        ss << "data/FF_" << l << ".dat";
        std::string out_name(ss.str());
        output.open(out_name);

        for (auto kx = 0ul; kx < Nx; ++kx) {
            for (auto ky = 0ul; ky < Ny; ++ky) {
                output << to_check.get_FF(l, kx, ky) << '\t';
            }
            output << '\n';
        }

        output.close();
    }
}
}
