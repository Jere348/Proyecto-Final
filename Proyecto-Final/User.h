#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class User
{
public:
    User();
    User(string pnombre, int pnumero, string pdistrito, string pcorreo, string pcontrase�a);
    ~User();
    void ListarCLiente();
    string getNombre() { return nombre; }
    string getDistrito();
    string getContrase�a() { return contrase�a; }

    // M�todos para archivo
    string serializar() const;
    static User* deserializar(const string& linea);

protected:
    string nombre;
    int numero;
    string distrito;
    string correo;
    string contrase�a;
};

User::User() : nombre(""), numero(0), distrito(""), correo(""), contrase�a("") {}

User::User(string pnombre, int pnumero, string pdistrito, string pcorreo, string pcontrase�a)
    : nombre(pnombre), numero(pnumero), distrito(pdistrito), correo(pcorreo), contrase�a(pcontrase�a) {}

User::~User() {}

inline void User::ListarCLiente()
{
    cout << "------------- Cliente " << nombre << "----------- " << endl;
    cout << "Correo:" << correo << endl;
    cout << "Contrase�a:" << contrase�a << endl;
    cout << "Distrito:" << distrito << endl;
    cout << "Numero de Contacto:" << numero << endl;
}

inline string User::getDistrito()
{
    return distrito;
}

// Serializa el usuario a una l�nea de texto
inline string User::serializar() const {
    stringstream ss;
    ss << nombre << "|" << numero << "|" << distrito << "|" << correo << "|" << contrase�a;
    return ss.str();
}

// Crea un usuario a partir de una l�nea de texto
inline User* User::deserializar(const string& linea) {
    stringstream ss(linea);
    string nombre, numeroStr, distrito, correo, contrase�a;
    getline(ss, nombre, '|');
    getline(ss, numeroStr, '|');
    getline(ss, distrito, '|');
    getline(ss, correo, '|');
    getline(ss, contrase�a, '|');
    int numero = stoi(numeroStr);
    return new User(nombre, numero, distrito, correo, contrase�a);
}
