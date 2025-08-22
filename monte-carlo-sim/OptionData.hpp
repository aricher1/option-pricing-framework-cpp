#ifndef OptionData_HPP
#define OptionData_HPP

#include <algorithm>

struct OptionData
{ // option data + behaviour

	double K;
	double T;
	double r;
	double sig;

	double H;		// down and out barrier
	double D;		// dividend
	double betaCEV;	// elasticity factor (CEV model)
	double scale;	// scale factor in CEV model
	int type;		// 1 == call, -1 == put

	double myPayOffFunction(double S)
	{ // payoff function
		if (type == 1)
		{ // call
			return std::max(S - K, 0.0);
		}
		else
		{ // put
			return std::max(K - S, 0.0);
		}
	}
};

#endif // OptionData_HPP