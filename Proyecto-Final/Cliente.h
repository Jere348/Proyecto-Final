#pragma once
#include "User.h"

class Cliente:public User
{
public:
	Cliente(string pnombre, int pnumero, string pdistrito, string pcorreo) {
		nombre = pnombre;
		numero = pnumero;
		distrito = pdistrito;
		correo = pcorreo;
	}
	void MostrarTdo();
	bool PedidoActivo() {
		return Pedido;
	}
private:
	bool Pedido;
	int CalificacionCliente;
};

void Cliente::MostrarTdo()
{
	cout << "------------- Cliente " << nombre << "----------- " << endl;
	cout << "Correo:" << correo;
	cout << "Contraseña:" << contraseña;
	cout << "Distrito:" << distrito;
	cout << "Numero de Contacto:" << numero;
	cout << "Calificacion del Cliente:" << CalificacionCliente;
}