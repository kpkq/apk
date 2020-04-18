#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define fh5 740
#define ah5 932
#define gh5 830
#define ch5 554
#define ch6 1109
#define b5 988
#define dh5 622

#define COUNT 50
#define DELAY 10
#define TIME_RUN 65536

#define noteLen 72

int port[] = {0x40, 0x41, 0x42};

void SoundGeneration();
void DivisionCoefficientComputation();
void StatusWord();
void RandomNumber();

void main()
{
	clrscr();
	char choice;
	do
	{
		printf("1. Generate sound.\n2. Division coefficient.\n3. Status word.\n4. Random number.\n0. Exit.\n");
		rewind(stdin);
		printf("\nYour choise: ");
		scanf("%c", &choice);

		switch (choice)
		{
		case '1': SoundGeneration(); break;
		case '2': DivisionCoefficientComputation(); break;
		case '3': StatusWord(); break;
		case '4': RandomNumber(); break;
		}
	} while (choice != '0');
}

void SoundGeneration() {
	int frequency[COUNT] = { fh5, ch6, ah5, ah5, gh5,		// all star smash mouth
							 fh5, fh5, b5, ah5, ah5,
							 gh5, gh5, fh5, fh5, ch6, 
							 ah5, ah5, gh5, gh5, fh5,
							 fh5, dh5, ch5, fh5, fh5,
							 ch5, ah5, ah5, gh5, gh5,
							 fh5, fh5, b5, ah5, ah5,
							 gh5, gh5, fh5, fh5, ch6,
							 ah5, ah5, gh5, fh5, fh5,
							 gh5, dh5, fh5, fh5, fh5 };

	int delayCounter[COUNT] = { 7, 3, 3, 7, 3,
								3, 3, 7, 3, 3,
								3, 3, 7, 3, 3,
								3, 3, 3, 3, 3,
								3, 7, 19, 3, 3, 
								3, 3, 3, 3, 3, 
								3, 7, 3, 3, 3,
								3, 3, 3, 7, 3,
								3, 7, 3, 3, 7,
								19, 7, 3, 3, 1 };
	long unsigned base = 1193180;
	int frequencyCounter;
	int divisionCoefficient;

	for (frequencyCounter = 0; frequencyCounter < COUNT; frequencyCounter++)
	{
		outp(0x43, 0xB6);
		divisionCoefficient = base / frequency[frequencyCounter];
		outp(0x42, divisionCoefficient % 256);
		divisionCoefficient /= 256;
		outp(0x42, divisionCoefficient);	

		outp(0x61, inp(0x61) | 3);		
		delay(noteLen);
		outp(0x61, inp(0x61) & 0xFC);		
		delay(delayCounter[frequencyCounter]*noteLen);
	}
}

void DivisionCoefficientComputation()
{
	unsigned long j;
	int iChan;
	int controlWords[] = { 0x00, 0x40, 0x80 };
	unsigned kdLow, kdHigh, kd, max;

	printf("\n\nCoefficient of division: \n");

	for (iChan = 0; iChan < 3; iChan++)		// для каждого порта
	{
		kd = 0;
		max = 0;

		if (iChan == 2)
		{
			outp(0x61, inp(0x61) | 3);		//вкл
		}

		for (j = 0; j < TIME_RUN; j++)
		{
			outp(0x43, controlWords[iChan]);	// выбор канала
			kdLow = inp(port[iChan]);
			kdHigh = inp(port[iChan]);
			kd = kdHigh * 256 + kdLow;

			if (kd > max)
			{
				max = kd;
			}
		}
		if (iChan == 2)
		{
			outp(0x61, inp(0x61) & 0xFC);	//выкл
		}
		printf("\nChannel %d: %4X\n", iChan, max);
	}
}

void StatusWord()
{
	printf("\n\nStatus word: \n");
	char stateWord[8];
	int iChan, state;
	int controlWords[] = {0xE2,
						  0xE4,
						  0xE8};	
	// для каждого канала
	for (iChan = 0; iChan < 3; iChan++)
	{
		outp(0x43, controlWords[iChan]);		// выбор канала
		state = inp(port[iChan]);		// чтение

		for (int i = 7; i >= 0; i--)		// перевод в двоичное представление
		{
			stateWord[i] = state % 2 + '0';
			state /= 2;

		}
		printf("\nChannel %d: ", iChan);
		for (i = 0; i < 8; i++)
		{
			printf("%c", stateWord[i]);
		}
		printf("\n");
	}
}

void RandomNumber()
{
	char choice;
	unsigned int limit = 65536 - 1, numberLow, numberHigh, number;
	do
	{
		printf("\nEnter a max number in range [1...65635].\n");
		fflush(stdin);
	} while (!scanf("%d", &limit) || limit < 1);

	outp(0x43, 0xB4);
	outp(0x42, limit % 256);
	limit /= 256;
	outp(0x42, limit);
	outp(0x61, inp(0x61) | 1);
	while(choice != 'n')
	{
		printf("Get num(y/n)\n");
		choice = getch();
		outp(0x43, 0x80);		// выбрать второй канал
		numberLow = inp(0x42);		// считать число как в случае с коэффициентом деления
		numberHigh = inp(0x42);
		number = numberHigh * 256 + numberLow;
		printf("\nRandom number: %u\n", number);
	}
	outp(0x61, inp(0x61) & 0xFC);	// выкл
}