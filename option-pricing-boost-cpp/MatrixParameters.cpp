#include "MatrixParameters.hpp"

namespace AidanRicher {
namespace Engine {

// default constructor
MatrixParameters::MatrixParameters() { }

// parameter constructor to construct from matrices directly
MatrixParameters::MatrixParameters(const std::vector<std::vector<double>>& strikes, const std::vector<std::vector<double>>& rates, const std::vector<std::vector<double>>& vols, const std::vector<std::vector<double>>& maturities, const std::vector<std::vector<double>>& carry, const std::vector<std::vector<double>>& spots)
: m_strikes(strikes), m_rates(rates), m_vols(vols), m_maturities(maturities), m_carry(carry), m_spots(spots) { }

// parameter constructor to construct from vectors, converts to single-row matrices
MatrixParameters::MatrixParameters(const std::vector<double>& strikes, const std::vector<double>& rates, const std::vector<double>& vols, const std::vector<double>& maturities, const std::vector<double>& carry, const std::vector<double>& spots)
: m_strikes{strikes}, m_rates{rates}, m_vols{vols}, m_maturities{maturities}, m_carry{carry}, m_spots{spots} { }

// copy constructor
MatrixParameters::MatrixParameters(const MatrixParameters& other) : m_strikes(other.m_strikes), m_rates(other.m_rates), m_vols(other.m_vols), m_maturities(other.m_maturities), m_carry(other.m_carry), m_spots(other.m_spots) { }

// destructor
MatrixParameters::~MatrixParameters() = default;

// assignment operator
MatrixParameters& MatrixParameters::operator = (const MatrixParameters& other)
{
    if (this == &other) { return *this; }

    m_strikes = other.m_strikes;
    m_rates = other.m_rates;
    m_vols = other.m_vols;
    m_maturities = other.m_maturities;
    m_carry = other.m_carry;
    m_spots = other.m_spots;

    return *this;
}

// getter functions
const std::vector<std::vector<double>>& MatrixParameters::GetStrikes() const
{
    return m_strikes;
}

const std::vector<std::vector<double>>& MatrixParameters::GetRates() const
{
    return m_rates;
}

const std::vector<std::vector<double>>& MatrixParameters::GetVols() const
{
    return m_vols;
}

const std::vector<std::vector<double>>& MatrixParameters::GetMaturities() const
{
    return m_maturities;
}

const std::vector<std::vector<double>>& MatrixParameters::GetCarry() const
{
    return m_carry;
}

const std::vector<std::vector<double>>& MatrixParameters::GetSpots() const
{
    return m_spots;
}

} // namespace Engine
} // namespace AidanRicher