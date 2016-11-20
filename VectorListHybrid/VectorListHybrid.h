#pragma once
#include <stdexcept>

template <typename T>
class VectorListHybrid
{
private:
	T **data;

	const int DEFAULT_CAPACITY = 16;
	size_t _capacity;
	size_t _size;
	size_t _blocksUsed;
	size_t _blocksReserved;
	static size_t toPowerOf2(size_t);
	static size_t determineBlock(size_t);

public:
	VectorListHybrid();
	VectorListHybrid(int);
	~VectorListHybrid();
	VectorListHybrid(const VectorListHybrid&);
	VectorListHybrid& operator=(const VectorListHybrid&);


	void push_back(const T&);
	void pop_back();
	T& front();
	T& back();

	void resize(const int& newsize, const T& var);
	void resize(const int& newsize);
	void reserve(const int&);
	void shrink_to_fit();
	void clear();
	bool empty();

	T& operator[](const int&);
	int size();
	template <typename U>
	class VLHIterator : public std::iterator<std::input_iterator_tag, VectorListHybrid<T> >
	{
	private:
		const VectorListHybrid<T>& vlh;
		U *p;
		size_t inBlock;
		size_t inSet;
	public:
		VLHIterator(const VectorListHybrid<T>&);
		VLHIterator(const VLHIterator& vlht);
		VLHIterator(const VectorListHybrid<T>& vlht, size_t inBlock, size_t inSet);
		VLHIterator& operator++();
		VLHIterator operator++(int);
		bool operator==(const VLHIterator& rhs);
		bool operator!=(const VLHIterator& rhs);
		U& operator*();
		~VLHIterator();
	};
	typedef VLHIterator<T> iterator;
	typedef VLHIterator<const T> const_iterator;

	iterator begin();
	const_iterator cbegin() const;
	iterator end();
	const_iterator cend() const;

};


template<typename T>
inline size_t VectorListHybrid<T>::toPowerOf2(size_t n)
{
#if defined(_MSC_VER)
	if (__popcnt(n) == 1)
		return n;

	unsigned long power;
	_BitScanReverse(&power, n);
	return 1U << (power + 1);
#elif defined(__GNUG__)
	return (__builtin_popcount(n) == 1) ? n : 1U << (8 * sizeof(size_t) - __builtin_clz(n));
#else

	index += 1;
	int counter = -1;
	while (index != 0)
	{
		index >>= 1;
		counter++;
}
	return counter;
#endif 
	return size_t();
}

template <typename T>
inline size_t VectorListHybrid<T>::determineBlock(size_t index)
{
#if defined(_MSC_VER)
	unsigned long block;
	_BitScanReverse(&block, index + 1);
	return block;
#elif defined(__GNUG__)
	return (sizeof(index) * 8 - __builtin_clz(index + 1) - 1);
#else

	index += 1;
	int counter = -1;
	while (index != 0)
	{
		index >>= 1;
		counter++;
	}
	return counter;
#endif 
}

template <typename T>
VectorListHybrid<T>::VectorListHybrid()
{
	_capacity = DEFAULT_CAPACITY;
	data = new T*[_capacity];

	for (size_t i = 0; i < _capacity; i++)
		data[i] = nullptr;

	_size = 0;
	_blocksUsed = 0;
	_blocksReserved = 0;
}

template <typename T>
VectorListHybrid<T>::VectorListHybrid(int capacity)
{
	_size = 0;
	_blocksUsed = 0;
	_blocksReserved = 0;


	_capacity = toPowerOf2(capacity);


	data = new T*[_capacity];
	for (size_t i = 0; i < _capacity; i++)
		data[i] = nullptr;
}

template <typename T>
VectorListHybrid<T>::~VectorListHybrid()
{
	for (size_t i = 0; i < _blocksReserved; i++)
	{
		delete[] data[i];
	}

	delete[] data;
}

template <typename T>
VectorListHybrid<T>::VectorListHybrid(const VectorListHybrid& var)
{
	_capacity = var._capacity;
	_size = var._size;
	_blocksUsed = var._blocksUsed;
	_blocksReserved = var._blocksReserved;

	data = new T*[_capacity];
	for (size_t i = 0; i < _blocksUsed; i++)
		data[i] = new T[1U << i];

	for (size_t i = 0; i < _blocksUsed; i++)
		for (size_t j = 0; j < (1U << i); j++)
			data[i][j] = var.data[i][j];

}

template <typename T>
VectorListHybrid<T>& VectorListHybrid<T>::operator=(const VectorListHybrid& var)
{
	_capacity = var._capacity;
	_size = var._size;
	_blocksUsed = var._blocksUsed;
	_blocksReserved = var._blocksReserved;

	data = new T*[_capacity];
	for (size_t i = 0; i < _blocksUsed; i++)
		data[i] = new T[1 << i];

	for (size_t i = 0; i < _blocksUsed; i++)
		for (int j = 0; j < (1 << i); j++)
			data[i][j] = var.data[i][j];
	return *this;
}

template <typename T>
void VectorListHybrid<T>::push_back(const T& var)
{
	size_t inBlock = determineBlock(_size);
	if (inBlock == _blocksReserved)
	{
		if (_blocksReserved == _capacity)
		{
			T **tmp_data;
			tmp_data = data;
			data = new T*[2 * _capacity];

			for (size_t i = 0; i < _capacity; i++)
				data[i] = tmp_data[i];

			for (size_t i = _capacity; i < _capacity * 2; i++)
				data[i] = nullptr;

			delete[] tmp_data;
			_capacity *= 2;
		}
		data[_blocksReserved] = new T[1 << _blocksReserved];
		_blocksUsed++;
		_blocksReserved++;
	}

	if (inBlock == _blocksUsed)
		_blocksUsed++;

	_size++;
	//std::cout << "Size: " << _size << " ->[" << inBlock << "][" << (_size - (1 << inBlock)) << "]" << std::endl;
	//std::cout << "Pushback Blocks used: " << _blocksUsed << "\tBlocks reserved: " << _blocksReserved << std::endl << std::endl;
	data[inBlock][_size - (1 << inBlock)] = var;

}

template <typename T>
inline void VectorListHybrid<T>::pop_back()
{
	if (_size == 0)
		throw std::length_error("Empty!");
	_size--;
}

template <typename T>
inline T& VectorListHybrid<T>::front()
{
	if (_size == 0)
		throw std::length_error("Empty!");

	return data[0][0];
}

template <typename T>
inline T& VectorListHybrid<T>::back()
{
	if (_size == 0)
		throw std::length_error("Empty!");

	return data[_blocksUsed - 1][_size - (1 << (_blocksUsed - 1))];
}

template <typename T>
void VectorListHybrid<T>::resize(const int&newsize, const T& var)
{
	if (newsize < _size)
	{
		int newBlock = determineBlock(newsize - 1);
		for (size_t i = newBlock + 1; i < _blocksReserved; i++)
		{
			delete[] data[i];
			data[i] = nullptr;
		}
		_blocksUsed = newBlock + 1;
		_blocksReserved = newBlock + 1;
		_size = newsize;
		return;
	}

	if (newsize > _size)
	{
		int newBlock = determineBlock(newsize - 1);
		if (newBlock >= _capacity)
		{
			int newCapacity = toPowerOf2(newBlock);

			T **tmp_data;
			tmp_data = data;
			data = new T*[newCapacity];

			for (size_t i = 0; i < _capacity; i++)
				data[i] = tmp_data[i];

			for (size_t i = _capacity; i < newCapacity; i++)
				data[i] = nullptr;

			delete[] tmp_data;
			_capacity = newCapacity;
		}
		for (size_t i = _size; i < newsize; i++)
		{
			size_t inBlock = determineBlock(i);
			if (inBlock == _blocksReserved)
			{
				data[_blocksReserved] = new T[1 << _blocksReserved];
				_blocksReserved++;
				_blocksUsed++;
			}
			if (inBlock == _blocksUsed)
				_blocksUsed++;

			data[inBlock][i - (1 << inBlock) + 1] = var;
		}
		_size = newsize;
		return;
	}
}

template <typename T>
inline void VectorListHybrid<T>::resize(const int & newsize)
{
	resize(newsize, 0);
}

template <typename T>
void VectorListHybrid<T>::reserve(const int & maxsize)
{
	int newBlock = determineBlock(maxsize - 1);
	if (newBlock >= _capacity)
	{
		int newCapacity = toPowerOf2(newBlock);

		T **tmp_data;
		tmp_data = data;
		data = new T*[newCapacity];

		for (size_t i = 0; i < _capacity; i++)
			data[i] = tmp_data[i];

		for (size_t i = _capacity; i < newCapacity; i++)
			data[i] = nullptr;

		delete[] tmp_data;
		_capacity = newCapacity;
	}
	while (determineBlock(maxsize - 1) >= _blocksReserved)
	{
		data[_blocksReserved] = new T[1 << _blocksReserved];
		_blocksReserved++;
	}

}

template <typename T>
void VectorListHybrid<T>::shrink_to_fit()
{
	for (size_t i = _blocksUsed; i < _blocksReserved; i++)
	{
		delete[] data[i];
		data[i] = nullptr;
	}
	_blocksReserved = _blocksUsed;
}

template <typename T>
inline void VectorListHybrid<T>::clear()
{
	_size = 0;
	_blocksUsed = 0;
	shrink_to_fit();
}

template <typename T>
inline bool VectorListHybrid<T>::empty()
{
	return size == 0;
}

template <typename T>
inline typename VectorListHybrid<T>::iterator VectorListHybrid<T>::begin()
{
	return VectorListHybrid<T>::iterator(*this);
}

template <typename T>
inline typename VectorListHybrid<T>::iterator VectorListHybrid<T>::end()
{
	if (determineBlock(_size) == _blocksUsed)
		return VectorListHybrid<T>::iterator(*this, _blocksUsed, 0);
	else
		return VectorListHybrid<T>::iterator(*this, _blocksUsed - 1, _size - (1 << (_blocksUsed - 1)) + 1);
}

template <typename T>
inline typename VectorListHybrid<T>::const_iterator VectorListHybrid<T>::cbegin() const
{
	return VectorListHybrid<T>::const_iterator(*this);
}

template <typename T>
inline typename VectorListHybrid<T>::const_iterator VectorListHybrid<T>::cend() const
{
	if (VectorListHybrid<T>::determineBlock(_size) == VectorListHybrid<T>::_blocksUsed)
		return VectorListHybrid<T>::const_iterator(*this, _blocksUsed, 0);
	else
		return VectorListHybrid<T>::const_iterator(*this, _blocksUsed - 1, _size - (1 << (_blocksUsed - 1)) + 1);
}

template <typename T>
T& VectorListHybrid<T>::operator[](const int& var)
{
	size_t inBlock = determineBlock(var);
	if (var >= _size || var < 0)
		throw std::out_of_range("VLH subscript out of range.");
	return data[inBlock][var - (1 << inBlock) + 1];
}

template <typename T>
inline int VectorListHybrid<T>::size()
{
	return _size;
}

template <typename T>
template <typename U>
inline VectorListHybrid<T>::VLHIterator<U>::VLHIterator(const VectorListHybrid<T>& vhl) : vlh(vhl), p(&vhl.data[0][0]), inBlock(0), inSet(0) {}

template <typename T>
template <typename U>
inline VectorListHybrid<T>::VLHIterator<U>::VLHIterator(const  VLHIterator<U>& vlht) : vlh(vlht.vlh), p(vlht.p), inBlock(vlht.inBlock), inSet(vlht.inSet) {}

template <typename T>
template <typename U>
inline VectorListHybrid<T>::VLHIterator<U>::VLHIterator(const VectorListHybrid<T>& vlht, size_t inBlock, size_t inSet) : vlh(vlht), p(&vlht.data[inBlock][inSet]), inBlock(inBlock), inSet(inSet) {}


template <typename T>
template <typename U>
#if defined(_MSC_VER)
typename VectorListHybrid<T>::VLHIterator<U>& VectorListHybrid<T>::VLHIterator<U>::operator++()
#else
typename VectorListHybrid<T>::template VLHIterator<U>& VectorListHybrid<T>::VLHIterator<U>::operator++()
#endif
{
	inSet++;
	if ((1U << inBlock) == inSet)
	{
		inBlock++;
		inSet = 0;
	}
	p = &vlh.data[inBlock][inSet];
	return *this;
}


template <typename T>
template <typename U>
#if defined(_MSC_VER)
typename VectorListHybrid<T>::VLHIterator<U> VectorListHybrid<T>::VLHIterator<U>::operator++(int)
#else
typename VectorListHybrid<T>::template VLHIterator<U> VectorListHybrid<T>::VLHIterator<U>::operator++(int)
#endif
{
	VLHIterator<U> tmp(*this);
	VLHIterator::operator++();
	return tmp;
}

template <typename T>
template <typename U>
bool VectorListHybrid<T>::VLHIterator<U>::operator==(const VLHIterator& rhs)
{
	return p == rhs;
}

template <typename T>
template <typename U>
bool VectorListHybrid<T>::VLHIterator<U>::operator!=(const VLHIterator & rhs)
{
	return p != rhs.p;
}

template <typename T>
template <typename U>
U& VectorListHybrid<T>::VLHIterator<U>::operator*()
{
	return *p;
}

template <typename T>
template <typename U>
VectorListHybrid<T>::VLHIterator<U>::~VLHIterator()
{

}