#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

struct Node
{
	int value;
	Node* keys[10];
};

Node* newNode()
{
	Node* temp = new Node;
	temp->value = 0;
	
	for (int a = 0; a < 10; a++)
	{
		temp->keys[a] = NULL;
	}
	
	return temp;
}

class Trie
{
private:
	
	Node* root;
	
	int size;
	void byteSize_helper(Node*);
	
	string padWithZeroes_helper(char*);
	string IPFormat_helper(string);
	
	string nextIP_helper(string);
	bool searchFromMiddle_helper(string, int, Node*);
	
public:

	Trie();

	bool search(string);
	bool searchRangeAll(string, string);
	bool searchRangeAny(string, string);
	
	void insert(string);
	void remove(string);
	
	int byteSize();
};

Trie::Trie()
{
	root = newNode();
	size = 0;
}

string Trie::padWithZeroes_helper(char* arr)
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

string Trie::IPFormat_helper(string str)
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

bool Trie::search(string str)
{
	str = IPFormat_helper(str);
	
	Node* temp = root;
	for (int a = 0; a < str.size(); a++)
	{		
		if (temp->keys[str[a] - '0'])
		{
			temp = temp->keys[str[a] - '0'];
		}
		
		else
		{
			return false;
		}
	}
	
	if (temp->value)
	{
		return true;
	}
	
	return false;
}

string Trie::nextIP_helper(string str)
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

bool Trie::searchFromMiddle_helper(string str, int index, Node* node)
{
	for (int a = index; a < str.size(); a++)
	{
		if (node->keys[str[a] - '0'])
		{
			node = node->keys[str[a] - '0'];
		}
		
		else
		{
			return false;
		}
	}
	
	if (node->value)
	{
		return true;
	}
	
	return false;
}

bool Trie::searchRangeAll(string A, string B)
{
	string temp1 = IPFormat_helper(A);
	string temp2 = IPFormat_helper(B);
	
	string begin = min(temp1, temp2);
	string end = max(temp1, temp2);
	
	int count = 0;
	Node* temp = root;
	while (begin[count] == end[count] && temp && count < begin.size())
	{
		temp = temp->keys[begin[count] - '0'];
		count++;
	}
	
	if (count == begin.size())
	{
		return true;
	}
	
	if (!temp)
	{
		return false;
	}
	
	if (!searchFromMiddle_helper(begin, count, temp))
	{
		return false;
	}
	
	string str = begin;
	while (str != end)
	{
		str = nextIP_helper(str);
		if (!searchFromMiddle_helper(str, count, temp))
		{
			return false;
		}
	}
	
	return true;
}

bool Trie::searchRangeAny(string A, string B)
{
	string temp1 = IPFormat_helper(A);
	string temp2 = IPFormat_helper(B);
	
	string begin = min(temp1, temp2);
	string end = max(temp1, temp2);
	
	int count = 0;
	Node* temp = root;
	while (begin[count] == end[count] && temp && count < begin.size())
	{
		temp = temp->keys[begin[count] - '0'];
		count++;
	}
	
	if (count == begin.size())
	{
		return true;
	}
	
	if (!temp)
	{
		return false;
	}
	
	if (searchFromMiddle_helper(begin, count, temp))
	{
		return true;
	}
	
	string str = begin;
	while (str != end)
	{
		str = nextIP_helper(str);
		if (searchFromMiddle_helper(str, count, temp))
		{
			return true;
		}
	}
	
	return false;
}

void Trie::insert(string str)
{
	str = IPFormat_helper(str);
	
	Node* temp = root;
	for (int a = 0; a < str.size(); a++)
	{		
		if (temp->keys[str[a] - '0'])
		{
			temp = temp->keys[str[a] - '0'];
		}
		
		else
		{
			temp->keys[str[a] - '0'] = newNode();
			temp = temp->keys[str[a] - '0'];
		}
	}
	
	temp->value = 1;
}

void Trie::remove(string str)
{
	str = IPFormat_helper(str);
	
	Node* temp = root;
	for (int a = 0; a < str.size(); a++)
	{		
		if (temp->keys[str[a] - '0'])
		{
			temp = temp->keys[str[a] - '0'];
		}
		
		else
		{
			return;
		}
	}
	
	temp->value = false;
}

void Trie::byteSize_helper(Node* node)
{
	if (!node)
	{
		return;
	}
	
	size += sizeof(Node);
	
	for (int a = 0; a < 10; a++)
	{
		byteSize_helper(node->keys[a]);
	}
}

int Trie::byteSize()
{
	size = 0;
	byteSize_helper(root);
	size += sizeof(Trie);
	
	return size;
}

int main()
{
	ios_base::sync_with_stdio(false);
	
	Trie DS;
	
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
