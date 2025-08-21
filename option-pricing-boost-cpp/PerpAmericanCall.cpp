#include "PerpAmericanCall.hpp"
#include <cmath>
#include <iostream>

namespace AidanRicher {
namespace Engine {

// default constructor
PerpAmericanCall::PerpAmericanCall() { }

// parameter constructor
PerpAmericanCall::PerpAmericanCall(const OptionData& data) : m_data(data) { }

// copy constructor
PerpAmericanCall::PerpAmericanCall(const PerpAmericanCall& other) : m_data(other.m_data) { }

// virtual destructor
PerpAmericanCall::~PerpAmericanCall() { }

// assignment operator
PerpAmericanCall& PerpAmericanCall::operator = (const PerpAmericanCall& other)
{
    if (this == &other) { return *this; }   // self-assignment

    m_data = other.m_data;

    return *this;
}

// getter for const reference to option data
const OptionData& PerpAmericanCall::GetData() const
{
    return m_data;
}

double PerpAmericanCall::Price(const double U) const
{ // return the price of the perpetual american call option
    if (U <= 0.0)
    {
        throw std::invalid_argument("Underlying spot price U must be positive.");
    }
    
    double sigma_squared = m_data.Sig() * m_data.Sig();
    double y1 = 0.5 - (m_data.B() / sigma_squared) + std::sqrt(std::pow((m_data.B() / sigma_squared - 0.5), 2.0) + (2.0 * m_data.R() / sigma_squared));
    double call_rhs = ((y1 - 1.0) / y1) * (U / m_data.K());

    // return call price
    return (m_data.K() / (y1 - 1.0)) * std::pow(call_rhs, y1);
}

double PerpAmericanCall::DividedDifferenceDelta(const double U, const double h) const 
{ // delta approximation using the divided difference method
    if (abs(Price(U + h) - Price(U - h)) <= std::pow(2, -53))
    {
        std::cout << "H is too small for accurate computation." << std::endl;
        return 0;
    }

    // return approximated delta
    return (Price(U + h) - Price(U - h)) / (2.0 * h);
}

double PerpAmericanCall::DividedDifferenceGamma(const double U, const double h) const
{ // gamma approximation using the divided difference method
    if (abs(Price(U + h) - 2.0 * Price(U) + Price(U - h)) <= std::pow(2, -53))
    {
        std::cout << "H is too small for accurate computation." << std::endl;
        return 0;
    }

    // return approximated gamma
    return (Price(U + h) - 2.0 * Price(U) + Price(U - h)) / (h * h);
}

std::ostream& operator << (std::ostream& os, const PerpAmericanCall& source)
{ // ostream << operator for option properties
    os << std::endl;
    os << "Perpetual American Call Option:\n" <<
        "K: " << source.m_data.K() << "\n" << 
        "R: " << source.m_data.R() << "\n" <<
        "Sig: " << source.m_data.Sig() << "\n" << 
        "T: " << source.m_data.T() << "\n" << 
        "B: " << source.m_data.B() << std::endl;

    // return description
    return os;
}

// delta and gamma were not implimented in this class, so we will just return 0.0 for now
double PerpAmericanCall::Delta(const double U) const 
{   
    // parameter U isn't being used
    (void)U;
    return 0.0; 
}

double PerpAmericanCall::Gamma(const double U) const 
{   
    // parameter U isn't being used
    (void)U;
    return 0.0; 
}

std::string PerpAmericanCall::Type() const 
{
    return "Perpetual American Call Option";
}

Option* PerpAmericanCall::Clone() const 
{
    return new PerpAmericanCall(*this);
}

} // namespace Engine
} // namespace AidanRicher