#pragma once
#include "User.h"
#include <vector>

vector<string> distritos = { "San Miguel","Barranco","Jesus Maria" };
vector<string> nombres = {
	"Valentina", "Mateo", "Sofía", "Sebastián", "Isabella",
	"Lucas", "Camila", "Santiago", "Emma", "Tomás",
	"Martina", "Diego", "Victoria", "Gabriel", "Renata",
	"Emilio", "Antonella", "Daniel", "Lucía", "Andrés"
};


class Repartidor
{
public:
	Repartidor(int id);//CONSTRUCTOR

	//FUNCIONES DE CALCULO Y MOSTRAR
	void MostrarTdo();

	//FUNCIONES GET
	string getdistrito() { return distrito; };
	int getCalificacion() { return calificacion; }
	string getnombre() { return nombre; };
	double gettarifa() { return tarifa; };
	int getId() { return id; }

private:
	string distrito;
	string nombre;
	double tarifa;
	int calificacion;
	int id;
};

inline Repartidor::Repartidor(int ide) {

	int idxDistrito = rand() % distritos.size();
	int idxNombre = rand() % nombres.size();

	distrito = distritos[idxDistrito];
	nombre = nombres[idxNombre];
	calificacion = rand() % 10 + 1;
	tarifa = (rand() % 500 + 70) / 100.0;
	id = ide;
}

void Repartidor::MostrarTdo()
{
	cout << "\n------------- Repartidor " << getnombre() << "-----------\n " ;
	cout << "\n - ID: " << getId();
	cout << "\n - Distrito: " << getdistrito();
	cout << "\n - Calificacion: " << getCalificacion();
	cout << "\n - Tarifa: " << gettarifa();

}