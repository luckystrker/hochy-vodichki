#include <iostream>
#include "longNum.h"


using namespace std;


int main() {
	string abc = "30000";
	LongNum a(abc), k(20000);
	LongNum b, c, d, e, f, g, h, i, j, l(1);
	unsigned long long def = 1000;
	b = a*def;
	cout << b << ": b = a * 1000" << endl;
	d = a*k;
	cout  << d << ": d = a*k" << endl;
	a+=k;
	cout << (a < b) << ": a < b" << endl;
	cout << (b == k) << ": b == k" << endl;
	cout << a << ": a+=k" << endl;
	b-=a;
	cout << b << ": b-=a " << endl;
	a+=256;
	cout << a << ": a+=256" << endl;
	f = a + 256;
	cout << f << ": f=a+256" << endl;
	b--;
	cout << b << ": b--" << endl;
	--b;
	cout << b << ": --b" << endl;
	
		
	return 0;
}
