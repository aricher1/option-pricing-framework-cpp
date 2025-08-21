#include "OptionData.hpp"

namespace AidanRicher {
namespace Engine {

// default constructor
OptionData::OptionData() : m_k(0.0), m_r(0.0), m_sig(0.0), m_t(0.0), m_b(0.0) { }

// parameter constructor
OptionData::OptionData(double k, double r, double sig, double t, double b) : m_k(k), m_r(r), m_sig(sig), m_t(t), m_b(b) { }

// copy constructor
OptionData::OptionData(const OptionData& other) : m_k(other.m_k), m_r(other.m_r), m_sig(other.m_sig), m_t(other.m_t), m_b(other.m_b) { }

// destructor
OptionData::~OptionData() { }   // no dynamic memory allocated

// getter functions (read only const)
double OptionData::K() const { return m_k; }
double OptionData::R() const { return m_r; }
double OptionData::Sig() const { return m_sig; }
double OptionData::T() const { return m_t; }
double OptionData::B() const { return m_b; }

// setter functions
void OptionData::K(double k) { m_k = k; }
void OptionData::R(double r) { m_r = r; }
void OptionData::Sig(double sig) { m_sig = sig; }
void OptionData::T(double t) { m_t = t; }
void OptionData::B(double b) { m_b = b; }

// assignment operator
OptionData& OptionData::operator = (const OptionData& other) 
{
    if (this == &other) { return *this; } // self-assignment

    m_k = other.m_k;
    m_r = other.m_r;
    m_sig = other.m_sig;
    m_t = other.m_t;
    m_b = other.m_b;
    
    return *this;
}

} // namespace Engine
} // namespace AidanRicher