/* 
Title: Introductory Computational Finance
Author: Aidan Richer (2025)

In this file we will test the following functionality of our program:
- Implementation of European and Perpetual American Call and Put option pricing.
- Computation of option Greeks under both exact (closed) solutions and Finite Difference methods (FDM).
- Computing option price, delta, and gamma matrices over matrices of option data.
- Mesh array testing to analyze option prices over a monotonically increasing mesh of spot prices.
- Validation of put-call parity for European options.
*/

#include "EuropeanCall.hpp"
#include "EuropeanPut.hpp"
#include "PerpAmericanCall.hpp"
#include "PerpAmericanPut.hpp"
#include "GlobalEngine.hpp"
#include "PricingMatrix.hpp"
#include "MatrixParameters.hpp"
#include "ArrayException.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace AidanRicher::Engine;
using namespace AidanRicher::Containers;

int main()
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Group A: Section 1:                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
// Question A)                                                                                                    
cout << "\n===== Group A, Section 1, Question A) ======" << endl;

// test on 4 batches of data, b = r for non-dividend paying stock
// vector of option data (k, r, sig, t, b)
vector<OptionData> batches = 
{
    OptionData(65.0, 0.08, 0.30, 0.25, 0.08),           // Call = 2.13337, Put = 5.84628
    OptionData(100.0, 0.0, 0.2, 1.0, 0.0),              // Call = 7.96557, Put = 7.96557
    OptionData(10.0, 0.12, 0.50, 1.0, 0.12),            // Call = 0.204058, Put = 4.07326
    OptionData(100.0, 0.08, 0.30, 30.0, 0.08),          // Call = 92.17570, Put = 1.24750
};

// vector of underlying spot prices
vector<double> a_spots = { 60.0, 100.0, 5.0, 100.0 };

// compute and output
for (size_t i = 0; i < batches.size(); ++i)
{ 
    EuropeanCall call(batches[i]);
    EuropeanPut put(batches[i]);
    
    cout << "Batch " << (i + 1) << ": " << endl;
    cout << "Call = " << call.Price(a_spots[i]) << endl;
    cout << "Put = " << put.Price(a_spots[i]) << endl;
}   // output is correct
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

// Question B)
cout << "\n===== Group A, Section 1, Question B) =====" << endl;

// apply the put-call parity relationship
cout << "Applying Put-Call Parity Relationship: " << endl;
for (size_t i = 0; i < batches.size(); ++i)
{
    EuropeanCall call(batches[i]);
    EuropeanPut put(batches[i]);

    cout << "Batch " << (i + 1) << ": " << endl;
    CheckPutCallParity(call, put, a_spots[i]);
}
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

// Question C)
cout << "\n===== Group A, Section 1, Question C) =====" << endl;

cout << "Testing option prices over a mesh of underlying prices for Batch 1:" << endl;

// create the mesh array vector
vector<double> underlying_prices = MeshArray<double>(60.0, 10.0, 5.0); // mesh array looks like <60.0, 70.0, 80.0, 90.0, 100.0>
OptionData batch1 = batches[0];     // using batch 1 data
EuropeanCall call(batch1);
EuropeanPut put(batch1);
vector<double> call_prices;         // vector to store computed call prices
vector<double> put_prices;          // vector to store computed put prices

// compute and store call and put prices in respective vectors
for (vector<double>::const_iterator itr = underlying_prices.begin(); itr != underlying_prices.end(); ++itr)
{
    call_prices.push_back(call.Price(*itr));
    put_prices.push_back(put.Price(*itr));
}

cout << "Spot\tCall\tPut" << endl;
for (size_t i = 0; i < underlying_prices.size(); ++i)
{
    cout << underlying_prices[i] << "\t" << call_prices[i] << "\t" << put_prices[i] << endl;
}
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
    
// Question D)
cout << "\n===== Group A, Section 1, Question D) =====" << endl;

try 
{   
    cout << "Computing European Call and Put option Price Matrices: " << endl;
    
    // vectors must be of same size for mesh array function
    size_t matrix_col_length = 10;
    
    // create vectors of parameters
    vector<double> spots = MeshArray(50.0, 10.0, matrix_col_length);
    vector<double> strikes = MeshArray(60.0, 10.0, matrix_col_length);
    vector<double> maturities = MeshArray(0.1, 0.1, matrix_col_length);
    vector<double> vols = MeshArray(0.1, 0.05, matrix_col_length);
    vector<double> rates = MeshArray(0.1, 0.02, matrix_col_length);
    vector<double> carry = MeshArray(0.1, 0.02, matrix_col_length);

    // use MatrixParameters class vector parameter constructor
    MatrixParameters vector_params(strikes, rates, vols, maturities, carry, spots);
    // use PricingMatrix class parameter constructor
    PricingMatrix call_matrix(vector_params, "EuropeanCall");
    PricingMatrix put_matrix(vector_params, "EuropeanPut");

    // compute price matrices
    call_matrix.ComputePriceMatrix();
    put_matrix.ComputePriceMatrix();

    // print price matrices
    call_matrix.PrintPriceMatrix();
    put_matrix.PrintPriceMatrix();

} catch (const ArrayException& e) {
    cout << "Error: " << e.GetMessage() << endl;
} catch (...) {
    cout << "Unexpected error." << endl;
}
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Group A: Section 2:                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Question A)
cout << "\n===== Group A, Section 2, Question A) =====" << endl;

OptionData partials_call(100.0, 0.1, 0.36, 0.5, 0.0);    
OptionData partials_put(100.0, 0.1, 0.36, 0.5, 0.0);
double partials_spot = 105.0;      // set spot price
EuropeanCall call_greeks(partials_call);
EuropeanPut put_greeks(partials_put);

cout << "Computing Delta and Gamma for European options: " << endl;
cout << "European Call Delta: " << call_greeks.Delta(partials_spot) << endl;   // call delta = 0.594629
cout << "European Call Gamma: " << call_greeks.Gamma(partials_spot) << endl;   // call gamma = 0.0134936
cout << "European Put Delta: " << put_greeks.Delta(partials_spot) << endl;     // put delta = -0.356601
cout << "European Put Gamma: " << put_greeks.Gamma(partials_spot) << endl;     // put gamma = 0.0134936
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

// Question B)
cout << "\n===== Group A, Section 2, Question B) =====" << endl;

// compute call deltas for a monotonically increasing range of underlying values S
vector<double> spot_mesh = MeshArray(105.0, 5.0, 5.0);  // create mesh array of spot prices
vector<double> call_deltas;                             // vector to store computed deltas
vector<double> call_gammas;                             // vector to store computed gammas (not required in question, but I want to do it)

// compute and store deltas and gammas in their respective vector
for (vector<double>::const_iterator itr = spot_mesh.begin(); itr != spot_mesh.end(); ++itr)
{
    call_deltas.push_back(call_greeks.Delta(*itr));
    call_gammas.push_back(call_greeks.Gamma(*itr));
}

cout << "Computing European call option deltas and gammas for a monotonically increasing mesh of spot prices:" << endl;
cout << "Spot\t\tDelta\tGamma" << endl;
for (size_t i = 0; i < spot_mesh.size(); ++i)
{
    cout << spot_mesh[i] << "\t" << call_deltas[i] << "\t" << call_gammas[i] << endl;
}
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

// Question C)
cout << "\n===== Group A, Section 2, Question C) =====" << endl;

// create matrices of option parameters (5 by 5 matrix)
vector<vector<double>> strikes = {{60.0, 65.0, 70.0, 75.0, 80.0},{85.0, 90.0, 95.0, 100.0, 105.0},{110.0, 115.0, 120.0, 125.0, 130.0},{135.0, 140.0, 145.0, 150.0, 155.0},{160.0, 165.0, 170.0, 175.0, 180.0}};
vector<vector<double>> rates = {{0.01, 0.02, 0.03, 0.04, 0.05},{0.06, 0.07, 0.08, 0.09, 0.10},{0.01, 0.02, 0.03, 0.04, 0.05},{0.06, 0.07, 0.08, 0.09, 0.10},{0.01, 0.02, 0.03, 0.04, 0.05}};
vector<vector<double>> vols = {{0.10, 0.15, 0.20, 0.25, 0.30},{0.12, 0.18, 0.22, 0.28, 0.32},{0.10, 0.15, 0.20, 0.25, 0.30},{0.12, 0.18, 0.22, 0.28, 0.32},{0.10, 0.15, 0.20, 0.25, 0.30}};
vector<vector<double>> maturities = {{0.1, 0.2, 0.3, 0.4, 0.5},{0.6, 0.7, 0.8, 0.9, 1.0},{0.1, 0.2, 0.3, 0.4, 0.5},{0.6, 0.7, 0.8, 0.9, 1.0},{0.1, 0.2, 0.3, 0.4, 0.5}};
vector<vector<double>> carry = {{0.01, 0.01, 0.01, 0.01, 0.01},{0.02, 0.02, 0.02, 0.02, 0.02},{0.01, 0.01, 0.01, 0.01, 0.01},{0.02, 0.02, 0.02, 0.02, 0.02},{0.01, 0.01, 0.01, 0.01, 0.01}};
vector<vector<double>> spots = {{50.0, 55.0, 60.0, 65.0, 70.0},{75.0, 80.0, 85.0, 90.0, 95.0},{100.0, 105.0, 110.0, 115.0, 120.0},{125.0, 130.0, 135.0, 140.0, 145.0},{150.0, 155.0, 160.0, 165.0, 170.0}};

// use MatrixParameters matrix constructor
MatrixParameters matrixParams(strikes, rates, vols, maturities, carry, spots);
PricingMatrix callMatrix(matrixParams, "EuropeanCall");
PricingMatrix putMatrix(matrixParams, "EuropeanPut");

try
{
    cout << "Computing European Call Delta Matrix:" << endl;
    callMatrix.ComputeDeltaMatrix();
    callMatrix.PrintDeltaMatrix();

    cout << "\nComputing European Call Gamma Matrix: " << endl;
    callMatrix.ComputeGammaMatrix();
    callMatrix.PrintGammaMatrix();

    cout << "\nComputing European Put Delta Matrix:" << endl;
    putMatrix.ComputeDeltaMatrix();
    putMatrix.PrintDeltaMatrix();

    cout << "\nComputing European Put Gamma Matrix: " << endl;
    putMatrix.ComputeGammaMatrix();
    putMatrix.PrintGammaMatrix();

} catch (const ArrayException& e) {
    cout << "Error: " << e.GetMessage() << endl;
} catch (...) {
    cout << "Unexpected error." << endl;
}
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

// Question D)
cout << "\n===== Group A, Section 2, Question D) =====" << endl;

// use the same option data as in part a) for comparison
OptionData divided_difference_call_params(100.0, 0.1, 0.36, 0.5, 0.0);    
OptionData divided_difference_put_params(100.0, 0.1, 0.36, 0.5, 0.0);
double divided_difference_strike = 105.0;                  
EuropeanCall divided_difference_call(divided_difference_call_params);
EuropeanPut divided_difference_put(divided_difference_put_params);

// to compare part a) results
cout << "Computing approximate Delta and Gamma for European options using Divided Differences: " << endl;
cout << "European Call Delta (h = 1.0): " << divided_difference_call.DividedDifferenceDelta(divided_difference_strike, 1) << endl;          // delta = 0.59458
cout << "European Call Delta (h = 1e-4): " << divided_difference_call.DividedDifferenceDelta(divided_difference_strike, 1e-4) << endl;      // delta = 0.59463
cout << "European Call Gamma (h = 1.0): " << divided_difference_call.DividedDifferenceGamma(divided_difference_strike, 1) << endl;          // gamma = 0.01349
cout << "European Call Gamma (h = 1e-4): " << divided_difference_call.DividedDifferenceGamma(divided_difference_strike, 1e-4) << endl;      // gamma = 0.01349
cout << "European Put Delta (h = 1.0): " << divided_difference_put.DividedDifferenceDelta(divided_difference_strike, 1) << endl;            // delta = -0.35665
cout << "European Put Delta (h = 1e-4): " << divided_difference_put.DividedDifferenceDelta(divided_difference_strike, 1e-4) << endl;        // delta = -0.35660
cout << "European Put Gamma (h = 1.0): " << divided_difference_put.DividedDifferenceGamma(divided_difference_strike, 1) << endl;            // gamma = 0.01349
cout << "European Put Gamma (h = 1e-4): " << divided_difference_put.DividedDifferenceGamma(divided_difference_strike, 1e-4) << endl;        // gamma = 0.01349

// to compare part b) results
cout << "\nComputing approximate European Call Delta and Gamma for a monotonically increasing mesh of spot prices: " << endl;
vector<double> divided_difference_mesh = MeshArray(105.0, 5.0, 5.0);  // same mesh array of spot prices as part b)
vector<double> mesh_deltas;     // vector to store computed deltas
vector<double> mesh_gammas;     // vector to store computed gammas

// calculate and store in respective vector
for (vector<double>::const_iterator itr = divided_difference_mesh.begin(); itr != divided_difference_mesh.end(); ++itr)
{
    mesh_deltas.push_back(divided_difference_call.DividedDifferenceDelta(*itr, 1e-4));
    mesh_gammas.push_back(divided_difference_call.DividedDifferenceGamma(*itr, 1e-4));
}

// output vectors
cout << "Spot\t\tDelta\tGamma" << endl;
for (size_t i = 0; i < divided_difference_mesh.size(); ++i)
{
    cout << divided_difference_mesh[i] << "\t" << mesh_deltas[i] << "\t" << mesh_gammas[i] << endl;
}   // approximation is very close to exact computation results
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Group B: Section 1:                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Question A and B)
cout << "\n===== Group B, Section 1, Question A and B) =====" << endl;

cout << "Computing Perpetual American Call and Put Prices:" << endl;
OptionData perpetual_options_data(100.0, 0.1, 0.1, 0.0, 0.02);  // set T = 0
PerpAmericanCall perp_call(perpetual_options_data);
PerpAmericanPut perp_put(perpetual_options_data);

cout << "Perpetual American Call Price: " << perp_call.Price(110.0) << endl;    // spot price = 110.0
// Call price: 18.50350
cout << "Perpetual American Put Price: " << perp_put.Price(110.0) << endl;      // spot price = 110.0
// Put price: 3.03106
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

// Question C)
cout << "\n===== Group B, Section 1, Question C) =====" << endl;

cout << "Computing Perpetual American Call and Put Prices for a monotonically increasing mesh of spot prices: " << endl;
vector<double> perp_options_mesh = MeshArray(110.0, 5.0, 5.0);  // create the mesh array
vector<double> perp_call_prices;    // vector to store call prices
vector<double> perp_put_prices;     // vector to store put prices

// calculate and store in respective vector, we will be using same option data parameters as used in Question a) and b)
for (vector<double>::const_iterator itr = perp_options_mesh.begin(); itr != perp_options_mesh.end(); ++itr)
{
    perp_call_prices.push_back(perp_call.Price(*itr));
    perp_put_prices.push_back(perp_put.Price(*itr));
}

// output calculated vectors
cout << "Spot\t\tCall\t\tPut" << endl;
for (size_t i = 0; i < perp_options_mesh.size(); ++i)
{
    cout << perp_options_mesh[i] << "\t" << perp_call_prices[i] << "\t" << perp_put_prices[i] << endl;
}
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

// Question D)
cout << "\n===== Group B, Section 1, Question D) =====" << endl;

// create matrices of option parameters (5 x 5 matrices)
// b < r for the computation to work
vector<vector<double>> perp_strikes = {{50.0, 55.0, 60.0, 65.0, 70.0},{75.0, 80.0, 85.0, 90.0, 95.0},{100.0, 105.0, 110.0, 115.0, 120.0},{125.0, 130.0, 135.0, 140.0, 145.0},{150.0, 155.0, 160.0, 165.0, 170.0}};
vector<vector<double>> perp_rates = {{0.10, 0.10, 0.10, 0.10, 0.10},{0.12, 0.12, 0.12, 0.12, 0.12},{0.14, 0.14, 0.14, 0.14, 0.14},{0.16, 0.16, 0.16, 0.16, 0.16},{0.18, 0.18, 0.18, 0.18, 0.18}};
vector<vector<double>> perp_vols = {{0.10, 0.15, 0.20, 0.25, 0.30},{0.12, 0.17, 0.22, 0.27, 0.32},{0.14, 0.19, 0.24, 0.29, 0.34},{0.16, 0.21, 0.26, 0.31, 0.36},{0.18, 0.23, 0.28, 0.33, 0.38}};
// T = 0 for perpetual American options
vector<vector<double>> perp_maturities = {{0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0, 0.0}};
vector<vector<double>> perp_carry = {{0.01, 0.02, 0.03, 0.04, 0.05},{0.01, 0.02, 0.03, 0.04, 0.05},{0.01, 0.02, 0.03, 0.04, 0.05},{0.01, 0.02, 0.03, 0.04, 0.05},{0.01, 0.02, 0.03, 0.04, 0.05}};
vector<vector<double>> perp_spots = {{50.0, 55.0, 60.0, 65.0, 70.0},{75.0, 80.0, 85.0, 90.0, 95.0},{100.0, 105.0, 110.0, 115.0, 120.0},{125.0, 130.0, 135.0, 140.0, 145.0},{150.0, 155.0, 160.0, 165.0, 170.0}};

// use MatrixParameters matrix constructor
MatrixParameters perp_matrix_params(perp_strikes, perp_rates, perp_vols, perp_maturities, perp_carry, perp_spots);
PricingMatrix perp_call_matrix(perp_matrix_params, "PerpAmericanCall");
PricingMatrix perp_put_matrix(perp_matrix_params, "PerpAmericanPut");

try
{
    cout << "Computing Perpetual American Call Price Matrix:" << endl;
    perp_call_matrix.ComputePriceMatrix();
    perp_call_matrix.PrintPriceMatrix();

    cout << "\nComputing Perpetual American Put Price Matrix:" << endl;
    perp_put_matrix.ComputePriceMatrix();
    perp_put_matrix.PrintPriceMatrix();

    cout << "\nApproximating Perpetual American Call Deltas Using the Divided Difference Method (h = 1e-4):" << endl;
    perp_call_matrix.ComputeDeltaMatrix();
    perp_call_matrix.PrintDeltaMatrix();

    cout << "\nApproximating Perpetual American Call Gammas Using the Divided Difference Method (h = 1e-4):" << endl;
    perp_call_matrix.ComputeGammaMatrix();
    perp_call_matrix.PrintGammaMatrix();

    cout << "\nApproximating Perpetual American Put Deltas Using the Divided Difference Method (h = 1e-4):" << endl;
    perp_put_matrix.ComputeDeltaMatrix();
    perp_put_matrix.PrintDeltaMatrix();

    cout << "\nApproximating Perpetual American Put Gammas Using the Divided Difference Method (h = 1e-4):" << endl;
    perp_put_matrix.ComputeGammaMatrix();
    perp_put_matrix.PrintGammaMatrix();

} catch (const ArrayException& e) {
    cout << "Error: " << e.GetMessage() << endl;
} catch (...) {
    cout << "Unexpected error." << endl;
}
cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

    return 0;
}