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
#include <windows.h>
#include <vector>
#include "Grafo.h"
//Hola

User* usuario = nullptr; // Variable global para el usuario actual;
CGrafo<int>* G = new CGrafo<int>();

void gotoxy(int x, int y) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int centrarX(const string& texto) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int ancho = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return (ancho - static_cast<int>(texto.length())) / 2;
}
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

string leerContrasenaOculta(short x, short y) {
    string pass = "";
    char ch;
    while ((ch = _getch()) != 13) {  // 13 = Enter
        if (ch == 8 && !pass.empty()) {  // Backspace
            pass.pop_back();
            cout << "\b \b";
        }
        else if (isprint(ch)) {
            pass.push_back(ch);
            cout << '*';
        }
    }
    return pass;
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
        if (indx < 0 || indx >= Cola_Repartidor->longitud()) {
            cerr << "Índice fuera de rango en CalcularTotal.\n";
            return precio;
        }
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
    User* usuario = nullptr;
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
    //

    G->adicionarVertice(0); //indice=0 SM
    G->adicionarVertice(1); //indice=1 bARRANCO
    G->adicionarVertice(2); //indice=2 JESUS MARIA


    //Agregar los arcos
    G->adicionarArco(0, 0); //indice=0
    G->modificarArco(0, 0, 0);
    G->adicionarArco(0, 1);//indice=1
    G->modificarArco(0, 1, 12);
    G->adicionarArco(0, 2);//indice=1
    G->modificarArco(0, 2, 13);

    G->adicionarArco(1, 0); //indice=0
    G->modificarArco(1, 0, 22);
    G->adicionarArco(1, 1);//indice=1
    G->modificarArco(1, 1, 0);
    G->adicionarArco(1, 2);//indice=1
    G->modificarArco(1, 2, 23);


    G->adicionarArco(2, 0); //indice=0
    G->modificarArco(2, 0, 34);
    G->adicionarArco(2, 1);//indice=1
    G->modificarArco(2, 1, 54);
    G->adicionarArco(2, 2);//indice=1
    G->modificarArco(2, 2, 0);



    int in;
    int on;

    //Imprimir los vértices con sus arcos
    for (int i = 0; i < G->cantidadVertices(); ++i) {
        cout << "Vertice : " << G->obtenerVertice(i) << endl;
        for (int j = 0; j < G->cantidadArcos(i); j++)
        {
            cout << "Arco->" << G->obtenerArco(i, j) << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < 5; i++)
    {
        string distrito = Cola_Repartidor->obtenerPos(i)->getdistrito();
        if (distrito == "SanMiguel")
        {
            in = 0;
        }
        else if (distrito == "Barranco")
        {
            in = 1;
        }
        else if (distrito == "JesusMaria")
        {
            in = 2;
        }
        else
        {
            cout << "Distrito no encontrado" << endl;
            continue;
        }

        string distrito2 = usuario->getDistrito();

        if (distrito2 == "SanMiguel")
        {
            on = 0;
        }
        else if (distrito2 == "Barranco")
        {
            on = 1;
        }
        else if (distrito2 == "JesusMaria")
        {
            on = 2;
        }
        else
        {
            cout << "Distrito no encontrado" << endl;
            continue;
        }

        int aas = G->obtenerArco(in, on); // Obtener el arco correspondiente al distrito del repartidor

        Cola_Repartidor->obtenerPos(i)->settarifa(aas);
    }

    cout << "- - - - - - - - - - - - - - - - - - - LISTA DE REPARTIDORES - - - - - - - - - - - - - - - - - - - - -" << endl;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Cola<Repartidor*>* temp_Cola = new Cola<Repartidor*>();
    vector<Repartidor*> repartidores;

    // Extraer y guardar en vector
    while (!Cola_Repartidor->esVacia()) {
        Repartidor* r = Cola_Repartidor->desencolar();
        repartidores.push_back(r);
        temp_Cola->encolar(r);
    }
    // Restaurar la cola original
    while (!temp_Cola->esVacia()) {
        Cola_Repartidor->encolar(temp_Cola->desencolar());
    }
    delete temp_Cola;

    int seleccion = 0;
    bool seleccionado = false;

    while (!seleccionado) {
        system("cls");

        string titulo = "LISTA DE REPARTIDORES";
        gotoxy(centrarX(titulo), 2);
        cout << "- - - - - - " << titulo << " - - - - - -" << endl;

        // Definir cabecera
        const int COL_ID = 4, COL_NOMBRE = 20, COL_DISTRITO = 15, COL_CALIF = 12, COL_TARIFA = 8;
        string lineaH = "+" + string(COL_ID + 2, '-') + "+"
            + string(COL_NOMBRE + 2, '-') + "+"
            + string(COL_DISTRITO + 2, '-') + "+"
            + string(COL_CALIF + 2, '-') + "+"
            + string(COL_TARIFA + 2, '-') + "+";

        string encabezado = "| "
            + string("ID  ") + " | "
            + string("Nombre              ") + " | "
            + string("Distrito        ") + " | "
            + string("Calificación") + " | "
            + string("Tarifa ") + " |";

        int x = centrarX(lineaH);
        int y = 4;

        gotoxy(x, y++); cout << lineaH;
        gotoxy(x, y++); cout << encabezado;
        gotoxy(x, y++); cout << lineaH;

        // Mostrar repartidores
        for (int i = 0; i < repartidores.size(); ++i) {
            Repartidor* r = repartidores[i];
            gotoxy(x, y);

            if (i == seleccion) {
                SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }

            cout << "| "
                << setw(COL_ID) << right << i + 1 << " | "
                << setw(COL_NOMBRE) << left << r->getnombre() << " | "
                << setw(COL_DISTRITO) << left << r->getdistrito() << " | "
                << setw(COL_CALIF) << right << fixed << setprecision(1) << r->getCalificacion() << " | "
                << setw(COL_TARIFA) << right << r->gettarifa() << " |";

            SetConsoleTextAttribute(hConsole, 7);
            y++;
        }

        gotoxy(x, y++); cout << lineaH;

        // Leer entrada del usuario
        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && seleccion > 0) seleccion--; // Flecha arriba
            if (tecla == 80 && seleccion < repartidores.size() - 1) seleccion++; // Flecha abajo
        }
        else if (tecla == 13) {
            seleccionado = true;
        }
    }

    // El usuario ha seleccionado un repartidor
    this->eleccionRepartidor = seleccion;

}

inline void Controlador::RegistrarProducto(int a) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    const string categorias[] = { "Comida", "Salud", "Bebidas" };
    const string archivos[] = { "productos_Comida.txt", "productos_Salud.txt", "productos_Bebidas.txt" };

    int seleccion = 0;
    bool seleccionado = false;

    while (!seleccionado) {
        system("cls");
        gotoxy(centrarX("SECCIONES DE PRODUCTOS"), 2);
        cout << "- - - - - - SECCIONES DE PRODUCTOS - - - - - -";
        for (int i = 0; i < 3; i++) {
            gotoxy(centrarX(categorias[i]) - 2, 5 + i * 2);
            if (i == seleccion) {
                SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << ">> " << categorias[i] << " <<";
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << "   " << categorias[i];
            }
        }
        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && seleccion > 0) seleccion--;
            if (tecla == 80 && seleccion < 2) seleccion++;
        }
        else if (tecla == 13) {
            seleccionado = true;
        }
    }

    string archivo = archivos[seleccion];
    List_productos->eliminarTodo();
    ifstream nomArch(archivo);
    ofstream tempArch;
    int codigo, inventario;
    string nombre, nombreBuscar;
    float precio;
    int i = 0;
    bool menor;
    int codigoBuscado, stockelegido;
    bool encontrado = false;

    if (!nomArch.is_open()) {
        gotoxy(centrarX("No se pudo abrir el archivo."), 15);
        cout << "No se pudo abrir el archivo.";
        gotoxy(centrarX("No se pudo abrir el archivo."), 16);
        system("pause");
        return;
    }

    while (nomArch >> codigo >> nombre >> precio >> inventario) {
        List_productos->agregaPos(new producto(codigo, nombre, precio, inventario), i++);
    }
    nomArch.close();

    int modo = 0;
    seleccionado = false;
    while (!seleccionado) {
        system("cls");
        gotoxy(centrarX("Ordenar o ver extremos de precio"), 2);
        cout << "Ordenar o ver extremos de precio" << endl;
        string opciones[] = { "Ordenar", "Ver productos mas baratos/caros" };
        for (int i = 0; i < 2; i++) {
            gotoxy(centrarX(opciones[i]), 5 + i * 2);
            if (i == modo) {
                SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << ">> " << opciones[i] << " <<";
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << "   " << opciones[i];
            }
        }
        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && modo > 0) modo--;
            if (tecla == 80 && modo < 1) modo++;
        }
        else if (tecla == 13) {
            seleccionado = true;
        }
    }

    if (modo == 0) {
        bool asc = true;
        seleccionado = false;
        while (!seleccionado) {
            system("cls");
            gotoxy(centrarX("Orden ascendente o descendente"), 2);
            cout << "Orden ascendente o descendente" << endl;
            string orden[] = { "Ascendente", "Descendente" };
            for (int i = 0; i < 2; i++) {
                gotoxy(centrarX(orden[i]), 5 + i * 2);
                if ((asc && i == 0) || (!asc && i == 1)) {
                    SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    cout << ">> " << orden[i] << " <<";
                    SetConsoleTextAttribute(hConsole, 7);
                }
                else {
                    cout << "   " << orden[i];
                }
            }
            int tecla = _getch();
            if (tecla == 224) {
                tecla = _getch();
                if (tecla == 72 || tecla == 80) asc = !asc;
            }
            else if (tecla == 13) {
                seleccionado = true;
            }
        }
        menor = asc;
        quicksort(List_productos, 0, List_productos->longitud() - 1, menor);
    }
    else {
        bool barato = true;
        seleccionado = false;
        while (!seleccionado) {
            system("cls");
            gotoxy(centrarX("Ver productos mas caros o mas baratos"), 2);
            cout << "Ver productos mas caros o mas baratos" << endl;

            string extremos[] = { "Mas baratos", "Mas caros" };
            for (int i = 0; i < 2; i++) {
                gotoxy(centrarX(extremos[i]), 5 + i * 2);
                if ((barato && i == 0) || (!barato && i == 1)) {
                    SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    cout << ">> " << extremos[i] << " <<";
                    SetConsoleTextAttribute(hConsole, 7);
                }
                else {
                    cout << "   " << extremos[i];
                }
            }
            int tecla = _getch();
            if (tecla == 224) {
                tecla = _getch();
                if (tecla == 72 || tecla == 80) barato = !barato;
            }
            else if (tecla == 13) {
                seleccionado = true;
            }
        }
        menor = barato;
        int n;
        gotoxy(centrarX("Cantidad de productos a ver:"), 10);
        cout << "Cantidad de productos a ver:";
        gotoxy(centrarX("Cantidad de productos a ver:") + 33, 10);
        cin >> n;
        ObtenerMasCoB(List_productos, menor, n);
        system("pause");
    }

    // Mostrar tabla de productos con bordes simples
    seleccionado = false;
    bool porNombre = false;
    while (!seleccionado) {
        system("cls");
        gotoxy(centrarX("Seleccion de producto"), 2);
        cout << "- - - - - - SELECCION DE PRODUCTO - - - - - -";
        string metodos[] = { "Buscar por nombre", "Seleccionar en la tabla" };
        for (int i = 0; i < 2; i++) {
            gotoxy(centrarX(metodos[i]), 5 + i * 2);
            if ((porNombre && i == 0) || (!porNombre && i == 1)) {
                SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << ">> " << metodos[i] << " <<";
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << "   " << metodos[i];
            }
        }
        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 || tecla == 80) porNombre = !porNombre;
        }
        else if (tecla == 13) {
            seleccionado = true;
        }
    }

    producto* productoSeleccionado = nullptr;
    if (!porNombre) {
        int indice = 0;
        bool elegido = false;
        while (!elegido) {
            system("cls");
            int ancho = 40;
            int alto = List_productos->longitud() + 4;
            int x0 = centrarX("+") - ancho / 2;
            int y0 = 1;

            gotoxy(x0, y0); cout << "+" << string(ancho - 2, '-') << "+";
            gotoxy(x0 + 2, y0 + 1); cout << " ID  | NOMBRE          | PRECIO   | STOCK ";
            for (int i = 0; i < List_productos->longitud(); i++) {
                string linea = List_productos->obtenerPos(i)->toString_tabla();
                gotoxy(x0, y0 + 2 + i);
                cout << "|";
                if (i == indice) {
                    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    cout << linea;
                    SetConsoleTextAttribute(hConsole, 7);
                }
                else {
                    cout << linea;
                }
                cout << "|";
            }
            gotoxy(x0, y0 + alto - 1); cout << "+" << string(ancho - 2, '-') << "+";

            int tecla = _getch();
            if (tecla == 224) {
                tecla = _getch();
                if (tecla == 72 && indice > 0) indice--;
                if (tecla == 80 && indice < List_productos->longitud() - 1) indice++;
            }
            else if (tecla == 13) {
                productoSeleccionado = List_productos->obtenerPos(indice);
                elegido = true;
            }
        }
    }
    else {
        gotoxy(centrarX("Ingrese el nombre del producto:"), 12);
        cout << "Ingrese el nombre del producto:";
        gotoxy(centrarX("Ingrese el nombre del producto:") + 33, 12);
        cin >> nombreBuscar;

        HashTablaString* ht = new HashTablaString(500);

        for (int i = 0; i < List_productos->longitud(); ++i) {
            producto* prod = List_productos->obtenerPos(i);
            if (prod == nullptr) {
                std::cerr << "[AVISO] Producto nulo en índice " << i << endl;
                continue;
            }

            string nombre = prod->getnombre();
            if (!nombre.empty()) {
                ht->insertar(nombre, prod);
            }
        }

        int index = ht->buscar(nombreBuscar);

        if (index != -1) {
            gotoxy(centrarX("Producto encontrado."), 14);
            cout << "Producto encontrado.";
            gotoxy(centrarX("Producto encontrado."), 15);
            system("pause");
            
        }
        else {
            gotoxy(centrarX("Producto no encontrado."), 14);
            cout << "Producto no encontrado.";
            gotoxy(centrarX("Producto no encontrado."), 15);
            system("pause");
        }

        delete ht;
    }

    if (productoSeleccionado) {
        gotoxy(centrarX("Ingrese cantidad a comprar:"), 30);
        cout << "Ingrese cantidad a comprar:";
        gotoxy(centrarX("Ingrese cantidad a comprar:") + 29, 30);
        cin >> stockelegido;
        if (productoSeleccionado->getstock() >= stockelegido) {
            productoSeleccionado->disminuir_stock(stockelegido);
            List_Carrito->agregaPos(new producto(productoSeleccionado->getid(), productoSeleccionado->getnombre(), productoSeleccionado->getprecio(), stockelegido), a);
            encontrado = true;
        }
        else {
            gotoxy(centrarX("Stock insuficiente."), 32);
            cout << "Stock insuficiente.";
            gotoxy(centrarX("Stock insuficiente."), 33);
        }
    }

    tempArch.open("temp.txt", ios::out);
    for (int i = 0; i < List_productos->longitud(); i++) {
        producto* p = List_productos->obtenerPos(i);
        tempArch << p->getid() << " " << p->getnombre() << " " << p->getprecio() << " " << p->getstock() << endl;
    }
    tempArch.close();
    remove(archivo.c_str());
    rename("temp.txt", archivo.c_str());

    if (encontrado) {
        gotoxy(centrarX("Producto agregado al carrito."), 34);
        cout << "Producto agregado al carrito.";
        gotoxy(centrarX("Producto agregado al carrito."), 35);
    }
    else {
    }
    system("pause");
}





inline void Controlador::RegistarCliente()
{
    string Correo, contraseña, nombre;
    int telefono;
    string distrito;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - REGISTRO DE USUARIO - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "Ingrese su correo :"; cin >> Correo;
    cout << "Ingrese su contraseña :"; cin >> contraseña;
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
    string nombre, correo, contraseña, distrito;
    int telefono;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    // Coordenadas
    COORD coordNombre = { (SHORT)centrarX("Nombre: "), 5 };
    COORD coordTelefono = { (SHORT)centrarX("Telefono: "), 7 };
    COORD coordDistrito = { (SHORT)centrarX("Distrito: "), 9 };
    COORD coordCorreo = { (SHORT)centrarX("Correo: "), 11 };
    COORD coordContrasena = { (SHORT)centrarX("Contraseña: "), 13 };

    // Mostrar campos
    SetConsoleCursorPosition(hConsole, coordNombre);     cout << "Nombre: ";
    SetConsoleCursorPosition(hConsole, coordTelefono);   cout << "Telefono: ";
    SetConsoleCursorPosition(hConsole, coordDistrito);   cout << "Distrito: ";
    SetConsoleCursorPosition(hConsole, coordCorreo);     cout << "Correo: ";
    SetConsoleCursorPosition(hConsole, coordContrasena); cout << "Contraseña: ";

    // Captura de datos centrados
    gotoxy((SHORT)(coordNombre.X + 10), coordNombre.Y); cin >> nombre;
    gotoxy((SHORT)(coordTelefono.X + 10), coordTelefono.Y); cin >> telefono;
    gotoxy((SHORT)(coordDistrito.X + 10), coordDistrito.Y); cin >> distrito;
    gotoxy((SHORT)(coordCorreo.X + 8), coordCorreo.Y); cin >> correo;
    gotoxy((SHORT)(coordContrasena.X + 12), coordContrasena.Y);
    contraseña = leerContrasenaOculta((SHORT)(coordContrasena.X + 12), coordContrasena.Y);

    // Verifica si ya existe
    if (arbolUsuarios.buscar(nombre) != nullptr) {
        gotoxy(centrarX("El usuario ya existe."), 15);
        cout << "El usuario ya existe." << endl;
        system("pause");
        return;
    }

    // Insertar y guardar
    User* nuevo = new User(nombre, telefono, distrito, correo, contraseña);
    arbolUsuarios.insertar(nuevo);
    guardarUsuariosEnArchivo(arbolUsuarios, "usuarios.txt");

    gotoxy(centrarX("Usuario registrado correctamente."), 15);
    cout << "Usuario registrado correctamente." << endl;
    system("pause");
}

bool Controlador::IniciarSesion() {
    string nombre, contraseña;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    COORD coordNombre = { (SHORT)centrarX("Nombre: "),      8 };
    COORD coordContrasena = { (SHORT)centrarX("Contraseña: "), 10 };

    // Etiquetas
    SetConsoleCursorPosition(hConsole, coordNombre);     cout << "Nombre: ";
    SetConsoleCursorPosition(hConsole, coordContrasena); cout << "Contraseña: ";

    // Entrada
    gotoxy((SHORT)(coordNombre.X + 9), coordNombre.Y);     cin >> nombre;
    gotoxy((SHORT)(coordContrasena.X + 12), coordContrasena.Y);
    contraseña = leerContrasenaOculta((SHORT)(coordContrasena.X + 12), coordContrasena.Y);

    /* ────────────────────────────────────────────────────────────────── */

    // Busca el usuario
    User* encontrado = arbolUsuarios.buscar(nombre);

    if (encontrado && encontrado->getContraseña() == contraseña) {

        /*  ⬇️  SOLUCIÓN: guardar el puntero en el atributo de la clase  */
        this->usuario = encontrado;

        gotoxy(centrarX("¡Bienvenido!"), 13);
        cout << "¡Bienvenido, " << encontrado->getNombre() << "!" << endl;
        system("pause");
        return true;
    }
    else {
        gotoxy(centrarX("Credenciales incorrectas."), 13);
        cout << "Usuario o contraseña incorrectos." << endl;
        system("pause");
        return false;
    }
}


inline void Controlador::Vercarrito()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    /* ========== 1. ELEGIR ORDEN DE LISTADO ========== */
    bool menor = false;
    int seleccionOrden = 0;
    bool seleccionado = false;

    while (!seleccionado) {
        system("cls");
        string titulo = "ORDENAR PRODUCTOS DEL CARRITO";
        gotoxy(centrarX(titulo), 2);  cout << "- - - - - - " << titulo << " - - - - - -";

        string opciones[] = { "Mayor a Menor", "Menor a Mayor" };
        for (int i = 0; i < 2; ++i) {
            gotoxy(centrarX(opciones[i]), 5 + i * 2);
            if (i == seleccionOrden) {
                SetConsoleTextAttribute(hConsole,
                    BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << ">> " << opciones[i] << " <<";
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << "   " << opciones[i];
            }
        }

        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && seleccionOrden > 0) seleccionOrden--;
            if (tecla == 80 && seleccionOrden < 1) seleccionOrden++;
        }
        else if (tecla == 13) {
            menor = (seleccionOrden == 1);
            seleccionado = true;
        }
    }

    /* ========== 2. ORDENAR Y MOSTRAR TABLA ========== */
    mergeSort(List_Carrito, 0, List_Carrito->longitud() - 1, menor);

    system("cls");
    string titulo = "CARRITO DE COMPRAS";
    gotoxy(centrarX("- - - - - - " + titulo + " - - - - - -"), 2);
    cout << "- - - - - - " << titulo << " - - - - - -" << endl;

    int y = 4;
    string headerTop = "+------+-----------------+----------+--------+";
    string headerMid = "| ID   | Nombre          | Precio   | Stock  |";
    int x = centrarX(headerTop);

    gotoxy(x, y++); cout << headerTop;
    gotoxy(x, y++); cout << headerMid;
    gotoxy(x, y++); cout << headerTop;

    for (int i = 0; i < List_Carrito->longitud(); i++) {
        producto* p = List_Carrito->obtenerPos(i);
        gotoxy(x, y++);
        cout << "| "
            << setw(4) << left << p->getid() << " | "
            << setw(15) << left << p->getnombre() << " | "
            << setw(8) << fixed << setprecision(2) << p->getprecio() << " | "
            << setw(6) << p->getstock() << " |";
    }
    gotoxy(x, y++); cout << headerTop;

    /* ========== 3. MENÚ DE OPCIONES (Confirmar / Eliminar / Volver) ========== */
    int seleccion = 0;
    seleccionado = false;
    string opcionesCarrito[] = { "Confirmar pedido", "Eliminar producto", "Volver" };
    const int n = sizeof(opcionesCarrito) / sizeof(opcionesCarrito[0]);

    while (!seleccionado) {
        for (int i = 0; i < n; ++i) {
            int yOpt = y + 2 + i * 2;
            gotoxy(0, yOpt);                          // Limpia toda la línea antes de escribir
            cout << string(125, ' ');                // Asume consola de 120 columnas

            gotoxy(centrarX(opcionesCarrito[i]), yOpt);
            if (i == seleccion) {
                SetConsoleTextAttribute(hConsole,
                    BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << ">> " << opcionesCarrito[i] << " <<";
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << "   " << opcionesCarrito[i];
            }
        }

        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && seleccion > 0) seleccion--;
            if (tecla == 80 && seleccion < n - 1) seleccion++;
        }
        else if (tecla == 13) {
            seleccionado = true;
        }
    }


    /* ========== 4. ACCIONES SEGÚN SELECCIÓN ========== */
    switch (seleccion) {

        /* ---- 4.1 CONFIRMAR / RESUMEN ---- */
    case 0: {
        system("cls");
        gotoxy(centrarX("Resumen del pedido"), 2);
        cout << "- - - - - -  Resumen del pedido  - - - - - -";

        if (List_Carrito->esVacia()) {
            gotoxy(centrarX("El carrito está vacío."), 5);
            cout << "El carrito está vacío.";
            system("pause");
            return;
        }

        // Encolar productos para el resumen
        for (int i = 0; i < List_Carrito->longitud(); ++i)
            Cola_resumen->encolar(new producto(*List_Carrito->obtenerPos(i)));

        // Calcular ancho dinámico para el nombre
        int maxNombre = 10;
        for (int i = 0; i < List_Carrito->longitud(); ++i)
            maxNombre = max<int>(maxNombre, List_Carrito->obtenerPos(i)->getnombre().length());

        const int COL_ID = 5, COL_NOMBRE = maxNombre + 2, COL_PRECIO = 10, COL_STOCK = 7;
        const int ANCHO = COL_ID + COL_NOMBRE + COL_PRECIO + COL_STOCK + 5 * 3 + 1;

        int x0 = centrarX(string(ANCHO, ' '));
        int y0 = 4;

        string lineaH = "+" + string(COL_ID + 2, '-') + "+"
            + string(COL_NOMBRE + 2, '-') + "+"
            + string(COL_PRECIO + 2, '-') + "+"
            + string(COL_STOCK + 2, '-') + "+";

        gotoxy(x0, y0++); cout << lineaH;
        gotoxy(x0, y0++); cout << "| "
            << setw(COL_ID) << left << "ID" << " | "
            << setw(COL_NOMBRE) << left << "NOMBRE" << " | "
            << setw(COL_PRECIO) << left << "PRECIO" << " | "
            << setw(COL_STOCK) << left << "CANT." << " |";
        gotoxy(x0, y0++); cout << lineaH;

        while (!Cola_resumen->esVacia()) {
            producto* P = Cola_resumen->desencolar();
            gotoxy(x0, y0++);
            cout << "| "
                << setw(COL_ID) << right << P->getid() << " | "
                << setw(COL_NOMBRE) << left << P->getnombre() << " | "
                << setw(COL_PRECIO) << right << fixed << setprecision(2)
                << P->getprecio() << " | "
                << setw(COL_STOCK) << right << P->getstock() << " |";
        }
        gotoxy(x0, y0++); cout << lineaH;

        system("pause");
        ListarRepartidores();
        break;
    }

          /* ---- 4.2 ELIMINAR ---- */
    case 1: {
        int eliminar;
        // Ajustamos +5 para bajarlo más visualmente desde el último elemento del menú
        int yPrompt = 6 + List_Carrito->longitud() + n * 2 + 5;

        gotoxy(centrarX("Ingrese el código del producto a eliminar:"), yPrompt);
        cout << "Ingrese el código del producto a eliminar: ";
        cin >> eliminar;

        for (int i = 0; i < List_Carrito->longitud(); ++i) {
            if (List_Carrito->obtenerPos(i)->getid() == eliminar) {
                List_Carrito->eliminaPos(i);
                break;
            }
        }
    }

          /* ---- 4.3 VOLVER ---- */
    case 2:
        // Nada, solo regresar
        break;
    }
}


void Controlador::Menu() {
    string opciones[] = {
        "Ingresar Sesion",
        "Registrar Nuevo Usuario",
        "Salir"
    };
    const int n = sizeof(opciones) / sizeof(opciones[0]);
    int seleccion = 0;
    bool corriendo = true;

    while (corriendo) {
        system("cls");

        // Arte centrado (puedes agregar más)
        string arte[] = {
            "                -=                ",
            "        .     -##:                ",
            "       :=     -###=          =:   ",
            "      +*      *####+.         *+  ",
            "     +#:      #######+.       :#+ ",
            "    -##.      -########.      .##-",
            "    *##:       =#######*      :##*",
            "    ###*        .+######      *###",
            "    *###*         .*###+     *###*",
            "    -#####-        :##*    -#####-",
            "     +######=:     :#=  :=######+ ",
            "      =#########*++#+*#########=  ",
            "       :*####################*:   ",
            "         .=################=:     ",
            "            :=*##########*+.      "
        };
        for (int i = 0; i < sizeof(arte) / sizeof(arte[0]); i++) {
            gotoxy(centrarX(arte[i]), 1 + i);
            cout << arte[i];
        }

        string titulo = "Llevalo";
        gotoxy(centrarX(titulo), 17);
        cout << titulo;

        // Mostrar opciones
        for (int i = 0; i < n; ++i) {
            string opcion = opciones[i];
            int y = 20 + i * 2;
            gotoxy(centrarX(opcion), y);

            if (i == seleccion) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

                cout << ">> " << opcion << " <<";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else {
                cout << "   " << opcion;
            }
        }

        int tecla = _getch();
        switch (tecla) {
        case 72: // ↑
            seleccion = (seleccion - 1 + n) % n;
            break;
        case 80: // ↓
            seleccion = (seleccion + 1) % n;
            break;
        case 13: // Enter
            switch (seleccion) {
            case 0:
                if (IniciarSesion()) InterfazUsuario();
                break;
            case 1:
                RegistrarUsuario();
                break;
            case 2:
                List_Bebidas = nullptr;
                List_Carrito = nullptr;
                List_Comida = nullptr;
                List_productos = nullptr;
                Cola_Repartidor = nullptr;
                List_Salud = nullptr;
                corriendo = false;
                break;
            }
            break;
        }
    }
}

inline void Controlador::InterfazUsuario() {
    string opciones[] = {
        "Pedir",
        "Ver Carrito",
        "Ver Mis Pedidos",
        "Ver Historial de Compra"
    };
    const int n = sizeof(opciones) / sizeof(opciones[0]);
    int seleccion = 0;
    int a = 0;
    bool activo = true;

    while (activo) {
        system("cls");

        string titulo = "BIENVENIDO";
        gotoxy(centrarX(titulo), 4);
        string tituloDecorado = "- - - - - - - -  " + titulo + "  - - - - - - - -";
        gotoxy(centrarX(tituloDecorado), 4);
        cout << tituloDecorado;

        // Mostrar opciones
        for (int i = 0; i < n; i++) {
            int y = 8 + i * 2;
            gotoxy(centrarX(opciones[i]), y);

            if (i == seleccion) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << ">> " << opciones[i] << " <<";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else {
                cout << "   " << opciones[i];
            }
        }

        int tecla = _getch();
        switch (tecla) {
        case 72: // Flecha arriba
            seleccion = (seleccion - 1 + n) % n;
            break;
        case 80: // Flecha abajo
            seleccion = (seleccion + 1) % n;
            break;
        case 13: // Enter
            system("cls");
            switch (seleccion) {
            case 0:
                RegistrarProducto(a);
                a++;
                break;
            case 1:
                Vercarrito();
                break;
            case 2:
                PedidoRealizado();
                break;
            case 3:
                VerHistorial();
                GenerarArbolBalanceado();
                break;
            }
            system("pause");
            break;
        case 27: // ESC para salir (opcional)
            activo = false;
            break;
        }
    }
}

inline void Controlador::PedidoRealizado()
{
    system("cls");

    double precio = 0.0;
    string titulo = "PEDIDOS REALIZADOS";
    gotoxy(centrarX("- - - - - - " + titulo + " - - - - - -"), 2);
    cout << "- - - - - - " << titulo << " - - - - - -" << endl;
    gotoxy(centrarX("Resumen del pedido:"), 4);
    cout << "Resumen del pedido:" << endl;

    // Encolar productos y calcular precio total
    for (int i = 0; i < List_Carrito->longitud(); i++)
    {
        Cola_resumen->encolar(new producto(
            List_Carrito->obtenerPos(i)->getid(),
            List_Carrito->obtenerPos(i)->getnombre(),
            List_Carrito->obtenerPos(i)->getprecio(),
            List_Carrito->obtenerPos(i)->getstock()
        ));
        precio += List_Carrito->obtenerPos(i)->getprecio();
    }

    // Preparar tabla
    int maxNombre = 10;
    for (int i = 0; i < List_Carrito->longitud(); ++i)
        maxNombre = max<int>(maxNombre, List_Carrito->obtenerPos(i)->getnombre().length());

    const int COL_ID = 5, COL_NOMBRE = maxNombre + 2, COL_PRECIO = 10, COL_STOCK = 7;
    const int ANCHO = COL_ID + COL_NOMBRE + COL_PRECIO + COL_STOCK + 5 * 3 + 1;

    int x0 = centrarX(string(ANCHO, ' '));
    int y0 = 6;

    string lineaH = "+" + string(COL_ID + 2, '-') + "+"
        + string(COL_NOMBRE + 2, '-') + "+"
        + string(COL_PRECIO + 2, '-') + "+"
        + string(COL_STOCK + 2, '-') + "+";

    // Encabezado tabla
    gotoxy(x0, y0++); cout << lineaH;
    gotoxy(x0, y0++); cout << "| "
        << setw(COL_ID) << left << "ID" << " | "
        << setw(COL_NOMBRE) << left << "NOMBRE" << " | "
        << setw(COL_PRECIO) << left << "PRECIO" << " | "
        << setw(COL_STOCK) << left << "CANT." << " |";
    gotoxy(x0, y0++); cout << lineaH;

    // Imprimir productos del resumen
    producto* Producto;
    while (!Cola_resumen->esVacia())
    {
        Producto = Cola_resumen->desencolar();
        gotoxy(x0, y0++);
        cout << "| "
            << setw(COL_ID) << right << Producto->getid() << " | "
            << setw(COL_NOMBRE) << left << Producto->getnombre() << " | "
            << setw(COL_PRECIO) << right << fixed << setprecision(2)
            << Producto->getprecio() << " | "
            << setw(COL_STOCK) << right << Producto->getstock() << " |";
    }

    gotoxy(x0, y0++); cout << lineaH;

    // Verificar repartidor y calcular total
    int totalRepartidores = Cola_Repartidor->longitud();
    gotoxy(centrarX(""), y0++);  // espacio

    if (eleccionRepartidor >= 0 && eleccionRepartidor < totalRepartidores)
    {
        double total = CalcularTotal(Cola_Repartidor, eleccionRepartidor, precio);
        gotoxy(centrarX(""), y0);
        cout << "EL TOTAL SERÍA: S/. " << fixed << setprecision(2) << total;
    }
    else
    {
        gotoxy(centrarX(""), y0);
        cout << "Índice del repartidor fuera de rango. Total disponibles: "
            << totalRepartidores;
    }

    gotoxy(0, y0 + 2);
    system("pause");
    GuardarHistorial();
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
        cout << "El historial de productos está vacío. No se puede generar un árbol AVL." << endl;
        return;
    }
    cout << "\n--- Generando Árbol AVL de Historial de Productos ---\n";
    for (int i = 0; i < Lista_Historial->longitud(); ++i) {
        producto* p = Lista_Historial->obtenerPos(i);
        arbol_historial.insertar(p);

    }
    cout << "Árbol AVL del historial de productos generado correctamente.\n";
    cout << "\n--- Contenido del Árbol AVL (ordenado por nombre) ---\n";
    arbol_historial.enOrden([](producto* p) {
        cout << "  - " << p->getnombre() << " (ID: " << p->getid() << ", Precio: $" << p->getprecio() << ")\n";
    });
    cout << "-----------------------------------------------------\n";
}