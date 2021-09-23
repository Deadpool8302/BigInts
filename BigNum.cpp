#include "BigNum.hpp"

BigNum::BigNum()
{
	this->num = "0";
}

BigNum::BigNum(const char* arr)
{
	string temp = arr;
	bool neg = false;
	if (temp[0] == '-') {
		neg = true;
		temp.erase(temp.begin());
	}
	while (*temp.begin() == '0' && temp.size() != 1) {
		temp.erase(temp.begin());
	}
	if (neg) this->num = '-' + temp;
	else this->num = temp;
}

BigNum::BigNum(const string& obj)
{
	string temp = obj;
	bool neg = false;
	if (temp[0] == '-') {
		neg = true;
		temp.erase(temp.begin());
	}
	while (*temp.begin() == '0' && temp.size() != 1) {
		temp.erase(temp.begin());
	}
	if (neg) this->num = '-' + temp;
	else this->num = temp;
}

BigNum BigNum::operator + (const BigNum& obj)
{
	string temp = obj.num;

	if (this->num[0] != '-' && temp[0] == '-') {
		temp.erase(temp.begin());
		return BigNum(this->num) - temp;
	}
	else if (this->num[0] == '-' && temp[0] != '-') {
		this->num.erase(this->num.begin());
		return BigNum(temp) - this->num;
	}

	bool neg = false;
	if (temp[0] == '-' && this->num[0] == '-') {
		neg = true;
		temp.erase(temp.begin());
		this->num.erase(this->num.begin());
	}

	if (temp.size() > this->num.size()) {
		int i = temp.size() - 1, j = this->num.size() - 1, carry = 0, sum = 0;
		do {
			if (j >= 0) sum = int(temp[i] - '0') + int(this->num[j] - '0') + carry;
			else sum = int(temp[i] - '0') + carry;
			temp[i] = char((sum % 10) + '0');
			carry = sum / 10;
			if (!carry && j == 0) break;
			i--;
			j--;
		} while (i >= 0);
		this->num.clear();
		this->num = temp;
		if (carry) this->num = char(carry + '0') + this->num;
	}
	else {
		int i = this->num.size() - 1, j = temp.size() - 1, carry = 0, sum = 0;
		do {
			if (j >= 0) sum = int(temp[j] - '0') + int(this->num[i] - '0') + carry;
			else sum = int(this->num[i] - '0') + carry;
			this->num[i] = char((sum % 10) + '0');
			carry = sum / 10;
			if (!carry && j == 0) break;
			i--;
			j--;
		} while (i >= 0);
		if (carry) this->num = char(carry + '0') + this->num;
	}
	while (*this->num.begin() == '0' && this->num.size() != 1) {
		this->num.erase(this->num.begin());
	}
	if (neg) this->num = '-' + this->num;
	return *this;
}


BigNum BigNum::operator-(const BigNum& obj)
{
	string temp = obj.num;

	if (this->num[0] == '-' && temp[0] != '-') return *this + string('-' + temp);
	else if (this->num[0] != '-' && temp[0] == '-') {
		temp.erase(temp.begin());
		return *this + temp;
	}
	else if (this->num[0] == '-' && temp[0] == '-') {
		temp.erase(temp.begin());
		return *this + temp;
	}

	if (*this >= obj) {
		int i = this->num.size() - 1, j = temp.size() - 1, dif = 0;
		do {
			if (this->num[i] == '/') {
				this->num[i] = char((int(this->num[i] - '0') + 10) + '0');
				--this->num[i - 1];
			}
			if (j >= 0) {
				dif = int(this->num[i] - '0') - int(temp[j] - '0');
				if (dif < 0) {
					dif += 10;
					--this->num[i - 1];
				}
			}
			else if (i == 0 || this->num[i - 1] != '/')break;
			this->num[i] = char(dif + '0');
			i--;
			j--;
		} while (i >= 0);
		while (*this->num.begin() == '0' && this->num.size() != 1) {
			this->num.erase(this->num.begin());
		}
	}
	else {
		int j = this->num.size() - 1, i = temp.size() - 1, dif = 0;
		do {
			if (temp[i] == '/') {
				temp[i] = char((int(temp[i] - '0') + 10) + '0');
				--temp[i - 1];
			}
			if (j >= 0) {
				dif = int(temp[i] - '0') - int(this->num[j] - '0');
				if (dif < 0) {
					dif += 10;
					--temp[i - 1];
				}
			}
			else if ( i == 0 || temp[i - 1] != '/')break;
			temp[i] = char(dif + '0');
			i--;
			j--;
		} while (i >= 0);
		while (*temp.begin() == '0' && temp.size() != 1) {
			temp.erase(temp.begin());
		}
		this->num = '-' + temp;
	}
	return *this;
}

BigNum BigNum::operator*(const BigNum& obj)
{
	string temp = obj.num;
	bool neg = false, neg2 = false;
	if (temp[0] == '-') {
		neg = true;
		temp.erase(temp.begin());
	}
	if (this->num[0] == '-') {
		neg2 = true;
		this->num.erase(this->num.begin());
	}
	if (temp == "0" || this->num == "0") return { "0" };

	BigNum test, fin;
	string ans , mul;
	//18 chars
	//1000000000000000000
	//9223372036854775807
	vector<ull> arr;
	if (temp.size() > this->num.size()) {
		int j = this->num.size() - 1, i = temp.size() - 1, carry = 0;
		ull product = 0;
		int j_ = j;

		reverse(temp.begin(), temp.end());
		stringstream str;
		for (int i = 0; i < temp.size(); i += 18) {
			str.clear();
			str << temp.substr(i, 18);
			string x1;
			str >> x1;
			reverse(x1.begin(), x1.end());
			str.clear();
			str << x1;
			ull x;
			str >> x;
			arr.push_back(x);
		}
		reverse(arr.begin(), arr.end());
		str.clear();

		while (j_ >= 0) {
			carry = 0;
			mul = "";
			fin = "0";
			for (int k = arr.size() - 1; k >= 0; k--) {
				product = int(this->num[j_] - '0') * arr[k];
				str << product;
				str >> ans;
				ans += mul;
				fin = fin + ans;
				mul += "000000000000000000";
				str.clear();
			}
			int z = j - j_;
			while (z--) {
				fin.num += '0';
			}
			test = test + fin;
			j_--;
			ans.clear();
		}
	}
	//-------------------------------------------------------------------------------------
	else {
		int i = this->num.size() - 1, j = temp.size() - 1, carry = 0;
		ull product = 0;
		int j_ = j;

		reverse(this->num.begin(), this->num.end());
		stringstream str;
		for (int i = 0; i < this->num.size(); i += 18) {
			str.clear();
			str << this->num.substr(i, 18);
			string x1;
			str >> x1;
			reverse(x1.begin(), x1.end());
			str.clear();
			str << x1;
			ull x;
			str >> x;
			arr.push_back(x);
		}
		reverse(arr.begin(), arr.end());
		str.clear();

		while (j_ >= 0) {
			carry = 0;
			mul = "";
			fin = "0";
			for (int k = arr.size() - 1; k >= 0; k--) {
				product = int(temp[j_] - '0') * arr[k];
				str << product;
				str >> ans;
				ans += mul;
				fin = fin + ans;
				mul += "000000000000000000";
				str.clear();
			}
			int z = j - j_;
			while (z--) {
				fin.num += '0';
			}
			test = test + fin;
			j_--;
			ans.clear();
		}
	}
	if (neg && neg2) return test;
	else if ((neg || neg2) && test.num != "0") test.num = '-' + test.num;
	return test;
}

BigNum BigNum::operator/(const BigNum& obj)
{
	if (*this == obj) return "1";

	string temp = obj.num;
	BigNum temp1;
	string ans;
	bool neg = false, neg2 = false;
	if (temp[0] == '-') {
		neg = true;
		temp.erase(temp.begin());
	}
	if (this->num[0] == '-') {
		neg2 = true;
		this->num.erase(this->num.begin());
	}
	if (*this < temp) return "0";
	
	int i = temp.size() - 1;
	BigNum rem = this->num.substr(0, i+1);
	for (; i < this->num.size(); i++) {
		temp1.num = temp;
		if (rem < temp1) {
			ans += '0';
			goto skip;
		}
		for (int k = 2; k <= 10; k++) {
			temp1 = temp1 + temp;
			if (temp1 >= rem) {
				if (temp1 != rem) {
					temp1 = temp1 - temp;
					ans += string({ char((--k) + '0') });
				}
				else ans += string({ char(k + '0') });
				rem = rem - temp1;
				break;
			}
		}
	skip: {
		if (i < this->num.size()) rem.num += this->num[i + 1];
		while (*rem.num.begin() == '0' && rem.num.size() != 1) {
			rem.num.erase(rem.num.begin());
		}
		}
	}
	if (neg && neg2 || (!neg && !neg2)) return{ ans };
	return { '-' + ans };
}

bool BigNum::operator>(const BigNum& obj)
{
	string temp = obj.num;
	bool neg = false;
	if (temp[0] == '-')	neg = true;

	if (this->num[0] == '-' && neg == false) return false;
	if (this->num[0] != '-' && neg == true) return true;
	if (this->num == temp) return false;
	if (neg) {
		temp.erase(temp.begin());
		this->num.erase(this->num.begin());
		if (this->num.size() > temp.size()) {
			this->num = '-' + this->num;
			return false;
		}
		if (temp.size() > this->num.size()) {
			this->num = '-' + this->num;
			return true;
		}
		if (temp > this->num) {
			this->num = '-' + this->num;
			return true;
		}
		this->num = "-" + this->num;
		return false;
	}
	if (this->num.size() > temp.size()) return true;
	if (temp.size() > this->num.size()) return false;
	if (this->num > temp) return true;
	return false;
}

bool BigNum::operator<(const BigNum& obj)
{
	if (this->num == obj.num) return false;
	if (*this > obj.num) return false;
	return true;
}

bool BigNum::operator==(const BigNum& obj)
{
	if (this->num == obj.num) return true;
	return false;
}

bool BigNum::operator!=(const BigNum& obj)
{
	if (this->num != obj.num) return true;
	return false;
}

bool BigNum::operator>=(const BigNum& obj)
{
	if (*this > obj || *this == obj)return true;
	return false;
}

bool BigNum::operator<=(const BigNum& obj)
{
	if (*this < obj || *this == obj) return true;
	return false;
}

string BigNum::getNum()
{
	return num;
}

ostream& operator<<(ostream& output, const BigNum& obj)
{
	output << obj.num;
	return output;
}

istream& operator>>(istream& input, BigNum& obj)
{
	input >> obj.num;
	return input;
}
