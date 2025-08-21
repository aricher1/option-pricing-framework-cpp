#ifndef PerpAmericanCall_HPP
#define PerpAmericanCall_HPP

#include "Option.hpp"
#include "OptionData.hpp"
#include <string>
#include <iostream>

namespace AidanRicher {
namespace Engine {

class PerpAmericanCall : public Option {
    private: 
        OptionData m_data;      // private member to hold option data

    public:
        PerpAmericanCall();                                 // default constructor
        PerpAmericanCall(const OptionData& data);           // parameter constructor
        PerpAmericanCall(const PerpAmericanCall& other);    // copy constructor
        virtual ~PerpAmericanCall();                        // destructor

        // assignment operator
        PerpAmericanCall& operator = (const PerpAmericanCall& other);

        // getter for const reference to option data
        const OptionData& GetData() const;

        // other methods
        double Price(const double U) const override;                            // return the price of the call
        double DividedDifferenceDelta(const double U, const double h) const;    // delta approximation using divided difference method
        double DividedDifferenceGamma(const double U, const double h) const;    // gamma approximation using divided difference method

        // ostream << operator
        friend std::ostream& operator << (std::ostream& os, const PerpAmericanCall& source);

        // pure virtual function overrides from Option base class
        double Delta(const double U) const override;
        double Gamma(const double U) const override;
        virtual std::string Type() const override;
        virtual Option* Clone() const override;
};

} // namespace Engine
} // namespace AidanRicher

#endif // PerpAmericanCall_HPP