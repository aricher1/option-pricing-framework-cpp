#include "GlobalEngine.hpp"
#include "EuropeanCall.hpp"
#include "EuropeanPut.hpp"
#include <iostream>

using namespace std;

namespace AidanRicher {
namespace Engine {

void CheckPutCallParity(const EuropeanCall& call, const EuropeanPut& put, double U)
{ // verify if parity holds between a given put and call option at underlying U
    const OptionData& c = call.GetData();
    const OptionData& p = put.GetData();

    if (c.K() == p.K() && c.R() == p.R() && c.Sig() == p.Sig() && c.T() == p.T() && c.B() == p.B())
    { // parameters are equal, can check for parity
        double callParity = put.PutCallParity(U);       // implied call price based off parity
        double putParity = call.PutCallParity(U);       // implied put price based off parity
        double callMkt = call.Price(U);                 // market call price
        double putMkt = put.Price(U);                   // market put price

        if (round(1000 * callParity) == round(1000 * call.Price(U)) && round(1000 * putParity) == round(1000 * put.Price(U)))
        { // parity holds
            cout << "Put-Call Parity holds: No-arbitrage.\n"
                 << "Call price: " << callMkt << "\n"
                 << "Put price: " << putMkt << endl;
        }
        else
        { // parity is violated
            cout << "Put-Call Parity is violated: Arbitrage opportunity detected.\n"
                 << "Call price: " << callMkt << "\n"
                 << "Put price: " << putMkt << endl;
        }
    }
    else 
    { // parameters are not equal, output error
        cout << "The option parameters are not equivalent." << endl;
    }
}

} // namespace Engine
} // namespace AidanRicher