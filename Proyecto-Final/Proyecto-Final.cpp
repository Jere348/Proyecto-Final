#include "pch.h"
#include "Menu.h"
#pragma comment(lib, "User32.lib")
using namespace System;
string generarContrase�a(int longitud = 8) {
    string caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string pass;
    for (int i = 0; i < longitud; ++i) {
        pass += caracteres[rand() % caracteres.size()];
    }
    return pass;
}

int generarTelefono() {
    int telefono = 9 * 100000000;
    for (int i = 0; i < 8; ++i) {
        telefono += (rand() % 10) * pow(10, 7 - i);
    }
    return telefono;
}

// Generar un solo usuario
User* generarUsuario(int id) {
    vector<string> nombres = { "Luis", "Ana", "Carlos", "Mar�a", "Pedro", "Luc�a", "Diego", "Sof�a", "Jorge", "Elena" };
    vector<string> apellidos = { "G�mez", "P�rez", "Ram�rez", "Torres", "S�nchez", "D�az", "Castro", "Vargas", "Mendoza", "R�os" };
    vector<string> distritos = { "SanMiguel", "Barranco", "SanIsidro" };

    string nombre = nombres[rand() % nombres.size()] + " " + apellidos[rand() % apellidos.size()];

    string correo = nombre;
    replace(correo.begin(), correo.end(), ' ', '.');
    transform(correo.begin(), correo.end(), correo.begin(), ::tolower);
    correo += to_string(id) + "@correo.com";

    string contrase�a = generarContrase�a();
    string distrito = distritos[rand() % distritos.size()];
    int telefono = generarTelefono();

    return new User(nombre, telefono, distrito, correo, contrase�a);
}


void cargarDataset() {
    srand(time(0));
    AVLArbol<User> UserFromDataSet;
    User* usuario = new User();

    for (int i = 0; i < 100; ++i) {
        usuario = generarUsuario(i);
        UserFromDataSet.insertar(usuario);
    }
	guardarUsuariosEnArchivo(UserFromDataSet, "usuarios.txt");
}

int main()
{
	cargarDataset();
    // Maximiza la ventana
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_MAXIMIZE);

    // Obtiene el tama�o m�ximo posible de la ventana de consola
    COORD maxTam = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));

    // Aplica el tama�o del b�fer (primero el b�fer SIEMPRE >= ventana)
    Console::SetBufferSize(maxTam.X, maxTam.Y);

    // Luego el tama�o de la ventana
    Console::SetWindowSize(maxTam.X, maxTam.Y);

	Controlador* incio = new Controlador();

	incio->Menu();
}
