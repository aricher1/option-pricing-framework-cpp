#ifndef Range_CPP
#define Range_CPP

#include "Range.hpp"
#include <vector>

// default constructor
template <class Type> Range<Type>::Range()
{
	Type def(0);
	lo = def;
	hi = def;
}

// parameter constructor
template <class Type> Range<Type>::Range(const Type& l, const Type& h)
{
	if (l < h)
	{
		lo = l;
		hi = h;
	}
	else
	{
		hi = l;
		lo = h;
	}
}

// copy constructor
template <class Type> Range<Type>::Range(const Range<Type>& r2)
{
	lo = r2.lo;
	hi = r2.hi;
}

// destructor
template <class Type> Range<Type>::~Range() { }

template <class Type> void Range<Type>::low(const Type& t1)
{
	lo = t1;
}

template <class Type> void Range<Type>::high(const Type& t1)
{
	hi = t1;
}

template <class Type> Type Range<Type>::low() const
{ // returns the lower bound of the range
	return lo;
}

template <class Type> Type Range<Type>::high() const
{ // returns the higher bound of the range
	return hi;
}

template <class Type> Type Range<Type>::spread() const
{ // returns the higher bound of the range
	return hi - lo;
}

template <class Type> bool Range<Type>::left(const Type& val) const
{ // check if current value is to the left of the range
	if (val < lo) 
	{
		return true;
	}
	return false;
}

template <class Type> bool Range<Type>::right(const Type& val) const
{ // check if current value is to the right of the range

	if (val > hi) 
	{
		return true;
	}
	return false;
}

template <class Type> bool Range<Type>::contains(const Type& t) const
{ // does range contain t

	if ((lo <= t) && (hi >= t))
	{
		return true;
	}
	return false;
}

// operator overloading
template <class Type> Range<Type>& Range<Type>::operator = (const Range<Type>& r2)
{ // assignment operator

	hi = r2.hi;
	lo = r2.lo;

	return *this;
}

// utility functions
template <class Type> std::vector<Type> Range<Type>::mesh(long nSteps) const
{ //  create a discrete mesh

	Type h = (hi - lo) / Type (nSteps);

	std::vector<Type>result(nSteps + 1);

	Type val = lo;

	for (long i = 0; i < nSteps + 1; i++)	
	{
		result[i] = val;
		val += h;
	}

	return result;
}

#endif //Range_CPP