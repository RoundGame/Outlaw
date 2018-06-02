#include "highscore.h"

void Quick(int a[], long lb, long hb, string name[])
{
	if (lb < hb)
	{
		long p = GetPiv(a, lb, hb, name); // получение опорной точки по разбиению ’оара
										  //рекурси€
		Quick(a, lb, p, name);
		Quick(a, p + 1, hb, name);
	}
}

int GetPiv(int a[], long lb, long hb, string name[])
{
	int pivot = a[lb]; //сначала за опорную точку считаетс€ первый элемент массива
	int i = lb - 1, j = hb + 1;
	//два индекса (в начале и в конце) сближаютс€, пока не найдетс€ пара
	//в которой один элемент слева и больше опорного, а второй - меньше и справа
	//элементы этой пары мен€ютс€ местами
	while (1) {
		do {
			i++;
		} while (a[i] < pivot);

		do {
			j--;
		} while (a[j] > pivot);

		if (i >= j)
			return j;
		int x = a[i];
		string z = name[i];
		a[i] = a[j];
		name[i] = name[j];
		a[j] = x;
		name[j] = z;
	}
}

void addplayer(pscore newplayer) {
	ofstream hstab("Highscores.txt", ios_base::app);
	hstab << newplayer.nickname << endl << newplayer.score << endl;
	hstab.close();
}

void sortscores(int& n,string nicksarr[255], int scorearr[255]) {
	ifstream hstab("Highscores.txt");
	if (!hstab.is_open()) {
		ofstream hstab("Highscores.txt");
		hstab.close();
	}
	else
	{
		while (1) {
			hstab >> nicksarr[n];
			hstab >> scorearr[n];
			if (hstab.eof())
				break;
			n++;
		}
		hstab.close();
		Quick(scorearr, 0, n - 1, nicksarr);
		ofstream hstab("Highscores.txt");
		for (int i = n - 1; i >= 0; i--) {
			hstab << nicksarr[i] << endl << scorearr[i] << endl;
		}
		hstab.close();
	}

}