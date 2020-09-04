#include <iostream>
#include <fstream> //para leer o escribir archivos
#include "json.hpp" //libreria para leer archivos en formato Json
//#include <string> //para convertir enteros a cadenas

using namespace std;
using json = nlohmann::json;

//json jfile;//declaro un objeto de tipo json

int *dobleX, *dobleY;//vectores para coordenadas x y y de fichas dobles

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

    listaSimple()
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
    nodoMatriz *head;
    string headers_X;

    matrix()
    {
        nodoMatriz *temp = new nodoMatriz("", "", 0, 0);
        head = temp;
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
            nodoMatriz *temp = new nodoMatriz("","", x, 0);
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
                nodoMatriz *new_temp = new nodoMatriz("", "", x, 0);
                temp->right = new_temp;
                new_temp->left = temp;
            }
            else if(temp->right != NULL && temp->right->xc != x)
            {
                nodoMatriz *new_temp = new nodoMatriz("", "", x, 0);
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
            nodoMatriz *temp = new nodoMatriz("", "", 0, y);
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
                nodoMatriz *new_temp = new nodoMatriz("", "", 0, y);
                temp->down = new_temp;
                new_temp->up = temp;
            }
            else if(temp->down != NULL && temp->down->yc != y)
            {
                nodoMatriz *new_temp = new nodoMatriz("", "", 0, y);
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

        ofstream f("matrix.txt");
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
            f<<"nodeX_"<<temp->xc<<" [label=\"     Nivel X     \"];" <<endl;
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
                    f<<"nodeX_"<<temp->xc<<" -> nodeX_"<<temp->right->xc<<" [dir=both];"<<endl;
                    temp = temp->right;
                }
            }

            //to link y headers nodes
            temp = head;
            if(temp->down != NULL)
            {
                f<<"nodeX_"<<temp->xc<<" -> nodeY_"<<temp->down->yc<<" [dir=both];"<<endl;
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

        system("dot -Tpng matrix.txt -o matrix.png");
        cout<<"\n";
        system("matrix.png");
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
                    cout<<"node not found"<<endl;
                }
            }
            else
            {
                cout<<"node not found"<<endl;
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
                    cout<<"Si existe"<<endl;
                    return 'S';
                }
                else
                {
                    cout<<"No existe"<<endl;
                    return 'N';
                }
            }
            else
            {
                cout<<"No existe"<<endl;
                return 'N';
            }
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
            root = temp;
            //root->primero = insertar->primero;
            root->lista = insertar;
            cout<<"Se ingreso correctamente "<<endl;
        }else{
            recursive_add(root, temp, insertar);
        }
    }

    void recursive_add(nodoBST *current, nodoBST *temp, listaObjetos *insertar){
        if(temp->id < current->id){//INSERT in left sub-tree
            if(current->left != NULL){//IS NOT NULL, make recursive call
                recursive_add(current->left, temp, insertar);
            }else{//IS NULL, INSERT NODE
                current->left = temp;
                //current->left->primero = insertar->primero;
                current->left->lista = insertar;
                cout<<"Se ingreso correctamente "<<endl;
            }
        }else if(temp->id > current->id){//INSERT in right sub-tree
            if(current->right != NULL){//IS NOT NULL, make recursive call
                recursive_add(current->right, temp, insertar);
            }else{//IS NULL, INSERT NODE
                current->right = temp;
                //current->left->primero = insertar->primero;
                current->left->lista = insertar;
                cout<<"Se ingreso correctamente "<<endl;
            }
        }else{
            //ERROR el nodo ya existe
            cout<<"El nombre ya existe"<<endl;
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
                //temp->setLeft(root->getLeft());
                if(root->left != temp){
                    //temp->setLeft(root->getLeft());
                    temp->left = root->left;
                }
                //temp->setRight(root->getRight());
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

    //Retornar la lista del nodo para mostrarla
    nodoBST *actualNodo(int id){
        nodoBST *temp = new nodoBST(id, "","","");
        if(root == NULL)
        {
            cout<<"el usuario no existe"<<endl;
            //existe = false;
            //return false;
        }
        else
        {
            return recursiveActualNodo(root, temp);
        }
    }

    //RETORNA EL NODO BUSCADO (CON SU LISTA)
    nodoBST *recursiveActualNodo(nodoBST *current, nodoBST *temp)
    {
        if(temp->id < current->id)
        {
            if(current->left != NULL)
            {
                recursiveActual(current->left, temp);
            }
            else
            {
                cout<<"el usuario no existe"<<endl;
                //existe = false;//No existe
                //return false;
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
                cout<<"el usuario no existe"<<endl;
                //existe = false;//No existe
                //return false;
            }
        }
        else
        {
            //existe = true;//Existe
            return current;
        }
    }


    //Retornar la lista del nodo para mostrarla
    listaObjetos *actual(int id){
        nodoBST *temp = new nodoBST(id, "","","");
        if(root == NULL)
        {
            cout<<"el usuario no existe"<<endl;
            //existe = false;
            //return false;
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
                cout<<"el usuario no existe"<<endl;
                //existe = false;//No existe
                //return false;
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
                cout<<"el usuario no existe"<<endl;
                //existe = false;//No existe
                //return false;
            }
        }
        else
        {
            //existe = true;//Existe
            return current->lista;
        }
    }
};

int main(){

/*========== VALORES DE PRUEBA PARA LAS ESTRUCTURAS ==========*/

    //string prueba = "#F6FB34";
    //string prueba = "";
    //cout<<prueba<<endl;
    /*matrix *matriz = new matrix;
    //add(int x, int y, string letra, string color)
    matriz->add(1,1,"P","#F6FB34");
    matriz->add(2,2,"P","#F6FB34");
    matriz->add(3,3,"P","#F6FB34");
    matriz->add(3,2,"P","#F6FB34");
    matriz->add(2,3,"P","#F6FB34");
    //matriz->add(1,1,"S","#F6FB34"); //VALIDAR QUE YA EXISTE FICHA, VER SI SE EVALUA EN EL METODO INSERTAR O APARTE EN LA FUNCIONAIDAD
    matriz->reporteMatriz();*/

    BST *tree = new BST();
    listaObjetos *nuevoObjeto = new listaObjetos();
    nuevoObjeto->insertar(3, 4);
    nuevoObjeto->insertar(5, 8);

    //Algoritmo para insertar un objeto al arbol
    //crear el nodo del objeto (nodoBST)
    //crear lista de puntos del objeto (listaObjetos)
    //enviar ambos al metodo add del arbol
    tree->add(new nodoBST(50, "kevin", "KS", "sadfsadf"), nuevoObjeto);
    system("pause");

    cout<<"impresion de lista desde el arbol"<<endl;
    tree->actualNodo(50)->lista->mostrar();
    cout<<"tamaño"<<endl;
    cout<<tree->actualNodo(50)->lista->magnitud()<<endl;

    system("pause");
    //mostrar->mostrar();

    /*tree->inorderReporter();
    cout<<"Recorrido en orden"<<endl;
    system("pause");
    tree->preOrderReporter();
    cout<<"Recorrido pre orden"<<endl;
    system("pause");
    tree->posOrderReporter();
    cout<<"Recorrido pos orden"<<endl;
    system("pause");*/

/*========== CODIGO DEL PROGRAMA ==========*/

/*========== VALORES INICIALES GENERALES ==========*/
//listaObjetos *nuevoObjeto = new listaObjetos();

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
        cout<<"7. Salir"<<endl;
        scanf("%s", aux);
        opc = atoi(aux);
        string fileName;

        if(opc != 0){
            switch(opc)
            {
            //Insert image
            case 1:
                system("cls");
                cout<<"ver proyectos"<<endl;
                system("Pause");
                break;
            case 2:
                system("cls");
                cout<<"editar proyectos"<<endl;
                system("Pause");
                break;
            case 3:
                system("cls");
                cout<<"cargar proyecto"<<endl;
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
                /*string fName = archivo;//nombre/ruta del archivo
                json jfile;//declaro un objeto de tipo json
                ifstream doc(fName);//modificar para recibir la ruta del archivo
                //if(-doc.is_open()) std::cout<<"ERROR: No se pudo abrir el archivo"<<endl;
                if(!doc.is_open()){
                    cout<<"ERROR: No se pudo abrir el archivo. Verifique la ruta o que el archivo exista"<<endl;
                }else{
                    doc>>jfile;//paso el contenido al reader.
                    //json casillas = jfile["niveles"];
                    //leo la lista de objetos
                    json objetos = jfile["objetos"];

                */
                system("Pause");
                break;
            case 7:
                system("cls");
                cout<<"hasta luego"<<endl;
                system("Pause");
                break;
            default:
                cout<<"Opcion incorrecta";
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

    }while(opc != 7);

    return 0;
}
