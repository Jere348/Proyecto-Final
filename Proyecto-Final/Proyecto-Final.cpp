#include "pch.h"
#include "Menu.h"
#pragma comment(lib, "User32.lib")
using namespace System;
string generarContraseña(int longitud = 8) {
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
    vector<string> nombres = { "Luis", "Ana", "Carlos", "María", "Pedro", "Lucía", "Diego", "Sofía", "Jorge", "Elena" };
    vector<string> apellidos = { "Gómez", "Pérez", "Ramírez", "Torres", "Sánchez", "Díaz", "Castro", "Vargas", "Mendoza", "Ríos" };
    vector<string> distritos = { "SanMiguel", "Barranco", "SanIsidro" };

    string nombre = nombres[rand() % nombres.size()] + " " + apellidos[rand() % apellidos.size()];

    string correo = nombre;
    replace(correo.begin(), correo.end(), ' ', '.');
    transform(correo.begin(), correo.end(), correo.begin(), ::tolower);
    correo += to_string(id) + "@correo.com";

    string contraseña = generarContraseña();
    string distrito = distritos[rand() % distritos.size()];
    int telefono = generarTelefono();

    return new User(nombre, telefono, distrito, correo, contraseña);
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

    // Obtiene el tamaño máximo posible de la ventana de consola
    COORD maxTam = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));

    // Aplica el tamaño del búfer (primero el búfer SIEMPRE >= ventana)
    Console::SetBufferSize(maxTam.X, maxTam.Y);

    // Luego el tamaño de la ventana
    Console::SetWindowSize(maxTam.X, maxTam.Y);

	Controlador* incio = new Controlador();

	incio->Menu();
}
