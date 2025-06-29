#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class User
{
public:
    User();
    User(string pnombre, int pnumero, string pdistrito, string pcorreo, string pcontraseña);
    ~User();
    void ListarCLiente();
    string getNombre() { return nombre; }
    string getDistrito();
    string getContraseña() { return contraseña; }

    // Métodos para archivo
    string serializar() const;
    static User* deserializar(const string& linea);

protected:
    string nombre;
    int numero;
    string distrito;
    string correo;
    string contraseña;
};

User::User() : nombre(""), numero(0), distrito(""), correo(""), contraseña("") {}

User::User(string pnombre, int pnumero, string pdistrito, string pcorreo, string pcontraseña)
    : nombre(pnombre), numero(pnumero), distrito(pdistrito), correo(pcorreo), contraseña(pcontraseña) {}

User::~User() {}

inline void User::ListarCLiente()
{
    cout << "------------- Cliente " << nombre << "----------- " << endl;
    cout << "Correo:" << correo << endl;
    cout << "Contraseña:" << contraseña << endl;
    cout << "Distrito:" << distrito << endl;
    cout << "Numero de Contacto:" << numero << endl;
}

inline string User::getDistrito()
{
    return distrito;
}

// Serializa el usuario a una línea de texto
inline string User::serializar() const {
    stringstream ss;
    ss << nombre << "|" << numero << "|" << distrito << "|" << correo << "|" << contraseña;
    return ss.str();
}

// Crea un usuario a partir de una línea de texto
inline User* User::deserializar(const string& linea) {
    stringstream ss(linea);
    string nombre, numeroStr, distrito, correo, contraseña;
    getline(ss, nombre, '|');
    getline(ss, numeroStr, '|');
    getline(ss, distrito, '|');
    getline(ss, correo, '|');
    getline(ss, contraseña, '|');
    int numero = stoi(numeroStr);
    return new User(nombre, numero, distrito, correo, contraseña);
}
