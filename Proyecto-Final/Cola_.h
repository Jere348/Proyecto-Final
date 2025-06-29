#pragma once
#include "NodoC.h"
#include <iostream>
#include <stdexcept> // For std::out_of_range
using namespace std;

template <class T>
class Cola
{
private:
	NodoC<T>* inicio;
	NodoC<T>* fin;
public:
	Cola()
	{
		this->inicio = NULL;
		this->fin = NULL;
	}

	// Destructor to prevent memory leaks when Cola object is destroyed
	~Cola() {
		while (!esVacia()) {
			desencolar(); // Use existing method to clear all nodes
		}
	}

	void encolar(T v);
	T desencolar();
	bool esVacia();
	T obtenerPos(int pos); // Declaration remains the same
};

//IMPLEMENTACIÓN

template <class T>
bool Cola<T>::esVacia()
{
	return (inicio == NULL);
}

template <class T>
void Cola<T>::encolar(T v)
{
	NodoC<T>* nodo = new NodoC<T>(v);
	if (esVacia())
	{
		inicio = nodo;
		fin = inicio;
	}
	else
	{
		fin->siguiente = nodo;
		fin = nodo;
	}
	// No need to set nodo to NULL, it's a local pointer that will go out of scope.
}

template <class T>
T Cola<T>::desencolar()
{
	if (esVacia()) {
		throw std::out_of_range("Cannot dequeue from an empty queue.");
	}

	T dato = inicio->dato;
	NodoC<T>* temp = inicio; // Store the node to be deleted

	if (inicio == fin)
	{
		inicio = NULL;
		fin = NULL;
	}
	else
	{
		inicio = inicio->siguiente;
	}

	delete temp; // Free the memory of the dequeued node
	return dato;
}

template <class T>
T Cola<T>::obtenerPos(int index) { // Corrected scope here!
	NodoC<T>* actual = inicio;
	int i = 0;

	while (actual != nullptr && i < index) {
		actual = actual->siguiente;
		i++;
	}

	if (actual != nullptr) {
		return actual->dato;
	}
	else {
		throw std::out_of_range("Índice fuera de rango en la cola.");
	}
}