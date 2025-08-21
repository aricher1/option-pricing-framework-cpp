#ifndef PerpAmericanPut_HPP
#define PerpAmericanPut_HPP

#include "Option.hpp"
#include "OptionData.hpp"
#include <string>
#include <iostream>

namespace AidanRicher {
namespace Engine {

class PerpAmericanPut : public Option {
    private:
        OptionData m_data;      // private member to hold option data

    public:
        PerpAmericanPut();                              // default constructor
        PerpAmericanPut(const OptionData& data);        // parameter constructor
        PerpAmericanPut(const PerpAmericanPut& other);  // copy constructor
        virtual ~PerpAmericanPut();                     // destructor

        // assignment operator
        PerpAmericanPut& operator = (const PerpAmericanPut& other);

        // getter for const reference to option data
        const OptionData& GetData() const;

        // other methods
        double Price(const double U) const override;                            // return the price of the put
        double DividedDifferenceDelta(const double U, const double h) const;    // delta approximation using the divided difference method
        double DividedDifferenceGamma(const double U, const double h) const;    // gamma approximation using the divided difference method

        // ostream << operator
        friend std::ostream& operator << (std::ostream& os, const PerpAmericanPut& source);

        // pure virtual function overrides from Option base class
        double Delta(const double U) const override;
        double Gamma(const double U) const override;
        virtual std::string Type() const override;
        virtual Option* Clone() const override;
};

} // namespace Engine
} // namespace AidanRicher

#endif // PerpAmericanPut_HPP