#include <vector>

class LongNum{  
	
	private:  
		std::vector<int> byte;  // ���� ����� � �������� �������������
		
	public:    
		const int base = 256;  // ��������� ������� ��������� 
		
		int operator [] (const int& x) const;   // �������, ������������ ������� ��� ���������� ������� 
		void setByte(const int& pos, const int& value);   // �������, �������� ������ ��� ���������� ������� 
		
		void removeZeros();    // �������, ����������� ������� ���� � �������
		
		LongNum();   // ����������� �� ���������
		LongNum(const long long int number);   // �����������, � ������� �� �������� ����� long long int 
		LongNum(std::string& str);    // �����������, � ������� �� �������� ������
		LongNum(const LongNum& other);    // �����������, � ������� �� �������� ������ ������ ������ LongNum
		void del(); // ����������
		
		friend std::ostream& operator<< (std::ostream &out, LongNum& output); // ������������� �������� ������, ����� ���� �� ������� �������� ������� � �������
		
		operator long(); // ������� ������� � ����� ���� long long
		operator std::string();  // ������� ������� � ������ �����
		
		const LongNum operator + () const;  // ������������ ������� �������� - ������ �� ������, ������ ���������� ��� this
		
		bool operator < (const LongNum& other) const;   // �������� ��������� (������)
		bool operator == (const LongNum& other) const;   // �������� ��������� (���������)
		
		bool operator > (const LongNum& other) const;
		bool operator != (const LongNum& other) const;
		bool operator <= (const LongNum& other) const;
		bool operator >= (const LongNum& other) const;
		
		LongNum& operator = (const LongNum& other); // �������� ������������ (������ ������)
		LongNum& operator = (const long long int num); // �������� ������������ (����� long long)
		
		friend const LongNum operator + (const LongNum &one, const LongNum& other); // �������� ��������
		friend const LongNum& operator += (LongNum &one, const LongNum& other);  // �������� �������� � ����������� 
		friend const LongNum operator += (LongNum &one, const long long int  var); // �������� ������� � ����� � �����������
		friend const LongNum operator + (LongNum &one, const long long int  var); // �������� ������� � ����� 
		friend const LongNum operator + (const long long int  var, LongNum &one);  // �������� ����� � �������
		friend const LongNum operator - (const LongNum &one, const LongNum& other);  // �������� ��������
		friend const LongNum& operator -= (LongNum &one, const LongNum& other);   // �������� �������� � �����������
		friend const LongNum operator - (const LongNum &one, const long long int  var);  // �������� ������� � �����
		friend const LongNum operator - (const long long int var, const LongNum &one);  // �������� ����� � �������
		friend const LongNum operator * (const LongNum &one, const long long int var);   // ��������� ������� �� �����
		friend const LongNum operator * (const long long int var, const LongNum &one);  //  ��������� ����� �� ������
		friend const LongNum& operator * (const LongNum &one, const LongNum& other);   // ������������ ��������
		friend const LongNum operator ++ (LongNum& one);   // ��������� �����������
		friend const LongNum& operator ++ (LongNum& one, int);   // ��������� ����������
		friend const LongNum operator -- (LongNum& one); // ��������� �����������
		friend const LongNum& operator -- (LongNum& one, int); // ��������� �����������
		
};
