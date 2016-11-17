#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <thread>
#include <algorithm>

#define MAX(a,b) (((a) > (b))? (a) : (b))
#define MIN(a,b) (((a) < (b))? (a) : (b))

using namespace std;

typedef unsigned int CARRIER;
inline bool extractBit(CARRIER broj, int pos) 
{
	return bool((broj >> pos) & 1);
}


class UMultiPrec {
private:
	vector<CARRIER> bits;
	static string Zbroji(string prvi, string drugi);
	static vector<string> pot2VEC(vector<unsigned int> pot);

public:
	UMultiPrec() {
		bits.push_back(0);
	}
	UMultiPrec(CARRIER var) {
		bits.push_back(var);
	}
	UMultiPrec(const UMultiPrec& var) {
		bits = var.bits;
	}

	UMultiPrec operator+(const UMultiPrec&);
	UMultiPrec& operator+=(const UMultiPrec&);

	UMultiPrec& operator++ ();
	//Uint10& operator-- ();
	UMultiPrec operator++ (int);
	//Uint10 operator-- (int);

	bool operator==(const UMultiPrec&);
	inline bool operator==(const CARRIER&);
	bool operator!=(const UMultiPrec&);
	inline bool operator!=(const CARRIER&);
	bool operator>(const UMultiPrec&);
	inline bool operator>(const CARRIER&);
	bool operator<(const UMultiPrec&);
	inline bool operator<(const CARRIER&);
	bool operator>=(const UMultiPrec&);
	inline bool operator>=(const CARRIER&);
	bool operator<=(const UMultiPrec&);
	inline bool operator<=(const CARRIER&);
	

	string toString();
	friend ostream& operator<<(ostream&, UMultiPrec&);

};

string UMultiPrec::Zbroji(string prvi, string drugi)
{
	int ost = 0;
	string Zbroj;
	if (drugi.size() > prvi.size())
	{
		prvi.swap(drugi);
	}

	Zbroj.resize(prvi.size());

	for (unsigned int i = 0; i < prvi.size(); i++)
	{
		if (i < drugi.size())
		{
			Zbroj[i] = prvi[i] + drugi[i] + ost;
			if (Zbroj[i] >= 10)
			{
				Zbroj[i] -= 10;
				ost = 1;
			}
			else
				ost = 0;
		}
		else
		{
			Zbroj[i] = prvi[i] + ost;
			if (Zbroj[i] >= 10)
			{
				Zbroj[i] -= 10;
				ost = 1;
			}
			else
				ost = 0;
		}
	}
	if (ost == 1)
		Zbroj.push_back(1);

	return Zbroj;
}
vector<string> UMultiPrec::pot2VEC(vector<unsigned int> pot)
{
	vector<string> rezultat;
	if (pot.size() == 0)
		return rezultat;
	unsigned int max = pot[pot.size() - 1];
	unsigned int br = 0;
	short save;
	string broj;
	broj.resize(1);
	broj[0] = 1;
	rezultat.resize(pot.size());
	for (unsigned int i = 0; i <= max; i++)
	{
		if (i == pot[br])
		{
			rezultat[br] = broj;

			br++;
		}
		save = 0;
		for (int j = 0, size = broj.size(); j < size; j++)
		{
			broj[j] = 2 * broj[j] + save;
			if (broj[j] >= 10)
			{
				broj[j] -= 10;
				save = 1;
			}
			else
			{
				save = 0;
			}
		}
		if (save == 1)
			broj.push_back(1);
	}
	return rezultat;
}

UMultiPrec UMultiPrec::operator+(const UMultiPrec& var)
{
	while (var.bits.size() > bits.size())
		bits.push_back(0);

	UMultiPrec zbroj;
	zbroj.bits = bits;
	zbroj.bits.push_back(0);
	int j, size = var.bits.size();
	bool* overflow;

	overflow = new bool[size];

	for (int i = 0; i < size; i++)
		overflow[i] = false;


	vector<thread> thrs;
	int N_thrs = MIN(size / 10000 + 1, 10);
	for (int i = 0; i < N_thrs; i++)
	{
		thrs.push_back(thread([&, i] {
			for (int j = int(((float)size / N_thrs - 1)*i); j < int(((float)size / N_thrs)*(i + 1)); j++)
			{
				zbroj.bits[j] = bits[j] + var.bits[j];
				if (zbroj.bits[j] < bits[j] || zbroj.bits[j] < var.bits[j])
					overflow[j] = true;
			}
		}));
	}

	for (auto& thr : thrs)
	{
		thr.join();
	}

	for (int i = 0; i < size; i++)
	{
		if (overflow[i])
		{
			j = i + 1;
			do
			{
				zbroj.bits[j]++;
			} while (zbroj.bits[j++] == 0);
		}
	}

	if (zbroj.bits.back() == 0)
		zbroj.bits.pop_back();

	delete[] overflow;
	return zbroj;
}
UMultiPrec& UMultiPrec::operator+=(const UMultiPrec& var)
{
	while (var.bits.size() > bits.size())
		bits.push_back(0);

	bits.push_back(0);
	int j, size = var.bits.size();
	bool* overflow;

	overflow = new bool[size];

	for (int i = 0; i < size; i++)
		overflow[i] = false;


	vector<thread> thrs;
	int N_thrs = MIN(size / 10000 + 1, 10);
	for (int i = 0; i < N_thrs; i++)
	{
		thrs.push_back(thread([&, i] {
			CARRIER tmp;
			for (int j = int(((float)size / N_thrs - 1)*i); j < int(((float)size / N_thrs)*(i + 1)); j++)
			{
				tmp = bits[j] + var.bits[j];
				if (tmp < bits[j] || tmp < var.bits[j])
					overflow[j] = true;

				bits[j] = tmp;
			}
		}));
	}

	for (auto& thr : thrs)
	{
		thr.join();
	}

	for (int i = 0; i < size; i++)
	{
		if (overflow[i])
		{
			j = i + 1;
			do
			{
				bits[j]++;
			} while (bits[j++] == 0);
		}
	}

	if (bits.back() == 0)
		bits.pop_back();

	delete[] overflow;
	return *this;
}

UMultiPrec& UMultiPrec::operator++()
{
	int i = 0;
	do
	{
		bits[i]++;
	} while (bits[i++] == 0);

	return *this;
}
UMultiPrec UMultiPrec::operator++(int)
{
	UMultiPrec tmp = *this;
	operator++();

	return tmp;
}

bool UMultiPrec::operator==(const UMultiPrec& var)
{
	if (bits.size() != var.bits.size())
		return false;

	for (int i = 0, size = bits.size(); i < size; i++)
	{
		if (bits[i] != var.bits[i])
			return false;
	}

	return true;
}
bool UMultiPrec::operator==(const CARRIER& var)
{
	return bits.size() == 1 && bits[0] == var;
}

bool UMultiPrec::operator!=(const UMultiPrec& var)
{
	if (bits.size() != var.bits.size())
		return true;

	for (int i = 0, size = bits.size(); i < size; i++)
	{
		if (bits[i] != var.bits[i])
			return true;
	}

	return false;
}
bool UMultiPrec::operator!=(const CARRIER& var)
{
	return bits.size() != 1 || bits[0] != var;
}

bool UMultiPrec::operator>(const UMultiPrec& var)
{
	if (bits.size() > var.bits.size())
		return true;

	else if (bits.size() < var.bits.size())
		return false;

	else
	{
		for (int i = bits.size() - 1; i >= 0; i--)
		{
			if (bits[i] > var.bits[i])
				return true;

			if (bits[i] < var.bits[i])
				return false;
		}
	}
	return false;
}
bool UMultiPrec::operator>(const CARRIER& var)
{
	return bits.size() > 1 || bits[0] > var;
}
bool UMultiPrec::operator>=(const UMultiPrec& var)
{
	if (bits.size() > var.bits.size())
		return true;

	else if (bits.size() < var.bits.size())
		return false;

	else
	{
		for (int i = bits.size() - 1; i >= 0; i--)
		{
			if (bits[i] > var.bits[i])
				return true;

			if (bits[i] < var.bits[i])
				return false;
		}
	}
	return true;
}
bool UMultiPrec::operator>=(const CARRIER& var)
{
	return bits.size() > 1 || bits[0] >= var;
}
bool UMultiPrec::operator<(const UMultiPrec& var)
{
	if (bits.size() < var.bits.size())
		return true;

	else if (bits.size() < var.bits.size())
		return false;

	else
	{
		for (int i = bits.size() - 1; i >= 0; i--)
		{
			if (bits[i] < var.bits[i])
				return true;

			if (bits[i] > var.bits[i])
				return false;
		}
	}
	return false;
}
bool UMultiPrec::operator<(const CARRIER& var)
{
	return bits.size() == 1 && bits[0] < var;
}
bool UMultiPrec::operator<=(const UMultiPrec& var)
{
	if (bits.size() < var.bits.size())
		return true;

	else if (bits.size() < var.bits.size())
		return false;

	else
	{
		for (int i = bits.size() - 1; i >= 0; i--)
		{
			if (bits[i] < var.bits[i])
				return true;

			if (bits[i] > var.bits[i])
				return false;
		}
	}
	return true;
}
bool UMultiPrec::operator<=(const CARRIER& var)
{
	return bits.size() == 1 && bits[0] <= var;
}




string UMultiPrec::toString()
{

	vector<unsigned int> Positions;
	string izlaz = "0";
	izlaz[0] -= '0';

	for (unsigned int i = 0; i < bits.size(); i++)
		for (unsigned int j = 0; j < sizeof(CARRIER) * 8; j++)
			if (extractBit(bits[i], j))
				Positions.push_back(i*(sizeof(CARRIER) * 8) + j);


	vector<string> Values = pot2VEC(Positions);

	int N_thrs = MIN(Values.size()/1000 + 1, 15);
	vector<string> tmpIzlaz(N_thrs);
	vector<thread> thrs;
	for (auto& izl : tmpIzlaz)
	{
		izl = "0";
		izl[0] -= '0';
	}

	int vel = Values.size();

	for (int i = 0; i < N_thrs; i++)
		thrs.push_back(thread([&, i] {
		for (int j = int(((float)vel / N_thrs - 1)*i); j < int(((float)vel / N_thrs)*(i + 1)); j++)
		{
			tmpIzlaz[i] = Zbroji(tmpIzlaz[i], Values[j]);
		}

	}));

	for (auto& thr : thrs)
		thr.join();

	for (auto& tmpI : tmpIzlaz)
		izlaz = Zbroji(tmpI, izlaz);


	/*for (unsigned int i = 0, size = Values.size(); i < size; i++)
	{
		izlaz = Zbroji(izlaz, Values[i]);
	}*/

	reverse(izlaz.begin(), izlaz.end());

	for (unsigned int i = 0, size = izlaz.size(); i < size; i++)
	{
		izlaz[i] += '0';
	}

	return izlaz;
}

ostream& operator<<(ostream& os, UMultiPrec& var)
{
	if (var.bits.size() == 1)
		os << var.bits[0];
	else
		os << var.toString();
	return os;
}

UMultiPrec FibonacciL(int n)
{
	if (n < 2)
		return 1;

	UMultiPrec s[3];
	s[0] = 1;
	s[1] = 1;
	for (int i = 2; i < n; i++)
	{
		s[i % 3] = s[(i - 1) % 3] + s[(i - 2) % 3];
	}

	return s[(n - 1) % 3];
}

int main()
{
	auto start = GetTickCount();
	int n;
	cin >> n;
	auto fib = FibonacciL(n);
	cout << GetTickCount() - start << endl;
	cout << fib << endl;
	cout << GetTickCount() - start << endl;
	cout << endl;

	system("pause");

	return 0;
}