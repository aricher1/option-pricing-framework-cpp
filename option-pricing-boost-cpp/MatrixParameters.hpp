#ifndef MatrixParameters_HPP
#define MatrixParameters_HPP

#include <vector>
#include <string>

namespace AidanRicher {
namespace Engine {

class MatrixParameters {
    private:
        // store paramters as 2D matrices (vector of vectors)
        std::vector<std::vector<double>> m_strikes;
        std::vector<std::vector<double>> m_rates;
        std::vector<std::vector<double>> m_vols;
        std::vector<std::vector<double>> m_maturities;
        std::vector<std::vector<double>> m_carry;
        std::vector<std::vector<double>> m_spots;

    public: 
        // default constructor
        MatrixParameters(); 

        // parameter constructor to construct from matrices directly
        MatrixParameters(const std::vector<std::vector<double>>& strikes, const std::vector<std::vector<double>>& rates, const std::vector<std::vector<double>>& vols, const std::vector<std::vector<double>>& maturities, const std::vector<std::vector<double>>& carry, const std::vector<std::vector<double>>& spots);

        // parameter constructor to construct from vectors, converts to single-row matrices
        MatrixParameters(const std::vector<double>& strikes, const std::vector<double>& rates, const std::vector<double>& vols, const std::vector<double>& maturities, const std::vector<double>& carry, const std::vector<double>& spots);

        // copy constructor
        MatrixParameters(const MatrixParameters& other);

        // destructor
        ~MatrixParameters();

        // assignment operator
        MatrixParameters& operator = (const MatrixParameters& other);

        // getter functions
        const std::vector<std::vector<double>>& GetStrikes() const;
        const std::vector<std::vector<double>>& GetRates() const;
        const std::vector<std::vector<double>>& GetVols() const;
        const std::vector<std::vector<double>>& GetMaturities() const;
        const std::vector<std::vector<double>>& GetCarry() const;
        const std::vector<std::vector<double>>& GetSpots() const;
};

} // namespace Engine
} // namespace AidanRicher

#endif // MatrixParameters_HPP