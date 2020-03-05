#include<iostream>

#include<fstream>
#include<string>

using namespace std;
struct bucket
{
	string ip;
	bucket* next;
};

bucket* newNode(string ip)
{
	bucket* temp = new bucket;
	temp->ip = ip;
	temp->next = NULL;
	
	return temp;	
}

class Hash_Table
{
	public:
		Hash_Table();
		bucket* chain[65536];
		long long int hash_size;
		int hash_func(string s);
		int net_part(string s);
		int byteSize();
		
		bool search(string a);
		bool searchRangeAll(string a, string b);
		bool searchRangeAny(string a, string b);
	
		void insert(string a);
		void remove(string a);		
};

Hash_Table::Hash_Table()
{
	hash_size = 65536*sizeof(bucket);
	for(int i=0;i<65536;i++)
	{
		chain[i]=new bucket;
	    chain[i]=NULL;
	}		
}

int Hash_Table::net_part(string s)
{
	string s1 = "";
	string s2 = "";
	int count = 0;
	for(int i = 0;count!=2;i++)
	{
		if(s[i] == '.')
		{
			count++;
			continue;
		}
		if(count == 0)
		{
			s1 = s1 + s[i];
		}
		if(count == 1)
		{
			s2 = s2+s[i];
		}
	}
	int x1=stoi(s1);
	int x2=stoi(s2);
	int value=x1*256+x2;
	return value;
}

int Hash_Table::hash_func(string s)
{
	int index;
	string s1="";
	string s2="";
	int count=0;
	for(int i=s.length()-1;count!=2;i--)
	{
		if(s[i] == '.')
		{
			count++;
			continue;
		}
		
		if(count==0)
		{
			s2 = s[i] + s2 ;	
		}
		
		if(count==1)
		{
			s1 = s[i] + s1 ;	
		}	
	}
	int x1 = stoi(s1);
	int x2 = stoi(s2);
	index = x1*256 + x2;
	return index;	
}

void Hash_Table::insert(string a)
{
	int index = hash_func(a);
	bucket* node = newNode(a);
	node->next = chain[index];
	chain[index] = node;
	hash_size += sizeof(bucket);	
}

void Hash_Table::remove(string a)
{
	int index = hash_func(a);
	if(chain[index] == NULL)
		return;
	bucket* temp =chain[index];
	if(chain[index]->ip==a){
		temp =chain[index];
		chain[index]=chain[index]->next;
		hash_size -= sizeof(bucket);
		delete temp;
		return;
	}
	while(temp->next){
		if(temp->next->ip==a){
			bucket* node=temp->next;
			temp->next=temp->next->next;
			delete node;
			hash_size-=sizeof(bucket);
			return;
		}
		temp=temp->next;
	}		
}

bool Hash_Table::search(string a)
{
	int index = hash_func(a);
	if(chain[index] == NULL)
		return 0;
	bucket* temp = chain[index];
	while(temp!=NULL)
	{
		if(temp->ip == a)
		{
			return 1;	
		}
		temp = temp->next;	
	}
	return 0;	
}

bool Hash_Table::searchRangeAll(string a, string b)
{
	int hosta = hash_func(a);
	int hostb = hash_func(b);
	int neta = net_part(a);
	int netb = net_part(b);
	long long int x1 = neta*256*256 + hosta;
	long long int x2 = netb*256*256 + hostb;
	if(x1 > x2)
	{
		int temp;
		temp = hosta;
		hosta = hostb;
		hostb = temp;
		temp = neta;
		neta = netb;
		netb = temp;
	}
	bool flag = true;
	bucket* temp = new bucket;
	while(neta < netb)
	{
		temp = chain[hosta];
		flag = false;
		while(temp!=NULL)
		{	
			if(net_part(temp->ip) == neta)
			{
				flag = true;
				break;	
			}
			temp = temp->next;
		}
		if(flag == true)
		{
			hosta++;
			if(hosta == 65536)
			{
				hosta = 0;
				neta ++;	
			}	
		}
		else
		{
			return false;	
		}		
	}
	if(neta == netb)
	{
		while(hosta <= hostb)
		{
			flag = false;
			temp = chain[hosta];
			while(temp!=NULL)
			{
				if(net_part(temp->ip) == neta)
				{
					flag = true;
					break;
				}
				temp = temp->next;
			}
			if(flag == true)
			{
				hosta ++;							
			}
			else
			{
				return false;
			}
			
		}
	}
	return true;
}
bool Hash_Table::searchRangeAny(string a, string b)
{
	int hosta = hash_func(a);
	int hostb = hash_func(b);
	int neta = net_part(a);
	int netb = net_part(b);
	long long int x1 = neta*256*256 + hosta;
	long long int x2 = netb*256*256 + hostb;
	if(x1 > x2)
	{
		int temp;
		temp = hosta;
		hosta = hostb;
		hostb = temp;
		temp = neta;
		neta = netb;
		netb = temp;
	}
	bool flag = true;
	bucket* temp = new bucket;
	while(neta < netb)
	{
		temp = chain[hosta];
		flag = false;
		while(temp!=NULL)
		{	
			if(net_part(temp->ip) == neta)
			{
				return true;	
			}
			temp = temp->next;
		}
		hosta++;
		if(hosta==65536){
			hosta=0;
			neta++;
		}			
	}
	if(neta == netb)
	{
		while(hosta <= hostb)
		{
			flag = false;
			temp = chain[hosta];
			while(temp!=NULL)
			{
				if(net_part(temp->ip) == neta)
				{
					return true;
				}
				temp = temp->next;
			}
			hosta ++;	
		}
	}
	return false;
}

int Hash_Table::byteSize()
{
	return hash_size;
}

int main()
{
	ios_base::sync_with_stdio(false);
	
	Hash_Table DS;
	
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
