#include "FF.h"
#include "globals.h"
#include "util.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//use complex numbers
typedef std::complex<double> dcomp;

constexpr int FF::char_table[];

FF::FF(std::string file_name)
{

    std::cout
        << "Constructing FF object"
        << "\n";
    //first element should be 1 at compiletime
    static_assert(char_table[0] == 1, "Char_table not initialized");

    //read in formfactor list
    read_picklist(file_name);

    //initialize vector of vectors for bonds and chars in IBZ
    init_bond_vectors();

    //set the values in FF_array
    set_FFarray();
}

//----------------------------------------

void FF::read_picklist(std::string file_name)
{

    Nl = 0ul;

    std::ifstream pick_in(file_name);
		if(pick_in.is_open())
		{
	    std::cout << '\n'
	              << "reading file : " << file_name << '\n';
		}
		else
		{
	    std::cout << '\n'
	              << "file " << file_name << " not properly opened" << '\n';
		}	


    //temporary to read in
    bool temp { 1 };
    int l { 0 }; //check if length of file is correct
    //read until file is empty
    while (pick_in >> temp) {
        //std::cout << temp << '\n';
        pick_list.push_back(temp);
        if (pick_list.back())
        	++Nl; //count true elements
        ++l;
    }
    //std::cout << '\n';

    assert(l == N);
    assert(Nl > 0);
    assert(Nl <= N);

    std::cout << "FF object has " << Nl << " Formfactors" << '\n';

    pick_in.close();
}

//------------------------------

//initialize bond_vectors
//this first creates all possible entries
//then it deletes all that are zero/linear dependent/excluded by picklist
void FF::init_bond_vectors()
{

    //holds bond in IBZ - 2 adjacent entries are x and y of bond coordinates
    std::vector<int> bond_list;

    int start_IBZ { 2ul };

    //lambda to return lattice coordinates of site #i in IBZ
    auto gen_IBZ = [&start_IBZ]() mutable {
        //To understand how this works:
        //1. draw IBZ for squarelattice on paper
        //2. number each site 0 to ...
        //3. remember 'kleiner Gauss'
        int n { 1ul };
        while (start_IBZ / 2 > (n * (n + 1)) / 2)
            n++;
        if (start_IBZ % 2 == 0) {
            ++start_IBZ;
            return n - 1;
        } else {
            ++start_IBZ;
            return start_IBZ / 2 - ((n - 1) * n) / 2 - 2;
        }
    };

    //lambda to check if a vector with a site and characters will yield a FF equal to zero
    auto const check_zero = [](const std::vector<int>& v) -> bool {
        //#characers = #sym_ops + dimensions (=2)
        assert(v.size() == sym_ops + 2);
        std::vector<int> compare(sym_ops, 0);

        //for each sym_op
        for (auto first = 0ul; first < sym_ops; ++first) {

            compare[first] += v[first + 2];

            //go through vector entries and see if the same bond is generated
            for (auto second = 0ul; second < sym_ops; ++second) {

                //if two bonds match add their characters
                if (sym_sqr(first, v)[0] == sym_sqr(second, v)[0]
                    && sym_sqr(first, v)[1] == sym_sqr(second, v)[1]
                    && first != second)
                    compare[first] += v[second + 2];
            }
        }
        //if all add up to zero the contribution is zero
        return std::all_of(compare.begin(), compare.end(), [](int i) { return i == 0; });
    };

    //lambda to check if two vectors are linear dependent - with a factor 1 or -1
    auto const lin_dep = [](const std::vector<int>& v1, const std::vector<int>& v2) -> bool {
        assert(v1.size() == v2.size());
        assert(v1.size() > 1);
        assert(v2.size() > 1);

        //check if bonds are linear dependent
        //only allowed factor in this case is a -1
        if ((v1[0] == v2[0] && v1[1] == v2[1]) || ((v1[0] == ((-v2[0] + Nx) % Nx)) && v1[1] == ((-v2[1] + Ny) % Ny)))
            //if so check if characters are the same and return result
            return std::equal(v1.begin() + 2, v1.end(), v2.begin() + 2);

        //else return false
        return false;
    };

    //length is size of IBZ
    bond_list = std::vector<int>((Nx / 2 + 1) * (Nx / 2 + 2), 0ul);
    //generate the points from the IBZ
    std::generate(bond_list.begin(), bond_list.end(), gen_IBZ);

    //generate vector of vectors {x, y, char1, char2, ..., char8}
    bond_vectors = std::vector<std::vector<int>>(bond_list.size() / 2 * sym_ops, std::vector<int>(2 + sym_ops, 0));

    //fill in the first two entries {x, y, ...}
    for (auto bonds = 0ul; bonds < bond_list.size() / 2; ++bonds) {
        for (auto representation = 0ul; representation < num_reps; ++representation) {

            if (representation < 5) {
                bond_vectors[bonds * num_reps + representation][0] = bond_list[2 * bonds];
                bond_vectors[bonds * num_reps + representation][1] = bond_list[2 * bonds + 1];
            }
            //for 2D representation one needs to start with different bonds
            else {
                int sym {};
                if (representation == 5)
                    sym = 7;
                else if (representation == 6)
                    sym = 2;
                else
                    sym = 5;
                std::vector<int> temp_bonds { bond_list[2 * bonds], bond_list[2 * bonds + 1] };
                temp_bonds = sym_sqr(sym, temp_bonds);
                bond_vectors[bonds * num_reps + representation][0] = temp_bonds[0];

                bond_vectors[bonds * num_reps + representation][1] = temp_bonds[1];
            }

            for (auto num_char = 0ul; num_char < sym_ops; ++num_char) {

                //the two is the dimension
                bond_vectors[bonds * num_reps + representation][num_char + 2] = char_table[sym_ops * representation + num_char];
            }
        }
    }

    //erase all that are zero
    bond_vectors.erase(std::remove_if(bond_vectors.begin(), bond_vectors.end(), check_zero), bond_vectors.end());

    //erase linear dependent ones
    bond_vectors.erase(my_unique<std::vector<int>>(bond_vectors.begin(), bond_vectors.end(), lin_dep), bond_vectors.end());

    //remove all excluded by picklist
    int pick_list_pos { 0 };
    bond_vectors.erase(
        remove_if(bond_vectors.begin(), bond_vectors.end(),
            [this, &pick_list_pos](const std::vector<int>& elem) mutable { return !pick_list[pick_list_pos++]; }),
        bond_vectors.end());

    //    //some test output
    //    std::cout << '\n';
    //    std::cout << "bond_vectors used for initialization of FF array : " << '\n';
    //    for (auto vec : bond_vectors) {
    //        for (auto var : vec) {
    //            std::cout << var << '\t';
    //        }
    //        std::cout << '\n';
    //    }
    //    std::cout << '\n';
}

// ----------------------------------

//function to set values in FF_array as FF(l, kx, ky)
void FF::set_FFarray()
{
    assert(bond_vectors.size() == Nl);
    assert(bond_vectors[0].size() == 2 + num_reps);

    //vector to hold complex intermediate values
    std::vector<std::complex<double>> FF_array_comp(Nl * Nx * Ny, std::complex<double>(0.0, 0.0));
    FF_array = std::vector<double>(Nl * Nx * Ny, {});

    double norm {};

    //not normed formfactors
    for (int l = 0; l < Nl; ++l) {
        for (int kx = 0; kx < Nx; ++kx) {
            for (int ky = 0; ky < Ny; ++ky) {

                for (int rep = 0; rep < num_reps; ++rep) {

                    //first cast everything into double
                    auto charac = static_cast<double>(bond_vectors[l][rep + 2]);

                    //compute symmetry operation of (x,y)
                    std::vector<int> xy = sym_sqr(rep, bond_vectors[l]);

                    auto x = static_cast<double>(xy[0]);
                    auto y = static_cast<double>(xy[1]);
                    auto px = static_cast<double>(2.0 * PI * kx / Nx);
                    auto py = static_cast<double>(2.0 * PI * ky / Ny);

                    FF_array_comp[l * Nx * Ny + kx * Ny + ky]
                        += charac
                        * exp(std::complex<double>(0.0, x * px))
                        * exp(std::complex<double>(0.0, y * py));
                }
            }
        }
    }

    //either the imaginary parts or the real parts should be zero
    for (auto elem : FF_array_comp) {
        assert(std::abs(imag(elem)) < 1e-11 || std::abs(real(elem)) < 1e-11);
    }
    //now norm all the formfactors and assign the double array
    for (int l = 0; l < Nl; ++l) {

        norm = 0.0;
        //determine norm
        for (int kx = 0; kx < Nx; ++kx) {
            for (int ky = 0; ky < Ny; ++ky) {
                norm += abs(FF_array_comp[l * Nx * Ny + kx * Ny + ky]
                    * FF_array_comp[l * Nx * Ny + kx * Ny + ky]);
            }
        }

        norm = sqrt(norm);

        //now actually asign values
        for (int kx = 0; kx < Nx; ++kx) {
            for (int ky = 0; ky < Ny; ++ky) {
                FF_array[l * Nx * Ny + kx * Ny + ky] = 1 / norm
                        * static_cast<double>(real(FF_array_comp[l * Nx * Ny + kx * Ny + ky]))
                    + 1 / norm
                        * static_cast<double>(imag(FF_array_comp[l * Nx * Ny + kx * Ny + ky]));
            }
        }
    }
}
