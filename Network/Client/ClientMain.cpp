#include "SocketClient.h"
const int maxThreadCount=2;
bool validateIpAddress(const std::string ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
	return result != 0;
}
/*int main(int argc, char *argv[])
{
	std::string tst;
	bool test=true;
	//Multithreading variables
	//int nThreads = 0;
	//DWORD dwThreadId[maxThreadCount];
	//HANDLE threadhandle;

	GameClass game;
	SocketClient<GameClass> client;
	//Sets up the link to the GameClass class.
	client.setPlayerContPtr(&game);
	//This is the loop which makes the user enter the server address.
	while (!client.isReady());
	do
	{
		if (!test)
		{
			printf("Could not connect to server. Try another IP.\n");
		}
		else
		{
			printf("Enter the server ip. \n");
		}
		getline(std::cin, tst);
		if (tst.length()==0)
		{
			tst="127.0.0.1";
		}
		if (validateIpAddress(tst))
		{
			//Tmp init connection message: set username
			char* tmp=new char[30];
			printf("What is your desired username?\n");
			std::cin.getline(tmp,30);
			if (strlen(tmp)==0)
			{
				tmp="Anonymous";
			}
			printf("Username set to %s\n", tmp);

			test=client.connectToIP(tst.c_str(), tmp, strlen(tmp));
		}
		else
		{
			printf("Invalid IPaddress. Please enter a new IPaddress.\n");
			test=false;
		}
	} while (!test);
	while (!client.isConnected());
	Sleep(1000);
	//Starts the receive loop
	//threadhandle=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&client.receiveDataThreadV,(LPVOID) &client,0,&dwThreadId[0]);
	client.startReceiveThread();
	//GetExitCodeThread(threadhandle, eCode);
	//This is just a loop to receive user input which creates a natural delay for sendUserData.
	printf("Write what you want to send\n");
	tst="tmp init message";
	while (tst.length()>0)
	{
		client.sendMessage(tst);
		client.sendUserData();
		getline(std::cin, tst);
	}
	//Kills off the thread and connection
	//DWORD eCode=0;
	//TerminateThread(threadhandle, eCode);
	client.closeConnection();
	return 0;
}*/