#pragma once
template <class T>
class NodoC
{
public:
	T dato;
	NodoC<T>* siguiente;
	NodoC(T v, NodoC<T>* sig = NULL)
	{
		dato = v;
		siguiente = sig;
	}
};
