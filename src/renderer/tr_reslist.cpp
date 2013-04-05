#include "tr_local.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;


	class Resolution
	{
	private:
		int x_res;
		int y_res;

	public:
		Resolution()
		{
			x_res = 0;
			y_res = 0;
		}
		Resolution(int x, int y)
		{
			x_res = x;
			y_res = y;
		}

		int getX()
		{
			return x_res;
		}

		int getY()
		{
			return y_res;
		}

		int getChecksum()
		{
			return x_res*10*1000 + y_res;
		}

	};

void quickSort(Resolution* arr[], int left, int right)
{
      int i = left, j = right;

      Resolution* tmp = new Resolution(0, 0);

      int pivot = arr[(left + right) / 2]->getChecksum();



      /* partition */

      while (i <= j) {

            while (arr[i]->getChecksum() < pivot)

                  i++;

            while (arr[j]->getChecksum() > pivot)

                  j--;

            if (i <= j) {

                  tmp = arr[i];

                  arr[i] = arr[j];

                  arr[j] = tmp;

                  i++;

                  j--;

            }

      };



      /* recursion */

      if (left < j)

            quickSort(arr, left, j);

      if (i < right)

            quickSort(arr, i, right);
}

class ResolutionList
{
private:
	Resolution* Reses[255];
	int length;

public:
	ResolutionList(Resolution* arr[], int i)
	{
		quickSort(arr, 0, i);
		length = 0;

		Resolution* Rreses[255];
		int lastChecksum = 0;

		for (int k = 0; k <= i; k++)
		{
			if (arr[k]->getChecksum() > lastChecksum)
			{
				Rreses[length] = new Resolution(arr[k]->getX(), arr[k]->getY());
				length++;
				lastChecksum = arr[k]->getChecksum();
			}
		}

		for (int k = 0; k < length; k++)
		{
			Resolution* r = new Resolution(Rreses[k]->getX(), Rreses[k]->getY());
			Reses[k] = r;
		}

	}

	int getLength()
	{
		return length;
	}

	int getWidth(int i)
	{
		return Reses[i]->getX();
	}

	int getHeight(int i)
	{
		return Reses[i]->getY();
	}
};


void getModes(vidmode_t* r_vidModes)
{
	LPDEVMODE lpDevMode;
	int err = 1;
	int i = 0;

	lpDevMode = (LPDEVMODE) LocalAlloc(LPTR, sizeof(DEVMODE));
	ZeroMemory(lpDevMode, sizeof(DEVMODE));
	lpDevMode->dmSize = sizeof(DEVMODE);

	Resolution* Vreses[255];
	while (err)
	{
		err = EnumDisplaySettings(NULL, i, lpDevMode);
		Resolution* res = new Resolution(lpDevMode->dmPelsWidth, lpDevMode->dmPelsHeight);
		Vreses[i] = res;
		i++;
	}

	ResolutionList* rl = new ResolutionList(Vreses, i-1);

	for (int i = 0; i < rl->getLength(); i++)
	{
		stringstream ss;
		ss << "Mode  " << i << ": " << rl->getWidth(i) << "x" << rl->getHeight(i);
		string s = ss.str();
		char *c;
		c = (char *) malloc(1024);
		strcpy(c, s.c_str());
		r_vidModes[i].description = c;
		r_vidModes[i].width = rl->getWidth(i);
		r_vidModes[i].height = rl->getHeight(i);
		r_vidModes[i].pixelAspect = 1;
	}

	int k = 5;

};

void getModeFromFile(char* c, vidmode_t* pvm)
{
	ifstream myfile;
	myfile.open(c);
	string line;
	vidmode_t vm = {0, 0, 0, 0};

	try
	{
		if (myfile.is_open())
		{
			getline(myfile,line);

			char* c = (char*) malloc(1024);
			strcpy(c, line.c_str());
			vm.description = c;

			getline(myfile,line);
			vm.width = atoi(line.c_str());

			getline(myfile,line);
			vm.height = atoi(line.c_str());

			getline(myfile,line);
			vm.pixelAspect = atoi(line.c_str());
			myfile.close();
		}
		else
		{
			throw;
		}

		*pvm = vm;

	}
	catch(...)
	{
	}

};