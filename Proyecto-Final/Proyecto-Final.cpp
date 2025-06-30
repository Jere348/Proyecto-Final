#include "pch.h"
#include "Menu.h"
#pragma comment(lib, "User32.lib")
using namespace System;


int main()
{
    
    // Maximiza la ventana
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_MAXIMIZE);

    // Obtiene el tamaño máximo posible de la ventana de consola
    COORD maxTam = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));

    // Aplica el tamaño del búfer (primero el búfer SIEMPRE >= ventana)
    Console::SetBufferSize(maxTam.X, maxTam.Y);

    // Luego el tamaño de la ventana
    Console::SetWindowSize(maxTam.X, maxTam.Y);

	Controlador* incio = new Controlador();

	incio->Menu();
}
