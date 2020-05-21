#include <Windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

int main() {

	char buf;		//Буфер для записи
	int f = 1;

	HANDLE hRead = CreateFile("COM2", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (!hRead) 
	{
		cout << "Open COM port error " << GetLastError() << endl;
		return 0;
	}

	HANDLE readingEnded = CreateEvent(NULL, TRUE, FALSE, "readingEnded");
	HANDLE readyToWrite = OpenEvent(EVENT_ALL_ACCESS, FALSE, "readyToWrite");
	HANDLE writeEnded = OpenEvent(EVENT_ALL_ACCESS, FALSE, "writeEnded");
	HANDLE readyToRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, "readyToRead");
	OVERLAPPED asynchRead = { 0 };
	asynchRead.hEvent = readingEnded;
	bool isFirst = 1;
	do {
		WaitForSingleObject(readyToRead, INFINITE);
		ResetEvent(readyToRead);
		ReadFile(hRead, &buf, 1, NULL, &asynchRead);
		WaitForSingleObject(readingEnded, INFINITE);
		SetEvent(readyToWrite);
		if (isFirst) cout << "New message: ";
		cout << buf;
		isFirst = 0;
		if (buf == '\n')
		{
			if (WaitForSingleObject(writeEnded, 0) == WAIT_OBJECT_0)
			{
				cout << "Receiving Ended" << endl;
				break;
			}
			cout << "New message: ";
		}

	} while (1);
	return 0;
}
