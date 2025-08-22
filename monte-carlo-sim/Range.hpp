#ifndef Range_HPP
#define Range_HPP

#include <vector>

template <class Type> class Range
{
	private:
		Type lo;
		Type hi;
	
	public:
		Range();									// default constructor
		Range(const Type& low, const Type& high);	// parameter constructor
		Range(const Range<Type>& ran2);				// copy constructor
		virtual ~Range();							// destructor

		void low(const Type& t1);					// sets the low value of current range
		void high(const Type& t1);					// sets the high value of current range

		Type low() const;							// lowest value in range
		Type high() const;							// highest value in the range
		Type spread() const;						// high - low value

		// boolean functions
		bool left(const Type& value) const;			// Is the value to the left of the range?
		bool right(const Type& value) const;		// Is the value to the right of the range?
		bool contains(const Type& value) const;		// Does range contain value?
		
		// utility functions
		std::vector<Type> mesh(long nSteps) const;	// create a discrete mesh

		// operator overloading
		Range<Type>& operator = (const Range<Type>& ran2);
};

#endif // Range_HPP