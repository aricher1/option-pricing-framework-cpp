#include "PricingMatrix.hpp"
#include "EuropeanCall.hpp"
#include "EuropeanPut.hpp"
#include "PerpAmericanCall.hpp"
#include "PerpAmericanPut.hpp"
#include "ArrayException.hpp"
#include <iostream>
#include <iomanip>

using namespace AidanRicher::Containers;

namespace AidanRicher {
namespace Engine {

// defualt constructor
PricingMatrix::PricingMatrix() { }

// parameter constructor
PricingMatrix::PricingMatrix(const MatrixParameters& params, const std::string& type) : m_params(params), m_type(type), m_priceMatrix(), m_deltaMatrix(), m_gammaMatrix() { }

// copy constructor
PricingMatrix::PricingMatrix(const PricingMatrix& other) : m_params(other.m_params), m_type(other.m_type), m_priceMatrix(other.m_priceMatrix), m_deltaMatrix(other.m_deltaMatrix), m_gammaMatrix(other.m_gammaMatrix) { }

// destructor
PricingMatrix::~PricingMatrix() = default;

// assignment operator
PricingMatrix& PricingMatrix::operator = (const PricingMatrix& other)
{
    if (this == &other) { return *this; }

    m_params = other.m_params;
    m_type = other.m_type;
    m_priceMatrix = other.m_priceMatrix;
    m_deltaMatrix = other.m_deltaMatrix;
    m_gammaMatrix = other.m_gammaMatrix;

    return *this;
}

void PricingMatrix::ComputePriceMatrix()
{
    const std::vector<std::vector<double>>& strikes = m_params.GetStrikes();
    const std::vector<std::vector<double>>& rates = m_params.GetRates();
    const std::vector<std::vector<double>>& vols = m_params.GetVols();
    const std::vector<std::vector<double>>& maturities = m_params.GetMaturities();
    const std::vector<std::vector<double>>& carry = m_params.GetCarry();
    const std::vector<std::vector<double>>& spots = m_params.GetSpots();

    // verify outer matrix dimenstions are equal
    if (strikes.size() != rates.size() || strikes.size() != vols.size() || strikes.size() != maturities.size() || strikes.size() != carry.size() || strikes.size() != spots.size())
    {
        throw SizeMismatchException();
    }
    
    // verify inner matrix dimensions per row are equal
    for (size_t i = 0; i < strikes.size(); ++i)
    {
        if (rates[i].size() != strikes[i].size() || vols[i].size() != strikes[i].size() || maturities[i].size() != strikes[i].size() || carry[i].size() != strikes[i].size() || spots[i].size() != strikes[i].size())
        {
            throw SizeMismatchException();
        }
    }

    // resize price matrix
    m_priceMatrix.clear();
    m_priceMatrix.resize(strikes.size());   // rows

    for (size_t i = 0; i < strikes.size(); ++i)
    {
        m_priceMatrix[i].resize(strikes[i].size());     // columns
        
        for (size_t j = 0; j < strikes[i].size(); ++j)
        {
            OptionData optionData(strikes[i][j], rates[i][j], vols[i][j], maturities[i][j], carry[i][j]);
            double spot = spots[i][j];

            if (m_type == "EuropeanCall")
            {
                EuropeanCall opt(optionData);
                m_priceMatrix[i][j] = opt.Price(spot);
            }
            else if (m_type == "EuropeanPut")
            {
                EuropeanPut opt(optionData);
                m_priceMatrix[i][j] = opt.Price(spot);
            }
            else if (m_type == "PerpAmericanCall")
            {
                PerpAmericanCall opt(optionData);
                m_priceMatrix[i][j] = opt.Price(spot);
            }
            else if (m_type == "PerpAmericanPut")
            {
                PerpAmericanPut opt(optionData);
                m_priceMatrix[i][j] = opt.Price(spot);
            }
            else
            {
                throw UnexpectedInputException();
            }
        }
    }
}

void PricingMatrix::ComputeDeltaMatrix(const double h)
{
    const std::vector<std::vector<double>>& strikes = m_params.GetStrikes();
    const std::vector<std::vector<double>>& rates = m_params.GetRates();
    const std::vector<std::vector<double>>& vols = m_params.GetVols();
    const std::vector<std::vector<double>>& maturities = m_params.GetMaturities();
    const std::vector<std::vector<double>>& carry = m_params.GetCarry();
    const std::vector<std::vector<double>>& spots = m_params.GetSpots();

    // verify outer matrix dimenstions are equal
    if (strikes.size() != rates.size() || strikes.size() != vols.size() || strikes.size() != maturities.size() || strikes.size() != carry.size() || strikes.size() != spots.size())
    {
        throw SizeMismatchException();
    }
    
    // verify inner matrix dimensions per row are equal
    for (size_t i = 0; i < strikes.size(); ++i)
    {
        if (rates[i].size() != strikes[i].size() || vols[i].size() != strikes[i].size() || maturities[i].size() != strikes[i].size() || carry[i].size() != strikes[i].size() || spots[i].size() != strikes[i].size())
        {
            throw SizeMismatchException();
        }
    }

    // resize delta matrix
    m_deltaMatrix.clear();
    m_deltaMatrix.resize(strikes.size());

    for (size_t i = 0; i < strikes.size(); ++i)
    {
        m_deltaMatrix[i].resize(strikes[i].size());

        for (size_t j = 0; j < strikes[i].size(); ++j)
        {
            OptionData optionData(strikes[i][j], rates[i][j], vols[i][j], maturities[i][j], carry[i][j]);
            double spot = spots[i][j];

            if (m_type == "EuropeanCall")
            {
                EuropeanCall opt(optionData);
                m_deltaMatrix[i][j] = opt.Delta(spot);
            }
            else if (m_type == "EuropeanPut")
            {
                EuropeanPut opt(optionData);
                m_deltaMatrix[i][j] = opt.Delta(spot);
            }
            else if (m_type == "PerpAmericanCall")
            {
                PerpAmericanCall opt(optionData);
                m_deltaMatrix[i][j] = opt.DividedDifferenceDelta(spot, h);
            }
            else if (m_type == "PerpAmericanPut")
            {
                PerpAmericanPut opt(optionData);
                m_deltaMatrix[i][j] = opt.DividedDifferenceDelta(spot, h);
            }
            else
            {
                throw UnexpectedInputException();
            }
        }
    }
}

void PricingMatrix::ComputeGammaMatrix(const double h)
{
    const std::vector<std::vector<double>>& strikes = m_params.GetStrikes();
    const std::vector<std::vector<double>>& rates = m_params.GetRates();
    const std::vector<std::vector<double>>& vols = m_params.GetVols();
    const std::vector<std::vector<double>>& maturities = m_params.GetMaturities();
    const std::vector<std::vector<double>>& carry = m_params.GetCarry();
    const std::vector<std::vector<double>>& spots = m_params.GetSpots();

    // verify outer matrix dimenstions are equal
    if (strikes.size() != rates.size() || strikes.size() != vols.size() || strikes.size() != maturities.size() || strikes.size() != carry.size() || strikes.size() != spots.size())
    {
        throw SizeMismatchException();
    }
    
    // verify inner matrix dimensions per row are equal
    for (size_t i = 0; i < strikes.size(); ++i)
    {
        if (rates[i].size() != strikes[i].size() || vols[i].size() != strikes[i].size() || maturities[i].size() != strikes[i].size() || carry[i].size() != strikes[i].size() || spots[i].size() != strikes[i].size())
        {
            throw SizeMismatchException();
        }
    }

    // resize gamma matrix
    m_gammaMatrix.clear();
    m_gammaMatrix.resize(strikes.size());

    for (size_t i = 0; i < strikes.size(); ++i)
    {
        m_gammaMatrix[i].resize(strikes[i].size());

        for (size_t j = 0; j < strikes[i].size(); ++j)
        {
            OptionData optionData(strikes[i][j], rates[i][j], vols[i][j], maturities[i][j], carry[i][j]);
            double spot = spots[i][j];

            if (m_type == "EuropeanCall")
            {
                EuropeanCall opt(optionData);
                m_gammaMatrix[i][j] = opt.Gamma(spot);
            }
            else if (m_type == "EuropeanPut")
            {
                EuropeanPut opt(optionData);
                m_gammaMatrix[i][j] = opt.Gamma(spot);
            }
            else if (m_type == "PerpAmericanCall")
            {
                PerpAmericanCall opt(optionData);
                m_gammaMatrix[i][j] = opt.DividedDifferenceGamma(spot, h);
            }
            else if (m_type == "PerpAmericanPut")
            {
                PerpAmericanPut opt(optionData);
                m_gammaMatrix[i][j] = opt.DividedDifferenceGamma(spot, h);
            }
            else
            {
                throw UnexpectedInputException();
            }
        }
    }
}

void PricingMatrix::PrintPriceMatrix()
{
    std::cout << m_type << " Price Matrix:\n";
    for (size_t i = 0; i < m_priceMatrix.size(); ++i)
    {
        for (size_t j = 0; j < m_priceMatrix[i].size(); ++j)
        {
            std::cout << std::fixed << std::setprecision(5) << std::setw(10) << m_priceMatrix[i][j];
        }
        std::cout << "\n";
    }
}

void PricingMatrix::PrintDeltaMatrix()
{
    std::cout << m_type << " Delta Matrix:\n";
    for (size_t i = 0; i < m_deltaMatrix.size(); ++i)
    {
        for (size_t j = 0; j < m_deltaMatrix[i].size(); ++j)
        {
            std::cout << std::fixed << std::setprecision(5) << std::setw(10) << m_deltaMatrix[i][j];
        }
        std::cout << "\n";
    }
}

void PricingMatrix::PrintGammaMatrix()
{
    std::cout << m_type << " Gamma Matrix:\n";
    for (size_t i = 0; i < m_gammaMatrix.size(); ++i)
    {
        for (size_t j = 0; j < m_gammaMatrix[i].size(); ++j)
        {
            std::cout << std::fixed << std::setprecision(5) << std::setw(10) << m_gammaMatrix[i][j];
        }
        std::cout << "\n";
    }
}

const std::vector<std::vector<double>>& PricingMatrix::GetPriceMatrix() const
{
    return m_priceMatrix;
}

const std::vector<std::vector<double>>& PricingMatrix::GetDeltaMatrix() const
{
    return m_deltaMatrix;
}

const std::vector<std::vector<double>>& PricingMatrix::GetGammaMatrix() const
{
    return m_gammaMatrix;
}

} // namespace Engine
} // namespace AidanRicher