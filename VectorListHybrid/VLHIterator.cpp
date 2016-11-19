#include "VLHIterator.h"

bool isPowerOf2(int n)
{
	return (n > 0 && ((n & (n - 1)) == 0));
}

template<typename T>
VLHIterator<T>::VLHIterator(const VectorListHybrid<T>& vhl) : vlh(vhl), p(&vhl.data[0][0]), inBlock(0), inSet(0) {}

template<typename T>
VLHIterator<T>::VLHIterator(const VLHIterator<T>& vlht) : vlh(vlht.vlh), p(vlht.p), inBlock(vlht.inBlock), inSet(vlht.inSet) {}

template<typename T>
VLHIterator<T>& VLHIterator<T>::operator++()
{
	inSet++;
	if (isPowerOf2(inSet))
	{
		inBlock++;
		if (inBlock == vlh._blocksUsed)
			throw std::exception("Out of bounds!");
		inSet = 0;
	}
	p = &vlh.data[inBlock][inSet];
	return *this;
}

template<typename T>
VLHIterator<T> VLHIterator<T>::operator++(int)
{
	VLHIterator tmp(*this);
	operator++();
	return tmp;
}

template<typename T>
bool VLHIterator<T>::operator==(const VLHIterator & rhs)
{
	return p == rhs;
}

template<typename T>
bool VLHIterator<T>::operator!=(const VLHIterator & rhs)
{
	return p != rhs.p;
}

template<typename T>
int & VLHIterator<T>::operator*()
{
	return *p;
}

template<typename T>
VLHIterator<T>::~VLHIterator()
{
	return;
}
