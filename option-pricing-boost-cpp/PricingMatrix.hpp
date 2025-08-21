#ifndef PricingMatrix_HPP
#define PricingMatrix_HPP

#include "MatrixParameters.hpp"
#include <vector>
#include <string>

namespace AidanRicher {
namespace Engine {

class PricingMatrix {
    private:
        MatrixParameters m_params;      // option parameter matrix
        std::string m_type;             // option type

        // store our computed matrices
        std::vector<std::vector<double>> m_priceMatrix;
        std::vector<std::vector<double>> m_deltaMatrix;
        std::vector<std::vector<double>> m_gammaMatrix;

    public:
        // default constructor
        PricingMatrix();

        // parameter constructor
        PricingMatrix(const MatrixParameters& params, const std::string& type);

        // copy constructor
        PricingMatrix(const PricingMatrix& other);

        // destructor
        ~PricingMatrix();

        // assignment operator
        PricingMatrix& operator = (const PricingMatrix& other);

        // computational functions
        void ComputePriceMatrix();
        // Delta and Gamma Matrix coomputation functions must have an h parameter as an input
        // contains the divided difference method calcualation for approximating perpetual american options
        // h is set to 0.0001, or h = 1e-4
        void ComputeDeltaMatrix(const double h = 0.0001);
        void ComputeGammaMatrix(const double h = 0.0001);

        // printing functions
        void PrintPriceMatrix();
        void PrintDeltaMatrix();
        void PrintGammaMatrix();

        // getter functions
        const std::vector<std::vector<double>>& GetPriceMatrix() const;
        const std::vector<std::vector<double>>& GetDeltaMatrix() const;
        const std::vector<std::vector<double>>& GetGammaMatrix() const;
};

} // namespace Engine
} // namespace AidanRicher

#endif // PricingMatrix_HPP