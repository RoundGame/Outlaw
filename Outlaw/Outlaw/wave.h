#pragma once
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <mmsystem.h>
#include <fstream>

struct Wave
{
	Wave(char * filename);
	Wave();

	void play(bool async = true);
	bool issuccess();

	char * buffer = 0;
	bool success = false;
	HINSTANCE HInstance;
};