#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>

using namespace std;

enum _retorno{OK, ERROR, NO_IMPLEMENTADA};
typedef _retorno TipoRet;

struct nodoListaArg{
    int pos;
    string info;
    nodoListaArg * ant;
    nodoListaArg * sig;
};
typedef nodoListaArg * ListaAgr;

struct nodoListaCelda{
    int nroCelda;
    string info;
    nodoListaCelda * sig;
};
typedef nodoListaCelda * ListaCelda;

struct nodoListaColum{
	string nombre;
	bool PK;
	int nroColum;
	nodoListaColum * ant;
	nodoListaColum * sig;
};
typedef nodoListaColum * ListaColum;

struct  nodoListaTupla{
	int indice;
	ListaCelda celda;
	nodoListaTupla * ant;
	nodoListaTupla * sig;
};
typedef nodoListaTupla * ListaTupla;

struct nodoListaTabla{
    string nombre;
    int nroColumna = 0;
    ListaColum columna;
    ListaTupla tupla;
    nodoListaTabla * ant;
    nodoListaTabla * sig;
};
typedef nodoListaTabla * ListaTabla;

//**** GLOBALES *****//
ListaTabla LTabla = new nodoListaTabla;

//** OPERACIONES ***/
TipoRet createTable(string nombreTabla);
TipoRet dropTable(string nombreTabla);
TipoRet addCol(string nombreTabla, string nombreCol);
TipoRet dropCol(string nombreTabla, string nombreCol);
TipoRet insertInto(string nombreTabla, string valoresTupla);
TipoRet deleteFrom(string nombreTabla, string condicionEliminar);
TipoRet update(string nombreTabla, string condicionModificar, string columnaModificar, string valorModificar );
TipoRet printDataTable(string nombreTabla);

/** FUNCIONES AUXILIARES */
void readInput(ListaTabla T, string comando ); //Interpreta el comando de entrada
void printHelp(); // Imprime la Ayuda con los comandos validos
bool addTabla( ListaTabla T, string nombreTabla );
string getParametro(ListaAgr L, int n);
void imprimirArg(ListaAgr L);
void clearArg(ListaAgr L);
void addArgFinal(ListaAgr L, string arg);
void cargarListaArg(ListaAgr L, string allArg);

int main(){
    extern ListaTabla LTabla;
    LTabla->ant = NULL;
    LTabla->sig = NULL;

    string comando;

    while(comando!="exit"){ //mantiene la terminal esperando ordenes
        getline(cin, comando);
        if( comando.empty()!=true )
            readInput(LTabla, comando);
        else
            cout << "\tNo ha ingresado ningun comando" <<endl;
    }
    cout<<endl<< "\tBye..." <<endl;

    return 0;
}

TipoRet createTable(string nombreTabla){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla aux = LTabla;
    while( aux->sig!=NULL ){
        if( aux->sig->nombre == nombreTabla ){//Valida si la tabla ya existe
            res = ERROR;
            return res;
        }else{
            aux = aux->sig;
        }
    }
    ListaTabla nuevaTabla = new nodoListaTabla;
    nuevaTabla->nombre = nombreTabla;
    nuevaTabla->sig = NULL;
    aux->sig = nuevaTabla;
    nuevaTabla->ant = aux;
    //Crea la columna dummy en la tabla nueva
    ListaColum nuevaColum = new nodoListaColum;//Columna dummy
    nuevaColum->ant = NULL;
    nuevaColum->sig = NULL;
    nuevaTabla->columna = nuevaColum;
    //Crea la tupla dummy en la tabla nueva
    ListaTupla nuevaTupla = new nodoListaTupla;//Tupla dummy
    nuevaTupla->ant = NULL;
    nuevaTupla->sig = NULL;
    nuevaTupla->indice = 0;
    nuevaTabla->tupla = nuevaTupla;
    return res;
}

TipoRet dropTable(string nombreTabla){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //Variable tipo listaTabla Global
    ListaTabla aux = LTabla, borrar;
    while( aux->sig!=NULL ){
        if( aux->sig->nombre==nombreTabla ){
            borrar = aux->sig;
            if(aux->sig->sig!=NULL) //Verifica si hay un nodo despues del que voy a borrar
                aux->sig->sig->ant = aux;
            aux->sig = aux->sig->sig;
            delete borrar;
            return res;
        }
        aux = aux->sig;
    }
    res = ERROR;
    return res;
}

TipoRet addCol(string nombreTabla, string nombreCol){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = LTabla;

    while( auxTabla!=NULL ){
        //strcpy(nombre, auxTabla->nombre.c_str());
        if( auxTabla->nombre == nombreTabla ){ //Si existe la tabla, se para apuntando sobre ella
            if( auxTabla->tupla->sig == NULL ){ // Chequea que la tabla no tenga ningun registro cargado **/
                ListaColum auxColmun = auxTabla->columna; //Puntero auxiliar para recorrer las columnas
                while( auxColmun->sig != NULL ){//Recorre la lista de columnas y cheque que no exista una columna con el mismo nombre
                    auxColmun = auxColmun->sig;
                    if( auxColmun->nombre == nombreCol ){
                        cout<<"\t¡Operacion no valida!. Ya existe una columna llamada \""<<nombreCol<<"\""<<endl;
                        res = ERROR;
                        return res;
                    }
                }
                //Si no se encontro ninguna columna con el mismo nombre:
                ListaColum nuevaColum = new nodoListaColum;
                auxColmun->sig = nuevaColum;
                nuevaColum->nombre = nombreCol;
                nuevaColum->sig = NULL;
                nuevaColum->ant = auxColmun;
                nuevaColum->nroColum = auxColmun->nroColum+1;
                if( auxColmun->ant == NULL )  // Verifica si la columna a agregar debe ser PK o no
                    nuevaColum->PK = true;
                return res;
            }else{
                cout<<"\t¡Operacion no valida!. La tabla ya tiene registros cargados"<<endl;
                res = ERROR;
                return res;
            }
        }else{
            auxTabla = auxTabla->sig;
        }
    }
    cout<<"\tLa tabla \""<<nombreTabla<<"\" no existe."<<endl;
    res = ERROR;
    return res;
}

TipoRet dropCol(string nombreTabla, string nombreCol){
    cout << "estas en dropCol, parmetros: "<<nombreTabla<<", "<<nombreCol <<endl;
}

TipoRet insertInto(string nombreTabla, string valoresTupla){
    cout << "estas en insertInto, parmetros: "<<nombreTabla<<", "<< valoresTupla <<endl;
}

TipoRet deleteFrom(string nombreTabla, string condicionEliminar){
    cout << "estas en deleteFrom, parmetros: "<<nombreTabla<<", "<<condicionEliminar <<endl;
}

TipoRet update(string nombreTabla, string condicionModificar, string columnaModificar, string valorModificar ){
    cout << "estas en update, parmetros: "<<nombreTabla<<", "<<condicionModificar <<", "<< columnaModificar<<", "<< valorModificar <<endl;
}

TipoRet printDataTable(string nombreTabla){
    cout << "estas en printDataTable, parmetros: "<< nombreTabla <<endl;
}

void printHelp(){
    cout <<endl<< "AYUDA DE COMANDOS: "<<endl;
    cout << "Nota: todos los comandos son 'case sensitive' "<<endl;
    cout << "  help  _______________________________________________________________________* IMPRIME LA AYUDA EN PANTALLA *" <<endl<<endl;
    cout << "  createTable(nombreTabla) ____________________________________________________* CREA UNA NUEVA TABLA *"<<endl<<endl;
    cout << "\t\tEjemplo:\t\tcreateTable(Empleados)"<<endl<<endl;
    cout << "  dropTable(nombreTabla) ______________________________________________________* ELIMINA UNA TABLA EXISTENTE *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tdropTable(Productos)"<<endl<<endl;
    cout << "  addCol(nombreTabla, nombreCol) ______________________________________________* AGREGA UNA NUEVA COLUMNA A LA TABLA EXISTENTE *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\taddCol(Empleados, Apellido)"<<endl<<endl;
    cout << "  dropCol(nombreTabla, nombreCol) _____________________________________________* ELIMINA UNA COLUMNA DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tdropCol(Proyectos, idProyecto)"<<endl<<endl;
    cout << "  insertInto(nombreTabla, valoresTupla) _______________________________________* INSERTA UN NUEVO REGISTRO EN LA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tinsertInto(Personas, 3333111: Telma: Perez)"<<endl<<endl;
    cout << "  deleteFrom(nombreTabla, condicionEliminar) __________________________________* ELIMINA UN REGISTRO DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tdeleteFrom(Personas, Perez)"<<endl<<endl;
    cout << "  update(nombreTabla, condicionModificar, columnaModificar, valorModificar) ___* caracterIZA UN CAMPO DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tupdate(Personas, Nombre=Pepe: CI: 1555000)"<<endl<<endl;
    cout << "  printDataTable(Clientes) _________________________________________________* IMPRIME TODOS LOS REGISTROS DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tprintDataTable(Clientes)"<<endl<<endl;
}


/****************     LEE EL INGRESO DE LOS COMANDOS     ************************/
void readInput(ListaTabla LTabla, string comando){
    string caracter;
    int nroComas =0;
    int nroArg=0;
    for (int i=0; i<comando.length(); ++i){ // Borra todos los espacios en blanco, cuenta las comas y los dos puntos
        caracter = comando[i];
        if( caracter == " " ){
            comando.erase(i, 1);
            i--;
        }
        if( caracter =="," ){
            nroComas++;
        }
    }
    size_t posApertura = comando.find("(");      // posicion del parentesis de apertura
    string sentencia = comando.substr(0,posApertura); //setencia ingresada con espacios
    size_t posCierre = comando.find(")");        // posicion de parentesis de cierre
    string allArg = comando.substr(posApertura+1 ,(posCierre - posApertura -1)); // obtiene el contenido de los argumentos
    //Calcula la cantidad de parametros por el nro de comas y el largo

 /*   cout << "El comando ingresado es: " << sentencia <<endl;
    cout << "Y los argumentos son: " << allArg << endl;
    cout << "El largo del comando es: " << comando.length()<<endl;
    cout << "Numero de argumentos: " << nroArg <<endl;
*/

    //Se cargan los argumentos recibidos en una lista
    ListaAgr listaArg = new nodoListaArg;
    listaArg->pos = 0;
    listaArg->ant = NULL;
    listaArg->sig = NULL;
    cargarListaArg(listaArg, allArg);
//    imprimirArg(listaArg);


    if( sentencia=="createTable" ){ //createTable( nombreTabla )
        if( createTable(getParametro(listaArg, 1)) == 0 )
            cout<< "\tQuery OK -> Se creo la tabla \""<<getParametro(listaArg,1)<<"\""<<endl;
        else
            cout<< "\tQuery ERROR -> La tabla "<<getParametro(listaArg,1)<<" ya existe"<<endl;
    }

    if( sentencia=="dropTable" ){ // dropTable( nombreTabla )
        if( dropTable(getParametro(listaArg, 1)) == 0 )
            cout<< "\tQuery OK -> La Tabla \""<<getParametro(listaArg,1)<<"\" fue eliminada"<<endl;
        else
            cout<< "\tQuery ERROR -> La tabla \""<<getParametro(listaArg,1)<<"\" no existe"<<endl;
    }

    if( sentencia == "addCol" ) {//addCol( nombreTabla, nombreCol )
        if( addCol(getParametro(listaArg, 1), getParametro(listaArg, 2)) == 0 )
            cout<< "\tQuery OK -> La columna \""<<getParametro(listaArg,2)<<"\" ha sido creada"<<endl;
        else
            cout<< "\tQuery ERROR -> No se puedo crear la columna \""<<getParametro(listaArg,2)<<"\""<<endl;
    }

    if( sentencia == "dropCol" && nroArg==2 ){ //   dropCol( nombreTabla, nombreCol)
        cout<<"Sin terminar"<<endl;
    }

    if( sentencia == "insertInto" && nroArg>1 )// insertInto( nombreTabla,valoresTupla")
        cout<<"Sin terminar"<<endl;
    if( sentencia == "deleteFrom" && nroArg==1 ) //deleteFrom( nombreTabla, condicionEliminar )
        cout<<"Sin terminar"<<endl;
    if( sentencia == "update" && nroArg>3  ) // update( nombreTabla, condicionModificar, columnaModificar, valorModificar)
        cout<<"Sin terminar"<<endl;
    if( sentencia == "printDataTable" )// printDataTable( nombreTabla );
        cout<<"Sin terminar"<<endl;
    if( sentencia == "help"  )//  printHelp()
        printHelp();
    if(sentencia!="createTable" && sentencia!="dropTable" && sentencia!="addCol" && sentencia!="dropCol" && sentencia!="insertInto" && sentencia!="deleteFrom" && sentencia!="update" && sentencia!="printDataTable" && sentencia!="help" && sentencia!="exit" )
        cout << "\t¡EL comando '" << comando <<"' no es valido!" << '\n';
}

//Obtiene el parametro en la posicion n de la lista. Nota: debe recibir un n valido
string getParametro(ListaAgr L, int n){
    if( L!=NULL && L->pos!=n )
        return getParametro(L->sig, n);
    if( L!=NULL && L->pos == n )
        return L->info;
}

void addArgFinal(ListaAgr L, string arg){
    ListaAgr nuevo = new nodoListaArg;
    ListaAgr aux = L;
    nuevo->info = arg;
    nuevo->sig = NULL;
    while( aux->sig!=NULL )
        aux = aux->sig;
    aux->sig = nuevo;
    nuevo->ant = aux;
    nuevo->pos = aux->pos+1;
}

void clearArg(ListaAgr L){//terminar
    ListaAgr aux = L;
    aux = aux->sig;
}

void imprimirArg(ListaAgr L){//esta funcion es solo para modo testing
    if( L != NULL ){
        cout<< "|"<<L->pos<< " = "<<L->info<<"|"<<endl;
        imprimirArg( L->sig );
    }
}

void cargarListaArg(ListaAgr L, string allArg){
    int largo = allArg.length();
    char parametros[largo];
    int inicio=0;
    string dato;
    strcpy(parametros,allArg.c_str());

    for(int i=0; i< largo; i++){
        if( parametros[i]!=','){
            dato += parametros[i];
        }else{
            addArgFinal(L, dato);
            dato="";
        }
    }
    addArgFinal(L, dato);
}

