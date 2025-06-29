#include "Nodo.h"
#include <functional>
typedef unsigned int uint;
template <class T>
class Lista {
private:
    Nodo<T>* nodo;
    Nodo<T>* ini;
    uint lon; // número de elementos en la lista

public:
    Lista() : ini(nullptr), lon(0) { nodo = new Nodo<T>(); }
    ~Lista() {
        eliminarTodo();
        delete nodo;
    }
    /*Lista() {
        nodo = new Nodo<T>();
    };*/
    Lista(Nodo<T>* pNodo) {
        nodo = pNodo;
    };
    uint longitud();
    bool esVacia();
    void agregaInicial(T elem);
    void agregaPos(T elem, uint pos);
    void agregaFinal(T elem);
    void modificarInicial(T elem);
    void modificarPos(T elem, uint pos);
    void modificarFinal(T elem);
    void eliminaInicial();
    void eliminaPos(uint pos);
    void eliminaFinal();
    void eliminarTodo();
    T obtenerInicial();
    T obtenerPos(uint pos);
    T obtenerFinal();
    T buscar(T elem);


    //~Lista(void);
    void agregar(T pNodo); //sumar nodos a la lista
    bool esvacia(void);
    T cabeza(void); //retorna el dato del primer nodo
    Lista* resto(void); //retorna el puntero al "resto" de la lista
                        //resto= lo que queda de la lista sin la cabeza
    T suma(T i);
    int size();
    void borrar(void); //borra la cabeza
    void borrar_last();//borra el ultimo
    void concat(Lista<T>* l1);// le transfiere los datos de l1 a this
    Lista<T>* copy(void);// hace una copia de la lista
    void tomar(int n);//deja "vivos" los n primeros nodos y borra el resto
    void addOrdenado(T d);// suma nodos ordenados de menor a mayor
    bool esta(T d); //retorna true cuando d est� en la lista
    void borrarDato(T d);// borra el nodo que contiene a d
    void intercambiar(uint pos1, uint pos2);


};
template <class T>
uint Lista<T>::longitud() {
    return lon;
}
template <class T>
bool Lista<T>::esVacia() {
    return lon == 0;
}
template <class T>
void Lista<T>::agregaInicial(T elem) {
    Nodo<T>* nuevo = new Nodo<T>(elem);
    if (nuevo != nullptr) {
        ini = nuevo;
        lon++;
    }
}
template <class T>
T Lista<T>::buscar(T elem) {
    Nodo<T>* aux = ini;
    while (aux != nullptr) {
        if (comparar(aux->elem, elem) == 0) {
            return aux->elem;
        }
        aux = aux->sgte;
    }
    return 0;
}

template <typename T>
void Lista<T>::agregaPos(T elem, uint pos) {
    Nodo<T>* nuevo = new Nodo<T>(elem);

    if (ini == nullptr || pos <= 0) {
        // Insert at the beginning if list is empty or pos is 0 or negative
        nuevo->set_Sgte(ini);
        ini = nuevo;
    }
    else {
        Nodo<T>* aux = ini;
        int i = 0;
        // Traverse to the node before the desired position
        while (aux->get_Sgte() != nullptr && i < pos - 1) {
            aux = aux->get_Sgte();
            i++;
        }
        // Insert the new node
        nuevo->set_Sgte(aux->get_Sgte());
        aux->set_Sgte(nuevo);
    }
    lon++; // Update the length of the list
}
template <class T>
void Lista<T>::agregaFinal(T elem) {
    agregarPos(elem, lon);
}

template <class T>
void Lista<T>::eliminaInicial() {
    if (lon > 0) {
        Nodo<T>* aux = ini;
        ini = ini->sgte;
        delete aux;
        lon--;
    }
}

template <class T>
void Lista<T>::eliminaPos(uint pos) {
    if (pos >= lon) return; // Fuera de rango

    Nodo<T>* anterior = ini;
    for (uint i = 1; i < pos; ++i) {
        anterior = anterior->get_Sgte();
    }
    Nodo<T>* aEliminar = anterior->get_Sgte();
    anterior->set_Sgte(aEliminar->get_Sgte());
    delete aEliminar;
    lon--;
}
template <class T>
void Lista<T>::eliminaFinal() {
}

template <class T>
void Lista<T>::modificarInicial(T elem) {
    if (lon > 0) {
        ini->elem = elem;
    }
}
template <class T>
void  Lista<T>::eliminarTodo()
{
    Nodo<T>* actual = ini;
    while (actual != nullptr) {
        Nodo<T>* temp = actual;
        actual = actual->get_Sgte();
        delete temp;
    }
    ini = nullptr;
    lon = 0;
}

template <class T>
void Lista<T>::modificarPos(T elem, uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo<T>* aux = ini;
        for (int i = 0; i < pos; i++) {
            aux = aux->get_Sgte();
        }
        aux->set_Elem(elem);
    }
}
template <class T>
void Lista<T>::modificarFinal(T elem) {
    modificar(elem, lon - 1);
}

template <class T>
T Lista<T>::obtenerInicial() {
    return obtenerPos(0);
}


template <class T>
T Lista<T>::obtenerPos(uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo<T>* aux = ini;
        for (int i = 0; i < pos; i++) {
            aux = aux->get_Sgte();
        }
        return aux->get_Elem();
    }
    else {
        return NULL;
    }
}
template <class T>
T Lista<T>::obtenerFinal() {
    return obtenerPos(lon - 1);
}

template <class T>
void Lista<T>::agregar(T d) //100
{
    Nodo<T>* nuevo = new Nodo<T>(d);
    nuevo->set_Sgte(nodo);
    nodo = nuevo;
}


template<class T>
inline void Lista<T>::intercambiar(uint pos1, uint pos2)
{
    // Validate that positions are within list bounds
    if (pos1 >= lon || pos2 >= lon) {
        std::cerr << "Error: Índices fuera de rango en intercambiar." << std::endl;
        return;
    }
    // If positions are the same, there's nothing to swap
    if (pos1 == pos2) {
        return;
    }

    // Find the first node at position 'pos1'
    Nodo<T>* nodo1 = ini;
    for (uint i = 0; i < pos1; ++i) {
        // Use get_Sgte() to traverse
        nodo1 = nodo1->get_Sgte();
    }

    // Find the second node at position 'pos2'
    Nodo<T>* nodo2 = ini;
    for (uint i = 0; i < pos2; ++i) {
        // Use get_Sgte() to traverse
        nodo2 = nodo2->get_Sgte();
    }

    // Get the elements from the nodes
    T elem1 = nodo1->get_Elem();
    T elem2 = nodo2->get_Elem();

    // Swap the retrieved elements
    std::swap(elem1, elem2);

    // Set the swapped elements back into the nodes
    nodo1->set_Elem(elem1);
    nodo2->set_Elem(elem2);
}


