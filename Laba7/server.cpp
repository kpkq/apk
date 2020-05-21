#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

int main(int argc, char** argv) 
{
	if (argc == 1) return 0;

	HANDLE hWrite = CreateFile("COM1", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (!hWrite) 
	{
		cout << "Open COM port error " << GetLastError() << endl;
		return 0;
	}

	HANDLE readyToWrite = CreateEvent(NULL, TRUE, TRUE, "readyToWrite");
	HANDLE readyToRead = CreateEvent(NULL, TRUE, FALSE, "readyToRead");
	HANDLE writeEnded = CreateEvent(NULL, TRUE, FALSE, "writeEnded");
	OVERLAPPED asynchWrite = { 0 };
	asynchWrite.hEvent = readyToRead;


	STARTUPINFO si;					
	ZeroMemory(&si, sizeof(STARTUPINFO));	
	si.cb = sizeof(STARTUPINFO);			
	PROCESS_INFORMATION procInfo;					
	ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));	
	if (!CreateProcess(NULL, argv[1], NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &procInfo))
	{
		cout << "CreateProcess failed " << GetLastError() << endl; ;
		return 0;
	}

	string str;
	int i;
	do {
		i = 0;
		cout << "Your message (Enter to exit): " << endl;
		getline(cin, str);
		str.append("\n");
		if (str.size() == 1)
		{
			SetEvent(writeEnded);
		}
		while (i < str.size())
		{
			WaitForSingleObject(readyToWrite, INFINITE);
			ResetEvent(readyToWrite);
			WriteFile(hWrite, &(str[i]), 1, NULL, &asynchWrite);
			i++;
		}
		if (str.size() == 1) break;
	} while (1);
	system("cls");
	cout << "Transmission ended" << endl;
	SetEvent(readyToWrite);
	SetEvent(writeEnded);
	WaitForSingleObject(procInfo.hProcess, INFINITE);
	return 0;
}