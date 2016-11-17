#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <algorithm>


using namespace std;

typedef unsigned int CARRIER;

inline bool extractBit(CARRIER broj, int pos) 
{
	return bool((broj >> pos) & 1);
}

string Zbroji(string prvi, string drugi)
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
vector<string> pot2VEC(vector<int> pot)
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



class UMultiPrec {
	

public:
	vector<CARRIER> bits;
	UMultiPrec() {
		bits.push_back(0);
	}
	UMultiPrec(CARRIER var) {
		bits.push_back(var);
	}
	UMultiPrec(const UMultiPrec& var) {
		bits = var.bits;
	}


	string toString()
	{
		vector<int> Positions;
		string izlaz = "0";
		izlaz[0] -= '0';

		for (int i = 0; i < bits.size(); i++)
			for (int j = 0; j < sizeof(CARRIER) * 8; j++)
				if (extractBit(bits[i], j))
					Positions.push_back(i*(sizeof(CARRIER) * 8) + j);


		vector<string> Values = pot2VEC(Positions);

		for (unsigned int i = 0, size = Values.size(); i < size; i++)
		{
			izlaz = Zbroji(izlaz, Values[i]);
		}

		reverse(izlaz.begin(), izlaz.end());

		for (unsigned int i = 0, size = izlaz.size(); i < size; i++)
		{
			izlaz[i] += '0';
		}

		return izlaz;
	}


};

UMultiPrec add(const UMultiPrec& first, const UMultiPrec& second)
{
	UMultiPrec zbroj(first);
	zbroj.bits.resize(max(first.bits.size(), second.bits.size()) + 1);
	for (int i = 0, size = second.bits.size(); i < size; i++)
	{
		zbroj.bits[i] = first.bits[i] + second.bits[i];
		if (zbroj.bits[i] < first.bits[i] || zbroj.bits[i] < second.bits[i])
			zbroj.bits[i + 1]++;
	}
	if (zbroj.bits.back() == 0)
		zbroj.bits.pop_back();

	return zbroj;
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
		s[i % 3] = add(s[(i - 1) % 3], s[(i - 2) % 3]);
	}

	return s[(n - 1) % 3];
}

int main()
{
	auto start = GetTickCount();
	auto fib = FibonacciL(5000);
	cout << GetTickCount() - start << endl;
	cout << fib.toString() << endl;
	
	cout << endl;

	system("pause");

	return 0;
}