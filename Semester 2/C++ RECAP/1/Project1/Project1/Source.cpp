#include "Header.h"

int main()
{
	int width, height;
	InputRect(width, height);
	OutputRect(width, height);
	return 0;
}

void InputRect(int& width, int& height)
{
	cout << "Please enter a width" << endl;
	cin >> width;
	cout << "Please enter a height" << endl;
	cin >> height;
}

void OutputRect(const int width, const int height)
{
	cout << "The area of the rectangle is: " << width * height << endl;
}