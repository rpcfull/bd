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
typedef nodoListaArg * ListaArg;

struct nodoListaCelda{
    int nroCelda;
    string info;
    nodoListaCelda * ant;
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
    int nroColumna;
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
string getParametro(ListaArg L, int n);
ListaArg crearListaArg();
void imprimirArg(ListaArg L);
void clearArg(ListaArg L);
void addArgFinal(ListaArg L, string arg);
void cargarListaArg(ListaArg L, string allArg, char separador);
ListaTabla buscaTabla(ListaTabla &LTabla, string nombreTabla);
int buscaColuma(ListaColum L, string nombreColum); //Busca una columna por su nombre y retorna su posicion
bool compararCelda(ListaCelda L, int nroCelda, char operador, string valor); //compara celdas y retorna un boolean
bool buscaTuplaPk(ListaTupla &sonda, string pk);//Recibe la pk y busca la tupla que lo tenga, si no esta devuelve NULL
bool addTuplaOrdenada(ListaTupla &auxTupla, string pk, ListaArg listaValores);//agrega una nueva tupla de forma ordenada por su pk
void addTuplaFinal(ListaTupla &auxTupla, ListaArg listaValores);    //agrega una nueva tupla al final de la lista
void addTuplaSiguiente(ListaTupla &auxTupla, ListaArg listaValores);//agrega la siguiente tupla a la posicion actual
void addCeldaFinal(ListaCelda &auxCelda, string dato);//agrega nueva celda al final de la lista
void ordenarIndices(ListaTupla &auxTupla);
void cargarTupla(ListaTupla &auxTupla, ListaArg listaArg);
int lengthArg(ListaArg L);
char getOperador( string condicion);
ListaTupla buscaTuplaValor(ListaTupla L, int nroColumna, char operador, string valor); //retorna un puntero a la tupla buscada
void borrarTupla(ListaTupla &auxTupla); //Borra la tupla actual
void borrarCeldasTupla(ListaCelda &auxCelda);
void borrarCelda(ListaCelda &auxCelda, int nroCelda);
bool comienzaCon(string valor, string datoCelda); //Comprueba si el dato de una celda comienda con un determinado valor
void modificarCelda(ListaCelda &auxCelda, int nroCelda, string nuevoValor);

int test = 0;

int main(){
    extern ListaTabla LTabla;
    LTabla->nroColumna = 0;
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
    //Crea la celda dummy en la tupla
    ListaCelda nuevaCelda = new nodoListaCelda;
    nuevaCelda->ant = NULL;
    nuevaCelda->sig = NULL;
    nuevaCelda->nroCelda = 0;
    nuevaTabla->tupla->celda = nuevaCelda;
    return res;
}

TipoRet dropTable(string nombreTabla){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //Variable tipo listaTabla Global
    ListaTabla aux = LTabla, borrar;
    while( aux->sig!=NULL ){
        if( aux->sig->nombre == nombreTabla ){
            borrar = aux->sig;
            if( aux->sig->sig != NULL ) //Verifica si hay un nodo despues del que voy a borrar
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
    TipoRet res = NO_IMPLEMENTADA;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = LTabla;
    ListaColum auxColum = NULL;
    ListaColum nuevaColum = NULL;
    while( auxTabla!=NULL ){
        if( auxTabla->nombre == nombreTabla ){ //Si existe la tabla, se para apuntando sobre ella
            if( auxTabla->tupla->sig == NULL ){ // Chequea que la tabla no tenga ningun registro cargado **/
                auxColum = auxTabla->columna; //Puntero auxiliar para recorrer las columnas
                while( auxColum->sig != NULL ){//Recorre la lista de columnas y cheque que no exista una columna con el mismo nombre
                    auxColum = auxColum->sig;
                    if( auxColum->nombre == nombreCol ){
                        cout<<"\t¡Operacion no valida!. Ya existe una columna llamada \""<<nombreCol<<"\""<<endl;
                        res = ERROR;
                        return res;
                    }
                }
                //Si no se encontro ninguna columna con el mismo nombre:
                nuevaColum = new nodoListaColum;
                auxColum->sig = nuevaColum;
                nuevaColum->nombre = nombreCol;
                nuevaColum->sig = NULL;
                nuevaColum->ant = auxColum;
                nuevaColum->nroColum = auxColum->nroColum+1;
                auxTabla->nroColumna++;
                if( auxColum->ant == NULL )  // Verifica si la columna a agregar debe ser PK o no
                    nuevaColum->PK = true;
                res = OK;
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
    TipoRet res = NO_IMPLEMENTADA;
    int nroColBuscada;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = LTabla;           //puntero auxiliar tipo ListaTabla
    ListaColum auxColum;
    ListaColum borraColum;
    ListaTupla auxTupla;
    ListaCelda auxCelda;
    ListaCelda borraCelda;
    /** Busca si existe la tabla **/
    while( auxTabla!=NULL ){
        if( auxTabla->nombre == nombreTabla ){ //Si existe la tabla, se para apuntando sobre ella
            auxColum = auxTabla->columna;
            while( auxColum->sig != NULL ){  //Recorre la lista de columnas y chequea que no exista una columna con el mismo nombre
                if( auxColum->sig->nombre == nombreCol ){
                    nroColBuscada = auxColum->nroColum; //guarda el nuero de columna buscada
                    if( auxColum->sig->PK==true && auxTabla->nroColumna>1 ){//si hay mas de una columna no se pude borrar la pk
                        cout<<"\tLa columna \""<<nombreCol<<"\" es Clave Primaria y hay otras columnas que se identifican por ella."<<endl;
                        res = ERROR;
                        return res;
                    }
                    else{
                        borraColum = auxColum->sig;
                        auxColum->sig = borraColum->sig;
                        if( borraColum->sig != NULL ){//si el que se eliminio no es el ultimo elemento
                            borraColum->sig->ant = auxColum;
                            auxColum = auxTabla->columna;//regreso el puntero
                            while( auxColum->sig != NULL ){ //actualiza todo los nro de las columnas
                                auxColum->sig->nroColum = auxColum->nroColum+1;
                                auxColum = auxColum->sig;
                            }
                        }
                        delete borraColum;
                        auxTabla->nroColumna--;
                        auxTupla = auxTabla->tupla->sig;//arranca en la tupla 1
                        while( auxTupla != NULL ){ //Busca en todas las tuplas
                            auxCelda = auxTupla->celda;
                            while( auxCelda->sig!=NULL ){ //Elimina las celdas correspondientes a esa columna en cada tupla
                                //si el nro de celda es igual al nro de columna
                                if( auxCelda->sig->nroCelda == nroColBuscada ){
                                    borraCelda = auxCelda->sig;
                                    auxCelda = borraCelda->sig;
                                    if( borraCelda->sig != NULL ){
                                        borraCelda->sig->ant = auxCelda;
                                        auxCelda = auxTupla->celda;  //vuelvo el puntero auxiliar a la posicion 0
                                        while( auxCelda->sig!=NULL){ //hay que actualizar todos los nro de celdas
                                            auxCelda->sig->nroCelda = auxCelda->nroCelda+1;
                                            auxCelda = auxCelda->sig;
                                        }
                                    }
                                    delete borraCelda;
                                }
                            }
                        }
                        cout<<"\tLa columna \""<<nombreCol<<"\" ha sido eliminada con existo"<<endl;
                        res = OK;
                        return res;
                    }
                }
                auxColum = auxColum->sig;
            }

        }else{
            auxTabla = auxTabla->sig;
        }
    }
    cout<<"\tLa tabla \""<<nombreTabla<<"\" no existe."<<endl;
    res = ERROR;
    return res;
}

TipoRet insertInto(string nombreTabla, string valoresTupla){
    TipoRet res = OK;
    extern ListaTabla LTabla;                           //ListaTabla Global
    ListaTabla auxTabla = NULL;
    ListaTupla auxTupla = NULL;
    ListaArg listaValores = crearListaArg();            //crea una lista de valores para recibir los argumentos
    cargarListaArg(listaValores, valoresTupla, ':');    //carga los valores en una lista
    auxTabla = buscaTabla(LTabla, nombreTabla); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        if( auxTabla->nroColumna == lengthArg(listaValores) ){//Chequea si la cantidad de valores pasados es igual a los campos que tiene a tabla
            auxTupla = auxTabla->tupla;
            string pk = getParametro(listaValores, 1); //obtiene la pk cursada
            if(addTuplaOrdenada(auxTupla, pk, listaValores)){  //Devuelve true si pudo insertar la tupla de forma ordenada
             //   cargarTupla(auxTupla, listaValores);
                cout<<"\tNuevo registro agregado con exito"<<endl;
                return res;
            }else{
                cout<<"\tNo se pudo insertar el registro"<<endl;
                res = ERROR;
                return res;
            }
        }
    }
}

TipoRet deleteFrom(string nombreTabla, string condicion){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = NULL;                //Tabla auxiliar
    ListaTupla auxTupla = NULL;
    ListaCelda auxCelda = NULL;
    char operador = getOperador(condicion); //obtiene el operador de la condicion a buscar
    ListaArg listaCondicion = crearListaArg(); //crea una lista con las condiciones
    cargarListaArg(listaCondicion, condicion, operador); // separa ambas partes de la condicion
    string nombreColumna    = getParametro(listaCondicion,1);
    string valorCondicion   = getParametro(listaCondicion,2);
    auxTabla = buscaTabla(LTabla, nombreTabla); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        int nroColumna = buscaColuma(auxTabla->columna, nombreColumna); //si el nombre de la columna existe retorna su posicion, si no retorna 1000
        if( nroColumna != 1000){ //Chequea que exista la columna
            auxTupla = auxTabla->tupla; //Puntero auxiliar para recorrer las tuplas
            while( auxTupla!= NULL ){

                auxCelda = auxTupla->celda;
                if( compararCelda( auxCelda, nroColumna, operador, valorCondicion) ){
                    cout<<"El registro nro "<< auxTupla->indice <<" contiene: "<< auxCelda->sig->sig->info<<" "<<auxCelda->sig->sig->sig->info  <<endl;
                    borrarCeldasTupla(auxCelda);
                    borrarTupla(auxTupla);
                }
                auxTupla = auxTupla->sig;
            }
            ordenarIndices(auxTupla);
        }
        else{
            cout<< "No hay campo con ese nombre"<<endl;
        }

    }
}

TipoRet update(string nombreTabla, string condicionModificar, string columnaModificar, string valorModificar ){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = NULL;                //Tabla auxiliar
    ListaTupla auxTupla = NULL;
    ListaCelda auxCelda = NULL;
    char operador = getOperador(condicionModificar); //obtiene el operador de la condicion a buscar
    ListaArg listaCondicion = crearListaArg();           //Crea una lista con las condiciones
    cargarListaArg(listaCondicion, condicionModificar, operador); // Separa ambas partes de la condicion
    string columnaCondicion = getParametro(listaCondicion,1);// Nombre de la columna por la cual filtrar
    string valorCondicion   = getParametro(listaCondicion,2); //Valor que debe cumplir el filtro

    auxTabla = buscaTabla(LTabla, nombreTabla); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        int nroColumnaMod = buscaColuma(auxTabla->columna, columnaModificar); //si el nombre de la columna existe retorna su posicion, si no retorna 1000
        int nroColumnaCond = buscaColuma(auxTabla->columna, columnaCondicion);
        if( nroColumnaCond != 1000  && nroColumnaMod != 1000 ){ //Chequea que existan ambas columnas en la tabla
            auxTupla = auxTabla->tupla; //Puntero auxiliar para recorrer las tuplas
            while( auxTupla!= NULL ){

                auxCelda = auxTupla->celda;
                if( !condicionModificar.empty() ){
                    if( compararCelda( auxCelda, nroColumnaCond, operador, valorCondicion) ){
                        cout<<"El registro nro "<< auxTupla->indice <<" contiene: "<< auxCelda->sig->sig->info<<" "<<auxCelda->sig->sig->sig->info  <<endl;
                        modificarCelda( auxCelda, nroColumnaMod, valorModificar);
                    }
                }else{
                    modificarCelda( auxCelda, nroColumnaMod, valorModificar);
                }
                auxTupla = auxTupla->sig;
            }
            return res;
        }
        else{
            cout<< "No hay campo con ese nombre"<<endl;
            res = ERROR;
            return res;
        }
    }
}

TipoRet printDataTable(string nombreTabla){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = LTabla;
    while( auxTabla!=NULL ){
        if( auxTabla->nombre == nombreTabla ){ //Si existe la tabla, para apuntando sobre ella
                cout<<"  Tabla "<<auxTabla->nombre<<endl;
                ListaColum auxColum = auxTabla->columna; //Puntero auxiliar para recorrer las columnas
                /** Recorre la lista campos */
                while( auxColum->sig != NULL ){ //Recorre la lista de columnas y chequea que no exista una columna con el mismo nombre
                    auxColum = auxColum->sig;
                    if( auxColum->ant->ant == NULL )
                        cout<<"  "<< auxColum->nombre;
                    if( auxColum->nroColum > 1 && auxColum->sig != NULL )
                        cout<<":"<< auxColum->nombre;
                    if( auxColum->sig == NULL && auxColum->nroColum > 1)
                        cout<<":"<< auxColum->nombre <<endl;
                }
                /** Recorre la lista tuplas (resgistros)*/
                ListaTupla auxTupla = auxTabla->tupla;   // puntero que apunta a la celda dummy de la lista tuplas
                ListaCelda auxCelda;
                auxCelda = NULL;
                while( auxTupla->sig != NULL ){         //Recorre la lista de tuplas
                    auxTupla = auxTupla->sig;
                    auxCelda = auxTupla->celda;  //Puntero que apunta a ListaCelda dummy de la tupla actual
                    /** Recorre las celdas dentro de una tupla */
                    while( auxCelda->sig != NULL ){
                        auxCelda = auxCelda->sig;
                        if( auxCelda->ant->ant == NULL )
                            cout<<"  "<< auxCelda->info;
                        if( auxCelda->nroCelda > 1 && auxCelda->sig != NULL )
                            cout<<":"<< auxCelda->info;
                        if( auxCelda->sig == NULL )
                            cout<<":"<< auxCelda->info <<endl;
                    }
                }
                return res;
        }else{
            auxTabla = auxTabla->sig;
        }
    }
    cout<<"\tLa tabla \""<<nombreTabla<<"\" no existe."<<endl;
    res = ERROR;
    return res;
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
    TipoRet res;
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

    //Se cargan los argumentos recibidos en una lista
    ListaArg listaArg = crearListaArg();
    cargarListaArg(listaArg, allArg, ',');

    string nombreTabla = getParametro(listaArg,1); //Obtiene el nombre de la tabla

    extern int test;
    if( test == 0 ){
        createTable("Empleados");
        addCol("Empleados", "Id");
        addCol("Empleados", "Nombre");
        addCol("Empleados", "Apellido");
        addCol("Empleados", "Direccion");
        insertInto("Empleados", "3976069-5:Juan:Garcia:Emilio Frugoni 740");
        insertInto("Empleados", "4239223-9:Maria:Gonzalez:18 de julio 2567");
        insertInto("Empleados", "3968289-5:Pedro:Almodovar:Calle 13");
        insertInto("Empleados", "2598099-3:Roberto:Gimenez:Av siempre vivas 1234");
        insertInto("Empleados", "1928059-2:Ana:Perez:Asamblea 539");
        test = 1;
    }

    if( sentencia=="createTable" ){ //createTable( nombreTabla )
        res = createTable(nombreTabla);
        if( res == 0 )
            cout<< "\tQuery OK -> Se creo la tabla \""<<nombreTabla<<"\""<<endl;
        if( res == 1)
            cout<< "\tQuery ERROR -> La tabla "<<nombreTabla<<" ya existe"<<endl;
    }

    if( sentencia=="dropTable" ){ // dropTable( nombreTabla )
        res = dropTable(nombreTabla);
        if( res == 0 )
            cout<< "\tQuery OK -> La Tabla \""<<nombreTabla<<"\" fue eliminada"<<endl;
        if( res == 1 )
            cout<< "\tQuery ERROR -> La tabla \""<<nombreTabla<<"\" no existe"<<endl;
    }

    if( sentencia == "addCol" ) {//addCol( nombreTabla, nombreCol )
        string nombreColumna = getParametro(listaArg, 2);//obtiene e nombre de la columna ha agregar
        res = addCol(nombreTabla, nombreColumna);
        if(  res == 0 )
            cout<< "\tQuery OK -> La columna \""<<nombreColumna<<"\" ha sido creada"<<endl;
        if( res == 1)
            cout<< "\tQuery ERROR -> No se puedo crear la columna \""<<nombreColumna<<"\""<<endl;
        if( res == 2 )
            cout<< "\tQuery ERROR -> No se realizo la operacion"<<endl;
    }

    if( sentencia == "dropCol" ){ //   dropCol( nombreTabla, nombreCol)
        string nombreColumna = getParametro(listaArg, 2);
        res = dropCol(nombreTabla, nombreColumna);
        if(  res == 0 )
            cout<< "\tQuery OK -> La columna \""<<nombreColumna<<"\" ha sido eliminada"<<endl;
        if( res == 1)
            cout<< "\tQuery ERROR -> No se puedo eliminar la columna \""<<nombreColumna<<"\""<<endl;
        if( res == 2 )
            cout<< "\tQuery ERROR -> No se realizo la operacion"<<endl;
    }

    if( sentencia == "insertInto" ){// insertInto( nombreTabla,valoresTupla")
        string valoresTupla = getParametro(listaArg, 2);
        res = insertInto(nombreTabla, valoresTupla);
        if(  res == 0 )
            cout<< "\tQuery OK -> Nuevo resitro en la tabla \""<<nombreTabla<<"\""<<endl;
        if( res == 1)
            cout<< "\tQuery ERROR -> No se puedo agregar el registro en la tabla \""<<nombreTabla<<"\""<<endl;
        if( res == 2 )
            cout<< "\tQuery ERROR -> No se realizo la operacion"<<endl;

    }
    if( sentencia == "deleteFrom" ){ //deleteFrom( nombreTabla, condicionEliminar )
        string condicionEliminar = getParametro(listaArg, 2);
        res = deleteFrom(nombreTabla, condicionEliminar);
        if(  res == 0 )
            cout<< "\tQuery OK -> El registro fue eliminado "<<endl;
        if( res == 1)
            cout<< "\tQuery ERROR -> Ningun registro ha sido eliminado"<<endl;
        if( res == 2 )
            cout<< "\tQuery ERROR -> No se realizo la operacion"<<endl;

    }
    if( sentencia == "update" ){ // update( nombreTabla, condicionModificar, columnaModificar, valorModificar)
        string condModificar = getParametro(listaArg, 2);
        string columna = getParametro(listaArg, 3);
        string valoModificar = getParametro(listaArg, 4);
        res = update(nombreTabla, condModificar, columna, valoModificar);
        if(  res == 0 )
            cout<< "\tQuery OK -> El registro se ha actualizado correctamente "<<endl;
        if( res == 1)
            cout<< "\tQuery ERROR -> Error al actulizar el registro"<<endl;
        if( res == 2 )
            cout<< "\tQuery ERROR -> No se realizo la operacion"<<endl;
    }

    if( sentencia == "printDataTable" ){  // printDataTable( nombreTabla );
        res = printDataTable(nombreTabla);
        if( res == 0 )
            cout<< "\n\tQuery OK"<<endl;
        if( res == 1 )
            cout<< "\tQuery ERROR -> No se pudo mostrar la tabla\""<< nombreTabla <<"\""<<endl;
    }
    if( sentencia == "help"  )//  printHelp()
        printHelp();
    if(sentencia!="createTable" && sentencia!="dropTable" && sentencia!="addCol" && sentencia!="dropCol" && sentencia!="insertInto" && sentencia!="deleteFrom" && sentencia!="update" && sentencia!="printDataTable" && sentencia!="help" && sentencia!="exit" )
        cout << "\t¡EL comando '" << comando <<"' no es valido!" << '\n';
}

//Obtiene el parametro en la posicion n de la lista. Nota: debe recibir un n valido
string getParametro(ListaArg L, int n){
    if( L!=NULL && L->pos!=n )
        return getParametro(L->sig, n);
    if( L!=NULL && L->pos == n )
        return L->info;
}

void borrarTupla(ListaTupla &auxTupla){
    ListaTupla borrar = auxTupla;
    auxTupla = auxTupla->ant;   //se mueve un lugar hacia atras para borrar el nodo actual
    auxTupla->sig = borrar->sig;
    if( borrar->sig != NULL )
        borrar->sig->ant = auxTupla;
    delete borrar;
}

void borrarCelda(ListaCelda &auxCelda, int nroCelda){
    if( auxCelda->sig != NULL && auxCelda->sig->nroCelda != nroCelda)
        borrarCelda(auxCelda->sig, nroCelda);
    if( auxCelda->sig != NULL && auxCelda->sig->nroCelda == nroCelda){
        ListaCelda borra = auxCelda->sig;
        auxCelda->sig = borra->sig;
        if( borra->sig != NULL )
            borra->sig->ant = auxCelda;
        delete borra;
    }
}

void borrarCeldasTupla(ListaCelda &auxCelda){//borra todas las celdas
    while( auxCelda!=NULL ){
        ListaCelda borrar = auxCelda;
        auxCelda = auxCelda->sig;
        delete borrar;
    }
}

/******/
ListaTupla buscaTuplaValor(ListaTupla L, int nroColumna, char operador, string valor){
    if( L != NULL ){
        if( compararCelda(L->celda, nroColumna, operador, valor) ){
            return L;
        }else{
            return buscaTuplaValor(L->sig, nroColumna, operador, valor);
        }
    }
}
/*** Va hasta el nroCelda indicado, realiza la comparacion y retorna un bool **/
bool compararCelda(ListaCelda L, int nroCelda, char operador, string valor){
    while( L->sig!= NULL && L->nroCelda != nroCelda ){
        L = L->sig;
    }

    if( operador == '=' ){
        if( valor.compare(L->info) == 0 )
            return true;
        else
            return false;
    }
    if( operador == '<' ){
        if( valor.compare(L->info) < 0 )
            return true;
        else
            return false;
    }
    if( operador == '>' ){
        if( valor.compare(L->info) > 0 )
            return true;
        else
            return false;
    }
    if( operador == '!' ){
        if( valor.compare(L->info) != 0 )
            return true;
        else
            return false;
    }

    if( operador == '*' ){
        if( comienzaCon(valor, L->info) )
            return true;
        else
            return false;
    }

}

//Fucion para comparar el incio de un string con otro
bool comienzaCon(string valor, string datoCelda){
    int largoValor = valor.length();
    int largoDato  = datoCelda.length();
    if( largoDato >= largoValor ){
        size_t posFin = largoValor;
        string datoCorto = datoCelda.substr(0, posFin);
        if( valor.compare(datoCorto) == 0 )//Si el valor es igual al inicio del valor de la celda retorna true
            return true;
        else
            return false;
    }else
        return false;

}

ListaArg crearListaArg(){
    ListaArg listaArg = new nodoListaArg;
    listaArg->pos = 0;
    listaArg->ant = NULL;
    listaArg->sig = NULL;
    return listaArg;
}

void addArgFinal(ListaArg L, string arg){
    ListaArg nuevo = new nodoListaArg;
    ListaArg aux = L;
    nuevo->info = arg;
    nuevo->sig = NULL;
    while( aux->sig!=NULL )
        aux = aux->sig;
    aux->sig = nuevo;
    nuevo->ant = aux;
    nuevo->pos = aux->pos+1;
}

int lengthArg(ListaArg L){ //obtiene el largo de la lista de argumentos
    if( L == NULL )
        return 0;
    if( L->pos == 0 )
        return lengthArg(L->sig);
    else
        return ( 1 + (lengthArg(L->sig)));
}

void imprimirArg(ListaArg L){//esta funcion es solo para modo testing
    if( L != NULL ){
        cout<< "|"<<L->pos<< " = "<<L->info<<"|"<<endl;
        imprimirArg( L->sig );
    }
}

void cargarListaArg(ListaArg L, string allArg, char separador){//Carga una lista de argumentos
    int largo = allArg.length();
    char parametros[largo];
    int inicio=0;
    string dato;
    strcpy(parametros,allArg.c_str());
    for(int i=0; i< largo; i++){
        if( parametros[i]!= separador){
            dato += parametros[i];
        }else{
            addArgFinal(L, dato);
            dato="";
        }
    }
    addArgFinal(L, dato);
}

ListaTabla buscaTabla(ListaTabla &LTabla, string nombreTabla){ //Busca una tabla por su nombre
    if( LTabla == NULL)
        return NULL;
    if( LTabla->nombre != nombreTabla )
        return buscaTabla( LTabla->sig, nombreTabla);
    else
        return LTabla;
}

/** Esta funcion busca la tupla por su PK y solo retorna true si la encentra, en ese caso el puntero recibido queda apuntando a
dicho registro.
Si no lo a encuentra pueden pasar dos cosas, o bien no existe el registro mayor y llega al final de la lista retornando false con
el puntero a NULL.
O tambien existe algun valor mayor a la pk y retorna false con el puntero en la posicion en la que debe insertar el nuevo registro.
**/
bool buscaTuplaPk(ListaTupla &sonda, string pk){
    while( sonda->sig!=NULL ){
        if( sonda->sig == NULL)
            return false;

        if( pk.compare(sonda->sig->celda->sig->info) > 0 ){
            sonda = sonda->sig;
            continue;
        }else{
            if( pk.compare(sonda->sig->celda->sig->info) == 0 ){ //si es igual retorna true y el puntero en la posicion del pk buscado
                sonda = sonda->sig;
                return true;
            }if( pk.compare(sonda->sig->celda->sig->info) < 0 ){
                return false;
            }
        }
    }
}

//Busca una columna por su nombre y retorna la posicion de la columna, o  retorna 1000 si no la encuentra
int buscaColuma(ListaColum L, string nombreColum){
    while( L != NULL ){
        if(L->nombre == nombreColum)
            return L->nroColum;
        else
            L = L->sig;
    }
    return 1000;
}
/*
ListaCelda buscaCelda(ListaCelda L, int nroCelda){
    if( L == NULL)
        return NULL;
    if( L->nroCelda != nroCelda ){
        addTuplaOrdenada(auxTupla, pk, listaValores)
        return buscaCelda( L->sig, nroCelda);
    }
    else
        return L;
}

*/

bool addTuplaOrdenada(ListaTupla &auxTupla, string pk, ListaArg listaValores){//agrega una nueva tupla de forma ordenada
    ListaTupla sonda = auxTupla;
    ListaCelda auxCelda;
//cout<< "salida de busca tupla-> "<<buscaTuplaPk(sonda, pk)<<endl;
    if( auxTupla->sig == NULL ){ //Si la primer tupla es NULL inserto en el primer lugar
        addTuplaFinal(auxTupla, listaValores);
        /****Aqui falta el codigo para las celdas con los datos***/
        return true;
    }
    if( buscaTuplaPk(sonda, pk)){//Si encontro un registro con la misma pk no se puede agregar el registro
        return false;
    }if( buscaTuplaPk(sonda, pk) == false && sonda != NULL ){
        auxTupla = sonda;
        addTuplaSiguiente(auxTupla, listaValores);
        return true;
    }
}

void addTuplaFinal(ListaTupla &auxTupla, ListaArg listaValores){
    if( auxTupla->sig == NULL ){
        ListaTupla nueva = new nodoListaTupla;//Nueva tupla
        nueva->ant = auxTupla;
        nueva->sig = NULL;
        nueva->indice = auxTupla->indice + 1;
        auxTupla->sig = nueva;
        auxTupla = nueva;
        nueva->celda = new nodoListaCelda;//Celda dummy para la nuevta tupla
        nueva->celda->ant = NULL;
        nueva->celda->sig = NULL;
        nueva->celda->nroCelda = 0;
        cargarTupla(auxTupla, listaValores);
        ordenarIndices(auxTupla);
    }else
        addTuplaFinal(auxTupla->sig, listaValores);
}

void addTuplaSiguiente(ListaTupla &auxTupla, ListaArg listaValores){
        ListaTupla nuevaTupla = new nodoListaTupla; //Nueva tupla
        nuevaTupla->ant = auxTupla;
        nuevaTupla->sig = auxTupla->sig;
        auxTupla->sig = nuevaTupla;
        if( nuevaTupla->sig != NULL)
            nuevaTupla->sig->ant = nuevaTupla;
        auxTupla = nuevaTupla;
        auxTupla->celda = new nodoListaCelda; //Celda dummy para la nuevta tupla
        auxTupla->celda->ant = NULL;
        auxTupla->celda->sig = NULL;
        auxTupla->celda->nroCelda = 0;
        cargarTupla(auxTupla, listaValores);
        ordenarIndices(auxTupla);
}

void cargarTupla(ListaTupla &auxTupla, ListaArg listaArg){
    int largo = lengthArg(listaArg);
    ListaCelda auxCelda;
    auxCelda = auxTupla->celda;
    while( auxCelda->sig!= NULL)
        auxCelda = auxCelda->sig;
    for( int i=1; i<= largo; i++ ){
        ListaCelda nueva = new nodoListaCelda;
        nueva->sig = NULL;
        nueva->ant = auxCelda;
        nueva->info = getParametro(listaArg, i);
        nueva->nroCelda = auxCelda->nroCelda +1;
        auxCelda->sig = nueva;
        auxCelda = nueva;
    }
}

char getOperador(string condicion){//Obtiene el operador para las comparaciones cursado en un string
    int largo = condicion.length();
    char arrCondicion[largo];
    strcpy(arrCondicion,condicion.c_str());
    char c;
    for(int i=0; i<largo; i++){
        c = arrCondicion[i];
        if( c=='*' || c=='<' || c=='>' || c=='!' || c=='='){
            return c;
        }
    }
}

void ordenarIndices(ListaTupla &auxTupla){//Ordena los indices de las tuplas
   /* if( auxTupla )
    while( auxTupla != NULL && auxTupla->ant != NULL )
        auxTupla = auxTupla->ant;
    auxTupla = auxTupla->sig;
    */
    while( auxTupla != NULL ){
        auxTupla->indice = auxTupla->ant->indice + 1;
        auxTupla = auxTupla->sig;
    }
}

void modificarCelda(ListaCelda &auxCelda, int nroCelda, string nuevoValor){
    if( auxCelda==NULL)
        return;
    if( auxCelda!=NULL && auxCelda->nroCelda!=nroCelda )
        modificarCelda(auxCelda->sig, nroCelda, nuevoValor);
    else
        auxCelda->info = nuevoValor;
}
