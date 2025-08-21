#ifndef ArrayException_HPP
#define ArrayException_HPP

#include <string>
#include <sstream>

namespace AidanRicher {
namespace Containers {

class ArrayException {
    public:
        ArrayException() { }                            // default constructor
        ArrayException(const ArrayException&) { }       // copy constructor
        virtual ~ArrayException() { }                   // destructor, virtual because its our base class

        // assignment operator
        ArrayException& operator = (const ArrayException&) { return *this; }

        // error message
        virtual std::string GetMessage() const = 0;
};

class SizeMismatchException : public ArrayException {   // derived SizeMismatchException class
    public:
        // override our GetMessage() function from ArrayException class
        std::string GetMessage() const override
        {
            return "Error: Array sizes do not match";
        }
};

class OutOfBoundsException : public ArrayException {    // derived OutOfBoundsException class
    private:
        int m_index;

    public: 
        // default constructor
        OutOfBoundsException() : m_index(-1) { }  
        // parameter constructor                              
        OutOfBoundsException(int index) : m_index(index) { }
        // copy constructor
        OutOfBoundsException(const OutOfBoundsException& other) : m_index(other.m_index) { }
        // destructor
        ~OutOfBoundsException() override { }

        // override our GetMessage() function from ArrayException class
        std::string GetMessage() const override 
        {
            std::ostringstream s;
            s << "Index " << m_index << " out of bounds.";
            return s.str();
        }
};

class EmptyArrayException : public ArrayException  {    // derived EmptyArrayException class
    public:
        // override our GetMessage() function from ArrayException class
        std::string GetMessage() const override
        {
            return "Error: Operation not allowed on an empty array.";
        }
};

class NegativeStepSizeException : public ArrayException {   // derived NegativeStepSizeException class
    public:
        // override our GetMessage() function from ArrayException
        std::string GetMessage() const override 
        {
            return "Error: Step size must be positive.";
        }
};

class UnexpectedInputException : public ArrayException {    // derived UnexpectedInputException class
    public:
        // ovveride the GetMessage() function from ArrayException
        std::string GetMessage() const override
        {
            std::ostringstream s;
            s << "Unexpected input entered by user.";
            return s.str();
        }
};

} // namespace Containers
} // namespace AidanRicher

#endif // ArrayException_HPP