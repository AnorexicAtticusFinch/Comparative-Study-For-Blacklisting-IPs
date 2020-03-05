#include <iostream>
#include <fstream>

#include <unordered_map>
#include <string.h>
using namespace std;

struct Node
{
	int value;
	unordered_map<string, Node*> keys;
};

Node* newNode()
{
	Node* temp = new Node;
	temp->value = 0;
	temp->keys.clear();
	
	return temp;
}

class Radix
{
private:
	
	Node* root;
	
	int size;
	void byteSize_helper(Node*);
	
	string padWithZeroes_helper(char*);
	string IPFormat_helper(string);
	
	string nextIP_helper(string);
	
	int findCommIndex_helper(string, string);
	void insert_helper(Node*, string);
	
	void remove_helper(Node*, string);
	
public:

	Radix();
	//Radix(Trie);

	bool search(string);
	bool searchRangeAll(string, string);
	bool searchRangeAny(string, string);
	
	void insert(string);
	void remove(string);
	
	int byteSize();
};

Radix::Radix()
{
	root = newNode();
	size = 0;
}

string Radix::padWithZeroes_helper(char* arr)
{
	string str(arr);
	
	if (str.size() == 1)
	{
		str = "00" + str;
	}
	
	if (str.size() == 2)
	{
		str = "0" + str;
	}
	
	return str;
}

string Radix::IPFormat_helper(string str)
{
	string temp = "";
	char* tempC = strtok((char*) str.c_str(), ".");
	while (tempC)
	{
		temp += padWithZeroes_helper(tempC);
		tempC = strtok(NULL, ".");
	}
	
	return temp;
}

bool Radix::search(string str)
{
	str = IPFormat_helper(str);

	Node* temp = root;
	int beg = 0, len = 1;
	string tKey;
	while ((beg + len) <= str.size())
	{
		tKey = str.substr(beg, len);
		
		if (temp->keys.count(tKey))
		{
			temp = temp->keys[tKey];
			beg += len;
			len = 1;
		}
		
		else
		{
			len++;
		}
	}
	
	if (beg != str.size())
	{
		return false;
	}
	
	return temp->value;
}

string Radix::nextIP_helper(string str)
{
	string temp[4] = {str.substr(0, 3), str.substr(3, 3), str.substr(6, 3), str.substr(9, 3)};
	
	for (int a = 3; a > -1; a--)
	{
		temp[a] = padWithZeroes_helper((char*) to_string((stoi(temp[a]) + 1) % 256).c_str());
		if (temp[a] != "000")
		{
			break;
		}
	}
	
	return (temp[0] + temp[1] + temp[2] + temp[3]);
}

bool Radix::searchRangeAll(string A, string B)
{
	string temp1 = IPFormat_helper(A);
	string temp2 = IPFormat_helper(B);
	
	string begin = min(temp1, temp2);
	string end = max(temp1, temp2);
	
	if (!search(begin))
	{
		return false;
	}
	
	string str = begin;	
	while (str != end)
	{
		str = nextIP_helper(str);
		if (!search(str))
		{
			return false;
		}
	}
	
	return true;
}

bool Radix::searchRangeAny(string A, string B)
{
	string temp1 = IPFormat_helper(A);
	string temp2 = IPFormat_helper(B);
	
	string begin = min(temp1, temp2);
	string end = max(temp1, temp2);
	
	if (search(begin))
	{
		return true;
	}
	
	string str = begin;	
	while (str != end)
	{
		str = nextIP_helper(str);
		if (search(str))
		{
			return true;
		}
	}
	
	return false;
}

int Radix::findCommIndex_helper(string A, string B)
{
	int comm = 0;
	
	while (comm < A.size() && comm < B.size())
	{
		if (A[comm] != B[comm])
		{
			break;
		}
		
		comm++;
	}
	
	return comm;
}

void Radix::insert_helper(Node* node, string str)
{	
	Node* temp = root;
	int comm;
	
	for (auto a = temp->keys.begin(); a != temp->keys.end(); a++)
	{
		comm = findCommIndex_helper(str, a->first);
		
		if (comm == str.size())
		{
			temp = temp->keys[str];
			temp->value = 1;
			return;
		}
		
		if (comm == 0)
		{
			continue;
		}
		
		if (comm == (a->first).size())
		{
			insert_helper(a->second, str.substr(comm, str.size() - comm));
			return;
		}
		
		string tempS = a->first;
		Node* tempN = a->second;
		
		temp->keys.erase(tempS);
		temp->keys[str.substr(0, comm)] = newNode();
		temp = temp->keys[str.substr(0, comm)];
		
		temp->keys[tempS.substr(comm, tempS.size() - comm)] = tempN;
		
		temp->keys[str.substr(comm, str.size() - comm)] = newNode();
		temp = temp->keys[str.substr(comm, str.size() - comm)];
		temp->value = 1;
		
		return;
	}
	
	temp->keys[str] = newNode();
	temp = temp->keys[str];
	temp->value = 1;;
}

void Radix::insert(string str)
{
	str = IPFormat_helper(str);
	
	if (search(str))
	{
		return;
	}
	
	insert_helper(root, str);
}

void Radix::remove_helper(Node* node, string str)
{
	Node* temp = node;
	int beg = 0, len = 1;
	string tKey;
	while (temp == node)
	{
		tKey = str.substr(beg, len);
		if (temp->keys.count(tKey))
		{
			temp = temp->keys[tKey];
			beg += len;
			len = 1;
		}

		else
		{
			len++;
		}
	}
	
	if (beg == str.size())
	{
		(node->keys[str])->value = 0;
		return;
	}
	
	remove_helper(temp, str.substr(beg, str.size() - beg));
	
	if (node->keys.size() != 1 || node->value)
	{
		return;
	}
	
	Node* child = (node->keys.begin())->second;
	
	if (child->keys.size() != 1)
	{
		return;
	}
	
	string temp1 = (node->keys.begin())->first;
	string temp2 = (child->keys.begin())->first;
	
	node->keys[temp1 + temp2] = (child->keys.begin())->second;
	
	delete node->keys[temp1];
	node->keys.erase(temp1);
}

void Radix::remove(string str)
{
	str = IPFormat_helper(str);
	
	if (!search(str))
	{
		return;
	}
	
	Node* temp = root;
	int beg = 0, len = 1;
	string tKey;
	while (temp == root)
	{
		tKey = str.substr(beg, len);
		if (temp->keys.count(tKey))
		{
			temp = temp->keys[tKey];
			beg += len;
			len = 1;
		}

		else
		{
			len++;
		}
	}
	
	if (beg == str.size())
	{
		(root->keys[str])->value = 0;
		return;
	}
	
	remove_helper(temp, str.substr(beg, str.size() - beg));
}

void Radix::byteSize_helper(Node* node)
{
	size += node->keys.size() * (sizeof(string) + sizeof(Node*));
	
	for (auto a = node->keys.begin(); a != node->keys.end(); a++)
	{
		byteSize_helper(a->second);
	}
}

int Radix::byteSize()
{
	size = 0;
	byteSize_helper(root);
	size += sizeof(Radix);
	
	return size;
}

int main()
{
	ios_base::sync_with_stdio(false);
	
	Radix DS;
	
	ifstream file;
	string temp;
	
	file.open("../IP_Generators_and_IPs/IPs.txt");
	int count = 1;
	while (!file.eof())
	{
		file >> temp;
		cout << temp << " Inserted: " << count++ << endl;
		DS.insert(temp);
	}
	file.close();

	cout << "Inserted all IPs" << endl << endl;
	
	int choice = -1;
	string temp1, temp2;
	while (choice != 7)
	{
		cout << "1. Insert an IP\n2. Search for an IP\n3. Search for all IPs in a range\n4. Search for any IPs in a range\n5. Remove an IP\n6. Get data stucture size\n7. Exit\n";
		cin >> choice;
		
		switch (choice)
		{
			case 1:
				cout << "Enter the IP: ";
				cin >> temp1;
				DS.insert(temp1);
				break;
			
			case 2:
				cout << "Enter the IP: ";
				cin >> temp1;
				cout << DS.search(temp1) << endl;
				break;
			
			case 3:
				cout << "Enter the 2 IPs: ";
				cin >> temp1;
				cin >> temp2;
				cout << DS.searchRangeAll(temp1, temp2) << endl;
				break;
			
			case 4:
				cout << "Enter the 2 IPs: ";
				cin >> temp1;
				cin >> temp2;
				cout << DS.searchRangeAny(temp1, temp2) << endl;
				break;
				
			case 5:
				cout << "Enter the IP: ";
				cin >> temp1;
				DS.remove(temp1);
				break;
				
			case 6:
				cout << DS.byteSize() << " bytes" << endl;
				break;
			
			case 7:
				break;
		}
		
		cout << endl;
	}
}
