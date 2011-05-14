#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
#include "xml.h"

using namespace std;
using namespace Gdiplus;


void DrawMap(const char* file, HDC hdc);
