// header file which declares the FormFactor(FF) class
#ifndef FF_H //inlcude guard
#define FF_H
#include "globals.h"
#include <complex>
#include <string>
#include <vector>
//use complex numbers
typedef std::complex<double> dcomp;

class FF {
private:
    //number of symmetry operations
    static constexpr int sym_ops = 8;

    //number of irreducible representations of the symmetry group
    //actually num_reps != sym_ops but sym_ops gives the number of possible linear independent basis functions
    //thus sym_ops is the relevant number - just saves a few ifs and further complications later on
    static constexpr int num_reps = sym_ops;

    //char table is the same for all sqr FF's
    //already defined at compiletime
    //according to above arguments some entries might be repeated
    //static constexpr int char_table[sym_ops * num_reps]{
    static constexpr int char_table[sym_ops * num_reps]{
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, -1, -1, -1, -1,
        1, 1, -1, -1, 1, 1, -1, -1,
        1, 1, -1, -1, -1, -1, 1, 1,
        2, -2, 0, 0, 0, 0, 0, 0,
        2, -2, 0, 0, 0, 0, 0, 0,
        2, -2, 0, 0, 0, 0, 0, 0,
        2, -2, 0, 0, 0, 0, 0, 0
    };

    int Nl; //# of formfactors

    //list that specifies which formfactors should be kept
    std::vector<bool> pick_list;

    //a vector of vectors (sorry Sandipan) which holds site in first two entries
    //and characters in s latter entries
    std::vector<std::vector<int>> bond_vectors;

    //array that holds FF(l, kx, ky)
    std::vector<double> FF_array;

    //function to initialize picklist
    //either read it in from a file or calculate it for a given numbet of neighbours
    //also calculated value of Nl
    void read_picklist(std::string file_name);

    //initialize bond_vectors
    void init_bond_vectors();

    //function to set values in FF_array as FF(l, kx, ky)
    void set_FFarray();

public:
    //function to get number of FF
    inline int get_NL() const noexcept
    {
        return Nl;
    }

    //function to get FF - values
    inline double get_FF(int l, int kx, int ky) const noexcept
    {
        return FF_array[l * Nx * Ny + kx * Ny + ky];
    }

    //function to get pointer to FF array
    inline const double* GetFFPtr(void) const noexcept
    {
        return FF_array.data();
    }

    //constructor that reads to be kept formfactors from file
    FF(std::string file_name);

    //delete copy constructor and assignment operator since one only needs one FF object at any time
    FF(const FF&) = delete;
    FF& operator=(const FF&) = delete;
};
#endif // ff.h
