#include<iostream>
#include<fstream>

using namespace std;

int test_array[] = {1,2,3,4,5};

int BinarySearch(int val, int head, int tail)
{
	int mid = (head+tail)/2;
	if(test_array[mid] == val)
		return mid;
	else if(test_array[mid] < val)
		return BinarySearch(val, mid+1, tail);
	else if(test_array[mid] > val)
		return BinarySearch(val, head, mid-1);
}

void main()
{
	//int length = string::size(test_array);
	int res = BinarySearch(3, 0, 5);
	cout << res << endl;
	while(1)
		;
}

