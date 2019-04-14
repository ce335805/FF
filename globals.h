// header file in which global varibales are put
#ifndef GLOBALS_H //inlcude guard
#define GLOBALS_H

#include <stddef.h>

constexpr int N { 100 };
constexpr int Nx { 10 };
constexpr int Ny { 10 };
constexpr int NxIBZ { (Nx / 2 + 1) * (Nx / 2 + 2) };
//positive frequencies
constexpr int Nw { 16 };

//temperature
constexpr double beta { 5.0 };
constexpr double U { 4.0 };

constexpr double PI { 3.14159265358979323846264338327950 };

#endif // globals.h
