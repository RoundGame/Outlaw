#pragma once
#include <string>
#include <fstream>

using namespace std;

struct pscore {
	int score;
	string nickname;
};

void Quick(int a[], long lb, long hb, string name[]);
int GetPiv(int a[], long lb, long hb, string name[]);

void addplayer(pscore newplayer);
void sortscores(int& n, string nicksarr[255], int scorearr[255]);