// header file which declares the util functions
#ifndef UTIL_H //inlcude guard
#define UTIL_H

#include <assert.h>
#include <functional>
#include <globals.h>
#include <iterator>
#include <vector>

//symmtry operations that returns a vector with two entries holding the changed coordinates
template <typename T>
void constexpr inline sym_sqr(const int s, T& x_coord, T& y_coord) noexcept
{
    //note:: std_swap is noexcept

    //check that the passed value is a number somehow - thus function can be noexcept
    assert(std::is_arithmetic<T>::value);

    switch (s) {
    //E
    case 0:
        break;
    //C2
    case 1:
        x_coord = -x_coord;
        y_coord = -y_coord;
        break;
    //C4
    case 2:
        std::swap(x_coord, y_coord);
        x_coord = -x_coord;
        break;
    //C4^-1
    case 3:
        std::swap(x_coord, y_coord);
        y_coord = -y_coord;
        break;
    //\sig_v^1
    case 4:
        y_coord = -y_coord;
        break;
    //\sig_v^2
    case 5:
        x_coord = -x_coord;
        break;
    //sig_d^1
    case 6:
        std::swap(x_coord, y_coord);
        x_coord = -x_coord;
        y_coord = -y_coord;
        break;
    //sig_d^2
    case 7:
        std::swap(x_coord, y_coord);
        break;
    }
    //periodic boundary conditions
    //BZ is [0, 2pi)
    x_coord = (x_coord + Nx) % Nx;
    y_coord = (y_coord + Ny) % Ny;
}

//symmetry operation leaving passed arguments intact
//acts on first two entries of vector only
template <typename T>
std::vector<T> constexpr inline sym_sqr(const int s, const std::vector<T> v_inital) noexcept
{
    //note:: std_swap is noexcept

    //check that the passed value is a number somehow + vector has entries
    assert(std::is_arithmetic<T>::value);
    assert(v_inital.size() > 1);

    T x_coord_new{ v_inital[0] }, y_coord_new{ v_inital[1] };

    switch (s) {
    //E
    case 0:
        break;
    //C2
    case 1:
        x_coord_new = -x_coord_new;
        y_coord_new = -y_coord_new;
        break;
    //C4
    case 2:
        std::swap(x_coord_new, y_coord_new);
        x_coord_new = -x_coord_new;
        break;
    //C4^-1
    case 3:
        std::swap(x_coord_new, y_coord_new);
        y_coord_new = -y_coord_new;
        break;
    //\sig_v^1
    case 4:
        y_coord_new = -y_coord_new;
        break;
    //\sig_v^2
    case 5:
        x_coord_new = -x_coord_new;
        break;
    //sig_d^1
    case 6:
        std::swap(x_coord_new, y_coord_new);
        x_coord_new = -x_coord_new;
        y_coord_new = -y_coord_new;
        break;
    //sig_d^2
    case 7:
        std::swap(x_coord_new, y_coord_new);
        break;
    }

    //periodic boundary conditions
    //BZ is [0, 2pi)
    return std::vector<T>{ (x_coord_new + Nx) % Nx, (y_coord_new + Ny) % Ny };
}

//function to remove all dublicates from a vector
//the std-library function only kills adjacent dublicates
template <typename T, typename Func>
typename std::vector<T>::iterator my_unique(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last, Func compare_function)
{
    //for each element ...
    while (first != last) {
        typename std::vector<T>::iterator comp(first);
        comp++;
        //... search for equal ones
        while (comp != last) {
            bool is_equal = compare_function(*first, *comp);
            if (is_equal) {
                //if they are equal move it to the back by swaping
                for (typename std::vector<T>::iterator swap_left(comp); swap_left != last - 1; ++swap_left) {
                    typename std::vector<T>::iterator swap_right(swap_left);
                    swap_right++;
                    std::swap(*swap_left, *swap_right);
                }
                //and decrement last
                last--;
            } //if
            else {
                comp++;
            }
        } //comp
        first++;
    } //first
    return last;
}

#endif // util.h
