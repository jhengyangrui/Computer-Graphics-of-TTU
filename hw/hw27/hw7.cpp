#include <vector>
#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace std;
void print_ints(vector<int> myints)
{
	for(int i = 0;i < myints.size(); i++){
		printf("%d\n\n",myints[i]);
	}
	vector<int>::iterator iter;
	/*for (iter = myints.begin(); iter != myints.end(); iter++ ){
		printf("%d",*iter);	
	}*/
}

bool intcompare (int a, int b)
{
	return a<b;
}
int main()
{
	vector<int> myints;
	myints.push_back(1);
	myints.push_back(5);
	myints.push_back(3);
	myints.push_back(4);
	myints.push_back(6);
	//printf("%d\n",myints.size());
	print_ints(myints);
	sort (myints.begin(),myints.end(),intcompare);
	myints.insert(myints.begin()+1,50);
	myints.erase(myints.end()-2);
	print_ints (myints);

}