#ifndef GlobalEngine_HPP
#define GlobalEngine_HPP

#include "EuropeanCall.hpp"
#include "EuropeanPut.hpp"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

namespace AidanRicher {
namespace Engine {

// generate a mesh array from start to end with step size h
template <typename T> 
vector<T> MeshArray(const T start, const T step_size_h, const size_t vector_size)
{
    vector<T> mesh;
    
    for (size_t i = 0; i < vector_size; ++i)
    {
        mesh.push_back(start + i * step_size_h);
    }

    return mesh;
}

// global function to check put-call parity
void CheckPutCallParity(const EuropeanCall& call, const EuropeanPut& put, double U);

} // namespace Engine
} // namespace AidanRicher

#endif // GlobalEngine_HPP