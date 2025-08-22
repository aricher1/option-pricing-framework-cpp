#ifndef NormalGenerator_HPP
#define NormalGenerator_HPP

#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

class NormalGenerator
{
	public:
		virtual double getNormal() const = 0;

		virtual ~NormalGenerator() { }
};

class BoostNormal : public NormalGenerator
{
	private:
		boost::lagged_fibonacci607 rng;
		boost::normal_distribution<> nor;
		boost::variate_generator<boost::lagged_fibonacci607&, boost::normal_distribution<> >* myRandom;
	
	public:
		BoostNormal();

		double getNormal() const;

		~BoostNormal();
};

#endif // NormalGenerator_HPP