#include <iostream>
#include <vector>
#include <vld.h>
#include "../NetworkDependencies/WinsockFunctions.h"

using namespace std;

int main()
{
	if(!InitWinSock())
	{
		cout << "errorMessage: unable to start winsock" << endl;
	}

	system("pause");

	return 0;
}