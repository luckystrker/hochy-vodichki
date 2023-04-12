#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class HashTable{
	private:
		unordered_map<int,vector<int>> table;
	public:
		HashTable() {

		}
		// 123456789 
		int HashFunc(int a) {
			return (a / 1000000 + (a % 1000000) / 1000 + a % 1000) % 1000;
		}

	/*	int HashFunc(int a) { //123456789 - example of key
			return  (a - a % 100000000) / 100000000 * 100 + (a - a % 100000) / 100000 % 10 * 10+ a % 10; // returns 149
		*/
				
		void searchItem(const int& k);
		void insertItem(const int& k);
		void removeItem(const int& k);
		
		int elementsNum() const;
		int tableSize() const;
		
		bool isEmpty() const;
		
		void out() const;
		
		void clearTable();
};
