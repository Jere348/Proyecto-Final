#pragma once
#include <string>
#include <algorithm>
#include "NodoAVL.h"

using namespace std;

// Árbol AVL genérico
template<typename T>
class AVLArbol
{
public:
    AVLArbol();
    ~AVLArbol();

    void insertar(T* dato);
    template<typename Func>
    void enOrden(Func visitar);
    T* buscar(const string& clave);

private:
    NodoAVL<T>* raiz;

    NodoAVL<T>* insertarRec(NodoAVL<T>* nodo, T* dato);
    T* buscarRec(NodoAVL<T>* nodo, const string& clave);
    template<typename Func>
    void enOrdenRec(NodoAVL<T>* nodo, Func visitar);
    void destruir(NodoAVL<T>* nodo);

    int altura(NodoAVL<T>* nodo);
    int balance(NodoAVL<T>* nodo);
    NodoAVL<T>* rotarDerecha(NodoAVL<T>* y);
    NodoAVL<T>* rotarIzquierda(NodoAVL<T>* x);
};

// Implementación

template<typename T>
AVLArbol<T>::AVLArbol() : raiz(nullptr) {}

template<typename T>
AVLArbol<T>::~AVLArbol() {
    destruir(raiz);
}

template<typename T>
void AVLArbol<T>::insertar(T* dato) {
    raiz = insertarRec(raiz, dato);
}

template<typename T>
T* AVLArbol<T>::buscar(const string& clave) {
    return buscarRec(raiz, clave);
}

template<typename T>
template<typename Func>
void AVLArbol<T>::enOrden(Func visitar) {
    enOrdenRec(raiz, visitar);
}

// Métodos privados

template<typename T>
NodoAVL<T>* AVLArbol<T>::insertarRec(NodoAVL<T>* nodo, T* dato) {
    if (!nodo)
        return new NodoAVL<T>(dato);

    if (dato->getNombre() < nodo->dato->getNombre())
        nodo->izq = insertarRec(nodo->izq, dato);
    else if (dato->getNombre() > nodo->dato->getNombre())
        nodo->der = insertarRec(nodo->der, dato);
    else
        return nodo; // No se permiten duplicados por nombre

    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);

    // Rotaciones para balancear
    if (bal > 1 && dato->getNombre() < nodo->izq->dato->getNombre())
        return rotarDerecha(nodo);
    if (bal < -1 && dato->getNombre() > nodo->der->dato->getNombre())
        return rotarIzquierda(nodo);
    if (bal > 1 && dato->getNombre() > nodo->izq->dato->getNombre()) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (bal < -1 && dato->getNombre() < nodo->der->dato->getNombre()) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

template<typename T>
T* AVLArbol<T>::buscarRec(NodoAVL<T>* nodo, const string& clave) {
    if (!nodo) return nullptr;
    if (clave == nodo->dato->getNombre())
        return nodo->dato;
    if (clave < nodo->dato->getNombre())
        return buscarRec(nodo->izq, clave);
    else
        return buscarRec(nodo->der, clave);
}

template<typename T>
template<typename Func>
void AVLArbol<T>::enOrdenRec(NodoAVL<T>* nodo, Func visitar) {
    if (!nodo) return;
    enOrdenRec(nodo->izq, visitar);
    visitar(nodo->dato);
    enOrdenRec(nodo->der, visitar);
}

template<typename T>
void AVLArbol<T>::destruir(NodoAVL<T>* nodo) {
    if (!nodo) return;
    destruir(nodo->izq);
    destruir(nodo->der);
    delete nodo->dato;
    delete nodo;
}

template<typename T>
int AVLArbol<T>::altura(NodoAVL<T>* nodo) {
    return nodo ? nodo->altura : 0;
}

template<typename T>
int AVLArbol<T>::balance(NodoAVL<T>* nodo) {
    return nodo ? altura(nodo->izq) - altura(nodo->der) : 0;
}

template<typename T>
NodoAVL<T>* AVLArbol<T>::rotarDerecha(NodoAVL<T>* y) {
    NodoAVL<T>* x = y->izq;
    NodoAVL<T>* T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = 1 + max(altura(y->izq), altura(y->der));
    x->altura = 1 + max(altura(x->izq), altura(x->der));
    return x;
}

template<typename T>
NodoAVL<T>* AVLArbol<T>::rotarIzquierda(NodoAVL<T>* x) {
    NodoAVL<T>* y = x->der;
    NodoAVL<T>* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = 1 + max(altura(x->izq), altura(x->der));
    y->altura = 1 + max(altura(y->izq), altura(y->der));
    return y;
}