#include <iostream>
#include <fstream>

#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
	srand((unsigned) time(0));
	
	int n;
	cout << "Enter number of Single IP Searches to Generate: "; //Around 500
	cin >> n;
	
	ofstream file("Single_IP_Searches.txt");
	
	string temp;
	for (int a = 0; a < n; a++)
	{
		temp = "";
		for (int b = 0; b < 4; b++)
		{
			temp += to_string(rand() % 256) + ".";
		}
		temp = temp.substr(0, temp.size() - 1);
		
		//cout << temp << endl;
		file << temp << endl;
	}
}
