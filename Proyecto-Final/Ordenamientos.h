#pragma once
#include <iostream>
#include "Lista.h"
#include "producto.h"

using namespace std;

//QUICKSORT
//Se usa para ordenar por precio

int particion(Lista<producto*>* A, int p, int r,bool menor) {

    double x = A->obtenerPos(r)->getprecio();

    int i = p - 1;

    if (!menor)
    {
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() >= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }else{
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() <= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }
    
    A->intercambiar(i + 1, r);
    return i + 1;
}

void quicksort(Lista<producto*>* A, int p, int r,bool menor) {
    int q; 

    if (p < r) {
        q = particion(A, p, r,menor); 
        quicksort(A, p, q - 1,menor); 
        quicksort(A, q + 1, r,menor);
    }
}

//MERGESORT

void merge(Lista<producto*>* lista, int left, int mid, int right, bool menor) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Crear sublistas temporales
    Lista<producto*>* L = new Lista<producto*>();
    Lista<producto*>* R = new Lista<producto*>();

    for (int i = 0; i < n1; i++)
        L->agregaPos(lista->obtenerPos(left + i), i);
    for (int j = 0; j < n2; j++)
        R->agregaPos(lista->obtenerPos(mid + 1 + j), j);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if ((menor && L->obtenerPos(i)->getprecio() <= R->obtenerPos(j)->getprecio()) ||
            (!menor && L->obtenerPos(i)->getprecio() >= R->obtenerPos(j)->getprecio())) {
            lista->modificarPos(L->obtenerPos(i), k);
            i++;
        }
        else {
            lista->modificarPos(R->obtenerPos(j), k);
            j++;
        }
        k++;
    }
    while (i < n1) {
        lista->modificarPos(L->obtenerPos(i), k);
        i++; k++;
    }
    while (j < n2) {
        lista->modificarPos(R->obtenerPos(j), k);
        j++; k++;
    }
    delete L;
    delete R;
}

// mergesort para Lista<producto*>
void mergeSort(Lista<producto*>* lista, int left, int right, bool menor) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(lista, left, mid, menor);
        mergeSort(lista, mid + 1, right, menor);
        merge(lista, left, mid, right, menor);
    }
}
//QUICKSELECT

int particionSelect(Lista<producto*>* A, int p, int r,bool menor) {
    int x = A->obtenerPos(r)->getprecio();
    int i = p - 1;
    if (menor) {
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() <= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }
    else {
        for (int j = p; j < r; j++) {
            if (A->obtenerPos(j)->getprecio() >= x) {
                i++;
                A->intercambiar(i, j);
            }
        }
    }
    A->intercambiar(i+1,r);
    return i + 1;
}

int quickselect(Lista<producto*>* A, int p, int r, int k,bool menor) {
    if (p == r) return A->obtenerPos(p)->getprecio();
    int q = particionSelect(A, p, r,menor);
    int l = q - p + 1;
    if (k == l)
        return A->obtenerPos(q)->getprecio();
    else if (k < l) {
        return quickselect(A, p, q - 1, k,menor);
    }
    else {
        return quickselect(A, q + 1, r, k - l, menor);
    }

}

void ObtenerMasCoB(Lista<producto*>* A, bool menor,int indice) {
    int indx = indice;
    producto* p;
    quickselect(A, 0, A->longitud() - 1, indx, menor);

    quicksort(A, 0, indx - 1, menor);


    for (int i = 0; i < indx; i++)
    {
        p = A->obtenerPos(i);
        cout << "Producto: " << p->getid() << ", Precio: " << p->getprecio() << ", Nombre: " << p->getnombre() << endl;
    }
}