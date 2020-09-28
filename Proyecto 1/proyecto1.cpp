#include <iostream>
#include <fstream> //para leer o escribir archivos
#include "json.hpp" //libreria para leer archivos en formato Json
//#include <string> //para convertir enteros a cadenas

using namespace std;
using json = nlohmann::json;

class nodoListaObjetos{
public:
    int x;
    int y;
    nodoListaObjetos *siguiente;

    nodoListaObjetos(int x, int y){
        this->x = x;
        this->y = y;
        siguiente = NULL;
    }
};

class listaObjetos{
public:
    nodoListaObjetos *primero;
    nodoListaObjetos *ultimo;
    int tam = 0;//tamaño

    listaObjetos()
    {
        primero = NULL;
        ultimo = NULL;
    }

    int magnitud(){
        return tam;
    }

    void insertar(int x, int y){
        nodoListaObjetos *temp = new nodoListaObjetos(x, y);
        if(primero == NULL)
        {
            primero = temp;
            ultimo = temp;
            tam++;
        }
        else
        {
           ultimo->siguiente = temp;
           ultimo = temp;
           tam++;
        }
    }

    void mostrar(){
        nodoListaObjetos *temp = primero;
        while (temp != NULL){
            cout<<temp->x<<", "<<temp->y<<endl;
            temp = temp->siguiente;
        }
    }

};

class nodoMatriz
{

public:
    string letra;
    string color;
    //coordenadas
    int xc;
    int yc;
    nodoMatriz *right;
    nodoMatriz *left;
    nodoMatriz *down;
    nodoMatriz *up;

    //Verificar si es necesario el atributo valor extra
    nodoMatriz(string  letra, string color, int xc, int yc)
    {
        this -> letra = letra;
        this -> color = color;
        this -> xc = xc;
        this -> yc = yc;
        right = NULL;
        left = NULL;
        down = NULL;
        up = NULL;
    }
};

class matrix
{

public:
    //string headers_X;
    nodoMatriz *head;
    //int level;
    int level = 0;

    matrix()
    {
        //nodoMatriz *temp = new nodoMatriz("", "", 0, 0);
        nodoMatriz *temp = new nodoMatriz("", "", -1, -1);
        head = temp;
    }

    matrix(const matrix &otro){
        //nodoMatriz *temp = new nodoMatriz("", "", -1, -1);
        //head = otro->head;
    }

    void add(int x, int y, string letra, string color)
    {
        //1. insert headers
        add_x_header(x);
        add_y_header(y);
        //2. insert node
        nodoMatriz *new_node = new nodoMatriz(letra, color, x, y);
        add_x(new_node, x);
        add_y(new_node, y);
    }

    void add_x_header(int x)
    {
        if (head->right == NULL)
        {
            //nodoMatriz *temp = new nodoMatriz("","", x, 0);
            nodoMatriz *temp = new nodoMatriz("","", x, -1);
            head->right = temp;
            temp->left = head;
        }
        else
        {
            nodoMatriz *temp = head;
            while (temp->right != NULL && temp->right->xc < x)
            {
                temp = temp->right;
            }
            if (temp->right == NULL)
            {
                //nodoMatriz *new_temp = new nodoMatriz("", "", x, 0);
                nodoMatriz *new_temp = new nodoMatriz("", "", x, -1);
                temp->right = new_temp;
                new_temp->left = temp;
            }
            else if(temp->right != NULL && temp->right->xc != x)
            {
                //nodoMatriz *new_temp = new nodoMatriz("", "", x, 0);
                nodoMatriz *new_temp = new nodoMatriz("", "", x, -1);
                nodoMatriz *der = temp->right;
                temp->right = new_temp;
                new_temp->left = temp;
                new_temp->right = der;
                der->left = new_temp;
            }
        }
    }

    void add_y_header(int y)
    {
        if (head->down == NULL)
        {
            //nodoMatriz *temp = new nodoMatriz("", "", 0, y);
            nodoMatriz *temp = new nodoMatriz("", "", -1, y);
            head->down = temp;
            temp->up = head;
        }
        else
        {
            nodoMatriz *temp = head;
            while (temp->down != NULL && temp->down->yc < y)
            {
                temp = temp->down;
            }
            if (temp->down == NULL)
            {
                //nodoMatriz *new_temp = new nodoMatriz("", "", 0, y);
                nodoMatriz *new_temp = new nodoMatriz("", "", -1, y);
                temp->down = new_temp;
                new_temp->up = temp;
            }
            else if(temp->down != NULL && temp->down->yc != y)
            {
                //nodoMatriz *new_temp = new nodoMatriz("", "", 0, y);
                nodoMatriz *new_temp = new nodoMatriz("", "", -1, y);
                nodoMatriz *der = temp->down;
                temp->down = new_temp;
                new_temp->up = temp;
                new_temp->down = der;
                der->up = new_temp;
            }
        }
    }

    void add_x(nodoMatriz *new_node, int x)
    {
        //int x is the x coordinate of the header in the x axis
        nodoMatriz *temp = head;
        while (temp->xc != x)
        {
            temp = temp->right;
        }
        //case: empty column
        if (temp->down == NULL)
        {
            temp->down = new_node;
            new_node->up = temp;
        }
        else
        {
            //if there isn't a node before where I want insert node
            if (temp->down->yc > new_node->yc)
            {
                temp->down->up = new_node;
                new_node->up = temp;
                new_node->down = temp->down;
                temp->down = new_node;
            }
            //if there is
            else
            {
                while(temp->down != NULL && temp->down->yc < new_node->yc)
                {
                    temp = temp->down;
                }
                if(temp->down == NULL)
                {
                    temp->down = new_node;
                    new_node->up = temp;
                }
                else
                {
                    temp->down->up = new_node;
                    new_node->up = temp;
                    new_node->down = temp->down;
                    temp->down = new_node;
                }
            }
        }
    }

    void add_y(nodoMatriz *new_node, int y)
    {
        //int y is the y coordinate of the header in the y axis
        nodoMatriz *temp = head;
        while (temp->yc != y)
        {
            temp = temp->down;
        }
        //case: empty row
        if(temp->right == NULL)
        {
            temp->right = new_node;
            new_node->left = temp;
        }
        else
        {
            //if there isn't a node before where I want insert node
            if(temp->right->xc > new_node->xc)
            {
                temp->right->left = new_node;
                new_node->left = temp;
                new_node->right = temp->right;
                temp->right = new_node;
            }
            //if there is
            else
            {
                while(temp->right != NULL && temp->right->xc < new_node->xc)
                {
                    temp = temp->right;
                }
                if(temp->right == NULL)
                {
                    temp->right = new_node;
                    new_node->left = temp;
                }
                else
                {
                    temp->right->left = new_node;
                    new_node->left = temp;
                    new_node->right = temp->right;
                    temp->right = new_node;
                }
            }
        }
    }

    void print_headers()
    {
        print_x_header();
        print_y_header();
    }

    void print_x_header()
    {
        nodoMatriz *temp = head;
        while(temp->right != NULL)
        {
            cout<<temp->xc;
            cout<<"->";
            temp = temp->right;
        }
        cout<<temp->xc;
        cout<<"\n";
    }

    void print_y_header()
    {
        nodoMatriz *temp = head;
        if(temp->down != NULL)
        {
            temp = temp->down;
            while(temp->down != NULL)
            {
                cout<<temp->yc;
                cout<<"\n";
                //down arrow
                temp = temp->down;
            }
            cout<<temp->yc;
            cout<<"\n";
        }
    }

    void print_nodes_x()
    {
        nodoMatriz *temp = head;
        if(temp->right != NULL)
        {
            temp = temp->right;
            while(temp->right != NULL)
            {
                cout<<temp->xc;
                cout<<"->";
                //print all nodes in the column
                nodoMatriz *new_temp = temp;
                if(temp->down != NULL)
                {
                    new_temp = new_temp->down;
                    while(new_temp->down != NULL)
                    {
                        cout<<new_temp->letra<<", "<<new_temp->color<<"\n";
                        cout<<temp->xc;
                        cout<<"->";
                        new_temp = new_temp->down;
                    }
                    cout<<new_temp->letra<<", "<<new_temp->color;
                    cout<<"\n";
                }
                temp = temp->right;
            }
            cout<<temp->xc;
            cout<<"->";
            nodoMatriz *new_temp = temp;
            if(temp->down != NULL)
            {
                new_temp = new_temp->down;
                while(new_temp->down != NULL)
                {
                    cout<<new_temp->letra<<", "<<new_temp->color<<"\n";
                    cout<<temp->xc;
                    cout<<"->";
                    new_temp = new_temp->down;
                }
                cout<<new_temp->letra<<", "<<new_temp->color;
            }
            cout<<"\n";
        }
    }

    void print_nodes_y()
    {
        nodoMatriz *temp = head;
        if(temp->down != NULL)
        {
            temp = temp->down;
            while(temp->down != NULL)
            {
                cout<<temp->yc;
                cout<<"->";
                //print all nodes in the crow
                nodoMatriz *new_temp = temp;
                if(temp->right != NULL)
                {
                    new_temp = new_temp->right;
                    while(new_temp->right != NULL)
                    {
                        cout<<new_temp->letra<<", "<<new_temp->color<<"\n";
                        cout<<temp->yc;
                        cout<<"->";
                        new_temp = new_temp->right;
                    }
                    cout<<new_temp->letra<<", "<<new_temp->color;
                    cout<<"\n";
                }
                temp = temp->down;
            }
            cout<<temp->yc;
            cout<<"->";
            nodoMatriz *new_temp = temp;
            if(temp->right != NULL)
            {
                new_temp = new_temp->right;
                while(new_temp->right != NULL)
                {
                    cout<<new_temp->letra<<", "<<new_temp->color<<"\n";
                    cout<<temp->yc;
                    cout<<"->";
                    new_temp = new_temp->right;
                }
                cout<<new_temp->letra<<", "<<new_temp->color;
            }
            cout<<"\n";
        }
    }

    void reporteMatriz()
    {
        string nivelMatriz;
        if(level == 0){
            nivelMatriz = "matrix.txt";
        }else{
            nivelMatriz = "matrix"+to_string(level)+".txt";
        }
        //ofstream f("matrix.txt");
        ofstream f(nivelMatriz);
        if(f.is_open())
        {
            f<<"digraph G {" << endl;
            f<<"rankdir = TB;" <<endl;
            f<<"node [shape = rectangle];" <<endl;
            f<<"graph [nodesep = 0.5];" <<endl;
            //f<<"label=\"Nivel x \";"<<endl;
            f<<"labelloc=t;"<<endl;
            f<<"fontsize=20;"<<endl;

            //generar cabeceras en x
            nodoMatriz *temp = head;
            //f<<"nodeX_"<<temp->xc<<" [label=\"     Nivel X     \"];" <<endl;
            if(level == 0){
                f<<"\"nodeX_"<<temp->xc<<"\" [label=\"     Nivel X     \"];" <<endl;
            }else{
                f<<"\"nodeX_"<<temp->xc<<"\" [label=\"     Nivel "<<level<<"     \"];" <<endl;
            }

            if(temp->right != NULL){
                temp = temp->right;
                while(temp->right != NULL)
                {
                    f<<"nodeX_"<<temp->xc<<" [label=\"        "<<temp->xc<<"       \"];" <<endl;
                    temp = temp->right;
                }
                f<<"nodeX_"<<temp->xc<<" [label=\"        "<<temp->xc<<"        \"];" <<endl;
            }

            //generar cabeceras en y
            temp = head;
            if(temp->down != NULL)
            {
                temp = temp->down;
                while(temp->down != NULL)
                {
                    f<<"nodeY_"<<temp->yc<<" [label=\"        "<<temp->yc<<"       \"];" <<endl;
                    temp = temp->down;
                }
                f<<"nodeY_"<<temp->yc<<" [label=\"        "<<temp->yc<<"       \"];" <<endl;
            }

            //print nodes by columns
            temp = head;
            if(temp->right != NULL)
            {
                temp = temp->right;
                //Recorrer la columna
                while(temp != NULL)
                {
                    if(temp->down != NULL)
                    {
                        nodoMatriz *new_temp = temp->down;
                        while(new_temp != NULL)
                        {
                            f<<"node"<<new_temp->xc<<"_"<<new_temp->yc<<" [label = \"        "<<new_temp->letra<<"       \" style=filled fillcolor=\" "<<new_temp->color<<"\"];"<<endl;
                            new_temp = new_temp->down;
                        }
                    }
                    temp = temp->right;
                }
            }

            //to align x headers row
            temp = head;
            if(temp->right != NULL){
            f<<"{rank=same;";
            if(temp->right != NULL)
            {
                f<<" \"nodeX"<<"_"<<temp->xc<<"\"";//para imprimir el nodo raiz de la matriz
                temp = temp->right;
                while(temp != NULL)
                {
                    f<<" nodeX"<<"_"<<temp->xc;
                    temp = temp->right;
                }
                f<<"}"<<endl;
            }
            //to align rows
            temp = head;
            if(temp->down != NULL)
            {
                temp = temp->down;
                while(temp != NULL)
                {
                    if(temp->right != NULL)
                    {
                        f<<"{rank=same; nodeY_"<<temp->yc;
                        nodoMatriz *new_temp = temp->right;
                        while(new_temp != NULL)
                        {
                            f<<" node"<<new_temp->xc<<"_"<<new_temp->yc;
                            new_temp = new_temp->right;
                        }
                        f<<"}"<<endl;
                    }
                    temp = temp->down;
                }
            }

            //to link x headers nodes
            temp = head;
            if(temp->right != NULL)
            {
                while(temp->right != NULL) //i put temp->right because I'll print actual node and next node
                {
                    f<<"\"nodeX_"<<temp->xc<<"\" -> nodeX_"<<temp->right->xc<<" [dir=both];"<<endl;
                    temp = temp->right;
                }
            }

            //to link y headers nodes
            temp = head;
            if(temp->down != NULL)
            {
                f<<"\"nodeX_"<<temp->xc<<"\" -> nodeY_"<<temp->down->yc<<" [dir=both];"<<endl;
                temp = temp->down;
                while(temp->down != NULL)
                {
                    f<<"nodeY_"<<temp->yc<<" -> nodeY_"<<temp->down->yc<<" [dir=both];"<<endl;
                    temp = temp->down;
                }
            }

            //to link nodes for each column
            temp = head;
            if(temp->right != NULL)
            {
                temp = temp->right;
                while(temp != NULL)
                {
                    if(temp->down != NULL)
                    {
                        nodoMatriz *new_temp = temp;
                        f<<"nodeX_"<<new_temp->xc<<" -> node"<<new_temp->down->xc<<"_"<<new_temp->down->yc<<" [dir=both];"<<endl;
                        new_temp = new_temp->down;
                        while(new_temp->down != NULL)
                        {
                            f<<"node"<<new_temp->xc<<"_"<<new_temp->yc<<" -> node"<<new_temp->down->xc<<"_"<<new_temp->down->yc<<" [dir=both];"<<endl;
                            new_temp = new_temp->down;
                        }
                    }
                    temp = temp->right;
                }
            }

            //to link nodes for each rows
            temp = head;
            if(temp->down != NULL)
            {
                temp = temp->down;
                while(temp != NULL)
                {
                    if(temp->right != NULL)
                    {
                        nodoMatriz *new_temp = temp;
                        f<<"nodeY_"<<new_temp->yc<<" -> node"<<new_temp->right->xc<<"_"<<new_temp->right->yc<<" [constraint=false,dir=both];"<<endl;
                        new_temp = new_temp->right;
                        while(new_temp->right != NULL)
                        {
                            f<<"node"<<new_temp->xc<<"_"<<new_temp->yc<<" -> node"<<new_temp->right->xc<<"_"<<new_temp->right->yc<<" [constraint=false,dir=both];"<<endl;
                            new_temp = new_temp->right;
                        }
                    }
                    temp = temp->down;
                }
            }
            }
            f<<"}" <<endl;
        }
        else cerr << "Error de apertura del archivo." << endl;

        string imprimir = "dot -Tpng "+nivelMatriz+" -o matrix"+to_string(level)+".png";
        //system("dot -Tpng matrix.txt -o matrix.png");
        system(imprimir.c_str());//.c_str() es para que me reconozca la cadena con numeros como cadena normal
        cout<<"\n";
        string abrir = "matrix"+to_string(level)+".png";
        //system("matrix.png");
        system(abrir.c_str());
    }

    void del(int x, int y)
    {
        nodoMatriz *temp = head;
        if(temp->right != NULL)
        {
            temp = temp->right;
            //move in X axis
            while(temp->xc < x && temp->right != NULL)
            {
                temp = temp->right;
            }//verify if column exist
            if(temp->xc == x && temp->down != NULL)
            {
                temp = temp->down;
                //move in Y axis
                while(temp->yc < y && temp->down != NULL)
                {
                    temp = temp->down;
                }//verify if row exist
                if(temp->yc == y)
                {
                    //case 1: there is only one node in the column
                    if(temp->up->yc == 0 && temp->down == NULL)
                    {
                        nodoMatriz *new_temp = temp->up;
                        new_temp->down = NULL;
                        if(new_temp->right == NULL)
                        {
                            new_temp->left->right = NULL;
                            new_temp->left = NULL;
                        }
                        else
                        {
                            new_temp->left->right = new_temp->right;
                            new_temp->right->left = new_temp->left;
                            new_temp->right = NULL;
                            new_temp->left = NULL;
                        }
                    }
                    //case 2: there are more nodes in the column
                    else
                    {
                        if(temp->down != NULL)
                        {
                            temp->up->down = temp->down;
                            temp->down->up = temp->up;
                            temp->down = NULL;
                            temp->up = NULL;
                        }
                        else
                        {
                            temp->up->down = NULL;
                            temp->up = NULL;
                        }
                    }
                    //case 1: there is only one node in the row
                    if(temp->left->xc == 0 && temp->right == NULL)
                    {
                        temp = temp->left;
                        if(temp->down == NULL)
                        {
                            temp->up->down = NULL;
                            temp->up = NULL;
                        }
                        else
                        {
                            temp->up->down = temp->down;
                            temp->down->up = temp->up;
                            temp->down = NULL;
                            temp->up = NULL;
                        }
                    }
                    //case 2: there are more nodes in the row
                    else
                    {
                        if(temp->right != NULL)
                        {
                            temp->right->left = temp->left;
                            temp->left->right = temp->right;
                            temp->right = NULL;
                            temp->left = NULL;
                        }
                        else
                        {
                            temp->left->right = NULL;
                            temp->left = NULL;
                        }
                    }
                }
                else
                {
                    cout<<"node not found 1"<<endl;
                }
            }
            else
            {
                cout<<"node not found 2"<<endl;
            }
        }
    }

    char verificarCasilla(int x, int y)
    {
        nodoMatriz *temp = head;
        if(temp->right != NULL)
        {
            temp = temp->right;
            //move in X axis
            while(temp->xc < x && temp->right != NULL)
            {
                temp = temp->right;
            }//verify if column exist
            if(temp->xc == x && temp->down != NULL)
            {
                temp = temp->down;
                //move in Y axis
                while(temp->yc < y && temp->down != NULL)
                {
                    temp = temp->down;
                }//verify if row exist
                if(temp->yc == y)
                {
                    //cout<<"Si existe"<<endl;
                    return 'S';
                }
                else
                {
                    //cout<<"No existe"<<endl;
                    return 'N';
                }
            }
            else
            {
                //cout<<"No existe"<<endl;
                return 'N';
            }
        }else{
            //cout<<"No existeA"<<endl;
            return 'N';
        }
    }
};

class nodoBST{
public:
    int id;
    string nombre;
    string letra;
    string color;
    listaObjetos *lista;//para apuntar a las listas de Score de cada usuario
    nodoBST *left;
    nodoBST *right;

    nodoBST(int Id, string nombre, string letra, string color){
        this->id = Id;
        this->nombre = nombre;
        this->letra = letra;
        this->color = color;
        lista = NULL;
        right = NULL;
        left = NULL;
    }
};

class BST{
public:
    nodoBST *root;

    BST(){
        root = NULL;
    }


    void add(nodoBST *temp, listaObjetos *insertar){
        if(root == NULL){
            temp->lista = insertar;
            root = temp;
            //cout<<"Se ingreso correctamente "<<endl;
        }else{
            temp->lista = insertar;
            recursive_add(root, temp);
        }
    }

    void recursive_add(nodoBST *current, nodoBST *temp){
        if(temp->id < current->id){//INSERT in left sub-tree
            if(current->left != NULL){//IS NOT NULL, make recursive call
                recursive_add(current->left, temp);
            }else{//IS NULL, INSERT NODE
                current->left = temp;
                //cout<<"Se ingreso correctamente "<<endl;
            }
        }else if(temp->id > current->id){//INSERT in right sub-tree
            if(current->right != NULL){//IS NOT NULL, make recursive call
                recursive_add(current->right, temp);
            }else{//IS NULL, INSERT NODE
                current->right = temp;
                //cout<<"Se ingreso correctamente "<<endl;
            }
        }else{
            //ERROR el nodo ya existe
            cout<<"El objeto "<<temp->nombre<<" ya existe. No se inserto"<<endl;
        }
    }

    char add2(nodoBST *temp, listaObjetos *insertar){
        if(root == NULL){
            temp->lista = insertar;
            root = temp;
            //cout<<"Se ingreso correctamente "<<endl;
            return 'S';
        }else{
            temp->lista = insertar;
            return recursive_add2(root, temp);
        }
    }

    char recursive_add2(nodoBST *current, nodoBST *temp){
        if(temp->id < current->id){//INSERT in left sub-tree
            if(current->left != NULL){//IS NOT NULL, make recursive call
                recursive_add2(current->left, temp);
            }else{//IS NULL, INSERT NODE
                current->left = temp;
                return 'S';
                //cout<<"Se ingreso correctamente "<<endl;
            }
        }else if(temp->id > current->id){//INSERT in right sub-tree
            if(current->right != NULL){//IS NOT NULL, make recursive call
                recursive_add2(current->right, temp);
            }else{//IS NULL, INSERT NODE
                current->right = temp;
                //cout<<"Se ingreso correctamente "<<endl;
                return 'S';
            }
        }else{
            //ERROR el nodo ya existe
            cout<<"El objeto "<<temp->nombre<<" ya existe. No se inserto"<<endl;
            return 'N';
        }
    }


    void inorder(){
        recursive_inorder(root);
    }

    void recursive_inorder(nodoBST *current){
        //llamar recursivamente al sub-arbol izquierdo
        if(current->left != NULL){
            recursive_inorder(current->left);
        }
        //imprimir el valir actual de la raiz
        cout<<current->id;
        cout<<"->";
        //llamar recursivamente al sub-arbol derecho
        if(current->right != NULL){
            recursive_inorder(current->right);
        }
    }

    void inorderObjetos(){
        recursive_inorderObjetos(root);
    }

    void recursive_inorderObjetos(nodoBST *current){
        if(current != NULL){
            recursive_inorderObjetos(current->left);
            cout<<current->id<<",  "<<current->nombre<<endl;
            recursive_inorderObjetos(current->right);
        }
    }

    void preorder(){
        recursive_preorder(root);
    }

    void recursive_preorder(nodoBST *current){
        //imprimir el valir actual de la raiz
        cout<<current->id;
        cout<<"->";
        //llamar recursivamente al sub-arbol izquierdo
        if(current->left != NULL){
            recursive_preorder(current->left);
        }
        //llamar recursivamente al sub-arbol derecho
        if(current->right != NULL){
            recursive_preorder(current->right);
        }
    }

    void posorder(){
        recursive_posorder(root);
    }

    void recursive_posorder(nodoBST *current){
        //llamar recursivamente al sub-arbol izquierdo
        if(current->left != NULL){
            recursive_posorder(current->left);
        }
        //llamar recursivamente al sub-arbol derecho
        if(current->right != NULL){
            recursive_posorder(current->right);
        }
        //imprimir el valir actual de la raiz
        cout<<current->id;
        cout<<"->";
    }

    //write file in real time
    void BSTReporter(){
        if(root != NULL){
            ofstream f("BST.txt");
            if(f.is_open()){
                f<<"digraph Tree {" << endl;
                //Mostrar si solo existe la raiz
                if(root->left == NULL && root->right == NULL){
                    f<<root->id<<endl;
                }
                recursiveReporter(root, f);
                f<<"}"<<endl;
            }
            else cerr << "Error de apertura del archivo." << endl;
            f.close();
            system("dot -Tpng BST.txt -o BST.png");
            cout<<"\n";
            system("BST.png");
        }else{
            cout<<"No hay objetos para mostrar"<<endl;
        }
    }

    void recursiveReporter(nodoBST *current, ofstream &f){
        if(current->left != NULL){
            //if is only child of the node
            if(current->right == NULL){
                f<<current->id<<" -> "<<current->left->id<<endl;
                //2 is central node and 3 is right node, both invisible
                f<<current->id<<"2"<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<current->id<<" -> "<<current->id<<"2"<<"[style = \"invis\"];"<<endl;
                f<<current->id<<"3"<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<current->id<<" -> "<<current->id<<"3"<<"[style = \"invis\"];"<<endl;
            }
            //if node have both nodes
            else{
                f<<current->id<<" -> "<<current->left->id<<endl;
            }
            recursiveReporter(current->left, f);
        }
        if(current->right != NULL){
            //if is only child of the node
            if(current->left == NULL){
                //2 is left node and 3 is central node, both invisible
                f<<current->id<<"2"<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<current->id<<" -> "<<current->id<<"2"<<"[style = \"invis\"];"<<endl;
                f<<current->id<<"3"<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<current->id<<" -> "<<current->id<<"3"<<"[style = \"invis\"];"<<endl;
                f<<current->id<<" -> "<<current->right->id<<endl;
            }
            //if are both child
            else{
                f<<current->id<<" -> "<<current->right->id<<endl;
            }
            recursiveReporter(current->right, f);
        }
    }

    //izquierdo-raiz-derecho
    void inorderReporter(){
        if(root != NULL){
            ofstream f("Inorder.txt");
            if(f.is_open()){
                f<<"digraph Inorder {" << endl;
                f<<"node [shape = record];" <<endl;
                f<<"rankdir = LR;" <<endl;
                recursiveInorderReport(root, f);
                f<<"}"<<endl;
            }
            else cerr << "Error de apertura del archivo." << endl;
            f.close();

            system("dot -Tpng Inorder.txt -o Inorder.png");
            cout<<"\n";
            system("Inorder.png");
        }else{
            cout<<"No hay objetos para mostrar"<<endl;
        }
    }

    //izquierdo-raiz-derecho
    void recursiveInorderReport(nodoBST *current, ofstream &f){
        //llamar recursivamente al sub-arbol izquierdo
        if(current != NULL){
            recursiveInorderReport(current->left, f);
            if(current->left != NULL)
            {
                f<<" -> ";
            }
            //imprimir el valor actual de la raiz
            f<<current->id;
            //llamar recursivamente al sub-arbol derecho
            if(current->right != NULL)
            {
                f<<" -> ";
            }
            recursiveInorderReport(current->right, f);
        }
    }

    //raiz-izquierda-derecha
    void preOrderReporter(){
        if(root != NULL){
            ofstream f("PreOrder.txt");
            if(f.is_open()){
                f<<"digraph Preorder {" << endl;
                f<<"node [shape = record];" <<endl;
                f<<"rankdir = LR;" <<endl;
                recursivePreOrderReport(root, f);
                f<<"}"<<endl;
            }
            else cerr << "Error de apertura del archivo." << endl;
            f.close();
            system("dot -Tpng PreOrder.txt -o PreOrder.png");
            cout<<"\n";
            system("PreOrder.png");
        }else{
            cout<<"No hay objetos para mostrar"<<endl;
        }
    }

    //raiz-izquierda-derecha
    void recursivePreOrderReport(nodoBST *current, ofstream &f){
        if(current != NULL){
            //imprimir el valor actual de la raiz
            f<<current->id;
            //llamar recursivamente al sub-arbol izquierdo
            if(current->left != NULL)
            {
                f<<" -> ";
            }
            recursivePreOrderReport(current->left, f);
            //llamar recursivamente al sub-arbol derecho
            if(current->right != NULL)
            {
                f<<" -> ";
            }
            recursivePreOrderReport(current->right, f);
        }
    }

    //izquierda-derecha-raiz
    void posOrderReporter(){
        if(root != NULL){
            ofstream f("PosOrder.txt");
            if(f.is_open()){
                f<<"digraph Preorder {" << endl;
                f<<"node [shape = record];" <<endl;
                f<<"rankdir = LR;" <<endl;
                recursivePosOrderReport(root, f);
                //f<<cadena<<endl;
                f<<"}"<<endl;
            }
            else cerr << "Error de apertura del archivo." << endl;
            f.close();
            system("dot -Tpng PosOrder.txt -o PosOrder.png");
            cout<<"\n";
            system("PosOrder.png");
        }else {
            cout<<"No hay objetos para mostrar"<<endl;
        }
    }

    //izquierda-derecha-raiz
    void recursivePosOrderReport(nodoBST *current, ofstream &f){
        if(current != NULL){
            //llamar recursivamente al sub-arbol izquierdo
            recursivePosOrderReport(current->left, f);
            if(current->left != NULL)
            {
                f<<" -> ";
            }
            //llamar recursivamente al sub-arbol derecho
            recursivePosOrderReport(current->right, f);
            if(current->right != NULL)
            {
                f<<" -> ";
            }
            //imprimir el valor actual de la raiz
            f<<current->id;
        }
    }

    //POR MAYOR DE LOS MENORES
    void eliminar1(int valor){
        nodoBST *temp = new nodoBST(valor, "", "", "");
        if(root == NULL){
            cout<<"No hay datos para eliminar"<<endl;
        }else if(root->id == valor){
            if(root->right == NULL && root->left != NULL){
                nodoBST *quitar = root;//Para eliminar si no hubiera recolector de basura
                root = root->left;
                quitar = NULL;
            }else if(root->left == NULL && root->right != NULL){
                nodoBST *quitar = root;//Para eliminar si no hubiera recolector de basura
                root = root->right;
                quitar = NULL;
            }else if(root->left != NULL && root->right != NULL){
                nodoBST *temp = mayorDeMenores(root->left, root, root);
                if(root->left != temp){
                    temp->left = root->left;
                }
                temp->right = root->right;
                /*CODIGO PARA ELIMINAR LA DATA EN C
                root->setLeft(NULL);
                root->setRight(NULL);
                root = NULL;*/
                root = temp;
            }else{
                root = NULL;//ya se que ambos hijos son nulos
            }
        }else{
            //cout<<"caso que no sea la raiz"<<endl;
            buscarEliminar1(root, temp, root);
        }
    }

                    //enviar nodo actual, nodo a eliminar, nodo padre
    void buscarEliminar1(nodoBST *actual, nodoBST *temp, nodoBST *padre){
        //temp es para comparar el nodo que busco
        //cout<<"metodo recursivo 1"<<endl;
        //BUSCO EL NODO A ELIMINAR
        if(temp->id < actual->id){
            if(actual->left != NULL){
                buscarEliminar1(actual->left, temp, actual);
            }else{
                cout<<"no existe"<<endl;
            }
        }else if(temp->id > actual->id){
            if(actual->right != NULL){
                buscarEliminar1(actual->right, temp, actual);
            }else{
                cout<<"no existe"<<endl;
            }
        }else{/*## ELIMINAR EL NODO ENCONTRADO##*/
            //actual es el que voy a quitar (actual ==  temp en este punto)
            //cout<<"eliminar: "<<actual->getKey()<<endl;
            if(actual->left != NULL && actual->right == NULL){
                /*NODO CON UN SOLO HIJO A LA IZQUIERDA*/
                if(actual->id < padre->id){
                    padre->left = actual->left;
                }else{
                    padre->right = actual->left;
                }
                actual = NULL;
            }else if(actual->right != NULL && actual->left == NULL){
                /*NODO CON UN SOLO HIJO A LA DERECHA*/
                if(actual->id < padre->id){
                    padre->left = actual->right;
                }else{
                    padre->right = actual->right;
                }
                actual = NULL;
            }else if(actual->right == NULL && actual->left == NULL){
                /*NODO SIN HIJOS*/
                if(actual->id < padre->id){
                    padre->left = NULL;
                }else{
                    padre->right = NULL;
                }
                actual = NULL;
            }else{
                //metodo para encontrar el mayor de los menores (nodo con dos hijos)
                //node *temp = new node(mayorDeMenores(actual->getLeft(), actual)->getKey());
                nodoBST *temp = mayorDeMenores(actual->left, actual, actual);
                //ACTUAL ES EL QUE ESTOY ELIMINANDO. PADRE, ES EL NODO PADRE DEL QUE ESTOY ELIMINANDO
                //si esta a la izquierda de su padre para que los hijos del que estoy eliminando queden del mismo lado
                if(actual->id < padre->id){
                    temp->right = actual->right;
                    //Para que no se encicle si el nodo izquierdo del que eliminare no tenia nada a la derecha
                    if(actual->left != temp){
                        temp->left = actual->left;
                    }
                    padre->left = temp;
                }
                else{//si esta a la derecha de su padre para que los hijos del que estoy eliminando queden del mismo lado
                    temp->right = actual->right;
                    if(actual->left != temp){
                        temp->left = actual->left;
                    }
                    padre->right = temp;
                }
                /*actual->setLeft(NULL);
                actual->setRight(NULL);
                actual = NULL;*/
                delete actual;//elimina el dato
            }
        }
    }

    //Para encontrar el nodo mayor de los menores
    nodoBST *mayorDeMenores(nodoBST *actual, nodoBST *padre, nodoBST *eliminando){//nodo actual, padre del actual, nodo que quiero eliminar
        //sin el return retornara el ultimo parametro que es a donde apunta el metodo, por eso usar el return
        if(actual->right != NULL){
            mayorDeMenores(actual->right, actual, eliminando);
        }
        else{
            //verificar que el nodo actual no sea hijo del que estoy eliminando
            if(padre != eliminando){
                //si el nodo que moveré, al lugar del que voy a eliminar, tiene hijo izquierdo
                if(actual->left != NULL){
                    padre->right = actual->left;//padre del actual apuntara al hijo izquierdo del actual
                    return actual;
                }
                else{//no tiene hijo izquierdo
                    padre->right = NULL;//padre del actual apuntara a null (quito el nodo de aqui)
                    return actual;
                }
            }else{
                return actual;//el izquierdo del que voy a eliminar
            }
        }
    }

    //RETORNA EL NODO BUSCADO (CON SU LISTA)
    nodoBST *actualNodo(int id){
        nodoBST *temp = new nodoBST(id, "","","");
        if(root == NULL)
        {
            cout<<"el objeto no existe"<<endl;
            return NULL;
        }
        else
        {
            //return recursiveActualNodo(root, id);
            return recursiveActualNodo(root, temp);
        }
    }

    //RETORNA EL NODO BUSCADO (CON SU LISTA)
    //nodoBST *recursiveActualNodo(nodoBST *current, int id)
    nodoBST *recursiveActualNodo(nodoBST *current, nodoBST *temp)
    {
        //if(id < current->id)
        if(temp->id < current->id)
        {
            if(current->left != NULL)
            {
                //recursiveActualNodo(current->left, id);
                recursiveActualNodo(current->left, temp);
            }
            else
            {
                cout<<"el objeto no existe"<<endl;
                return NULL;
            }
        }
        else if(temp->id > current->id)
        {
            if(current->right != NULL)
            {
                recursiveActualNodo(current->right, temp);
            }
            else
            {
                cout<<"el objeto no existe"<<endl;
                return NULL;
            }
        }
        else
        {
            //Existe
            return current;
        }
    }

    //Retornar la lista del nodo para mostrarla
    listaObjetos *ListaActual(int id){
        nodoBST *temp = new nodoBST(id, "","","");
        if(root == NULL)
        {
            cout<<"el objeto no existe"<<endl;
        }
        else
        {
            return recursiveActual(root, temp);
        }
    }

    listaObjetos *recursiveActual(nodoBST *current, nodoBST *temp)
    {
        if(temp->id < current->id)
        {
            if(current->left != NULL)
            {
                recursiveActual(current->left, temp);
            }
            else
            {
                cout<<"el objeto no existe"<<endl;
            }
        }
        else if(temp->id > current->id)
        {
            if(current->right != NULL)
            {
                recursiveActual(current->right, temp);
            }
            else
            {
                cout<<"el el objeto no existe"<<endl;
            }
        }
        else
        {
            return current->lista;
        }
    }

    //Mostrar la lista de cada nodo del arbol (en orden)
    void mostrarArbolLista(){
        recursiveMostrarArbolLista(root);
    }

    void recursiveMostrarArbolLista(nodoBST *actual){
        if(actual != NULL){
            recursiveMostrarArbolLista(actual->left);
            cout<<actual->id<<", "<<actual->nombre<<", "<<actual->letra<<", "<<actual->color<<endl;
            cout<<"Lista de puntos: "<<endl;
            actual->lista->mostrar();
            cout<<""<<endl;

            recursiveMostrarArbolLista(actual->right);
        }
    }
};

class nodoListaNiveles{
public:
    int numNivel;
    nodoListaNiveles *siguiente;
    matrix *nivel;
    BST *objetos;

    nodoListaNiveles(int x){
        this->numNivel = x;
        nivel = NULL;
        objetos = NULL;
        siguiente = NULL;
    }
};

class listaNiveles{
public:
    nodoListaNiveles *primero;
    nodoListaNiveles *ultimo;
    int tam = 0;//tamaño

    listaNiveles(){
        primero = NULL;
        ultimo = NULL;
    }

    int magnitud(){
        return tam;
    }

    void insertar(int x, matrix *matriz, BST *abb){
        nodoListaNiveles *nuevo = new nodoListaNiveles(x);
        matriz->level = x;//ingresa el nivel en que estara la matriz
        nuevo->nivel = matriz;//inserta la matriz
        nuevo->objetos = abb;//inserta el arbol de objetos

        if(primero == NULL)
        {
            primero = nuevo;
            ultimo = nuevo;
            tam++;
            //cout<<"inserto"<<endl;
        }
        else
        {
            nodoListaNiveles *temp = primero;
            while (temp->siguiente != NULL && temp->siguiente->numNivel < x){
                temp = temp->siguiente;
            }
            if(temp->siguiente == NULL){
                temp->siguiente = nuevo;
                ultimo = nuevo;
                tam++;
            }else if(temp->siguiente != NULL && temp->numNivel != x){
                if(temp != primero){
                    nuevo->siguiente = temp->siguiente;
                    temp->siguiente = nuevo;
                }else{
                    nuevo->siguiente = temp;
                    primero = nuevo;
                }
                tam++;
            }else{
                cout<<"El nivel ya existe"<<endl;
            }
        }
    }

    void mostrar(){
        nodoListaNiveles *temp = primero;
        while (temp != NULL){
            cout<<"Nivel: "<<temp->numNivel<<endl;
            temp = temp->siguiente;
        }
    }

    void mostrarProyecto(){
        nodoListaNiveles *temp = primero;
        while (temp != NULL){
            //cout<<"Nivel: "<<temp->numNivel<<endl;
            temp->nivel->reporteMatriz();
            temp = temp->siguiente;
        }
    }

    void eliminarNivel(int nivel){
        if(primero != NULL){
            nodoListaNiveles *temp = primero;
            if(temp->numNivel != nivel){
                while (temp->siguiente != NULL && temp->siguiente->numNivel != nivel){
                    temp = temp->siguiente;
                }
                if(temp->siguiente != NULL && temp->siguiente->numNivel == nivel){
                    nodoListaNiveles *aux = temp->siguiente;//el que voy a eliminar
                    if(aux->siguiente != NULL){
                        temp->siguiente = aux->siguiente;//si el nivel esta entre la cabeza y el ultimo
                        aux->siguiente = NULL;
                        cout<<"Nivel eliminado"<<endl;
                    }else{
                        temp->siguiente = NULL;//si es el ultimo nivel
                        cout<<"Nivel eliminado"<<endl;
                    }
                }else{
                    cout<<"nivel no existe"<<endl;
                }
            }else{
                //nodoListaNiveles *aux = temp;//el que voy a eliminar
                primero = temp->siguiente;
                temp = NULL;
                cout<<"Nivel eliminado"<<endl;
            }
        }else{
            cout<<"el nivel no existe"<<endl;
        }
    }

    //Retorna la matriz del nivel
    matrix *matrizVer(int nivel){
        nodoListaNiveles *temp = primero;
        while (temp->numNivel != nivel && temp->siguiente != NULL){
            temp = temp->siguiente;
        }
        if(temp->numNivel == nivel){
            return temp->nivel;
        }else{
            //cout<<"No existe el nivel"<<endl;
            return NULL;
        }
    }

    //retorna el arbol del nivel
    BST *abbVer(int nivel){
        nodoListaNiveles *temp = primero;
        while (temp->numNivel != nivel && temp->siguiente != NULL){
            temp = temp->siguiente;
        }
        if(temp->numNivel == nivel){
            return temp->objetos;
        }else{
            cout<<"No existe el nivel"<<endl;
        }
    }
};

class nodoAVL{
public:
    string key;//nombre del proyecto
    int fe;//factor de quilibrio del nodo
    nodoAVL *left;//hijo izquierdo
    nodoAVL *right;//hijo derecho
    listaNiveles *listaN;//lista de niveles (matriz y abb de objetos)

    nodoAVL(string key){
        this->key = key;
        this->fe = 0;
        left = right  = NULL;
        listaN = NULL;
    }

    nodoAVL(string key, listaNiveles *nivel){
        this->key = key;
        this->fe = 0;
        left = right  = NULL;
        listaN = nivel;
    }
};

class AVL{
public:
    nodoAVL *root;

    AVL(){
        root = NULL;
    }

    //Para calcular la altura del nodo o factor de equilibrio.
    //devuelve el factor de equilibrio mas grande de los dos
    int maximo(int val1, int val2){
        if(val1 > val2)
            return val1;
        return val2;
    }

    //Obtener el factor de equilibrio
    int obtenerFe(nodoAVL *x){
        if(x == NULL){
            return -1;
        }else{
            return x->fe;
        }
    }

    //Rotacion simple izquierda
    nodoAVL *rotacionIzquierda(nodoAVL *rotar){
        nodoAVL *aux = rotar->left;
        rotar->left = aux->right;
        aux->right = rotar;
        //obtengo el factor de equilibrio mas grande de los dos hijos
        rotar->fe = maximo(obtenerFe(rotar->left), obtenerFe(rotar->right)) + 1;//calculo el fe de rotar
        aux->fe = maximo(obtenerFe(aux->left), obtenerFe(aux->right)) + 1;//calculo el fe de auxiliar
        return aux;
    }

    //Rotacion simple derecha
    nodoAVL *rotacionDerecha(nodoAVL *rotar){
        nodoAVL *aux = rotar->right;
        rotar->right = aux->left;
        aux->left = rotar;
        rotar->fe = maximo(obtenerFe(rotar->left), obtenerFe(rotar->right)) + 1;
        aux->fe = maximo(obtenerFe(aux->left), obtenerFe(aux->right)) + 1;
        return aux;
    }

     //Rotacion doble a la derecha
    nodoAVL *rotacionDobleIzquierda(nodoAVL *rotar){
        nodoAVL *temp;
        rotar->left = rotacionDerecha(rotar->left);
        temp = rotacionIzquierda(rotar);
        return temp;
    }

    //Rotacion doble a la izquierda
    nodoAVL *rotacionDobleDerecha(nodoAVL *rotar){
        nodoAVL *temp;
        rotar->right = rotacionIzquierda(rotar->right);
        temp = rotacionDerecha(rotar);
        return temp;
    }

    nodoAVL *insertarRecursivo(nodoAVL *nuevo, nodoAVL *actual){
        //si esta a la izquierda
        if(nuevo->key < actual->key){
            if(actual->left == NULL){
                actual->left = nuevo;
            }else{
                actual->left = insertarRecursivo(nuevo, actual->left);
                //Obtener la diferencia entre los factores de equilibrio para ver si esta desbalanceado (si es igual a 2 esta desbalanceado)
                if((obtenerFe(actual->left) - obtenerFe(actual->right)) == 2){
                    if(nuevo->key < actual->left->key){
                        actual = rotacionIzquierda(actual);//si esta a la izquierda rotacion simple a la izquierda
                    }else{
                        actual = rotacionDobleIzquierda(actual);//si esta a la derecha es una rotacion doble (derecha/izquierda)
                    }
                }
            }
        }//si esta a la derecha
        else if(nuevo->key > actual->key){
            if(actual->right == NULL){
                actual->right = nuevo;
            }else{
                actual->right = insertarRecursivo(nuevo, actual->right);
                if((obtenerFe(actual->right) - obtenerFe(actual->left)) == 2){
                    if(nuevo->key > actual->right->key){
                        actual = rotacionDerecha(actual);//si esta a la derecha es rotacion simple a la derecha
                    }else{
                        actual = rotacionDobleDerecha(actual);//si esta a la izquiera es rotacion doble derecha (izquierda/derecha)
                    }
                }
            }
        }else{
            cout<<"nodo duplicado"<<endl;
        }
        //actualizando la altura o factor de equilibrio
        if(actual->left == NULL && actual->right != NULL){
            actual->fe = actual->right->fe + 1;//hijo derecha (fe de ese hijo)
        }else if(actual->right == NULL && actual->left != NULL){
            actual->fe = actual->left->fe + 1;//hijo izquierda (fe de ese hijo)
        }else{
            actual->fe = maximo(obtenerFe(actual->left), obtenerFe(actual->right)) + 1;//si tiene ambos hijos obtiene el fe mayor de ambos
        }
        return actual;
    }

    //Metodo insertar inicial
    //void insertar(string d){
    void insertar(string d, listaNiveles *nivelesProyecto){
        //nodoAVL *nuevo = new nodoAVL(d);
        //nuevo->listaN = nivelesProyecto;
        nodoAVL *nuevo = new nodoAVL(d, nivelesProyecto);
        if(root == NULL){
            root = nuevo;
        }else{
            root = insertarRecursivo(nuevo, root);
        }
    }

    //POR MAYOR DE LOS MENORES
    void eliminar(string valor){
        nodoAVL *temp = new nodoAVL(valor);
        if(root == NULL){
            cout<<"No hay datos para eliminar"<<endl;
        }else if(root->key == valor){
            if(root->right == NULL && root->left != NULL){
                nodoAVL *quitar = root;//Para eliminar si no hubiera recolector de basura
                root = root->left;
                quitar = NULL;
                root = rebalancear(root);//vuelvo a balancear la raiz
            }else if(root->left == NULL && root->right != NULL){
                nodoAVL *quitar = root;//Para eliminar si no hubiera recolector de basura
                root = root->right;
                quitar = NULL;
                root = rebalancear(root);
            }else if(root->left != NULL && root->right != NULL){
                nodoAVL *temp = mayorDeMenores(root->left, root, root);
                if(root->left != temp){
                    temp->left = root->left;
                }
                temp->right = root->right;
                /*CODIGO PARA ELIMINAR LA DATA EN C
                root->setLeft(NULL);
                root->setRight(NULL);
                root = NULL;*/
                root = temp;
                root = rebalancear(root);
            }else{
                root = NULL;//ya se que ambos hijos son nulos
            }
        }else{
            //cout<<"caso que no sea la raiz"<<endl;
            buscarEliminar1(root, temp, root);
        }
    }

                    //enviar nodo actual, nodo a eliminar, nodo padre
    void buscarEliminar1(nodoAVL *actual, nodoAVL *temp, nodoAVL *padre){
        //temp es para comparar el nodo que busco
        //cout<<"metodo recursivo 1"<<endl;
        //BUSCO EL NODO A ELIMINAR
        if(temp->key < actual->key){
            if(actual->left != NULL){
                buscarEliminar1(actual->left, temp, actual);
                if(actual != root){
                    actual = rebalancear(actual);//balancea por si elimino
                }else{
                    actual = rebalancear(actual);//balancea por si elimino
                    root = actual;//lo vuelvo raiz por si al rotar la raiz del arbol cambia
                }
            }else{
                cout<<"no existe"<<endl;
            }
        }else if(temp->key > actual->key){
            if(actual->right != NULL){
                buscarEliminar1(actual->right, temp, actual);
                if(actual != root){
                    actual = rebalancear(actual);//balancea por si elimino
                }else{
                    actual = rebalancear(actual);//balancea por si elimino
                    root = actual;
                }
            }else{
                cout<<"no existe"<<endl;
            }
        }else{/*## ELIMINAR EL NODO ENCONTRADO##*/
            //actual es el que voy a quitar (actual ==  temp en este punto)
            //cout<<"eliminar: "<<actual->getKey()<<endl;
            if(actual->left != NULL && actual->right == NULL){
                /*NODO CON UN SOLO HIJO A LA IZQUIERDA*/
                if(actual->key < padre->key){
                    padre->left = actual->left;
                    actual->left = NULL;//elimine el nodo

                }else{
                    padre->right = actual->left;
                }
                actual = NULL;
            }else if(actual->right != NULL && actual->left == NULL){
                /*NODO CON UN SOLO HIJO A LA DERECHA*/
                if(actual->key < padre->key){
                    padre->left = actual->right;
                }else{
                    padre->right = actual->right;
                }
                actual = NULL;
            }else if(actual->right == NULL && actual->left == NULL){
                /*NODO SIN HIJOS*/
                if(actual->key < padre->key){
                    padre->left = NULL;
                }else{
                    padre->right = NULL;
                }
                actual = NULL;
            }else{
                //metodo para encontrar el mayor de los menores (nodo con dos hijos)
                //node *temp = new node(mayorDeMenores(actual->getLeft(), actual)->getKey());
                nodoAVL *temp = mayorDeMenores(actual->left, actual, actual);
                //ACTUAL ES EL QUE ESTOY ELIMINANDO. PADRE, ES EL NODO PADRE DEL QUE ESTOY ELIMINANDO
                //si esta a la izquierda de su padre para que los hijos del que estoy eliminando queden del mismo lado
                if(actual->key < padre->key){
                    temp->right = actual->right;
                    //Para que no se encicle si el nodo izquierdo del que eliminare no tenia nada a la derecha
                    if(actual->left != temp){
                        temp->left = actual->left;
                    }
                    padre->left = temp;
                }
                else{//si esta a la derecha de su padre para que los hijos del que estoy eliminando queden del mismo lado
                    temp->right = actual->right;
                    if(actual->left != temp){
                        temp->left = actual->left;
                    }
                    padre->right = temp;
                }
                /*actual->setLeft(NULL);
                actual->setRight(NULL);
                actual = NULL;*/
                delete actual;//elimina el dato (uso delete por ser clase, si fuera struct uso "free"
            }
        }
    }

    //Para encontrar el nodo mayor de los menores
    nodoAVL *mayorDeMenores(nodoAVL *actual, nodoAVL *padre, nodoAVL *eliminando){//nodo actual, padre del actual, nodo que quiero eliminar
        //sin el return retornara el ultimo parametro que es a donde apunta el metodo, por eso usar el return
        if(actual->right != NULL){
            mayorDeMenores(actual->right, actual, eliminando);
        }
        else{
            //verificar que el nodo actual no sea hijo del que estoy eliminando
            if(padre != eliminando){
                //si el nodo que moveré, al lugar del que voy a eliminar, tiene hijo izquierdo
                if(actual->left != NULL){
                    padre->right = actual->left;//padre del actual apuntara al hijo izquierdo del actual
                    return actual;
                }
                else{//no tiene hijo izquierdo
                    padre->right = NULL;//padre del actual apuntara a null (quito el nodo de aqui)
                    return actual;
                }
            }else{
                return actual;//el izquierdo del que voy a eliminar
            }
        }
    }

    nodoAVL *rebalancear(nodoAVL *raiz){
        int lado = (obtenerFe(raiz->left) - obtenerFe(raiz->right));
        if(lado == 2){
            if((obtenerFe(raiz->left->left) - obtenerFe(raiz->left->right)) >= 0){
                raiz = rotacionIzquierda(raiz);//si esta a la izquierda rotacion simple a la izquierda
            }else{
                raiz = rotacionDobleIzquierda(raiz);//si esta a la derecha es una rotacion doble (derecha/izquierda)
            }
        }else if(lado == -2){
            if((obtenerFe(raiz->right->left) - obtenerFe(raiz->right->right)) <= 0){
                raiz = rotacionDerecha(raiz);//si esta a la derecha es rotacion simple a la derecha
            }else{
                raiz = rotacionDobleDerecha(raiz);//si esta a la izquiera es rotacion doble derecha (izquierda/derecha)
            }
        }
        raiz->fe = maximo(obtenerFe(raiz->left), obtenerFe(raiz->right)) + 1;//si tiene ambos hijos obtiene el fe mayor de ambos
        return raiz;
    }

    void AVLReporter(){
        if(root != NULL){
            ofstream f("AVL.txt");
            if(f.is_open()){
                f<<"digraph Tree {" << endl;
                //Mostrar si solo existe la raiz
                if(root->left == NULL && root->right == NULL){
                    f<<"\""<<root->key<<"\""<<endl;
                }
                recursiveReporter(root, f);
                f<<"}"<<endl;
            }
            else cerr << "Error de apertura del archivo." << endl;
            f.close();
            system("dot -Tpng AVL.txt -o AVL.png");
            cout<<"\n";
            system("AVL.png");
        }else{
            cout<<"No hay objetos para mostrar"<<endl;
        }
    }

    void recursiveReporter(nodoAVL *current, ofstream &f){
        if(current->left != NULL){
            //if is only child of the node
            if(current->right == NULL){
                f<<"\""<<current->key<<"\" -> \""<<current->left->key<<"\""<<endl;
                //2 is central node and 3 is right node, both invisible
                f<<"\""<<current->key<<"2\""<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<"\""<<current->key<<"\" -> \""<<current->key<<"2\""<<"[style = \"invis\"];"<<endl;
                f<<"\""<<current->key<<"3\""<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<"\""<<current->key<<"\" -> \""<<current->key<<"3\""<<"[style = \"invis\"];"<<endl;
            }
            //if node have both nodes
            else{
                f<<"\""<<current->key<<"\" -> \""<<current->left->key<<"\""<<endl;
            }
            recursiveReporter(current->left, f);
        }
        if(current->right != NULL){
            //if is only child of the node
            if(current->left == NULL){
                //2 is left node and 3 is central node, both invisible
                f<<"\""<<current->key<<"2\""<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<"\""<<current->key<<"\" -> \""<<current->key<<"2\""<<"[style = \"invis\"];"<<endl;
                f<<"\""<<current->key<<"3\""<<"[label=\"\", style=\"invis\"];"<<endl;
                f<<"\""<<current->key<<"\" -> \""<<current->key<<"3\""<<"[style = \"invis\"];"<<endl;
                f<<"\""<<current->key<<"\" -> \""<<current->right->key<<"\""<<endl;
            }
            //if are both child
            else{
                f<<"\""<<current->key<<"\" -> \""<<current->right->key<<"\""<<endl;
            }
            recursiveReporter(current->right, f);
        }
    }

    void inOrden(){
        if(root != NULL){
            recursiveInOrden(root);
        }else{
            cout<<"Aun no se han agregado proyectos"<<endl;
        }

    }

    void recursiveInOrden(nodoAVL *actual){
        if(actual != NULL){
            recursiveInOrden(actual->left);
            cout<<actual->key<<endl;
            recursiveInOrden(actual->right);
        }
    }

    nodoAVL *proyecto(string nombre){
        nodoAVL *temp = new nodoAVL(nombre);
        if(root != NULL){
            //return proyectoRecursivo(nombre, root);
            return proyectoRecursivo(temp, root);
        }else{
            //cout<<"Aun no se agregan proyectos"<<endl;
            return NULL;
        }

    }

    //nodoAVL *proyectoRecursivo(string nombre, nodoAVL *actual){
    nodoAVL *proyectoRecursivo(nodoAVL *retornar, nodoAVL *actual){
        //cout<<"actual"<<actual->key<<endl;
        //if(nombre < actual->key){
        if(retornar->key < actual->key){
            if(actual->left != NULL){
                //proyectoRecursivo(nombre, actual->left);
                proyectoRecursivo(retornar, actual->left);
                //cout<<"actual"<<actual->key<<endl;
            }else{
                //cout<<"no existe"<<endl;
                return NULL;
                //cout<<"actual"<<actual->key<<endl;
            }
        //}else if(nombre > actual->key){
        }else if(retornar->key > actual->key){
            //cout<<"actual"<<actual->key<<endl;
            if(actual->right != NULL){
                //cout<<"actual"<<actual->right->key<<endl;
                //proyectoRecursivo(nombre, actual->right);
                proyectoRecursivo(retornar, actual->right);
            }else{
                //cout<<"no existe"<<endl;
                return NULL;
            }
        }else{//## RETORNAR EL NODO ENCONTRADO##*/
            //cout<<"el nodo encontrado es: "<<actual->key<<endl;
            //cout<<"proyecto desde el arbol"<<actual->key<<endl;
            return actual;
        }
    }
};

class nodoListaAscendete{
public:
    nodoListaAscendete *siguiente;
    string proyecto;
    int noNiveles;

    nodoListaAscendete(string nombre, int niveles){
        this->proyecto = nombre;
        this->noNiveles = niveles;
        siguiente = NULL;
    }
};
/*
class listaAscendente{
public:
    nodoListaObjetos *primero;
    nodoListaObjetos *ultimo;
    int tam = 0;//tamaño

    listaAscendente()
    {
        primero = NULL;
        ultimo = NULL;
    }

    int magnitud(){
        return tam;
    }

    void insertar(string nombre, int niveles){
        nodoListaAscendete *temp = new nodoListaAscendete(nombre, niveles);
        if(primero == NULL)
        {
            primero = temp;
            ultimo = temp;
            tam++;
        }
        else
        {
           nodoListaAscendete() *temp = primero;
            while (temp->siguiente != NULL && temp->siguiente->noNiveles < niveles){
                temp = temp->siguiente;
            }
            if(temp->siguiente == NULL){
                temp->siguiente = nuevo;
                ultimo = nuevo;
                tam++;
            }else if(temp->siguiente != NULL && temp->numNivel != x){
                if(temp != primero){
                    nuevo->siguiente = temp->siguiente;
                    temp->siguiente = nuevo;
                }else{
                    nuevo->siguiente = temp;
                    primero = nuevo;
                }
                tam++;
            }else{
                cout<<"El nivel ya existe"<<endl;
            }
        }
    }

    void mostrar(){
        nodoListaObjetos *temp = primero;
        while (temp != NULL){
            cout<<temp->x<<", "<<temp->y<<endl;
            temp = temp->siguiente;
        }
    }
};
*/

void leerJsonLibrerias(BST *global, string archivo){
    string fName = archivo;
    json jfile;//declaro un objeto de tipo json
    ifstream doc(fName);//modificar para recibir la ruta del archivo
    //if(-doc.is_open()) std::cout<<"ERROR: No se pudo abrir el archivo"<<endl;
    if(!doc.is_open()){
        cout<<"ERROR: No se pudo abrir el archivo. Verifique la ruta o que el archivo exista"<<endl;
    }else{
        doc>>jfile;//paso el contenido al reader.
        //CARGO LOS OBJETOS INICIALES
        //leo la lista libreria
        json objetos = jfile["Libreria"];
        int dcant = objetos.size();
        for(int i = 0; i < dcant; i++){
            json objeto = objetos[i];
            //creo el nodo del arbol
            int jIdentificador = objeto["identificador"];
            string jNombre = objeto["nombre"];
            string jLetra = objeto["letra"];
            string jColor = objeto["color"];
            nodoBST *nuevoObjeto = new nodoBST(jIdentificador, jNombre, jLetra, jColor);
            //cout<<nuevoObjeto->id<<", "<<nuevoObjeto->nombre<<", "<<nuevoObjeto->letra<<", "<<nuevoObjeto->color<<endl;
            //creo la lista del arbol
            json coordenadas = objeto["puntos"];
            int cant = coordenadas.size();
            listaObjetos *nuevaLista = new listaObjetos();
            for(int j = 0; j < cant; j++){
                json valores = coordenadas[j];
                int x = valores["x"];
                int y = valores["y"];
                nuevaLista->insertar(x, y);
            }
            //inserto el objeto al arbol
            global->add(nuevoObjeto, nuevaLista);
            //nuevaLista->mostrar();
        }
        cout<<"el archivo fue leido exitosamente"<<endl;
    }
    //global->mostrarArbolLista();
}

void leerNivel(string archivo, listaNiveles *listaNivel, BST* general){
    //crear matriz
    matrix *nuevaMatriz = new matrix;
    //crear arbol ABB
    BST *nuevoArbol = new BST;

    string fName = archivo;
    json jfile;//declaro un objeto de tipo json
    ifstream doc(fName);//modificar para recibir la ruta del archivo

    //if(-doc.is_open()) std::cout<<"ERROR: No se pudo abrir el archivo"<<endl;
    if(!doc.is_open()){
        cout<<"ERROR: No se pudo abrir el archivo. Verifique la ruta o que el archivo exista"<<endl;
    }else{
        doc>>jfile;//paso el contenido al reader.
        //CARGO LOS OBJETOS INICIALES
        //leo la lista libreria
        json niveles = jfile["niveles"];
        json nuevoNivel = niveles[0];//no me da problema porque solo leo la posicion 0
        int nivel = nuevoNivel["nivel"];
        //cout<<"nivel leido: "<<nivel<<endl;
        //leo la sublista de nuevo nivel puesto que hay mas atributos
        //si dentro de la lista solo hubieran listas podría acceder a ellas directamente

        //##########INSERTAR PAREDES##########
        json paredes = nuevoNivel["paredes"];
        int cant = paredes.size();
        for(int i = 0; i < cant; i++){
            json pared = paredes[i];
            json inicio = pared["inicio"];
            int xi = inicio[0];//xinicial
            int yi = inicio[1];//yinicial
            json fin = pared["final"];
            int xf = fin[0];//xfinal
            int yf = fin[1];//yfinal
            string color = pared["color"];
            if((xf - xi) != 0){
                for(int x = xi; x <= xf; x++){
                    char existeCasilla;
                    existeCasilla = nuevaMatriz->verificarCasilla(x, yi);
                    if(existeCasilla == 'N'){
                        nuevaMatriz->add(x, yi, "p", color);
                    }
                }
            }else if((yf - yi) != 0){
                for(int y = yi; y <= yf; y++){
                    if(nuevaMatriz->verificarCasilla(xi, y) == 'N'){
                        nuevaMatriz->add(xi, y, "p", color);
                    }
                }
            }else{
                if(nuevaMatriz->verificarCasilla(xi, yi) == 'N'){
                        nuevaMatriz->add(xi, yi, "p", color);
                }
            }
        }

        //##########INSERTAR VENTANAS##########
        json ventanas = nuevoNivel["ventanas"];
        int cantv = ventanas.size();
        for(int i = 0; i < cantv; i++){
            json ventana = ventanas[i];
            json inicio = ventana["inicio"];
            int xi = inicio[0];//xinicial
            int yi = inicio[1];//yinicial
            json fin = ventana["final"];
            int xf = fin[0];//xfinal
            int yf = fin[1];//yfinal
            string color = ventana["color"];
            if((xf - xi) != 0){
                for(int x = xi; x <= xf; x++){
                    char existeCasilla;
                    existeCasilla = nuevaMatriz->verificarCasilla(x, yi);
                    if(existeCasilla == 'N'){
                        nuevaMatriz->add(x, yi, "v", color);
                    }
                }
            }else if((yf - yi) != 0){
                for(int y = yi; y <= yf; y++){
                    if(nuevaMatriz->verificarCasilla(xi, y) == 'N'){
                        nuevaMatriz->add(xi, y, "v", color);
                    }
                }
            }else{
                if(nuevaMatriz->verificarCasilla(xi, yi) == 'N'){
                        nuevaMatriz->add(xi, yi, "v", color);
                }
            }
        }
        //nuevaMatriz->reporteMatriz();
        //##########INSERTAR objetos##########
        json objetos = nuevoNivel["objetos"];
        int dcant = objetos.size();
        //BST *nuevoArbol = new BST;
        for(int i = 0; i < dcant; i++){
            json objeto = objetos[i];
            //creo el nodo del arbol
            int jIdentificador = objeto["identificador"];
            string jNombre = objeto["nombre"];
            string jLetra = objeto["letra"];
            string jColor = objeto["color"];
            //nodoBST *nuevoObjeto = new nodoBST(jIdentificador, jNombre, jLetra, jColor);
            //cout<<nuevoObjeto->id<<", "<<nuevoObjeto->nombre<<", "<<nuevoObjeto->letra<<", "<<nuevoObjeto->color<<endl;

            json coordenadas = objeto["puntos"];
            int cant = coordenadas.size();
            //creo la lista del nodo del arbol
            listaObjetos *nuevoObjeto = new listaObjetos();
            //creo la lista del nodo del arbol general
            listaObjetos *nuevoObjetoOrigen = new listaObjetos();
            json valorInicial = coordenadas[0];
            int a = valorInicial["x"];
            int b = valorInicial["y"];

            for(int j = 0; j < cant; j++){
                json valores = coordenadas[j];
                int x = valores["x"];
                int y = valores["y"];
                int cx = x - a;
                int cy = y - b;
                nuevoObjeto->insertar(x, y);
                nuevoObjetoOrigen->insertar(cx, cy);
            }
            /*cout<<"objeto en nivel"<<endl;
            nuevoObjeto->mostrar();
            cout<<"\n objeto en general"<<endl;
            nuevoObjetoOrigen->mostrar();*/

            //verifico que se pueda insertar en la matriz
            bool insertar = true;
            nodoListaObjetos *temp = nuevoObjeto->primero;
            while (insertar == true && temp != NULL){
                char verificar = nuevaMatriz->verificarCasilla(temp->x, temp->y);
                if(verificar != 'N'){
                    insertar = false;
                    //DECIR QUE OBJETO NO SE PUDO INSERTAR
                    cout<<"el objeto "<<jNombre<<" no puede insertarse en esta posicion"<<endl;
                }
                temp = temp->siguiente;
            }

            //inserto el objeto al arbol y a la matriz
            if(insertar){
                nuevoArbol->add(new nodoBST(jIdentificador, jNombre, jLetra, jColor), nuevoObjeto);
                general->add(new nodoBST(jIdentificador, jNombre, jLetra, jColor), nuevoObjetoOrigen);
                nodoListaObjetos *ingresar = nuevoObjeto->primero;
                while(ingresar != NULL){
                    nuevaMatriz->add(ingresar->x, ingresar->y, jLetra, jColor);
                    ingresar = ingresar->siguiente;
                }
            }
            //listaNivel->insertar(nivel, nuevaMatriz, nuevoArbol);
            //nuevaLista->mostrar();
        }
        listaNivel->insertar(nivel, nuevaMatriz, nuevoArbol);
        cout<<"el archivo fue leido exitosamente"<<endl;
    }
}

void leerProyecto(string archivo, AVL *principal, BST *general){

    string fName = archivo;
    json jfile;//declaro un objeto de tipo json
    ifstream doc(fName);//modificar para recibir la ruta del archivo

    //if(-doc.is_open()) std::cout<<"ERROR: No se pudo abrir el archivo"<<endl;
    if(!doc.is_open()){
        cout<<"ERROR: No se pudo abrir el archivo. Verifique la ruta o que el archivo exista"<<endl;
    }else{
        doc>>jfile;//paso el contenido al reader.
        //leo el proyecto
        json proyectos = jfile["proyectos"];
        int pcant = proyectos.size();
        for(int p = 0; p < pcant; p++){
            json nuevoProyecto = proyectos[p];
            if(nuevoProyecto.size() != 0){
                //crear la lista de niveles del proyecto
                listaNiveles *nuevaListaNiveles = new listaNiveles;
                //nombre del proyecto
                string nombre = nuevoProyecto["nombre"];
                //cargo los niveles
                json niveles = nuevoProyecto["niveles"];
                int nivcant = niveles.size();
                for(int n = 0; n < nivcant; n++){
                    json nuevoNivel = niveles[n];
                    if(nuevoNivel.size() != 0){
                        //crear matriz
                        matrix *nuevaMatriz = new matrix;
                        //crear arbol ABB
                        BST *nuevoArbol = new BST;
                        //cout<<"vacio"<<endl;
                        int nivel = nuevoNivel["nivel"];
                        //cout<<"nivel leido: "<<nivel<<endl;
                        //##########INSERTAR PAREDES##########
                        json paredes = nuevoNivel["paredes"];
                        int cant = paredes.size();
                        for(int i = 0; i < cant; i++){
                            json pared = paredes[i];
                            if(pared.size() != 0){
                                json inicio = pared["inicio"];
                                int xi = inicio[0];//xinicial
                                int yi = inicio[1];//yinicial
                                json fin = pared["final"];
                                int xf = fin[0];//xfinal
                                int yf = fin[1];//yfinal
                                string color = pared["color"];
                                if((xf - xi) != 0){
                                    for(int x = xi; x <= xf; x++){
                                        char existeCasilla;
                                        existeCasilla = nuevaMatriz->verificarCasilla(x, yi);
                                        if(existeCasilla == 'N'){
                                            nuevaMatriz->add(x, yi, "p", color);
                                        }
                                    }
                                }else if((yf - yi) != 0){
                                    for(int y = yi; y <= yf; y++){
                                        if(nuevaMatriz->verificarCasilla(xi, y) == 'N'){
                                            nuevaMatriz->add(xi, y, "p", color);
                                        }
                                    }
                                }else{
                                    if(nuevaMatriz->verificarCasilla(xi, yi) == 'N'){
                                            nuevaMatriz->add(xi, yi, "p", color);
                                    }
                                }
                            }//else{posicion vacia}

                        }//fin insertar paredes

                        //##########INSERTAR VENTANAS##########
                        json ventanas = nuevoNivel["ventanas"];
                        int cantv = ventanas.size();
                        for(int i = 0; i < cantv; i++){
                            json ventana = ventanas[i];
                            if(ventana.size() != 0){
                                json inicio = ventana["inicio"];
                                int xi = inicio[0];//xinicial
                                int yi = inicio[1];//yinicial
                                json fin = ventana["final"];
                                int xf = fin[0];//xfinal
                                int yf = fin[1];//yfinal
                                string color;
                                json ventanaColor = ventana["color"];
                                if(ventanaColor.size() != 0){
                                    color = ventana["color"];
                                }else{
                                    color = "#B0E0E6";
                                }

                                if((xf - xi) != 0){
                                    for(int x = xi; x <= xf; x++){
                                        char existeCasilla;
                                        existeCasilla = nuevaMatriz->verificarCasilla(x, yi);
                                        if(existeCasilla == 'N'){
                                            nuevaMatriz->add(x, yi, "v", color);
                                        }
                                    }
                                }else if((yf - yi) != 0){
                                    for(int y = yi; y <= yf; y++){
                                        if(nuevaMatriz->verificarCasilla(xi, y) == 'N'){
                                            nuevaMatriz->add(xi, y, "v", color);
                                        }
                                    }
                                }else{
                                    if(nuevaMatriz->verificarCasilla(xi, yi) == 'N'){
                                            nuevaMatriz->add(xi, yi, "v", color);
                                    }
                                }
                            }
                        }//fin insertar ventanas

                        //##########INSERTAR objetos##########
                        json objetos = nuevoNivel["objetos"];
                        int dcant = objetos.size();
                        for(int i = 0; i < dcant; i++){
                            json objeto = objetos[i];
                            if(objeto.size() != 0){
                                //creo el nodo del arbol
                                int jIdentificador = objeto["identificador"];
                                string jNombre = objeto["nombre"];
                                string jLetra = objeto["letra"];
                                string jColor = objeto["color"];

                                //creo la lista del nodo del arbol del nivel
                                json coordenadas = objeto["puntos"];
                                int cant = coordenadas.size();
                                listaObjetos *nuevoObjeto = new listaObjetos();
                                //creo la lista del nodo del arbol general
                                listaObjetos *nuevoObjetoOrigen = new listaObjetos();
                                json valorInicial = coordenadas[0];
                                int a = valorInicial["x"];
                                int b = valorInicial["y"];

                                for(int j = 0; j < cant; j++){
                                    json valores = coordenadas[j];
                                    int x = valores["x"];
                                    int y = valores["y"];
                                    int cx = x - a;
                                    int cy = y - b;
                                    nuevoObjeto->insertar(x, y);
                                    nuevoObjetoOrigen->insertar(cx, cy);
                                }
                                //cout<<"\n objeto en nivel"<<endl;
                                //nuevoObjeto->mostrar();
                                //cout<<"\n objeto en general"<<endl;
                                //nuevoObjetoOrigen->mostrar();

                                //verifico que se pueda insertar en la matriz
                                bool insertar = true;
                                nodoListaObjetos *temp = nuevoObjeto->primero;
                                while (insertar == true && temp != NULL){
                                    char verificar = nuevaMatriz->verificarCasilla(temp->x, temp->y);
                                    if(verificar != 'N'){
                                        insertar = false;
                                        //DECIR QUE OBJETO NO SE PUDO INSERTAR
                                        cout<<"el objeto "<<jNombre<<" no puede insertarse en esta posicion"<<endl;
                                    }
                                    temp = temp->siguiente;
                                }

                                //inserto el objeto al arbol y a la matriz
                                if(insertar){
                                    nuevoArbol->add(new nodoBST(jIdentificador, jNombre, jLetra, jColor), nuevoObjeto);
                                    general->add(new nodoBST(jIdentificador, jNombre, jLetra, jColor), nuevoObjetoOrigen);
                                    nodoListaObjetos *ingresar = nuevoObjeto->primero;
                                    while(ingresar != NULL){
                                        nuevaMatriz->add(ingresar->x, ingresar->y, jLetra, jColor);
                                        ingresar = ingresar->siguiente;
                                    }
                                }
                                //nuevaLista->mostrar();
                            }
                        }//Cierro insertar objeto
                        nuevaListaNiveles->insertar(nivel, nuevaMatriz, nuevoArbol);
                    }else{
                        cout<<"Nivel vacio"<<endl;
                    }
                    //nuevaListaNiveles->insertar(nivel, nuevaMatriz, nuevoArbol);
                }//Cierra el nivel
                cout<<nombre<<endl;
                nuevaListaNiveles->mostrar();
                //nuevaListaNiveles->mostrarProyecto();
                system("pause");
                principal->insertar(nombre, nuevaListaNiveles);
            }//esta llave cierra la lectura de UN proyecto
            //principal->insertar(nombre, nuevaListaNiveles);
        }//cierro la lectura de varios archivos
        cout<<"el archivo fue leido exitosamente"<<endl;
    }//esta llave cierra la lectura del documento json
}

/*########## PRUEBA GENERAL DE TODAS LAS ESTRUCTURAS ##########*/
void insertaravl(AVL *intentar){
    //Matrices a insertar
    matrix *matrizI = new matrix;
    matrizI->add(1,1,"K","#F6FB34");
    matrizI->add(2,2,"E","#F6FB34");
    matrizI->add(3,3,"V","#F6FB34");
    matrizI->add(3,2,"I","#F6FB34");
    matrizI->add(2,3,"N","#F6FB34");

    matrix *matrizII = new matrix;
    matrizII->add(1,1,"M","#F6FB34");
    matrizII->add(2,2,"A","#F6FB34");
    matrizII->add(3,3,"R","#F6FB34");
    matrizII->add(3,2,"T","#F6FB34");
    matrizII->add(2,5,"I","#F6FB34");
    matrizII->add(2,6,"N","#F6FB34");

    matrix *matrizIII = new matrix;
    matrizIII->add(1,1,"S","#F6FB34");
    matrizIII->add(2,2,"A","#F6FB34");
    matrizIII->add(3,3,"M","#F6FB34");
    matrizIII->add(3,2,"A","#F6FB34");
    matrizIII->add(2,3,"Y","#F6FB34");
    matrizIII->add(2,5,"O","#F6FB34");
    matrizIII->add(2,6,"A","#F6FB34");

    matrix *matrizIIII = new matrix;
    matrizIIII->add(1,1,"U","#F6FB34");
    matrizIIII->add(2,2,"R","#F6FB34");
    matrizIIII->add(3,3,"I","#F6FB34");
    matrizIIII->add(3,2,"Z","#F6FB34");
    matrizIIII->add(2,3,"A","#F6FB34");
    matrizIIII->add(2,4,"R","#F6FB34");

    //Lista de los objetos del arbol
    listaObjetos *nuevoObjeto = new listaObjetos();
    nuevoObjeto->insertar(1, 1);
    nuevoObjeto->insertar(2, 2);

    listaObjetos *nuevoObjeto2 = new listaObjetos();
    nuevoObjeto2->insertar(3, 3);
    nuevoObjeto2->insertar(4, 4);

    listaObjetos *nuevoObjeto3 = new listaObjetos();
    nuevoObjeto3->insertar(5, 5);
    nuevoObjeto3->insertar(6, 6);

    listaObjetos *nuevoObjeto4 = new listaObjetos();
    nuevoObjeto4->insertar(7, 7);
    nuevoObjeto4->insertar(8, 8);

    listaObjetos *nuevoObjeto5 = new listaObjetos();
    nuevoObjeto5->insertar(9, 9);
    nuevoObjeto5->insertar(10, 10);

    listaObjetos *nuevoObjeto6 = new listaObjetos();
    nuevoObjeto6->insertar(11, 11);
    nuevoObjeto6->insertar(12, 12);

    listaObjetos *nuevoObjeto7 = new listaObjetos();
    nuevoObjeto7->insertar(13, 13);
    nuevoObjeto7->insertar(14, 14);

    listaObjetos *nuevoObjeto8 = new listaObjetos();
    nuevoObjeto8->insertar(15, 15);
    nuevoObjeto8->insertar(16, 16);

    //Arboles a insertar
    BST *arbolI = new BST;
    arbolI->add(new nodoBST(100, "kevin", "KS", "sadfsadf"), nuevoObjeto);
    arbolI->add(new nodoBST(200, "Juan", "J", "fdsldf"), nuevoObjeto2);

    BST *arbolII = new BST;
    arbolII->add(new nodoBST(300, "kevin", "KS", "sadfsadf"), nuevoObjeto3);
    arbolII->add(new nodoBST(400, "Juan", "J", "fdsldf"), nuevoObjeto4);

    BST *arbolIII = new BST;
    arbolIII->add(new nodoBST(500, "Juan", "J", "fdsldf"), nuevoObjeto5);
    arbolIII->add(new nodoBST(600, "Juan", "J", "fdsldf"), nuevoObjeto6);

    BST *arbolIIII = new BST;
    arbolIIII->add(new nodoBST(700, "Juan", "J", "fdsldf"), nuevoObjeto7);
    arbolIIII->add(new nodoBST(800, "Juan", "J", "fdsldf"), nuevoObjeto8);

    //Niveles a insertar
    listaNiveles *ln = new listaNiveles;
    ln->insertar(1, matrizI, arbolI);
    ln->insertar(2, matrizII, arbolII);

    listaNiveles *ln2 = new listaNiveles;
    ln2->insertar(3, matrizIII, arbolIII);
    ln2->insertar(4, matrizIIII, arbolIIII);

    intentar->insertar("valor1", ln);
    intentar->insertar("valor2", ln2);
    intentar->insertar("valor3", ln);
    intentar->insertar("valor4", ln2);
    intentar->insertar("valor5", ln);
    intentar->insertar("valor6", ln2);

}


int main(){

/*========== VALORES DE PRUEBA PARA LAS ESTRUCTURAS ==========*/
    //AVL *intentar = new AVL;
    //insertaravl(intentar);
    //intentar->AVLReporter();

    //Mostrar lista de niveles completa
    //cout<<"lista de niveles del proyecto"<<endl;
    //intentar->proyecto("valor4")->listaN->mostrar();
    //cout<<"proyecto: "<<endl;
    //cout<<intentar->proyecto("valor6")->key<<endl;
    //Mostrar matriz de cierto nivel
    //intentar->proyecto("valor4")->listaN->mostrarProyecto();
    //Mostrar arbol del nivel
    //intentar->proyecto("valor2")->listaN->abbVer(5)->BSTReporter();
    //Mostrar lista de objetos por nivel
    //intentar->proyecto("valor2")->listaN->abbVer(5)->mostrarArbolLista();
    //intentar->proyecto("valor2")->listaN->abbVer(4)->actualNodo(100)->lista->mostrar();
    //system("pause");
    //ELIMINAR UN NIVEL
    /*matrix *matrizI = new matrix;
    BST *arbolI = new BST;
    listaNiveles *prueba = new listaNiveles;
    prueba->insertar(1, matrizI, arbolI);
    prueba->insertar(2, matrizI, arbolI);
    prueba->insertar(3, matrizI, arbolI);
    prueba->insertar(4, matrizI, arbolI);
    prueba->mostrar();
    prueba->eliminarNivel(1);
    cout<<"nueva lista"<<endl;
    prueba->mostrar();
    system("pause");*/

/*========== CODIGO DEL PROGRAMA ==========*/

/*========== VALORES INICIALES GENERALES ==========*/
    BST *objetosGlobal = new BST;
    AVL *proyectos = new AVL;
/*========== CODIGO DEL PROGRAMA ==========*/

    int opc;
    char aux [20];
    do
    {
        system("cls");
        cout<<"************************************************************"<<endl;
        cout<<"* USAC                                                     *"<<endl;
        cout<<"* Estructuras de Datos                                     *"<<endl;
        cout<<"* Seccion C                                                *"<<endl;
        cout<<"* Kevin Martin Samayoa Urizar                              *"<<endl;
        cout<<"* 200915348                                                *"<<endl;
        cout<<"************************************************************"<<endl;
        cout<<""<<endl;
        cout<<"ingrese una opcion:"<<endl;
        cout<<"1. Ver Proyectos (AVL)"<<endl;
        cout<<"2. Editar Proyectos"<<endl;
        cout<<"3. Cargar Proyecto"<<endl;
        cout<<"4. Graficar Proyectos"<<endl;
        cout<<"5. Guardar Proyectos"<<endl;
        cout<<"6. Cargar Librerias"<<endl;
        cout<<"7. Reportes"<<endl;
        cout<<"8. Salir"<<endl;
        scanf("%s", aux);
        opc = atoi(aux);
        string fileName;//Para recibir ruta/nombre de archivo json
        string nombreProyecto;//para buscar un proyecto

        if(opc != 0){
            switch(opc){
            case 1:
                system("cls");
                cout<<"ver proyectos"<<endl;
                proyectos->inOrden();
                cout<<""<<endl;
                int opcVP;
                char aux [20];
                do
                {
                    cout<<""<<endl;
                    cout<<"1. Seleccionar proyecto"<<endl;
                    cout<<"2. Salir a menu principal"<<endl;
                    scanf("%s", aux);
                    opcVP = atoi(aux);
                    string nombreP;
                    if(opcVP != 0){
                        switch(opcVP){
                        case 1:
                            cout<<""<<endl;
                            cout<<"ingrese el nombre del proyecto: "<<endl;
                            cin>>nombreP;
                            //ver todas las matrices de un proyecto
                            //proyectos->proyecto(nombreP)->listaN->mostrar();
                            proyectos->proyecto(nombreP)->listaN->mostrarProyecto();
                            opcVP = 2;
                        case 2:
                            //system("cls");
                            cout<<""<<endl;
                            //system("Pause");
                            break;
                        default:
                            cout<<"Opcion incorrecta ";
                            system("pause");
                            break;
                        }
                    }else{
                        cout<<""<<endl;
                        cout<<"Error, se esperaba un numero"<<endl;
                        cout<<""<<endl;
                        opcVP = -1;
                        system("pause");
                    }
                }while(opcVP != 2);
                system("Pause");
                break;
            case 2:
                system("cls");
                cout<<"editar proyectos"<<endl;
                cout<<""<<endl;
                cout<<"ingrese el nombre del proyecto: "<<endl;
                cin>>nombreProyecto;

                if(proyectos->proyecto(nombreProyecto) != NULL){
                    nodoAVL *pro = proyectos->proyecto(nombreProyecto);

                    //MENU PARA EDITAR EL PROYECTO
                    int opcEP;
                    char aux [20];
                    do
                    {
                        system("cls");
                        cout<<"         MENU"<<endl;
                        cout<<""<<endl;
                        cout<<"1. Agregar Nivel"<<endl;
                        cout<<"2. Editar Nivel"<<endl;
                        cout<<"3. Eliminar Nivel"<<endl;
                        cout<<"4. Eliminar Proyecto"<<endl;
                        cout<<"5. Crear cantidad de pisos"<<endl;
                        cout<<"6. Salir"<<endl;
                        scanf("%s", aux);
                        opcVP = atoi(aux);
                        int eliminarNivel;
                        int editarNivel;
                        int pisos;
                        int ultimoPiso;
                        //string nombreP;
                        if(opcVP != 0){
                            switch(opcVP){
                            case 1:
                                system("cls");
                                cout<<"Agregar Nivel"<<endl;
                                cout<<"formato de ruta: C:/Users/Usuario/Desktop/archivo.json"<<endl;
                                cout<<"formato de nombre: archivo.json (Para este caso, el archivo debe estar en la carpeta donde se ejecuta el programa)"<<endl;
                                cout<<""<<endl;
                                cout<<"ingrese el nombre o direccion del archivo: ";
                                cin>>fileName;//nombre/ruta del archivo
                                leerNivel(fileName, pro->listaN, objetosGlobal);
                                system("Pause");
                                break;
                            case 2:
                                system("cls");
                                cout<<"Editar Nivel"<<endl;
                                cout<<""<<endl;
                                pro->listaN->mostrar();
                                cout<<""<<endl;
                                cout<<"Ingrese el numero de nivel que desea editar"<<endl;
                                cin>>editarNivel;
                                if(pro->listaN->matrizVer(editarNivel) != NULL){
                                    matrix *editar = new matrix;
                                    BST *editArbol = new BST;
                                    editar = pro->listaN->matrizVer(editarNivel);
                                    editArbol = pro->listaN->abbVer(editarNivel);
                                    editar->reporteMatriz();
                                    //MENU OBJETOS
                                    int opcObj;
                                    char aux [20];
                                    do
                                    {
                                        system("cls");
                                        cout<<"      MENU"<<endl;
                                        cout<<""<<endl;
                                        cout<<"1. Agregar Objeto"<<endl;
                                        cout<<"2. Eliminar objeto"<<endl;
                                        cout<<"3. Mover objeto"<<endl;
                                        cout<<"4. Salir"<<endl;
                                        scanf("%s", aux);
                                        opcObj = atoi(aux);
                                        int idObjeto;
                                        int posX;
                                        int posY;

                                        if(opcObj != 0){
                                            switch(opcObj){
                                            case 1:
                                                system("cls");
                                                //INSERTAR OBJETO
                                                cout<<"Objetos"<<endl;
                                                cout<<""<<endl;
                                                cout<<"id, nombre"<<endl;
                                                objetosGlobal->inorderObjetos();
                                                cout<<""<<endl;
                                                cout<<"ingrese el numero de id del objeto que desea agregar"<<endl;
                                                cin>>idObjeto;
                                                //OOBTENER EL NODO DEL OBJETO QUE VOY A INSERTAR
                                                if(objetosGlobal->actualNodo(idObjeto) != NULL){
                                                    nodoBST *insertar = objetosGlobal->actualNodo(idObjeto);
                                                    //insertar = objetosGlobal->actualNodo(idObjeto);
                                                    //cout<<insertar->id<<", "<<insertar->nombre<<endl;
                                                    //insertar->lista->mostrar();
                                                    cout<<"ingrese la posicion x"<<endl;
                                                    cin>>posX;
                                                    cout<<"ingrese la posicion y"<<endl;
                                                    cin>>posY;
                                                    //cout<<"coordenada a insertar: "<<posX<<", "<<posY<<endl;
                                                    listaObjetos *nuevoEditar = new listaObjetos;
                                                    nodoListaObjetos *tempo = insertar->lista->primero;
                                                    while(tempo != NULL){
                                                        int x = tempo->x + posX;
                                                        int y = tempo->y + posY;
                                                        nuevoEditar->insertar(x,y);
                                                        tempo = tempo->siguiente;
                                                    }
                                                    //nuevoEditar->mostrar();
                                                    //verifico que se pueda insertar en la matriz
                                                    bool inser = true;
                                                    nodoListaObjetos *temp = nuevoEditar->primero;
                                                    while (inser == true && temp != NULL){
                                                        char verificar = editar->verificarCasilla(temp->x, temp->y);
                                                        if(verificar != 'N'){
                                                            inser = false;
                                                        }
                                                        temp = temp->siguiente;
                                                    }
                                                    //inserto el objeto al arbol y a la matriz
                                                    if(inser){
                                                        if(editArbol->add2(new nodoBST(insertar->id, insertar->nombre, insertar->letra, insertar->color), nuevoEditar) == 'S'){
                                                            nodoListaObjetos *ingresar = nuevoEditar->primero;
                                                            while(ingresar != NULL){
                                                                editar->add(ingresar->x, ingresar->y, insertar->letra, insertar->color);
                                                                ingresar = ingresar->siguiente;
                                                            }
                                                        }
                                                    }
                                                    //editar->reporteMatriz();
                                                    //editArbol->BSTReporter();
                                                    //editArbol->mostrarArbolLista();
                                                }
                                                system("Pause");
                                                break;
                                            case 2:
                                                system("cls");
                                                cout<<"Eliminar Objeto"<<endl;
                                                cout<<""<<endl;
                                                cout<<"id, nombre"<<endl;
                                                editArbol->inorderObjetos();
                                                cout<<""<<endl;
                                                cout<<"ingrese el numero de id del objeto que desea agregar"<<endl;
                                                cin>>idObjeto;
                                                if(editArbol->actualNodo(idObjeto) != NULL){
                                                    nodoBST *eliminar = editArbol->actualNodo(idObjeto);
                                                    nodoListaObjetos *eliminarObjeto = eliminar->lista->primero;
                                                    while(eliminarObjeto != NULL){
                                                        editar->del(eliminarObjeto->x, eliminarObjeto->y);
                                                        eliminarObjeto = eliminarObjeto->siguiente;
                                                    }
                                                    editArbol->eliminar1(idObjeto);
                                                }
                                                system("Pause");
                                                break;
                                            case 3:
                                                system("cls");
                                                cout<<"Mover Objeto"<<endl;
                                                cout<<""<<endl;
                                                cout<<"id, nombre"<<endl;
                                                editArbol->inorderObjetos();
                                                cout<<""<<endl;
                                                cout<<"ingrese el numero de id del objeto que desea Mover"<<endl;
                                                cin>>idObjeto;
                                                if(editArbol->actualNodo(idObjeto) != NULL){
                                                    cout<<"ingrese la posicion x"<<endl;
                                                    cin>>posX;
                                                    cout<<"ingrese la posicion y"<<endl;
                                                    cin>>posY;
                                                    //objeto que quiero mover
                                                    nodoBST *mover = editArbol->actualNodo(idObjeto);
                                                    //Matriz temporal para ver que el movimiento es valido
                                                    matrix *matrizTemporal = new matrix();
                                                    matrizTemporal = editar;

                                                    nodoListaObjetos *moverObjeto = mover->lista->primero;
                                                    //Elimina el objeto para dejar ese espacio libre
                                                    while(moverObjeto != NULL){
                                                        matrizTemporal->del(moverObjeto->x, moverObjeto->y);
                                                        moverObjeto = moverObjeto->siguiente;
                                                    }

                                                    listaObjetos *nuevoMover = new listaObjetos;
                                                    nodoListaObjetos *tempo = objetosGlobal->actualNodo(idObjeto)->lista->primero;
                                                    //nodoListaObjetos *tempo = mover->lista->primero;
                                                    while(tempo != NULL){
                                                        int x = tempo->x + posX;
                                                        int y = tempo->y + posY;
                                                        nuevoMover->insertar(x,y);
                                                        tempo = tempo->siguiente;
                                                    }
                                                    //nuevoEditar->mostrar();
                                                    //verifico que se pueda insertar en la matriz
                                                    bool inser = true;
                                                    nodoListaObjetos *temp = nuevoMover->primero;
                                                    while (inser == true && temp != NULL){
                                                        char verificar = matrizTemporal->verificarCasilla(temp->x, temp->y);
                                                        if(verificar != 'N'){
                                                            inser = false;
                                                        }
                                                        temp = temp->siguiente;
                                                    }

                                                    //nodoBST *insertar = objetosGlobal->actualNodo(idObjeto);
                                                    //cout<<"antes de mover los puntos"<<endl;
                                                    //editArbol->mostrarArbolLista();

                                                    //si se puede mover
                                                    if(inser){

                                                        nodoListaObjetos *moverFinal = nuevoMover->primero;
                                                        while(moverFinal != NULL){
                                                            editar->add(moverFinal->x, moverFinal->y, mover->letra, mover->color);
                                                            moverFinal = moverFinal->siguiente;
                                                        }

                                                        //actualizo la lista del nodo del arbol que estoy editando
                                                        editArbol->actualNodo(idObjeto)->lista = nuevoMover;
                                                    }else{
                                                        nodoListaObjetos *moverObjeto2 = mover->lista->primero;
                                                        while(moverObjeto2 != NULL){
                                                            editar->add(moverObjeto2->x, moverObjeto2->y, mover->letra, mover->color);
                                                            moverObjeto2 = moverObjeto2->siguiente;
                                                        }
                                                    }
                                                    /*editar->reporteMatriz();
                                                    editArbol->BSTReporter();
                                                    cout<<"\" despues de mover los puntos"<<endl;
                                                    editArbol->mostrarArbolLista();*/

                                                }
                                                system("pause");
                                                break;
                                            case 4:
                                                system("cls");

                                                break;
                                            default:
                                                cout<<"Opcion incorrecta "<<endl;
                                                system("pause");
                                                break;
                                            }
                                        }else{
                                            cout<<""<<endl;
                                            cout<<"Error, se esperaba un numero"<<endl;
                                            cout<<""<<endl;
                                            opcObj = -1;
                                            system("pause");
                                        }
                                    }while(opcObj != 4);
                                }else{
                                    cout<<"Nivel incorrecto"<<endl;
                                }
                                system("Pause");
                                break;
                            case 3:
                                system("cls");
                                cout<<"Eliminar Nivel"<<endl;
                                cout<<""<<endl;
                                cout<<"ingrese el numero de nivel que desea eliminar"<<endl;
                                cin>>eliminarNivel;
                                pro->listaN->eliminarNivel(eliminarNivel);
                                system("Pause");
                                break;
                            case 4:
                                system("cls");
                                cout<<"Eliminar Proyecto"<<endl;
                                cout<<""<<endl;
                                proyectos->eliminar(nombreProyecto);
                                system("Pause");
                                break;
                            case 5:
                                system("cls");
                                cout<<"Crear pisos"<<endl;
                                cout<<""<<endl;
                                cout<<"Ingrese cantidad de pisos que desea agregar"<<endl;
                                ultimoPiso = pro->listaN->ultimo->numNivel;
                                cin>>pisos;
                                cout<<"cantidad de pisos a insertar: "<<ultimoPiso<<endl;
                                cout<<"Ultimo piso: "<<ultimoPiso<<endl;
                                for(int i = 0; i < pisos; i++){
                                    matrix *matriz = new matrix;
                                    BST *arbol = new BST;
                                    ultimoPiso = ultimoPiso + 1;
                                    pro->listaN->insertar(ultimoPiso, matriz, arbol);
                                }
                                //proyectos->eliminar(nombreProyecto);
                                //pro->listaN->mostrar();
                                system("Pause");
                                break;
                            case 6:
                                system("cls");
                                //cout<<""<<endl;
                                //system("Pause");
                                break;
                            default:
                                cout<<"Opcion incorrecta ";
                                system("pause");
                                break;
                            }
                        }else{
                            cout<<"Error, se esperaba un numero"<<endl;
                            opcVP = -1;
                            system("pause");
                        }
                    }while(opcVP != 6);




                }else{
                    cout<<"El proyecto no existe"<<endl;
                }

                system("Pause");
                break;
            case 3:
                system("cls");
                cout<<"cargar proyecto"<<endl;
                cout<<"formato de ruta: C:/Users/Usuario/Desktop/archivo.json"<<endl;
                cout<<"formato de nombre: archivo.json (Para este caso, el archivo debe estar en la carpeta donde se ejecuta el programa)"<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<"ingrese el nombre o direccion del archivo: ";
                cin>>fileName;//nombre/ruta del archivo
                //leerNivel(fileName, pruebaLista, objetosGlobal);
                leerProyecto(fileName, proyectos, objetosGlobal);
                system("Pause");
                break;
            case 4:
                system("cls");
                cout<<"graficar proyectos"<<endl;
                system("Pause");
                break;
            case 5:
                system("cls");
                cout<<"guardar proyecto"<<endl;
                system("Pause");
                break;
            case 6:
                system("cls");
                cout<<"cargar librerias"<<endl;
                cout<<"formato de ruta: C:/Users/Usuario/Desktop/archivo.json"<<endl;
                cout<<"formato de nombre: archivo.json (Para este caso, el archivo debe estar en la carpeta donde se ejecuta el programa)"<<endl;
                cout<<""<<endl;
                cout<<"ingrese el nombre o direccion del archivo: ";
                cin>>fileName;//nombre/ruta del archivo
                leerJsonLibrerias(objetosGlobal, fileName);
                //objetosGlobal->mostrarArbolLista();
                system("Pause");
                break;
            case 7:
                system("cls");
                cout<<"Reportes"<<endl;
                int opcReportes;
                char auxReportes [20];
                do
                {
                    system("cls");
                    cout<<"Ingrese un tipo de reporte:"<<endl;
                    cout<<"1.  Arbol AVL de proyectos"<<endl;
                    cout<<"2.  Arbol binario de busqueda general de objetos"<<endl;
                    cout<<"3.  Proyectos con mayor numero de niveles de forma descendente"<<endl;
                    cout<<"4.  Proyectos con mayor numero de niveles de forma ascendente"<<endl;
                    cout<<"5.  Niveles de forma ordenada por el número de objetos de menor a mayor (en un proyecto)"<<endl;
                    cout<<"6.  Nivel con mas espacio (en un proyecto)"<<endl;
                    cout<<"7.  Nivel con menos paredes (en un proyecto)"<<endl;
                    cout<<"8.  Nivel con mas paredes (en un proyecto)"<<endl;
                    cout<<"9.  Nivel con mas espacio y mas ventanas (en un proyecto)"<<endl;
                    cout<<"10. Nivel con mas espacio y menos ventanas (en un proyecto)"<<endl;
                    cout<<"11. Salir"<<endl;
                    scanf("%s", auxReportes);
                    opcReportes = atoi(auxReportes);
                    string UsuarioReporte;
                    if(opcReportes != 0){
                        switch(opcReportes)
                        {
                        //Insert image
                        case 1:
                            system("cls");
                            cout<<"Arbol AVL de proyectos"<<endl;
                            proyectos->AVLReporter();
                            system("Pause");
                            break;
                        case 2:
                            system("cls");
                            cout<<"Libreria de objetos (ABB principal)"<<endl;
                            objetosGlobal->BSTReporter();
                            system("Pause");
                            break;
                        case 3:
                            system("cls");
                            cout<<"Proyectos con mayor numero de niveles de forma descendente"<<endl;

                            system("Pause");
                            break;
                        case 4:
                            system("cls");
                            cout<<"Proyectos con mayor numero de niveles de forma ascendente"<<endl;

                            system("Pause");
                            break;
                        case 5:
                            system("cls");
                            cout<<"Niveles de forma ordenada por el número de objetos de menor a mayor (en un proyecto)"<<endl;

                            system("Pause");
                            break;
                        case 6:
                            system("cls");
                            cout<<"Nivel con mas espacio (en un proyecto)"<<endl;

                            system("Pause");
                            break;
                        case 7:
                            system("cls");
                            cout<<"Nivel con menos paredes (en un proyecto)"<<endl;

                            system("Pause");
                            break;
                        case 8:
                            system("cls");
                            cout<<"Nivel con mas paredes (en un proyecto)"<<endl;

                            system("Pause");
                            break;
                        case 9:
                            system("cls");
                            cout<<"Nivel con mas espacio y mas ventanas (en un proyecto)"<<endl;

                            system("Pause");
                            break;
                        case 10:
                            system("cls");
                            cout<<"Nivel con mas espacio y menos ventanas (en un proyecto)"<<endl;

                            system("Pause");
                            break;
                        case 11:
                            system("cls");
                            cout<<"Regresar a menu principal"<<endl;

                            //system("Pause");//comentado para que no aparezca dos veces
                            //presionar una tecla para continuar
                            break;
                        default:
                            cout<<"Opcion incorrecta";
                            system("pause");
                            break;
                        }
                    }else{
                        cout<<"Error, se esperaba un numero"<<endl;
                        opcReportes = -1;
                        system("pause");
                    }
                }while(opcReportes != 11);
                system("Pause");
                break;
            case 8:
                system("cls");
                cout<<"hasta luego"<<endl;
                system("Pause");
                break;
            default:
                cout<<"Opcion incorrecta ";
                system("pause");
                break;
            }
        }
        else
        {
            cout<<"Error, se esperaba un numero"<<endl;
            opc = -1;
            system("pause");
        }

    }while(opc != 8);

    return 0;
}
