#pragma once
#include<exception>

template <typename T>
class VectorListHybrid
{
private:
	T **data;

	const int DEFAULT_CAPACITY = 16;
	int _capacity;
	int _size;
	int _blocksUsed;
	int _blocksReserved;
	static bool isPowerOf2(int);
	static int determineBlock(int);
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
	class VLHIterator : public std::iterator<std::input_iterator_tag, VectorListHybrid<T> >
	{
	protected:
		int inBlock;
		int inSet;
		T *p;
		const VectorListHybrid<T>& vlh;
	public:
		VLHIterator(const VectorListHybrid<T>&);
		VLHIterator(const VLHIterator& vlht);
		VLHIterator(const VectorListHybrid<T>& vlht, int inBlock, int inSet);
		VLHIterator& operator++();
		VLHIterator operator++(int);
		bool operator==(const VLHIterator& rhs);
		bool operator!=(const VLHIterator& rhs);
		int& operator*();
		~VLHIterator();
	};
	class const_VLHIterator : public VLHIterator
	{
	public:
		using VLHIterator::VLHIterator;
		const int& operator*()
		{
			return *p;
		}
	};

	typename VLHIterator begin();
	typename VLHIterator end();

	typename const_VLHIterator cbegin() const;
	typename const_VLHIterator cend() const;
};

template<typename T>
bool VectorListHybrid<T>::isPowerOf2(int n)
{
	return (n > 0 && ((n & (n - 1)) == 0));
}

template<typename T>
int VectorListHybrid<T>::determineBlock(int index)
{
	index += 1;
	int counter = -1;
	while (index != 0)
	{
		index >>= 1;
		counter++;
	}
	return counter;
}

template <typename T>
VectorListHybrid<T>::VectorListHybrid()
{
	_capacity = DEFAULT_CAPACITY;
	data = new T*[_capacity];

	for (int i = 0; i < _capacity; i++)
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

	int counter = 0;
	_capacity = capacity;
	if (!isPowerOf2(capacity))
	{
		while (capacity != 0)
		{
			capacity >>= 1;
			counter++;
		}

		_capacity = 1 << counter;
	}


	data = new T*[_capacity];
	for (int i = 0; i < _capacity; i++)
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

template<typename T>
VectorListHybrid<T>::VectorListHybrid(const VectorListHybrid& var)
{
	_capacity = var._capacity;
	_size = var._size;
	_blocksUsed = var._blocksUsed;
	_blocksReserved = var._blocksReserved;

	data = new T*[_capacity];
	for (int i = 0; i < _blocksUsed; i++)
		data[i] = new T[1 << i];

	for (int i = 0; i < _blocksUsed; i++)
		for (int j = 0; j < (1 << i); j++)
			data[i][j] = var.data[i][j];

}

template<typename T>
VectorListHybrid<T>& VectorListHybrid<T>::operator=(const VectorListHybrid& var)
{
	VectorListHybrid(var);
	return *this;
}

template<typename T>
void VectorListHybrid<T>::push_back(const T& var)
{
	int inBlock = determineBlock(_size);
	if (inBlock == _blocksReserved)
	{
		if (_blocksReserved == _capacity)
		{
			T **tmp_data;
			tmp_data = data;
			data = new T*[2 * _capacity];

			for (int i = 0; i < _capacity; i++)
				data[i] = tmp_data[i];

			for (int i = _capacity; i < _capacity * 2; i++)
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

template<typename T>
void VectorListHybrid<T>::pop_back()
{
	if (_size == 0)
		throw std::exception("Already empty!");

	/*if (isPowerOf2(_size))
	{
	delete[] data[determineBlock(_size)];
	data[determineBlock(_size)] = nullptr;
	_blocksUsed--;
	_blocksReserved--;
	}*/

	_size--;
}

template<typename T>
T & VectorListHybrid<T>::front()
{
	if (_size == 0)
		throw std::exception("Empty!");

	return data[0][0];
}

template<typename T>
T & VectorListHybrid<T>::back()
{
	if (_size == 0)
		throw std::exception("Empty!");

	return data[_blocksUsed - 1][_size - (1 << (_blocksUsed - 1))];
}

template<typename T>
void VectorListHybrid<T>::resize(const int&newsize, const T& var = 0)
{
	if (newsize < _size)
	{
		int newBlock = determineBlock(newsize - 1);
		for (int i = newBlock + 1; i < _blocksReserved; i++)
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
			int newCapacity = 0;

			if (isPowerOf2(newBlock))
				newCapacity = newBlock;
			else
			{
				while (newBlock != 0)
				{
					newBlock >>= 1;
					newCapacity++;
				}

				newCapacity = 1 << newCapacity;
			}

			T **tmp_data;
			tmp_data = data;
			data = new T*[newCapacity];

			for (int i = 0; i < _capacity; i++)
				data[i] = tmp_data[i];

			for (int i = _capacity; i < newCapacity; i++)
				data[i] = nullptr;

			delete[] tmp_data;
			_capacity = newCapacity;
		}
		for (int i = _size; i < newsize; i++)
		{
			int inBlock = determineBlock(i);
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

template<typename T>
inline void VectorListHybrid<T>::resize(const int & newsize)
{
	resize(newsize, 0);
}

template<typename T>
void VectorListHybrid<T>::reserve(const int & maxsize)
{
	int newBlock = determineBlock(maxsize - 1);
	if (newBlock >= _capacity)
	{
		int newCapacity = 0;

		if (isPowerOf2(newBlock))
			newCapacity = newBlock;
		else
		{
			while (newBlock != 0)
			{
				newBlock >>= 1;
				newCapacity++;
			}

			newCapacity = 1 << newCapacity;
		}

		T **tmp_data;
		tmp_data = data;
		data = new T*[newCapacity];

		for (int i = 0; i < _capacity; i++)
			data[i] = tmp_data[i];

		for (int i = _capacity; i < newCapacity; i++)
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

template<typename T>
void VectorListHybrid<T>::shrink_to_fit()
{
	for (int i = _blocksUsed; i < _blocksReserved; i++)
	{
		delete[] data[i];
		data[i] = nullptr;
	}
	_blocksReserved = _blocksUsed;
}

template<typename T>
void VectorListHybrid<T>::clear()
{
	_size = 0;
	_blocksUsed = 0;
	shrink_to_fit();
}

template<typename T>
bool VectorListHybrid<T>::empty()
{
	return size == 0;
}

template<typename T>
typename VectorListHybrid<T>::VLHIterator VectorListHybrid<T>::begin()
{
	return VectorListHybrid<T>::VLHIterator(*this);
}

template<typename T>
typename VectorListHybrid<T>::VLHIterator VectorListHybrid<T>::end()
{
	if (determineBlock(_size) == _blocksUsed)
		return VectorListHybrid<T>::VLHIterator(*this, _blocksUsed, 0);
	else
		return VectorListHybrid<T>::VLHIterator(*this, _blocksUsed - 1, _size - (1 << (_blocksUsed - 1)) + 1);
}

template<typename T>
inline typename VectorListHybrid<T>::const_VLHIterator VectorListHybrid<T>::cbegin() const
{
	return VectorListHybrid<T>::const_VLHIterator(*this);
}

template<typename T>
inline typename VectorListHybrid<T>::const_VLHIterator VectorListHybrid<T>::cend() const
{
	if (VectorListHybrid<T>::determineBlock(_size) == VectorListHybrid<T>::_blocksUsed)
		return VectorListHybrid<T>::const_VLHIterator(*this, _blocksUsed, 0);
	else
		return VectorListHybrid<T>::const_VLHIterator(*this, _blocksUsed - 1, _size - (1 << (_blocksUsed - 1)) + 1);
}

template<typename T>
T& VectorListHybrid<T>::operator[](const int& var)
{
	int inBlock = determineBlock(var);
	if (var >= _size)
		throw std::exception("Index out of range");

	return data[inBlock][var - (1 << inBlock) + 1];
}

template<typename T>
int VectorListHybrid<T>::size()
{
	return _size;
}

template<typename T>
inline VectorListHybrid<T>::VLHIterator::VLHIterator(const VectorListHybrid<T>& vhl) : vlh(vhl), p(&vhl.data[0][0]), inBlock(0), inSet(0) {}

template<typename T>
inline VectorListHybrid<T>::VLHIterator::VLHIterator(const typename VectorListHybrid<T>::VLHIterator& vlht) : vlh(vlht.vlh), p(vlht.p), inBlock(vlht.inBlock), inSet(vlht.inSet) {}

template<typename T>
inline VectorListHybrid<T>::VLHIterator::VLHIterator(const VectorListHybrid<T>& vlht, int inBlock, int inSet) : vlh(vlht), p(&vlht.data[inBlock][inSet]), inBlock(inBlock), inSet(inSet) {}


template<typename T>
typename VectorListHybrid<T>::VLHIterator& VectorListHybrid<T>::VLHIterator::operator++()
{
	inSet++;
	if ((1 << inBlock) == inSet)
	{
		inBlock++;
		inSet = 0;
	}
	p = &vlh.data[inBlock][inSet];
	return *this;
}

template<typename T>
typename VectorListHybrid<T>::VLHIterator VectorListHybrid<T>::VLHIterator::operator++(int)
{
	VLHIterator tmp(*this);
	VLHIterator::operator++();
	return tmp;
}

template<typename T>
bool VectorListHybrid<T>::VLHIterator::operator==(const VLHIterator & rhs)
{
	return p == rhs;
}

template<typename T>
bool VectorListHybrid<T>::VLHIterator::operator!=(const VLHIterator & rhs)
{
	return p != rhs.p;
}

template<typename T>
int & VectorListHybrid<T>::VLHIterator::operator*()
{
	return *p;
}

template<typename T>
VectorListHybrid<T>::VLHIterator::~VLHIterator()
{

}