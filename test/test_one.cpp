#include <cmath>

static double a;

float someOtherFunc(int i)
{
    return fabs(i * a);
}

int somedbl_func(int i)
{
	int j = i + 100.;

	const float some_second_term = j * 200.;

	return j - 12 + some_second_term;
}

int some_intentional_dbl_func(int i)
{
	const int j = i / static_cast<double>(100.);

	return j - 12;
}

int somefloat_func(int i)
{
	int j = i + 33.f;

	return j - 12;
}

int somefunc(int i)
{
	int j = i + 100;

	return j - 12;
}
