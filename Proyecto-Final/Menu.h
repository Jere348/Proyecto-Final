#include "Cliente.h"
#include "Repartidor.h"
#include "Carrito.h"
#include "Cola_.h"
#include "Ordenamientos.h"
#include <fstream>
#include <iostream>
#include "TablaHash.h"
#include <string>
#include "User.h"
#include "AVL.h"
#include "GestorUsuarios.h"
#include <conio.h>
//Hola
void ordenarListaPorPrecio(Lista<producto*>* lista) {
    if (lista->esVacia()) return;

    bool cambio;
    do {
        cambio = false;
        for (int i = 0; i < lista->longitud() - 1; i++) {
            producto* prod1 = lista->obtenerPos(i);
            producto* prod2 = lista->obtenerPos(i + 1);
            if (prod1->getprecio() < prod2->getprecio()) {
                lista->modificarPos(prod2, i);
                lista->modificarPos(prod1, i + 1);
                cambio = true;
            }
        }
    } while (cambio);
}

string leerContrasena() {
    string contrasena;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' es Enter
        if (ch == '\b') { // Retroceso
            if (!contrasena.empty()) {
                contrasena.pop_back();
                cout << "\b \b";
            }
        }
        else {
            contrasena += ch;
            cout << '*';
        }
    }
    cout << endl;
    return contrasena;
}


class Controlador
{
public:
    Controlador();
    //Funciones para repartidores
    void ListarRepartidores();
    //Funciones Para Productos
    void RegistrarProducto(int a);
    //Funciones Para Cliente
    void RegistarCliente();
    void Vercarrito();
	//Funciones de usuario
    void RegistrarUsuario();
    bool IniciarSesion();
    //Funciones Main
    void Menu();
    void InterfazUsuario();
    void PedidoRealizado();

    double CalcularTotal(Cola<Repartidor*>* Cola_Repartidor, int indx, double precio, int i = 0) {
        if (i == indx) {
            return precio + Cola_Repartidor->obtenerPos(indx)->gettarifa();
        }

        return CalcularTotal(Cola_Repartidor, indx, precio, i + 1);
    }
    void GuardarHistorial() {
        ofstream archivo("historial_Productos.txt", ios::app);
        if (archivo.is_open()) {
            for (int i = 0; i < List_Carrito->longitud(); i++) {
                archivo << List_Carrito->obtenerPos(i)->getid() << " "
                    << List_Carrito->obtenerPos(i)->getnombre() << " "
                    << List_Carrito->obtenerPos(i)->getprecio() << " "
                    << List_Carrito->obtenerPos(i)->getstock() << endl;
            }
            archivo.close();
        }
        else {
            cout << "No se pudo abrir el archivo de historial." << endl;
        }
    }
    void VerHistorial();

    void GenerarArbolBalanceado();
private:
    Lista<producto*>* List_Comida;
    Lista<producto*>* List_Salud;
    Lista<producto*>* List_Bebidas;
    Cola<Repartidor*>* Cola_Repartidor;
    Lista<producto*>* List_Carrito;
    Lista<producto*>* List_productos;
    Cola<producto*>* Cola_resumen;
    Lista<producto*>* Lista_Historial;

    Cliente* user;
    Repartidor* repart;
    AVLArbol<User> arbolUsuarios;
    AVLArbol<producto> arbol_historial;
    int nro_Productos = 5;
    int nro_Repartidores = 0;
    int nro_Carrito;
    int eleccionRepartidor;
};

inline Controlador::Controlador()
{
    srand(time(0));
    List_Bebidas = new Lista<producto*>();
    List_Carrito = new Lista<producto*>();
    List_Comida = new Lista<producto*>();
    List_Salud = new Lista<producto*>();
    Cola_Repartidor = new Cola<Repartidor*>();
    List_productos = new Lista<producto*>();
    Cola_resumen = new Cola<producto*>();
    Lista_Historial = new Lista<producto*>();

    for (int i = 0; i < 5; i++)
    {
        repart = new Repartidor(nro_Repartidores);
        Cola_Repartidor->encolar(repart);
        nro_Repartidores++;
    }

    cargarUsuariosDesdeArchivo(arbolUsuarios, "usuarios.txt");
}

inline void Controlador::ListarRepartidores()
{
    cout << "- - - - - - - - - - - - - - - - - - - LISTA DE REPARTIDORES - - - - - - - - - - - - - - - - - - - - -" << endl;
	Cola<Repartidor*>* temp_Cola = new Cola<Repartidor*>();//Cola temporal para almacenar repartidores
	// Mostrar repartidores y almacenarlos en la cola temporal
    while (!Cola_Repartidor->esVacia())
    {
        Repartidor* repar = Cola_Repartidor->desencolar();
        repar->MostrarTdo();
        temp_Cola->encolar(repar);
    }
	// Reinsertar repartidores en la cola original
    while (!temp_Cola->esVacia()) {
        Cola_Repartidor->encolar(temp_Cola->desencolar());
    }
    delete temp_Cola; //Se eliminar la cola temp

    cout << endl << "Elije a tu repartidor:"; cin >> eleccionRepartidor;
}

inline void Controlador::RegistrarProducto(int a)
{
    int seleccion;
    int Ordyaf;
    int codigo, codigoBuscado, inventario;
    string nombre;
    float precio;
    bool menor;
    int stockelegido = 0;
    bool encontrado = false;
    ifstream nomArch;
    ofstream tempArch;
    string nombreBuscar;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE PRODUCTOS - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "                                                         1.Comida" << endl;
    cout << "                                                         2.Salud" << endl;
    cout << "                                                        3.Bebidas" << endl;
    cin >> seleccion;
    int i = 0;
    
    switch (seleccion)
    {
    case 1:
        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE COMIDA - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl;
        //ACA VA LA LISTAR SALUD
        nomArch.open("productos_Comida.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {
                
                
                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario),i);
                i++;
            }



            int selecionOrdenar;
            cout << "Ordenar de mayor a menor? (1) || Ver lo productos mas Baratos o Caros (2) \n";
            cin >> selecionOrdenar;
            switch (selecionOrdenar)
            {
            case 1:
                cout << "Ordenar de mayor a menor(0: Mayor A Menor || 1: Menor a Mayor):\n";
                cin >> menor;
                quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
                break;
            case 2:
                cout << "Ver los productos mas baratos o Caros (0: Mas Caros || 1: Mas Baratos):\n";
                cin >> menor; int indiceBaratosCaros;
                cout << "Cuantos productos desea ver? (Ingrese un numero):\n"; cin >> indiceBaratosCaros;
                ObtenerMasCoB(List_productos, menor, indiceBaratosCaros);
                cout << "\nVolviendo a mostrar la lista...\n";
            }

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                
                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();



            //--HAST TABLE DE PRODUCTOS
            
            HashTablaString* ht = new HashTablaString(100);
            for (int i = 0; i < List_productos->longitud(); i++)
            {
                ht->insertar(List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i));
                

			}

            
            while (true)
            {
                cout << "Desea ingresar un producto por nombre? (1 = Si || 0 = No): ";
                cin >> menor;
                if (menor == 1)
                {
                    std::cout << "\nBuscar nombre: ";
                    std::cin >> nombreBuscar;

                    int index = ht->buscar(nombreBuscar);
                    if (index != -1)
                        std::cout << nombreBuscar << " se encuentra en la posicion " << index << std::endl;
                    else
                        std::cout << nombreBuscar << " no se encontro en la tabla." << std::endl;


                }
                else
                {
                    break;
                }
            }
            delete ht;






            //-----------------------

            cout << "\nIngrese el c�digo del producto que desea seleccionar: ";
            cin >> codigoBuscado;
            cout << "Ingrese la cantidad de stock que desea: ";
            cin >> stockelegido;
            // Leer y actualizar inventario 
            nomArch.open("productos_Comida.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            //

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid()==codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock(stockelegido);
                        cout << "\nProducto seleccionado:\n";
                        cout << "C�digo: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(), List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i)->getprecio(), List_productos->obtenerPos(i)->getstock()), a);
                       

                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " " << List_productos->obtenerPos(i)->getnombre() << " " << List_productos->obtenerPos(i)->getprecio() << " " << List_productos->obtenerPos(i)->getstock() << endl;
            }
            //
                       

            nomArch.close();
            tempArch.close();

            // Reemplazar archivo original
            remove("productos_Comida.txt");
            rename("temp.txt", "productos_Comida.txt");

            if (!encontrado)
            {
                cout << "Producto con c�digo " << codigoBuscado << " no encontrado o sin inventario." << endl;
            }
        }
        else
        {
            cout << "No se pudo abrir el archivo." << endl;
        }
        
        break;

    case 2:

        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE SALUD - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl;
        //ACA VA LA LISTAR SALUD
        nomArch.open("productos_Salud.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {


                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario), i);
                i++;
            }
            int selecionOrdenar;
            cout << "Ordenar de mayor a menor? (1) || Ver lo productos mas Baratos o Caros (2) \n";
            cin >> selecionOrdenar;
            switch (selecionOrdenar)
            {
            case 1:
                    cout << "Ordenar de mayor a menor(0: Mayor A Menor || 1: Menor a Mayor):\n";
                    cin >> menor;
                    quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
					break;
            case 2:
                    cout << "Ver los productos mas baratos o Caros (0: Mas Caros || 1: Mas Baratos):\n";
					cin >> menor; int indiceBaratosCaros;
					cout << "Cuantos productos desea ver? (Ingrese un numero):\n"; cin >> indiceBaratosCaros;
					ObtenerMasCoB(List_productos, menor,indiceBaratosCaros);
                    cout << "\nVolviendo a mostrar la lista...\n";
            }

            for (int i = 0; i < List_productos->longitud(); i++)
            {

                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();



            //--HAST TABLE DE PRODUCTOS

            HashTablaString* ht = new HashTablaString(100);
            for (int i = 0; i < List_productos->longitud(); i++)
            {
                ht->insertar(List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i));


            }


            while (true)
            {
                cout << "Desea ingresar un producto por nombre? (1 = Si || 0 = No): ";
                cin >> menor;
                if (menor == 1)
                {
                    std::cout << "\nBuscar nombre: ";
                    std::cin >> nombreBuscar;

                    int index = ht->buscar(nombreBuscar);
                    if (index != -1)
                        std::cout << nombreBuscar << " se encuentra en la posicion " << index << std::endl;
                    else
                        std::cout << nombreBuscar << " no se encontro en la tabla." << std::endl;


                }
                else
                {
                    break;
                }
            }
            delete ht;



            cout << "\nIngrese el c�digo del producto que desea seleccionar: ";
            cin >> codigoBuscado;
            cout << "Ingrese la cantidad de stock que desea: ";
            cin >> stockelegido;
            // Leer y actualizar inventario
            nomArch.open("productos_Salud.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            //

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid() == codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock(stockelegido);
                        cout << "\nProducto seleccionado:\n";
                        cout << "C�digo: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(), List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i)->getprecio(), List_productos->obtenerPos(i)->getstock()), a);
                        a++;
                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " " << List_productos->obtenerPos(i)->getnombre() << " " << List_productos->obtenerPos(i)->getprecio() << " " << List_productos->obtenerPos(i)->getstock() << endl;
            }
            //


            nomArch.close();
            tempArch.close();

            // Reemplazar archivo original
            remove("productos_Salud.txt");
            rename("temp.txt", "productos_Salud.txt");

            if (!encontrado)
            {
                cout << "Producto con c�digo " << codigoBuscado << " no encontrado o sin inventario." << endl;
            }
        }
        else
        {
            cout << "No se pudo abrir el archivo." << endl;
        }
        
        break;
    case 3:

        i = 0;
        List_productos->eliminarTodo();
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - SECCIONES DE BEBIDAS - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl;
        //ACA VA LA LISTAR SALUD
        nomArch.open("productos_Bebidas.txt", ios::in);
        if (nomArch.is_open())
        {
            cout << "Lista de productos disponibles:\n";
            cout << "----------------------------------\n";
            while (nomArch >> codigo >> nombre >> precio >> inventario)
            {
                
                
                List_productos->agregaPos(new producto(codigo, nombre, precio, inventario),i);
                i++;
            }
            int selecionOrdenar;
            cout << "Ordenar de mayor a menor? (1) || Ver lo productos mas Baratos o Caros (2) \n";
            cin >> selecionOrdenar;
            switch (selecionOrdenar)
            {
            case 1:
                cout << "Ordenar de mayor a menor(0: Mayor A Menor || 1: Menor a Mayor):\n";
                cin >> menor;
                quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
                break;
            case 2:
                cout << "Ver los productos mas baratos o Caros (0: Mas Caros || 1: Mas Baratos):\n";
                cin >> menor; int indiceBaratosCaros;
                cout << "Cuantos productos desea ver? (Ingrese un numero):\n"; cin >> indiceBaratosCaros;
                ObtenerMasCoB(List_productos, menor, indiceBaratosCaros);
                cout << "\nVolviendo a mostrar la lista...\n";
            }

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                
                List_productos->obtenerPos(i)->toString();
            }
            nomArch.close();

            


            //--HAST TABLE DE PRODUCTOS

            HashTablaString* ht = new HashTablaString(100);
            for (int i = 0; i < List_productos->longitud(); i++)
            {
                ht->insertar(List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i));


            }

            //ht->imprimir();

            while (true)
            {
                cout << "Desea ingresar un producto por nombre? (1 = Si || 0 = No): ";
                cin >> menor;
                if (menor == 1)
                {
                    std::cout << "\nBuscar nombre: ";
                    std::cin >> nombreBuscar;

                    int index = ht->buscar(nombreBuscar);
                    if (index != -1)
                        std::cout << nombreBuscar << " se encuentra en la posicion " << index << std::endl;
                    else
                        std::cout << nombreBuscar << " no se encontro en la tabla." << std::endl;

                    
                }
                else
                {
                    break;
                }
            }
			delete ht; // Liberar memoria de la tabla hash

            //
            




            

            cout << "\nIngrese el c�digo del producto que desea seleccionar: ";
            cin >> codigoBuscado;
            cout << "Ingrese la cantidad de stock que desea: ";
            cin >> stockelegido;
            // Leer y actualizar inventario
            nomArch.open("productos_Bebidas.txt", ios::in);
            tempArch.open("temp.txt", ios::out);

            //

            for (int i = 0; i < List_productos->longitud(); i++)
            {
                if (List_productos->obtenerPos(i)->getid()==codigoBuscado)
                {
                    if (List_productos->obtenerPos(i)->getstock() > 0)
                    {
                        List_productos->obtenerPos(i)->disminuir_stock(stockelegido);
                        cout << "\nProducto seleccionado:\n";
                        cout << "C�digo: " << List_productos->obtenerPos(i)->getid() << endl;
                        cout << "Nombre: " << List_productos->obtenerPos(i)->getnombre() << endl;
                        cout << "Precio: $" << List_productos->obtenerPos(i)->getprecio() << endl;
                        cout << "Inventario restante: " << List_productos->obtenerPos(i)->getstock() << endl;
                        encontrado = true;
                        List_Carrito->agregaPos(new producto(List_productos->obtenerPos(i)->getid(), List_productos->obtenerPos(i)->getnombre(), List_productos->obtenerPos(i)->getprecio(), List_productos->obtenerPos(i)->getstock()), a);
                        a++;
                    }
                    else
                    {
                        cout << "\nProducto sin stock disponible." << endl;
                    }
                }
                tempArch << List_productos->obtenerPos(i)->getid() << " " << List_productos->obtenerPos(i)->getnombre() << " " << List_productos->obtenerPos(i)->getprecio() << " " << List_productos->obtenerPos(i)->getstock() << endl;
            }
            //
                       

            nomArch.close();
            tempArch.close();

            // Reemplazar archivo original
            remove("productos_Bebidas.txt");
            rename("temp.txt", "productos_Bebidas.txt");

            if (!encontrado)
            {
                cout << "Producto con c�digo " << codigoBuscado << " no encontrado o sin inventario." << endl;
            }
        }
        else
        {
            cout << "No se pudo abrir el archivo." << endl;
        }

        break;
    }
}

inline void Controlador::RegistarCliente()
{
    string Correo, contrase�a, nombre;
    int telefono;
    string distrito;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - REGISTRO DE USUARIO - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "Ingrese su correo :"; cin >> Correo;
    cout << "Ingrese su contrase�a :"; cin >> contrase�a;
    cout << "Ingrese su nombre:"; cin >> nombre;
    cout << "Ingrese su numero de telefono :"; cin >> telefono;
    cout << "Ingrese su distrito :" << endl;
    cout << "1.San Miguel" << endl;
    cout << "2.Barranco" << endl;
    cout << "3.Jesus Maria" << endl;
    cin >> distrito;
    Cliente* user = new Cliente(nombre, telefono, distrito, Correo);
}

inline void Controlador::RegistrarUsuario() {
    string nombre, correo, contrase�a, distrito;
    int telefono;
    cout << "Nombre: "; cin >> nombre;
    cout << "Telefono: "; cin >> telefono;
    cout << "Distrito: "; cin >> distrito;
    cout << "Correo: "; cin >> correo;
    cout << "Contrase�a: "; cin >> contrase�a;

    // Verifica si ya existe el usuario
    if (arbolUsuarios.buscar(nombre) != nullptr) {
        cout << "El usuario ya existe." << endl;
        return;
    }

    User* nuevo = new User(nombre, telefono, distrito, correo, contrase�a);
    arbolUsuarios.insertar(nuevo);
    guardarUsuariosEnArchivo(arbolUsuarios, "usuarios.txt");
    cout << "Usuario registrado correctamente." << endl;
}

bool Controlador::IniciarSesion() {
    string nombre, contrase�a;
    cout << "Nombre: "; cin >> nombre;
    cout << "Contrase�a: ";
    contrase�a = leerContrasena(); // Aqu� se oculta la contrase�a con asteriscos

    User* usuario = arbolUsuarios.buscar(nombre);
    if (usuario && usuario->getContrase�a() == contrase�a) {
        cout << "�Bienvenido, " << usuario->getNombre() << "!" << endl;
        return true;
    }
    else {
        cout << "Usuario o contrase�a incorrectos." << endl;
        return false;
    }
}

inline void Controlador::Vercarrito()
{
    bool menor = false;
    int eliminar;
    int opccarito;
    cout << "- - - - - - - - - - - - - - - - - - - - - CARRITO - - - - - - - - - - - - - - - - - - -" << endl;

    cout << "Ordenar de mayor a menor(0: Mayor A Menor || 1: Menor a Mayor):\n";
    cin >> menor;
    mergeSort(List_Carrito, 0, List_Carrito->longitud() - 1, menor); // true: menor a mayor, false: mayor a menor

    for (int i = 0; i < List_Carrito->longitud(); i++)
    {
        List_Carrito->obtenerPos(i)->toString();
    }
    cin >> opccarito;
    switch (opccarito)
    {
    case 1:
        system("cls");
        cout << "Resumen del pedido: "<<endl;
        
        for (int i = 0; i < List_Carrito->longitud(); i++)
        {
            Cola_resumen->encolar(new producto(List_Carrito->obtenerPos(i)->getid(),
                List_Carrito->obtenerPos(i)->getnombre(), List_Carrito->obtenerPos(i)->getprecio(), List_Carrito->obtenerPos(i)->getstock()));

        }
        producto* Producto;
        do
        {
            Producto = Cola_resumen->desencolar();
            Producto->toString();

        } while (!Cola_resumen->esVacia());

        ListarRepartidores();

        break;
        
        




    case 2:

        cout << "Ingrese el c�digo del producto a eliminar: "; cin >> eliminar;


        for (int i = 0; i < List_Carrito->longitud(); i++)
        {
            if (List_Carrito->obtenerPos(i)->getid() == eliminar)
            {
                List_Carrito->eliminaPos(i);
            }
            
        }

        break;
    default:
        break;
    }
}

void Controlador::Menu()
{
    int mainmenu;
    
    cout << "                          "; cout << "                       -=                " << endl;
    cout << "                          "; cout << "               .     -##:                " << endl;
    cout << "                          "; cout << "              :=     -###=          =:   " << endl;
    cout << "                          "; cout << "             +*      *####+.         *+  " << endl;
    cout << "                          "; cout << "            +#:      #######+.       :#+ " << endl;
    cout << "                          "; cout << "           -##.      -########.      .##-" << endl;
    cout << "                          "; cout << "           *##:       =#######*      :##*" << endl;
    cout << "                          "; cout << "           ###*        .+######      *###" << endl;
    cout << "                          "; cout << "           *###*         .*###+     *###*" << endl;
    cout << "                          "; cout << "           -#####-        :##*    -#####-" << endl;
    cout << "                          "; cout << "            +######=:     :#=  :=######+ " << endl;
    cout << "                          "; cout << "             =#########*++#+*#########=  " << endl;
    cout << "                          "; cout << "              :*####################*:   " << endl;
    cout << "                          "; cout << "                .=################=:     " << endl;
    cout << "                          "; cout << "                   :=*##########*+.      " << endl;
    cout << "                                                                                                                     " << endl;

    cout << "                                                  Llevalo" << endl << endl << endl;
    cout << "                                             1.Ingresar Sesion" << endl;
    cout << "                                         2.Registrar Nuevo Usuario" << endl;
    cout << "                                                  3.Salir" << endl;

    cin >> mainmenu;
    do
    {
        switch (mainmenu)
        {

        case 1:
            if (IniciarSesion())
            {
                InterfazUsuario();
            }
            break;
        case 2:

			RegistrarUsuario();
            break;

        case 3:
            List_Bebidas = nullptr;
            List_Carrito = nullptr;
            List_Comida = nullptr;
            List_productos = nullptr;
            Cola_Repartidor = nullptr;
            List_Salud = nullptr;
            exit(0);
            break;
        default:

            break;
        }

    } while (mainmenu != 3);
    
}

inline void Controlador::InterfazUsuario()
{
    int a = 0;

    int interfaz;
    do
    {
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - BIENVENIDO  - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl << endl << endl << endl << endl;
        cout << "                                                          1.Pedir" << endl;
        cout << "                                                        2.Ver Carrito" << endl;
        cout << "                                                      3.Ver Mis Pedidos" << endl;
        cout << "                                                      4.Ver Historial de compra" << endl;
        cin >> interfaz;
        switch (interfaz)
        {
        case 1:
            system("cls");
            RegistrarProducto(a);
            a++;
            break;
        case 2:
            system("cls");
            Vercarrito();
            break;
        case 3:
            PedidoRealizado();
            break;
        case 4:
            VerHistorial();
            GenerarArbolBalanceado();
            break;
        default:
            break;
        }

    } while (0 < interfaz < 3);

}

inline void Controlador::PedidoRealizado()
{
    system("cls");

    double precio= 0.0;
    cout << "- - - - - - - - - - - - - - - - - - - - - PEDIDOS REALIZADOS - - - - - - - - - - - - -" << endl;
    cout << "Resumen del pedido: " << endl;

    for (int i = 0; i < List_Carrito->longitud(); i++)
    {
        Cola_resumen->encolar(new producto(List_Carrito->obtenerPos(i)->getid(),
            List_Carrito->obtenerPos(i)->getnombre(), List_Carrito->obtenerPos(i)->getprecio(), List_Carrito->obtenerPos(i)->getstock()));
        precio += List_Carrito->obtenerPos(i)->getprecio();
        
    }
    producto* Producto;
    do
    {
        Producto = Cola_resumen->desencolar();
        Producto->toString();

    } while (!Cola_resumen->esVacia());

    double total = CalcularTotal(Cola_Repartidor, eleccionRepartidor, precio);

    GuardarHistorial();

    cout << " EL TOTAL SERIA :" << total << endl;;
}

inline void Controlador::VerHistorial()
{
    int codigo, inventario;
    string nombre;
    float precio;
    ifstream nomArch("historial_productos.txt", ios::in);

    Lista_Historial->eliminarTodo();

    if (nomArch.is_open()) {

        int i = 0;
        while (nomArch >> codigo >> nombre >> precio >> inventario) {
            producto* p = new producto(codigo, nombre, precio, inventario);
            Lista_Historial->agregaPos(p, i);
            i++;
        }
        nomArch.close();

        // Display products from the list
        //for (int j = 0; j < Lista_Historial->longitud(); j++) {
        //    Lista_Historial->obtenerPos(j)->toString();
        //}
    }
    else {
        cout << "No se pudo abrir el archivo historial_productos.txt\n";
    }
}

inline void Controlador::GenerarArbolBalanceado()
{
    if (Lista_Historial->esVacia()) {
        cout << "El historial de productos est� vac�o. No se puede generar un �rbol AVL." << endl;
        return;
    }
    cout << "\n--- Generando �rbol AVL de Historial de Productos ---\n";
    for (int i = 0; i < Lista_Historial->longitud(); ++i) {
        producto* p = Lista_Historial->obtenerPos(i);
        arbol_historial.insertar(p);

    }
    cout << "�rbol AVL del historial de productos generado correctamente.\n";
    cout << "\n--- Contenido del �rbol AVL (ordenado por nombre) ---\n";
    arbol_historial.enOrden([](producto* p) {
        cout << "  - " << p->getnombre() << " (ID: " << p->getid() << ", Precio: $" << p->getprecio() << ")\n";
    });
    cout << "-----------------------------------------------------\n";
}