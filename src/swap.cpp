#include "stdafx.h"
#include "swap.h"

Swap::Swap()
{
	error = false;
	A = B = IPoint(0, 0);
	changeType = -1;
}

Swap::~Swap()
{

}

void Swap::toFile(std::ofstream& out)
{
	out << A.x << " " << A.y << " "
		<< B.x << " " << B.y << " " 
		<< changeType << " ";
}

std::string Swap::toString()
{
	return utils::lexical_cast<int>(A.x) + " " + utils::lexical_cast<int>(A.y) + " "
		+ utils::lexical_cast<int>(B.x) + " " + utils::lexical_cast<int>(B.y) + " " 
		+ utils::lexical_cast<int>(changeType) + " ";
}

void Swap::fromFile(std::ifstream& in)
{
	in >> A.x;
	in >> A.y;
	in >> B.x;
	in >> B.y;
	in >> changeType;
	error = false;
}

Swap::Swap(std::ifstream& in)
{
	fromFile(in);
}

Swap Swap::getZeroSwap()
{
	Swap zero;
	return zero;
}

Swap Swap::gerErrorSwap()
{
	Swap error = getZeroSwap();
	error.error = true;
	return error;
}
