#include "Index.h"
#include "mpi.h"
#include "gtest/gtest.h"

int main(int argc, char** argv)
{

    //initialization of test library
    ::testing::InitGoogleTest(&argc, argv);

    MPI_Init(nullptr, nullptr);

    //perform all tests
    return RUN_ALL_TESTS();

    MPI_Finalize();
}
