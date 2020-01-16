#include <iostream>

using namespace std;

int main()
{
	int option;
	do
	{
		cout << "1. Sunny \n2. Cloudy\n3. Raining\n4. Exit" << endl;
		cin >> option;
		if (option >0 || option < 5)
		{
			switch (option)
			{
			case 1:
				cout << "Its gonna be sunny today" << endl;
				break;
			case 2:
				cout << "clouds appear" << endl;
				break;
			case 3:
				cout << "dont forget your brolly" << endl;
				break;
			case 4:
				cout << "cya" << endl;
				break;
			}
		}
		else
		{
			cout << "the value must be between 1 and 4" << endl;
		}
	} while (option !=4);
	return 0;
}