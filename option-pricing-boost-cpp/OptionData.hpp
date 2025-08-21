#ifndef OptionData_HPP
#define OptionData_HPP

namespace AidanRicher {
namespace Engine {

class OptionData {
    private:
        double m_k;             // strike price
        double m_r;             // interest rate
        double m_sig;           // volatility
        double m_t;             // time to expiry
        double m_b;             // cost of carry
    
    public:
        // constructors and destructor
        OptionData();                                                       // default constructor
        OptionData(double k, double r, double sig, double t, double b);     // parameter constructor
        OptionData(const OptionData& other);                                // copy constructor
        ~OptionData();                                                      // destructor

        // getter functions    
        double K() const;
        double R() const;
        double Sig() const;
        double T() const;
        double B() const;

        // setter functions
        void K(double k);
        void R(double r);
        void Sig(double sig);
        void T(double t);
        void B(double b);

        // assignment operator
        OptionData& operator = (const OptionData& other);
};

} // namespace Engine
} // namespace AidanRicher

#endif // OptionData_HPP