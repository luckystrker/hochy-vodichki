#include <iostream>
#include <Math.h>
#include <vector>
#include "longNum.h"
#include <cstring>
#include <cstdint>
#include <stdexcept>

using namespace std;  // longnum a; a[2];	
		
	int LongNum::operator [] (const int& x) const {
		return this->byte[x];
	}
	
	void LongNum::setByte(const int& pos, const int& value) {
		this->byte[pos] = value;
	}
////////////////////////////////////////////////////////////////	

	void LongNum::removeZeros() { 
		while(this->byte.size() > 1 && this->byte.back() == 0) this->byte.pop_back();   // ���� ���������� �������� ������ ������ � � ������� ���� ������� ����, ������� ��
	}

//////////////////////////////////////////////////////////////////////////	
		
	LongNum::LongNum() {
		
	}   // 10001111 01011100 11100101
		//
	
	LongNum::LongNum(const long long int number) {
		int size = 0;    // ������� �������� � ����� 
		long long int buff = number, buff2 = number;   // �������� ��� ���� ���������� number - buff �� ���������� ��� ���������� �������, buff2 ��� �������� ��������
		while(buff2 != 0) { 
			buff2 = buff2 / pow(this->base, size); // ����� buff2 �� ����� 256^size, ���� ��� �� ������ ������ ����
			size++; // ����������� ������ ��� ���������� �������� 
		}
		for(int i = 0; i < size; ++i) {
			uint8_t temp = buff >> (i * 8); // uint8_t - ��� ����, ��� �� ����� ���������� ������ ��������� ������ ���� ������� ����� � ������
			this->byte.push_back((int)temp);
		}
		this->removeZeros(); // ������� ������� ����
	}
	
	LongNum::LongNum(string& str) {           
            for (long long i = str.length(); i > 0; i -= 9) { // ��������� ������ � ������ ������ ����� (256-������ ������� ���������)
                    if (i < 9)
                        this->byte.push_back(atoi(str.substr(0, i).c_str())); // atoi - ����������� ������ � ������������� ������
                    else
                        this->byte.push_back(atoi(str.substr(i - 9, 9).c_str()));
            }
            this->removeZeros();
    }

	LongNum::LongNum(const LongNum& other) { // size_t - ��� ����������� ��� ������, ������������ �� ������� size(������������ ��� ����������� ������� �������)
		for(size_t i = 0; i < other.byte.size(); ++i) { // �������� ������ ������� �� ������� other � this; 
			this->byte.push_back(other.byte[i]);
		}
	}
	
	void LongNum::del() {
		vector<int>::iterator it1 = this->byte.begin(), it2 = this->byte.end(); // it1 ���������� �� ������ �������, it2 ���������� �� �����
		this->byte.erase(it1, it2); // ������� ��� �������� ������� � ������ �� �����
	}
	
///////////////////////////////
		
	std::ostream& operator<< (ostream &out, LongNum& output) {  // ostream - ����� ������, ���� �� ����� �������� ���
		if(output.byte.empty()) { // ���� ��� ������ ������, �� ������� ����
			out << 0;
		}
		for(vector<int>::iterator it = output.byte.begin(); it != output.byte.end(); ++it) out << *it << " "; // ������� ������ �������� ������� � ������� �� ���������
		return out;
	}
	
///////////////////////////////
	
	LongNum::operator long () {
		unsigned long long int temp; // �������� ����������
		for(size_t i = 0; i < this->byte.size(); ++i) {
			temp += this->byte[i] * pow(base, i); // ������� ����� ���� �������� �����
		}
		return temp;
	} 
	
	LongNum::operator string () {
		string temp;  // �������� ���������� 
		for(size_t i = 0; i < this->byte.size(); ++i) temp += to_string(this->byte[i]) + " ";
		return temp;
	} 
	
///////////////////////////////////////////////

	const LongNum LongNum::operator + () const{
		return LongNum(*this);
	}
		

	bool LongNum::operator == (const LongNum& other) const {
		if(this->byte.size() != other.byte.size()) return false; // �������� �� ���������� ��������
		else {
			for(size_t i = 0; i < this->byte.size(); ++i) {
				if(this->byte[i] == other.byte[i]) continue;   // ���������� ���������� �����
				else return false;   // ���� ������� ����� - �� ���� ������, ���� ��� - �� ����� ���������� false
			}
			return true;   // ���� ��� ������� �������� �����, ���������� true	
		}
	}
	
	bool LongNum::operator < (const LongNum& other) const {
		if((*this) == other) return false; // �������� �� ��������� ��������
		else {
			if(this->byte.size() != other.byte.size()) return this->byte.size() < other.byte.size();  // �������� �� ���������� ��������
			else {
				for(size_t i = 0; i < this->byte.size(); ++i) {
					if(this->byte[i] != other.byte[i]) return this->byte[i] < other.byte[i]; // ���������� ���������� �����
				}
			}
		}
	}
	
	bool LongNum::operator != (const LongNum& other) const {   // �������, ����������� �� ������� ���������
		return !((*this) == other);
	}
	
	bool LongNum::operator <= (const LongNum& other) const {   // �������, ����������� �� ������� ��������� � ������� "������"
		return ((*this) < other || (*this) == other);
	}
	
	bool LongNum::operator > (const LongNum& other) const { 
		return !((*this) <= other);
	}
	
	bool LongNum::operator >= (const LongNum& other) const {
		return !((*this) < other);
	}
	
/////////////////////////////////////////
	
	LongNum& LongNum::operator = (const LongNum& other) {
		this->byte.clear();   // ������� ������ � this
		for(size_t i = 0; i < other.byte.size(); ++i) {
			this->byte.push_back(other.byte[i]);   // � �������� � ���� �������� �� other
		}
		return *this;		
	}
	
	LongNum& LongNum::operator = (const long long int num) {
		LongNum a(num); // ��������� num � LongNum
		this->byte.clear();    // ������� ������ � this
		for(size_t i = 0; i < a.byte.size(); ++i) {
			this->byte.push_back(a.byte[i]);  // � �������� � ���� �������� �� a
		}
		return *this;
	} 
	// 12 123 14 17 || 225 33 2 
	const LongNum operator + (const LongNum &one, const LongNum& other) { 
		vector<int> result(0);  // �������� ������, � ������� ����� ������������ ��������� ��������
		result.resize(max(one.byte.size(), other.byte.size()) + 1);  // ������ ��� ������ (������� �� ��������)
		for (size_t i = 0; i < result.size(); ++i) { // ����������� �������� � �������
                int temp = (i < one.byte.size() ? one[i] : 0) + (i < other.byte.size() ? other[i] : 0); // ��������
                result[i] += temp % one.base;
                result[i+1] += temp / one.base;
        }
        LongNum a;
        for(size_t i = 0; i < result.size(); ++i) { // �������� �������� �� ������� result � ������ ������ a
        	a.byte.push_back(result[i]);
		}
		a.removeZeros(); // ������� ������� ����
        return a;
	}

	
	const LongNum& operator += (LongNum &one, const LongNum& other) {
		vector<int> result(0);
		result.resize(max(one.byte.size(), other.byte.size()) + 1);
		for (size_t i = 0; i < result.size(); ++i) {
                int temp = one[i] + (i< other.byte.size() ? other[i] : 0);
                result[i] += temp % one.base;
                result[i+1] += temp / one.base;
        }
        one.del(); // ������� ������ ������ � �������� � ���� �������� �� result
        for(int i = 0; i < result.size(); ++i) one.byte.push_back(result[i]);
		one.removeZeros();
        return one;
	} 

	const LongNum& operator -= (LongNum &one, const LongNum& other) {
		if(one < other) throw runtime_error("Other number is greater, substraction cannot be completed.\n"); // ��� ��� ����� ���������������, �� ���������� ������, ���� ������ ������ �������
		int buff = 0;
		for (size_t i = 0; i < other.byte.size() || buff; ++i) { // ��������� � �������
			one.byte[i] -= buff + (i < other.byte.size() ? other.byte[i] : 0);
			buff = one.byte[i] < 0;
			if (buff)  one.byte[i] += one.base;
		}
		one.removeZeros();
		return one;					
	}
	
	const LongNum operator - (const LongNum &one, const LongNum& other) {
		LongNum result(one);
		if(one < other) throw runtime_error("Other number is greater, substraction cannot be completed.\n");
		int buff = 0;
		for (size_t i = 0; i < other.byte.size() || buff; ++i) {
			result.byte[i] -= buff + (i < other.byte.size() ? other.byte[i] : 0);
			buff = result.byte[i] < 0;
			if (buff)  result.byte[i] += result.base;
		}
		result.removeZeros();
		return result;					
	}
//		123 32 4  99999
	const LongNum operator * (const LongNum &one, long long int var) {
		LongNum c(one);
	    int buff = 0;
	    for (size_t i = 0; i < c.byte.size() || buff; i++) {
	        if (i == c.byte.size()) c.byte.push_back(0);
	        buff += c[i] * var;
	        c.setByte(i, buff % c.base);
	        buff /= c.base;
	    }
	    c.removeZeros();
	    return c;
	}
		
	const LongNum operator * (long long int var, const LongNum &one) {
		LongNum c(one);
	    int carry = 0;
	    for (size_t i = 0; i < c.byte.size() || carry; i++) {
	        if (i == c.byte.size()) c.byte.push_back(0);
	        carry += c[i] * var;
	        c.setByte(i, carry % c.base);
	        carry /= c.base;
	    }
	    c.removeZeros();
	    return c;
	}
	
	const LongNum& operator * (const LongNum &one, const LongNum& other) {
		LongNum c(0), abc, buff(one);
		c.byte.resize(one.byte.size()+other.byte.size());
		for(int i = 0; i < other.byte.size(); ++i) {
			abc = buff*(other[i]*pow(other.base, i));
			c = c + abc;
		}
		c.removeZeros();
		LongNum *result = new LongNum(c);
		return *result;
		result->del();
	}
	
	const LongNum operator += (LongNum &one, const long long int var) {
		for (size_t i = 0; i < one.byte.size(); ++i) {
                if(i==0)one.byte[i] += var;
                one.byte[i+1] += one[i] / one.base;
                one.byte[i] = one[i] % one.base;
        }
		one.removeZeros();
        return one;
	}
	
	const LongNum operator + (LongNum &one, const long long int var) {
		LongNum result(one);
		for(size_t i = 0; i < result.byte.size(); ++i) {
                if(i==0)result.byte[i] += var; // ���������� ����� ������ ���� ���, � ������� �������
                result.byte[i+1] += result[i] / one.base;
                result.byte[i] = result[i] % one.base;
		}
		result.removeZeros();
		return result;
	}
	
	const LongNum operator + (const long long int var, LongNum &one) {
		LongNum result(one);
		for(size_t i = 0; i < result.byte.size(); ++i) {
                if(i==0)result.byte[i] += var;
                result.byte[i+1] += result[i] / one.base;
                result.byte[i] = result[i] % one.base;
		}
		result.removeZeros();
		return result;
	}
	
	const LongNum operator - (const LongNum &one, const long long int var) {
		LongNum comp(var), result(one);
		if(result < comp) throw runtime_error("Other number is greater, substraction cannot be completed.\n");
		result -= comp;
		return result;
	}
	const LongNum operator - (const long long int var, const LongNum &one) {
		LongNum comp(var), result(one);
		if(result > comp) throw runtime_error("Other number is greater, substraction cannot be completed.\n");
		comp -= result;
		return comp;
	}

	const LongNum operator ++ (LongNum& one) {
		one += 1;
		return one;
	}
	
	const LongNum& operator ++ (LongNum& one, int) {
		one += 1;
		return one - 1;
	}
	
	const LongNum operator -- (LongNum& one) {
		one -= 1;
		return one;
	}
	
	const LongNum& operator -- (LongNum& one, int) {
		one -= 1;
		return one + 1;
	}

		
