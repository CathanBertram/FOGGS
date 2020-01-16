#include <iostream>

using namespace std;

int main()
{
	int num;

	cout << "enter a value between 1 and 10" << endl;
	cin >> num;
	if (num > 0 && num < 11)
	{
		for (int i = 0; i < num; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				cout << j + 1 << "     ";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "Thats the wrong number" << endl;
	}
}