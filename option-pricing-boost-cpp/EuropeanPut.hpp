#ifndef EuropeanPut_HPP
#define EuropeanPut_HPP

#include "Option.hpp"           
#include "OptionData.hpp"      
#include <string>
#include <iostream>

namespace AidanRicher {
namespace Engine {

class EuropeanPut : public Option {
    private:
        OptionData m_data;                          // holds fixed contract params (k, r, sig, t, b)

    public:
        EuropeanPut();                              // default constructor
        EuropeanPut(const OptionData& data);        // parameter constructor
        EuropeanPut(const EuropeanPut& other);      // copy constructor
        virtual ~EuropeanPut();                     // destructor (virtual, no dynamic memory)

        // assignment operator
        EuropeanPut& operator = (const EuropeanPut& other);
        
        // getter for const reference to option data
        const OptionData& GetData() const;

        // core methods
        // parameter (U) passed as the underlying asset spot price
        double Price(const double U) const override;                            // return the price of the put, U as underlying
        double Delta(const double U) const override;                            // return the delta of the put
        double Gamma(const double U) const override;                            // return the gamma of the put
        double PutCallParity(const double U) const;                             // return call price implied by put-call parity
        double DividedDifferenceDelta(const double U, const double h) const;    // approximate delta using divided difference method
        double DividedDifferenceGamma(const double U, const double h) const;    // approximate gamma using divided difference method

        // ostream << operator
        friend std::ostream& operator << (std::ostream& os, const EuropeanPut& source);

        // overrides of pure virtual methods from Option base class
        virtual std::string Type() const override;
        virtual Option* Clone() const override;
};

} // namespace Engine
} // namespace AidanRicher

#endif // EuropeanPut_HPP