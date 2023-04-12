
class Matrix {
	private:
		double** matrix;
		int rows; //количество строк
		int cols; //количество столбцов

	public:
		//
		// Функции, задающие размеры матрицы или возвращающие какие либо из значений матрицы
		//
		void setSize(const int& rows, const int& cols);
		int rowsNum() const;
		int colsNum() const;

		//
		// Функции - конструкторы и деструктор матрицы
		//

		Matrix();
		Matrix(const int rows, const int cols);
		Matrix(const Matrix& other);
		void del();

		//
		// Функции ввода и вывода матрицы
		//

		friend std::ostream& operator<< (std::ostream &out, const Matrix& target);
		friend std::istream& operator>> (std::istream &in, Matrix& target);
		//
		// Перегруженные операторы
		//
		Matrix& operator = (const Matrix& other);
		Matrix& operator + (const Matrix& other);
		Matrix& operator - (const Matrix& other);
		Matrix& operator * (const Matrix& other);
		
		Matrix& operator + (const double& a);
		Matrix& operator - (const double& a);
		Matrix& operator * (const double& a);
		Matrix& operator / (const double& a);
		
		Matrix& operator += (const Matrix& other);
		Matrix& operator -= (const Matrix& other);
		
		Matrix& operator += (const double& a);
		Matrix& operator -= (const double& a);
		Matrix& operator *= (const double& a);
		Matrix& operator /= (const double& a);
		
		Matrix& operator ++ (int);
		Matrix& operator ++ ();
		Matrix& operator -- (int);
		Matrix& operator -- ();
		
		double& operator () (const int a, const int b) const;
		
		bool operator==(const Matrix& other) const;
		bool operator!=(const Matrix& other) const;
};

		
