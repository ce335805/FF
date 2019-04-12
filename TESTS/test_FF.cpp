#include "FF.h"
#include "globals.h"
#include "gtest/gtest.h"

#include <string>

#define MKL_Complex16 std::complex<double>
#include "mkl.h"

namespace {

TEST(FFTest, orthogonality)
{

    std::string file_name = "../data/inidata/pick_list_10.init";
    //new FF object for checking
    FF ff_check(file_name);

    //some very short sanity checks
    EXPECT_GT(ff_check.get_NL(), 0) << "Number of formfactors is zero";
    EXPECT_LE(ff_check.get_NL(), Nx * Ny) << "Number of formfactors is greater than BZ";
    EXPECT_EQ(ff_check.get_FF(0, 0, 0), ff_check.get_FF(0, Nx - 1, 1)) << "0th formfactor not constant";

    //check orthogonality
    std::vector<double> unit_matrix(ff_check.get_NL() * ff_check.get_NL(), 0.0);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
        ff_check.get_NL(), ff_check.get_NL(), Nx * Ny, 1.0,
        ff_check.GetFFPtr(), Nx * Ny,
        ff_check.GetFFPtr(), Nx * Ny,
        0.0, unit_matrix.data(), ff_check.get_NL());

    //    for(auto to_output : unit_matrix){
    //        std::cout << to_output << '\n';
    //    }

    for (int l1 = 0; l1 < ff_check.get_NL(); ++l1) {
        for (int l2 = 0; l2 < ff_check.get_NL(); ++l2) {
            if (l1 == l2) {
                EXPECT_NEAR(unit_matrix[l1 * ff_check.get_NL() + l2], 1.0, 1e-12);
            } else {
                EXPECT_NEAR(unit_matrix[l1 * ff_check.get_NL() + l2], 0.0, 1e-12);
            }
        }
    }
}
}
