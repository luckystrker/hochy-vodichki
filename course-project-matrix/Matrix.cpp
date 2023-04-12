#include <stdexcept>
#include "Matrix.h"
#include <iostream>


using namespace std;
		//
		// Функции, задающие размеры матрицы или возвращающие какие либо из значений матрицы
		//
		void Matrix::setSize(const int& rows, const int& cols) { // функция, задающая размер матрице
			if(rows <= 0 || cols <= 0) throw runtime_error("Wrong input of dimensions.\n");
			this->rows = rows;
			this->cols = cols;
		}

		int Matrix::rowsNum() const { // возвращает количество строк матрицы
			return this->rows;
		}

		int Matrix::colsNum() const { // возвращает количество столбцов матрицы
			return this->cols;
		}

		//
		// Функции - конструкторы и деструктор матрицы
		//

		Matrix::Matrix() {  // конструктор по-умолчанию

		}

		Matrix::Matrix(const int rows, const int cols) { // конструктор со значениями количества строчек и столбцов
			setSize(rows, cols);
			matrix = new double* [rows];
			for (int i = 0; i < rows; ++i) {
				matrix[i] = new double[cols];
			}
		}

		Matrix::Matrix(const Matrix& other) { // конструктор копирования
			this->setSize(other.rowsNum(), other.colsNum());
			matrix = new double* [this->rowsNum()];
			for (int i = 0; i < this->rowsNum(); ++i) {
				matrix[i] = new double[this->colsNum()];
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = other.matrix[i][j];
				}
			}
		}

		void Matrix::del()	{  //деструктор
			for (int i = 0; i < rowsNum(); i++)	delete[] this->matrix[i];
			delete[] this->matrix;
		}

		//
		// Функции ввода и вывода матрицы
		//

		
		std::ostream& operator<< (std::ostream &out, const Matrix& target){ // перегруженный оператор вывода матрицы
			if(target.matrix == NULL) {
				out << "Matrix is empty";
			}
			for (int i = 0; i < target.rowsNum(); ++i)	{
				for (int j = 0; j < target.colsNum(); ++j)	{
					out << target.matrix[i][j] << " ";
				}
				out << endl;
			}
		}
		
		std::istream& operator>> (std::istream &in, Matrix& target) {// перегруженный оператор ввода матрицы
			for (int i = 0; i < target.rowsNum(); ++i)	{
				for (int j = 0; j < target.colsNum(); ++j)	{
					in >> target.matrix[i][j];
				}
			}
		}

		//
		// Перегруженные операторы
		//

		Matrix& Matrix::operator = (const Matrix& other) { // присваивание значения одной матрицы другой
			this->del();
			this->matrix = new double* [other.rowsNum()];
			for (int i = 0; i < this->rowsNum(); ++i) {
				this->matrix[i] = new double[other.colsNum()];
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = other.matrix[i][j];
				}
			}
			return *this;
		}

		Matrix& Matrix::operator + (const Matrix& other) { // сумма матриц
			if (this->rowsNum() != other.rowsNum() || this->colsNum() != other.colsNum()) throw runtime_error("Matrices has not the same size.\n");
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < this->rowsNum(); ++i)	{
				for (int j = 0; j < this->colsNum(); ++j)	{
					L->matrix[i][j] = this->matrix[i][j] + other.matrix[i][j];
				}
			}
			return *L;
			L->del();
		}

		Matrix& Matrix::operator - (const Matrix& other) { // разность матриц
			if (this->rowsNum() != other.rowsNum() || this->colsNum() != other.colsNum()) throw runtime_error("Matrices has not the same size.\n");
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < rows; ++i)	{
				for (int j = 0; j < cols; ++j)	{
					L->matrix[i][j] = this->matrix[i][j] - other.matrix[i][j];
				}
			}
			return *L;
			L->del();
		}

		Matrix& Matrix::operator * (const Matrix& other) { // произведение матриц
			if(this->rowsNum() != other.colsNum()) throw runtime_error("Number of rows in first matrix is not equal to the number of columns in the second one.\n");
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < this->rowsNum(); ++i)	{
				for (int j = 0; j < this->rowsNum(); ++j)	{
					L->matrix[i][j] = 0;
					for (int k = 0; k < this->rowsNum(); ++k)	{
						L->matrix[i][j] += this->matrix[i][k] * other.matrix[k][j];
					}
				}
			}
			return *L;
			L->del();
		}

///////////////////////////////////////////////////////////////////////////////////
		
		Matrix& Matrix::operator + (const double& a) { // сумма матрицы и числа
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < this->rowsNum(); ++i)	{
				for (int j = 0; j < this->colsNum(); ++j)	{
					L->matrix[i][j] = this->matrix[i][j] + a;
				}
			}
			return *L;
			L->del();
		}	
		
		Matrix& Matrix::operator - (const double& a) { // разность матрицы и числа
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < this->rowsNum(); ++i)	{
				for (int j = 0; j < this->colsNum(); ++j)	{
					L->matrix[i][j] = this->matrix[i][j] - a;
				}
			}
			return *L;
			L->del();
		}
		
		Matrix& Matrix::operator * (const double& a) { // произведение матрицы и числа
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < this->rowsNum(); ++i)	{
				for (int j = 0; j < this->colsNum(); ++j)	{
					L->matrix[i][j] = this->matrix[i][j] * a;
				}
			}
			return *L;
			L->del();
		}
		
		Matrix& Matrix::operator / (const double& a) { // частное матрицы и числа
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < this->rowsNum(); ++i)	{
				for (int j = 0; j < this->colsNum(); ++j)	{
					L->matrix[i][j] = this->matrix[i][j] / a;
				}
			}
			return *L;
			L->del();
		}
	
		
///////////////////////////////////////////////////////////////////////////////////		
		
		Matrix& Matrix::operator += (const Matrix& other) {  // сумма матриц (с накоплением)
			if (this->rowsNum() != other.rowsNum() || this->colsNum() != other.colsNum()) throw runtime_error("Matrices has not the same size.\n");
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = this->matrix[i][j] + other.matrix[i][j];
				}
			}
			return *this;
		}
		
		Matrix& Matrix::operator -= (const Matrix& other) { // разность матриц (с накоплением)
			if (this->rowsNum() != other.rowsNum() || this->colsNum() != other.colsNum()) throw runtime_error("Matrices has not the same size.\n");
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = this->matrix[i][j] - other.matrix[i][j];
				}
			}
			return *this;
		}

///////////////////////////////////////////////////////////////////////////////////

		Matrix& Matrix::operator += (const double& a) { // сумма матрицы и числа (с накоплением)
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = this->matrix[i][j] + a;
				}
			}
			return *this;
		}
		
		Matrix& Matrix::operator -= (const double& a) { // разность матрицы и числа (с накоплением)
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = this->matrix[i][j] - a;
				}
			}
			return *this;
		}
		
		Matrix& Matrix::operator *= (const double& a) { // произведение матрицы и числа (с накоплением)
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = this->matrix[i][j] * a;
				}
			}
			return *this;
		}
		
		Matrix& Matrix::operator /= (const double& a) { // частное матрицы и числа (с накоплением)
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					this->matrix[i][j] = this->matrix[i][j] / a;
				}
			}
			return *this;
		}

///////////////////////////////////////////////////////////////////////////////////
		
		Matrix& Matrix::operator ++ (int) {	// инкремент постфиксный		
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < rows; ++i)	{
				for (int j = 0; j < cols; ++j)	{
					L->matrix[i][j] = this->matrix[i][j];
					this->matrix[i][j] = this->matrix[i][j] + 1;
				}
			}
			return *L;
			L->del();
		}
		
		Matrix& Matrix::operator ++ () { // инкремент префиксный
			for(int i = 0; i < rowsNum();++i) {
				for(int j = 0; j < colsNum();++j) {
					matrix[i][j] = matrix[i][j] + 1;
				}
			}
			return *this;
		}
		
		Matrix& Matrix::operator -- (int) {	// декремент постфиксный
			Matrix* L = new Matrix(this->rowsNum(), this->colsNum());
			for (int i = 0; i < rows; ++i)	{
				for (int j = 0; j < cols; ++j)	{
					L->matrix[i][j] = this->matrix[i][j];
					this->matrix[i][j] = this->matrix[i][j] - 1;
				}
			}
			return *L;
			L->del();
		}
		
		Matrix& Matrix::operator -- () { // декремент префиксный
			for(int i = 0; i < rowsNum();++i) {
				for(int j = 0; j < colsNum();++j) {
					matrix[i][j] = matrix[i][j] - 1;
				}
			}
			return *this;
		}

///////////////////////////////////////////////////////////////////////////////////

		double& Matrix::operator () (const int a, const int b) const{ // доступ к определенному элементу
			if (a > this->rowsNum() || b > this->colsNum() || a < 0 || b <0) throw runtime_error("Wrong input (out of range).\n");
			else return matrix[a][b];
		}

///////////////////////////////////////////////////////////////////////////////////

		bool Matrix::operator==(const Matrix& other) const{ // сравнение двух матриц
			if (this->rowsNum() != other.rowsNum() || this->colsNum() != other.colsNum()) return false;
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					if (this->matrix[i][j] == other.matrix[i][j]) {
						continue;
					} else {
						return false;
					}
				}
			}
			return true;
		}

		bool Matrix::operator!=(const Matrix& other) const{ // сравнение двух матриц
			if (this->rowsNum() != other.rowsNum() || this->colsNum() != other.colsNum()) return true;
			for (int i = 0; i < this->rowsNum(); ++i) {
				for (int j = 0; j < this->colsNum(); ++j) {
					if (this->matrix[i][j] == other.matrix[i][j]) {
						continue;
					} else {
						return true;
					}
				}
			}
			return false;
		}

	

