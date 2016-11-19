#include <iostream>
#include "VectorListHybrid.h"

int main()
{
	VectorListHybrid<int> prvi(2);
	for (int i = 0; i < 12; i++)
	{
		prvi.push_back(i*i);
	}
	for (auto it = prvi.begin(); it != prvi.end(); ++it)
	{
		std::cout << *it << std::endl;
	}


	for (VectorListHybrid<int>::const_VLHIterator it = prvi.cbegin(); it != prvi.cend(); it++)
	{
		VectorListHybrid<int>::VLHIterator iter = it;
		std::cout << *it << std::endl;
	}
	system("pause");
}