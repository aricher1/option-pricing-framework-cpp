#ifndef EuropeanCall_HPP
#define EuropeanCall_HPP

#include "Option.hpp"               
#include "OptionData.hpp"           
#include <string>
#include <iostream>    

namespace AidanRicher {
namespace Engine {

class EuropeanCall : public Option {
    private: 
        OptionData m_data;          // holds fixed contract params (k, r, sig, t, b)

    public:
        // default constructor
        EuropeanCall();

        // parameter constructor                      
        EuropeanCall(const OptionData& data);          

        // copy constructor
        EuropeanCall(const EuropeanCall& other);

        // destructor
        virtual ~EuropeanCall();

        // assignment operator
        EuropeanCall& operator = (const EuropeanCall& other);

        // getter for const reference to option data
        const OptionData& GetData() const;

        // core methods
        // parameter (U) passed as the underlying asset spot price
        double Price(const double U) const override;                            // return the price of the call, accept U as underlying
        double Delta(const double U) const override;                            // return the delta of the call 
        double Gamma(const double U) const override;                            // return the gamma of the call
        double PutCallParity(const double U) const;                             // return put price implied by put-call parity
        double DividedDifferenceDelta(const double U, const double h) const;    // delta approximation using divided difference method
        double DividedDifferenceGamma(const double U, const double h) const;    // gamma approximation using divided difference method

        // ostream << operator
        friend std::ostream& operator << (std::ostream& os, const EuropeanCall& source); 

        // overrides of pure virtual methods from Option base class
        virtual std::string Type() const override;
        virtual Option* Clone() const override;
};

} // namespace Engine
} // namespace AidanRicher

#endif // EuropeanCall_HPP