#pragma once

template <typename T>
class NodoAVL {
public:
    T* dato;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;

    NodoAVL(T* pdato)
        : dato(pdato), izq(nullptr), der(nullptr), altura(1) {
    }
};