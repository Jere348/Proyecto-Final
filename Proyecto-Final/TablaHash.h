#pragma once


#include <iostream>
#include "HashEntidad.h"

class HashTablaString {
private:
	HashEntidadString** tabla;
	int numElementos;
	int TABLE_SIZE;

	int calcularHash(std::string key) {
		int suma = 0;
		for (char c : key) {
			suma += int(c);  // Código ASCII
		}
		return suma % TABLE_SIZE;
	}

public:
	HashTablaString(int TABLE_SIZE = 1000) {
		this->TABLE_SIZE = TABLE_SIZE;
		tabla = new HashEntidadString * [TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; ++i) {
			tabla[i] = nullptr;
		}
		numElementos = 0;
	}

	~HashTablaString() {
		for (int i = 0; i < TABLE_SIZE; ++i) {
			if (tabla[i] != nullptr) {
				delete tabla[i];
			}
		}
		delete[] tabla;
	}

	void insertar(std::string key, producto* prod) {
		if (numElementos == TABLE_SIZE) return;

		int base = calcularHash(key);
		int hash = base;
		int step = 0;

		while (tabla[hash] != nullptr) {
			hash = (base + step) % TABLE_SIZE;
			step++;
		}

		tabla[hash] = new HashEntidadString(key, prod);
		numElementos++;
	}

	int buscar(std::string key) {
		int base = calcularHash(key);
		int i = base;
		int step = 0;

		while (true) {
			if (tabla[i] == nullptr) return -1;
			else if (tabla[i]->getKey() == key) return tabla[i]->getValue()->getid();
			else step++;

			i = (base + step) % TABLE_SIZE;
		}
	}

	void imprimir() {
		std::cout << "TABLA HASH (Nombre -> Valor almacenado):\n";
		for (int i = 0; i < TABLE_SIZE; ++i) {
			if (tabla[i] != nullptr) {
				cout << "Codigo: " << tabla[i]->getValue()->getid() << " | Nombre: " << tabla[i]->getValue()->getnombre()<< " | Precio: $" << tabla[i]->getValue()->getprecio() << " | Stock: " << tabla[i]->getValue()->getstock() << endl;
			}
			
		}
	}
};



