#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<sstream>

typedef unsigned long long ull;

using namespace std;

class BigNum {
	string num;
public:
	BigNum();
	BigNum(const char*);
	BigNum(const string&);

	BigNum operator + (const BigNum&);

	BigNum operator - (const BigNum&);

	BigNum operator * (const BigNum&);

	BigNum operator / (const BigNum&);

	bool operator > (const BigNum&);

	bool operator < (const BigNum&);

	bool operator == (const BigNum&);

	bool operator != (const BigNum&);

	bool operator >= (const BigNum&);

	bool operator <= (const BigNum&);
	
	friend ostream& operator<<(ostream& output, const BigNum&);
	friend istream& operator>>(istream& input, BigNum&);
	string getNum();
};