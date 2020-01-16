#include <iostream>

using namespace std;

int main()
{
	int temp;
	int counter = 0;
	int value[10];

	for (int i = 0; i < 10; i++)
	{
		cin >> value[i];

		if (value[i] > value[0])
		{
			temp = value[0];
			value[0] = value[i];
			value[i] = temp;
		}
	}
	cout << value[0];
}