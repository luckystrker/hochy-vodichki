#include <vector>

class LongNum{  
	
	private:  
		std::vector<int> byte;  // само число в байтовом представлении
		
	public:    
		const int base = 256;  // основание системы счисления 
		
		int operator [] (const int& x) const;   // функция, возвращающая элемент под конкретным номером 
		void setByte(const int& pos, const int& value);   // функция, меняющая разряд под конкретным номером 
		
		void removeZeros();    // функция, удаляляющая ведущие нули в векторе
		
		LongNum();   // конструктор по умолчанию
		LongNum(const long long int number);   // конструктор, в который мы передаем число long long int 
		LongNum(std::string& str);    // конструктор, в который мы передаем строку
		LongNum(const LongNum& other);    // конструктор, в который мы передаем другой обьект класса LongNum
		void del(); // деструктор
		
		friend std::ostream& operator<< (std::ostream &out, LongNum& output); // перегруженный оператор вывода, через него мы выводим элементы вектора в консоль
		
		operator long(); // перевод вектора в число типа long long
		operator std::string();  // перевод вектора в строку чисел
		
		const LongNum operator + () const;  // единственный унарный оператор - ничего не делает, просто возвращает нам this
		
		bool operator < (const LongNum& other) const;   // оператор сравнения (меньше)
		bool operator == (const LongNum& other) const;   // оператор сравнения (равенство)
		
		bool operator > (const LongNum& other) const;
		bool operator != (const LongNum& other) const;
		bool operator <= (const LongNum& other) const;
		bool operator >= (const LongNum& other) const;
		
		LongNum& operator = (const LongNum& other); // оператор присваивания (обьект класса)
		LongNum& operator = (const long long int num); // оператор присваивания (число long long)
		
		friend const LongNum operator + (const LongNum &one, const LongNum& other); // сложение векторов
		friend const LongNum& operator += (LongNum &one, const LongNum& other);  // сложение векторов с накоплением 
		friend const LongNum operator += (LongNum &one, const long long int  var); // сложение вектора и числа с накоплением
		friend const LongNum operator + (LongNum &one, const long long int  var); // сложение вектора и числа 
		friend const LongNum operator + (const long long int  var, LongNum &one);  // сложение числа и вектора
		friend const LongNum operator - (const LongNum &one, const LongNum& other);  // разность векторов
		friend const LongNum& operator -= (LongNum &one, const LongNum& other);   // разность векторов с накоплением
		friend const LongNum operator - (const LongNum &one, const long long int  var);  // разность вектора и числа
		friend const LongNum operator - (const long long int var, const LongNum &one);  // разность числа и вектора
		friend const LongNum operator * (const LongNum &one, const long long int var);   // умножение вектора на число
		friend const LongNum operator * (const long long int var, const LongNum &one);  //  умножение числа на вектор
		friend const LongNum& operator * (const LongNum &one, const LongNum& other);   // произведение векторов
		friend const LongNum operator ++ (LongNum& one);   // инкремент постфиксный
		friend const LongNum& operator ++ (LongNum& one, int);   // инкремент префиксный
		friend const LongNum operator -- (LongNum& one); // декремент постфиксный
		friend const LongNum& operator -- (LongNum& one, int); // декремент постфиксный
		
};
