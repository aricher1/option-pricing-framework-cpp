#include "OptionData.hpp" 
#include "NormalGenerator.hpp"
#include "Range.cpp"
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

template <class T> void print(const std::vector<T>& myList)
{  // generic print function for vectors
	
	std::cout << std::endl << "Size of vector is " << myList.size() << "\n[";

	typename std::vector<T>::const_iterator i;
	for (i = myList.begin(); i != myList.end(); ++i)
	{
		std::cout << *i << ",";
	}

	std::cout << "]\n";
}

namespace SDEDefinition
{ // defines drift + diffusion + data

	OptionData* data;				
	// The data for the option MC

	double drift(double t, double X)
	{ // drift term
		return (data->r)*X; // r - D
	}

	double diffusion(double t, double X)
	{ // diffusion term
		double betaCEV = 1.0;
		return data->sig * pow(X, betaCEV);
	}

	double diffusionDerivative(double t, double X)
	{ // diffusion term, needed for the Milstein method
		double betaCEV = 1.0;
		return 0.5 * (data->sig) * (betaCEV) * pow(X, 2.0 * betaCEV - 1.0);
	}

} // namespace SDEDefinition

double StandardDeviation(const std::vector<double>& NSIM, double r, double T)
{
    if (NSIM.size() < 2) {
		return 0.0;
	}

    double sum = 0.0;
	double sum_squared = 0.0;

    for (std::vector<double>::const_iterator itr = NSIM.begin(); itr != NSIM.end(); ++itr)
    {
        sum += *itr;
        sum_squared += (*itr) * (*itr);
    }

    double mean_squared = (sum * sum) / static_cast<double>(NSIM.size());
    double variance = (sum_squared - mean_squared) / static_cast<double>(NSIM.size() - 1);

    return sqrt(variance) * exp(-r * T);
}

double StandardError(const std::vector<double>& NSIM, double r, double T)
{
    if (NSIM.size() == 0) {
		return 0.0;
	}

    double sd = StandardDeviation(NSIM, r, T);

    return sd / sqrt(static_cast<double>(NSIM.size()));
}

int main()
{
	std::cout << "1 factor MC with explicit Euler\n";
	OptionData myOption;
	myOption.K = 100.0;
	myOption.T = 1.0;
	myOption.r = 0.00;
	myOption.sig = 0.2;
	myOption.type = 1;	// Put -1, Call +1
	double S_0 = 100;
	
	/*
	Batch 1: T = 0.25, K = 65, sig = 0.30, r = 0.08, S = 60 (then C = 2.13337, P = 5.84628).
	Batch 2: T = 1.0, K = 100, sig = 0.2, r = 0.0, S = 100 (then C = 7.96557, P = 7.96557).
	Batch 4: T = 30.0, K = 100.0, sig = 0.30, r = 0.08, S = 100.0 (C = 92.17570, P = 1.24750).
	*/
	
	long N = 100;
	std::cout << "Number of subintervals in time: ";
	std::cin >> N;

	// create the basic SDE (Context class)
	Range<double> range (0.0, myOption.T);
	double VOld = S_0;
	double VNew;

	std::vector<double> x = range.mesh(N);
	
	// V2 mediator stuff
	long NSim = 50000;
	std::cout << "Number of simulations: ";
	std::cin >> NSim;

	double k = myOption.T / double (N);
	double sqrk = sqrt(k);

	// normal random number
	double dW;
	double price = 0.0;	// option price
	std::vector<double> payoffs;

	// normalGenerator is a base class
	NormalGenerator* myNormal = new BoostNormal();

	using namespace SDEDefinition;
	SDEDefinition::data = &myOption;

	std::vector<double> res;
	int coun = 0; // number of times S hits origin

	for (long i = 1; i <= NSim; ++i)
	{ // calculate a path at each iteration
			
		if ((i/10000) * 10000 == i)
		{ // give status after each 1000th iteration
			std::cout << i << std::endl;
		}

		VOld = S_0;
		for (unsigned long index = 1; index < x.size(); ++index)
		{
			// create a random number
			dW = myNormal->getNormal();
				
			// the FDM (in this case explicit Euler)
			VNew = VOld  + (k * drift(x[index-1], VOld)) + (sqrk * diffusion(x[index-1], VOld) * dW);
			VOld = VNew;

			// spurious values
			if (VNew <= 0.0) 
			{
				coun++;
			}
		}
			
		double tmp = myOption.myPayOffFunction(VNew);
		payoffs.push_back(tmp);
		price += (tmp)/double(NSim);
	}
	
	// finally, discounting the average price
	price *= exp(-myOption.r * myOption.T);
	double sd = StandardDeviation(payoffs, myOption.r, myOption.T);
	double se = StandardError(payoffs, myOption.r, myOption.T);

	// cleanup; V2 use scoped pointer
	delete myNormal;

	// output MC results
	std::cout << "Price, after discounting: " << price << std::endl;
	std::cout << "Standard Deviation: " << sd << std::endl;
	std::cout << "Standard Error: " << se << std::endl;
	std::cout << "Number of times origin is hit: " << coun << std::endl;

	return 0;
}