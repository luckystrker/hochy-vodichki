#include <iostream>
#include "Matrix.h"
using namespace std;


int main()
{
	try {
		int m, n;
		cout << "Enter matrix M: " << endl << "Enter num of rows: ";
		cin >> m;
		cout << "enter num of cols: ";
		cin >> n;
	
		Matrix M(m, n);
		cin >> M;
		cout << M;
		cout << "\n\n";
		
		cout << "Enter matrix N: " << endl << "Enter num of rows: ";
		cin >> m;
		cout << "enter num of cols: ";
		cin >> n;
	
		Matrix N(m, n);
		cin >> N;
		cout << N;
		cout << "\n\n";
		
		M = M + 2;
		cout << "M = M + 2 : " << "\n";
		cout << M;
		cout << "\n\n";
		
		M = M - 5;
		cout << "M = M - 5 : " << "\n";
		cout << M;
		cout << "\n\n";
		
		N = N * 3.5;
		cout << "N = N * 3.5 : " << "\n";
		cout << N;
		cout << "\n\n";
		
		N = N / 0.03;
		cout << "N = N / 0.03 : " << "\n";
		cout << N;
		cout << "\n\n";
	
		M += N;
		cout << "M += N : " << "\n";
		cout << M;
		cout << "\n\n";
		
		N -= M;
		cout << "N += M : " << "\n";
		cout << N;
		cout << "\n\n";
		
		N += 3;
		cout << "N += 3 : " << "\n";
		cout << N;
		cout << "\n\n";
		
		N -= 7;
		cout << "N -= 7 : " << "\n";
		cout << N;
		cout << "\n\n";
		
		N *= 10;
		cout << "N *= 10 : " << "\n";
		cout << N;
		cout << "\n\n";
		
		N /= 23;
		cout << "N /= 23 : " << "\n";
		cout << N;
		cout << "\n\n";
	
		Matrix S = Matrix(m, n);
		S = (M + N);
		cout << "M + N = " << "\n";
		cout << S;
		cout << "\n\n" << endl;
		S.del();
	
		Matrix U = Matrix(m, n);
		U = (M * N);
		cout << "M * N = " << "\n";
		cout << U;
		cout << "\n\n" << "\n";
	
		cout << "M++ : " << "\n";
		cout << M++;
		cout << "\n\n" << "\n";
		
		cout << "++M : " << "\n";
		cout << ++M;
		cout << "\n\n" << "\n";
		
		cout << "N-- : " << "\n";
		cout << --N;
		cout << "\n\n" << "\n";
		
		cout << "--N : " << "\n";
		cout << N--;
		cout << "\n\n" << "\n";
	
		cout << "Enter position of needed element in last matrix: " << "\n";
		int k, l;
		cout << "Enter X: ";
		cin >> k;
		cout << "Enter Y: ";
		cin >> l;
		cout << N(k,l) << "\n";
		U.del();
		cout << "\n\n" << "\n";
	
		cout << "Comparison of given matrices:" << "\n";
		if (M != N) { cout << "Matrices are not equal" << "\n"; }
		else { cout << "Matrices are equal" << "\n"; }
		
		N.del();
		M.del();
	}
	catch(exception& exc) {
		cerr << exc.what() << endl;
		exit(0);
	}

	return 0;
}
