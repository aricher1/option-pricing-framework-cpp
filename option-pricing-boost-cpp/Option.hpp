#ifndef Option_HPP
#define Option_HPP

#include <string>

namespace AidanRicher {
namespace Engine {

class Option {
    public:
        virtual ~Option() = default;
        
        virtual double Price(double U) const = 0;
        virtual double Delta(double U) const = 0;
        virtual double Gamma(double U) const = 0;
        virtual std::string Type() const = 0;
        virtual Option* Clone() const = 0;
};

} // namespace Engine
} // namespace AidanRicher

#endif // Option_HPP