#include "HashTable.h"
#include <iostream>

using namespace std;


void HashTable::searchItem(const int& k) {
	int key = HashFunc(k);
	auto it = this->table.find(key);
	if (it != this->table.end() && table.size() > 1) {
		cout << "Failed to find!";
	}
	else cout << key << " : " << k;
}


void HashTable::insertItem(const int& k) {
	int key = HashFunc(k);
	auto it = this->table.find(key);
	if (it != this->table.end()) {
		it->second.push_back(k);
	}
	else {
		vector<int> a = { k };
		table.insert(make_pair(key, a));
	}
}


void HashTable::removeItem(const int& k) {
	int key = HashFunc(k);
	auto it = this->table.find(key);
	if (it != this->table.end()) {
		if (it->second.size() > 1) {
			for (auto i = it->second.begin(); i != it->second.end(); ++it) {
				if (*i == k) {
					it->second.erase(i);
					break;
				}
			}
		}
		else table.erase(it);
	}
}


int HashTable::elementsNum() const {
	int count = 0;
	for (auto it = table.begin(); it != table.end(); ++it) {
		count += it->second.size();
	}
	return count;
}

int HashTable::tableSize() const {
	return table.size();
}


bool HashTable::isEmpty() const {
	return (tableSize() > 0 ? true : false);
}

void HashTable::out() const {
	for (auto it = table.begin(); it != table.end(); ++it) {
		cout << it->first << " :\n";
		for(auto ii = it->second.begin(); ii != it->second.end(); ++ii) cout << "\t" << * ii << endl;
	}
}

void HashTable::clearTable() {
	table.clear();
}

