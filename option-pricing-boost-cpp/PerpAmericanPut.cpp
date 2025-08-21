#include "PerpAmericanPut.hpp"
#include <cmath>
#include <iostream>

namespace AidanRicher {
namespace Engine {

// default constructor
PerpAmericanPut::PerpAmericanPut() { }

// parameter constructor
PerpAmericanPut::PerpAmericanPut(const OptionData& data) : m_data(data) { }

// copy constructor
PerpAmericanPut::PerpAmericanPut(const PerpAmericanPut& other) : m_data(other.m_data) { }

// virtual destructor
PerpAmericanPut::~PerpAmericanPut() { }

// assignment operator
PerpAmericanPut& PerpAmericanPut::operator = (const PerpAmericanPut& other)
{
    if (this == &other) { return *this; }   // self-assignment

    m_data = other.m_data;

    return *this;
}

// getter for const reference to option data
const OptionData& PerpAmericanPut::GetData() const
{
    return m_data;
}

double PerpAmericanPut::Price(const double U) const 
{ // return the price of the perpetual american put option
    if (U <= 0.0)
    {
        throw std::invalid_argument("Underlying spot price U must be postive.");
    }

    double sigma_squared = m_data.Sig() * m_data.Sig();
    double y2 = 0.5 - (m_data.B() / sigma_squared) - std::sqrt(std::pow((m_data.B() / sigma_squared - 0.5), 2.0) + (2.0 * m_data.R() / sigma_squared));
    double put_rhs = ((y2 - 1.0) / y2) * (U / m_data.K());

    // return put price
    return (m_data.K() / (1.0 - y2)) * std::pow(put_rhs, y2);
}

double PerpAmericanPut::DividedDifferenceDelta(const double U, const double h) const
{ // delta approximation using the divided difference method
    if (abs(Price(U + h) - Price(U - h)) <= std::pow(2, -53))
    {
        std::cout << "H is too small for accurate computation." << std::endl;
        return 0;
    }

    // return approximated delta
    return (Price(U + h) - Price(U - h)) / (2.0 * h);
}

double PerpAmericanPut::DividedDifferenceGamma(const double U, const double h) const
{ // gamma approximation using the divided difference method
    if (abs(Price(U + h) - 2.0 * Price(U) + Price(U - h)) <= std::pow(2, -53))
    {
        std::cout << "H is too small for accurate computation." << std::endl;
        return 0;
    }

    // return approximated gamma
    return (Price(U + h) - 2.0 * Price(U) + Price(U - h)) / (h * h);
}

std::ostream& operator << (std::ostream& os, const PerpAmericanPut& source)
{ // ostream << operator for option properties
    os << std::endl;
    os << "Perpetual American Put Option:\n" <<
        "K: " << source.m_data.K() << "\n" << 
        "R: " << source.m_data.R() << "\n" <<
        "Sig: " << source.m_data.Sig() << "\n" << 
        "T: " << source.m_data.T() << "\n" << 
        "B: " << source.m_data.B() << std::endl;

    // return description
    return os;
}

// delta and gamma are not implemented for this class, so we will just return 0.0 for now
double PerpAmericanPut::Delta(const double U) const 
{   
    // parameter U isn't being used
    (void)U; 
    return 0.0; 
}

double PerpAmericanPut::Gamma(const double U) const 
{   
    // parameter U isn't being used
    (void)U;
    return 0.0; 
}

std::string PerpAmericanPut::Type() const 
{
    return "Perpetual American Put Option";
}

Option* PerpAmericanPut::Clone() const 
{
    return new PerpAmericanPut(*this);
}

} // namespace Engine 
} // namespace AidanRicher