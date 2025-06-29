#pragma once
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class producto
{
private:
	int id;
	string nombre;
	double precio;
	int stock;
public:
	producto(int pid, string pnombre, double pprecio, int pstock);
	producto();
	~producto();

	int getid();
	string getnombre();
	double getprecio();
	int getstock();
	string getNombre()
	{
		return nombre;
	}

	void disminuir_stock(int num);
	void toString()
	{
		
		cout << "Codigo: " << id << " | Nombre: " << nombre << " | Precio: $" << precio << " | Stock: " << stock << endl;
	}
};

producto::producto(int pid, string pnombre, double pprecio, int pstock)
{
	this->id = pid;
	this->nombre = pnombre;
	this->precio = pprecio;
	this->stock = pstock;
}

inline producto::producto()
{
	this->id = 0;
	this->nombre = "";
	this->precio = 0;
	this->stock = 0;
}

producto::~producto()
{
}

int producto::getid()
{
	return this->id;


}
string producto::getnombre()
{
	return this->nombre;

}
double producto::getprecio()
{
	return this->precio;
}
int producto::getstock()
{
	return this->stock;

}
void producto::disminuir_stock(int num)
{
	this->stock=stock-num;
}