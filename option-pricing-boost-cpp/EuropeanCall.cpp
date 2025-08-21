#include "EuropeanCall.hpp"
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace boost::math;

namespace AidanRicher {
namespace Engine {

// default constructor
EuropeanCall::EuropeanCall() : m_data() { }

// parameter constructor
EuropeanCall::EuropeanCall(const OptionData& data) : m_data(data) { }

// copy constructor
EuropeanCall::EuropeanCall(const EuropeanCall& other) : m_data(other.m_data) { }

// virtual destructor
EuropeanCall::~EuropeanCall() { }

// assignment operator
EuropeanCall& EuropeanCall::operator = (const EuropeanCall& other) 
{
    if (this == &other) { return *this; }   // self-assignment
    
    m_data = other.m_data;
    
    return *this;
}

// getter for const reference to option data
const OptionData& EuropeanCall::GetData() const
{
    return m_data;      // when private members need to be accessed
}

// core methods
// parameter (U) is passed as the underlying asset spot price
double EuropeanCall::Price(const double U) const
{ // return the price of the European call option
    if (U <= 0.0) 
    {
        throw std::invalid_argument("Underlying spot price U must be positive.");
    }

    normal_distribution<> myNormal;                     // boost normal distribution
    double d1 = (log(U / m_data.K()) + (m_data.B() + 0.5 * m_data.Sig() * m_data.Sig()) * m_data.T()) / (m_data.Sig() * sqrt(m_data.T())); 
    double d2 = d1 - m_data.Sig() * sqrt(m_data.T());
    
    // return call price
    return U * exp((m_data.B() - m_data.R()) * m_data.T()) * cdf(myNormal, d1) - m_data.K() * exp(-m_data.R() * m_data.T()) * cdf(myNormal, d2);
}

double EuropeanCall::Delta(const double U) const
{ // return the delta of the European call option
    if (U <= 0.0) 
    {
        throw std::invalid_argument("Underlying spot price U must be positive.");
    }

    normal_distribution<> myNormal;                     // boost normal distribution
    double d1 = (log(U / m_data.K()) + (m_data.B() + 0.5 * m_data.Sig() * m_data.Sig()) * m_data.T()) / (m_data.Sig() * sqrt(m_data.T()));  

    // return delta
    return exp((m_data.B() - m_data.R()) * m_data.T()) * cdf(myNormal, d1);
}

double EuropeanCall::Gamma(const double U) const
{ // return the gamma of the European call option
    if (U <= 0.0) 
    {
        throw std::invalid_argument("Underlying spot price U must be positive.");
    }
    
    normal_distribution<> myNormal;
    double d1 = (log(U / m_data.K()) + (m_data.B() + 0.5 * m_data.Sig() * m_data.Sig()) * m_data.T()) / (m_data.Sig() * sqrt(m_data.T())); 

    // return gamma
    return exp((m_data.B() - m_data.R()) * m_data.T()) * pdf(myNormal, d1) / (U * m_data.Sig() * sqrt(m_data.T()));
}

double EuropeanCall::PutCallParity(const double U) const 
{ // return put price implied by put-call parity
    return Price(U) - U + m_data.K() * exp(-m_data.R() * m_data.T());
} 

double EuropeanCall::DividedDifferenceDelta(const double U, const double h) const
{ // delta approximation using the divided difference method
    if (abs(Price(U + h) - Price(U - h)) <= std::pow(2, -53))
    {
        std::cout << "H is too small for accurate computation." << std::endl;
        return 0;
    }

    // return approximated delta
    return (Price(U + h) - Price(U - h)) / (2.0 * h);
}

double EuropeanCall::DividedDifferenceGamma(const double U, const double h) const
{ // gamma approximation using the divided difference method
    if (abs(Price(U + h) - 2.0 * Price(U) + Price(U - h)) <= std::pow(2, -53))
    {
        std::cout << "H is too small for accurate computation." << std::endl;
        return 0;
    }
    
    // return approximated gamma
    return (Price(U + h) - 2.0 * Price(U) + Price(U - h)) / (h * h); 
}

std::ostream& operator << (std::ostream& os, const EuropeanCall& source)
{ // ostream << operator for option properties
    os << std::endl;
    os << "European Call Option:\n" <<
        "K: " << source.m_data.K() << "\n" <<
        "R: " << source.m_data.R() << "\n" <<
        "Sig: " << source.m_data.Sig() << "\n" <<
        "T: " << source.m_data.T() << "\n" <<
        "B: " << source.m_data.B() << std::endl;
    
    // return description
    return os;
}

// overrides
std::string EuropeanCall::Type() const 
{
    return "European Call Option";
}

Option* EuropeanCall::Clone() const 
{
    return new EuropeanCall(*this);
}

} // namespace Engine
} // namespace AidanRicher