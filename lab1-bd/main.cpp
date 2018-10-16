#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>

using namespace std;

enum _retorno{OK, ERROR, NO_IMPLEMENTADA};
typedef _retorno TipoRet;

struct nodoListaParamentro{
    string info;
    nodoListaParamentro * sig;
};
typedef nodoListaParamentro * Parametro;

struct nodoListaCelda{
    int nroCelda;
    string info;
    nodoListaCelda * sig;
};
typedef nodoListaCelda * Celda;

struct nodoListaCampo{
	string nombreCampo;
	string tipoCampo;
	int nroCampo;
	nodoListaCampo * sig;
};
typedef nodoListaCampo * Campo;

struct  nodoListaTupla{
	int indice;
	Celda celda;
};
typedef nodoListaTupla * Tupla;

struct nodoListaTabla{
    string nombre;
    int nroCampos = 0;
    Campo campo;
    Tupla tupla;
    nodoListaTabla * sig;
};
typedef nodoListaTabla * Tabla;

/** FUNCIONES AUXILIARES */
void readInput( string comando ); //Interpreta el comando de entrada
void printHelp(); // Imprime la Ayuda con los comandos validos
bool addTabla( Tabla &T, string nombreTabla );
Parametro getParametros(string argumentos, int n);


int main(){
    Tabla T = new nodoListaTabla; //dummy
    T->sig = NULL;

    string comando;
    while(comando!="exit"){
        getline(cin, comando);
        if( comando.empty()!=true )
            readInput(comando);
        else
            cout << "No ha ingresado ningun comando" <<endl;
    }
    return 0;
}

void printHelp(){
    cout <<endl<< "AYUDA DE COMANDOS: "<<endl;
    cout << "Nota: todos los comandos son 'case sensitive' "<<endl;
    cout << "  help  _______________________________________________________________________* IMPRIME LA AYUDA EN PANTALLA *" <<endl<<endl;
    cout << "  createTable(nombreTabla) ____________________________________________________* CREA UNA NUEVA TABLA *"<<endl<<endl;
    cout << "\t\tEjemplo:\t\tcreateTable(\"Empleados\")"<<endl<<endl;
    cout << "  dropTable(nombreTabla) ______________________________________________________* ELIMINA UNA TABLA EXISTENTE *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tdropTable(\"Empleados\")"<<endl<<endl;
    cout << "  addCol(nombreTabla, nombreCol) ______________________________________________* AGREGA UNA NUEVA COLUMNA A LA TABLA EXISTENTE *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\taddCol(\"Empleados\",\"Apellido\")"<<endl<<endl;
    cout << "  dropCol(nombreTabla, nombreCol) _____________________________________________* ELIMINA UNA COLUMNA DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tdropCol(\"Proyectos\", \"idProyecto\")"<<endl<<endl;
    cout << "  insertInto(nombreTabla, valoresTupla) _______________________________________* INSERTA UN NUEVO REGISTRO EN LA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tinsertInto(\"Personas\", \"3333111\":\"Telma\":\"Perez\")"<<endl<<endl;
    cout << "  deleteFrom(nombreTabla, condicionEliminar) __________________________________* ELIMINA UN REGISTRO DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tdeleteFrom(\"Personas\", \"Perez\")"<<endl<<endl;
    cout << "  update(nombreTabla, condicionModificar, columnaModificar, valorModificar) ___* ACTUALIZA UN CAMPO DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tupdate(\"Personas\", \"Nombre=Pepe\":\"CI\":\"1555000\")"<<endl<<endl;
    cout << "  printDataTable(nombreTabla) _________________________________________________* IMPRIME TODOS LOS REGISTROS DE UNA TABLA *" <<endl<<endl;
    cout << "\t\tEjemplo:\t\tprintDataTable(\"Clientes\")"<<endl<<endl;
}

void readInput(string comando){
    string blanco = " ";
    string actual;
    int nroComas = 0;
    for (int i=0; i<comando.length(); ++i){ // Borro todos los espacios en blanco
        actual = comando[i];
        if( blanco == actual ){
           // cout <<"posicion "<< i << " caracter " << actual << endl;
            comando.erase(i, 1);
            i--;
        }
        if( actual == ",")
            nroComas++;
    }
    size_t posApertura = comando.find("(");      // posicion del parentesis de apertura
    string sentencia = comando.substr(0,posApertura); //setencia ingresada con espacios
    size_t posCierre = comando.find(")");        // posicion de parentesis de cierre
    string argALL = comando.substr(posApertura+1 ,(posCierre - posApertura -1)); // obtiene el contenido de los argumentos
    cout << "El comando ingresado es: " << sentencia <<endl;
    cout << "Y los argumentos son: " << argALL << endl;
    cout << "el largo del comando es: " << comando.length()<<endl;

    if( !argALL.empty() ){
        size_t fin = argALL.find(":");
    }else{
        cout << "NO HAY ARGUMENTOS" <<endl;
    }

    Parametro LParam;
    LParam = NULL;
    LParam = getParametros(argALL, nroComas);//devuelve una lista con los parametros
    cout << LParam->sig->info <<endl<<endl;

    if( sentencia == "createTable" ) //createTable("nombreTabla")
        cout << "Operacion: createTable(\"nombreTabla\")"<< endl;

    if( sentencia == "dropTable" ) // dropTable("nombreTabla");
        cout << "Operacion: dropTable(\"nombreTabla\")" <<endl;
    if( sentencia == "addCol" ) //addCol("nombreTabla","nombreCol")
        cout << "Operacion: addCol(\"nombreTabla\",\"nombreCol\")"<<endl;
    if( sentencia == "dropCol" )//   dropCol("nombreTabla","nombreCol")
        cout << "Operacion: dropCol(\"nombreTabla\",\"nombreCol\")"<<endl;
    if( sentencia == "insertInto" )// insertInto("nombreTabla","valoresTupla")
        cout << "Operacion: insertInto(\"nombreTabla\",\"valoresTupla\")" <<endl;
    if( sentencia == "deleteFrom") //deleteFrom("nombreTabla", "condicionEliminar")
        cout << "Operacion: deleteFrom(\"nombreTabla\", \"condicionEliminar\")" <<endl;
    if( sentencia == "update" ) // update("nombreTabla", "condicionModificar", "columnaModificar", "valorModificar")
        cout << "Operacion: update(\"nombreTabla\", \"condicionModificar\", \"columnaModificar\", \"valorModificar\")"<<endl;
    if( sentencia == "printDataTable" )// printDataTable( "nombreTabla");
        cout << "Operacion: printDataTable( \"nombreTabla\")" <<endl;
    if( sentencia == "help" )//  printHelp()
        cout << "Operacion: printHelp()" << endl;
    if(sentencia!="createTable" && sentencia!="dropTable" && sentencia!="addCol" && sentencia!="dropCol" && sentencia!="insertInto" && sentencia!="deleteFrom" && sentencia!="update" && sentencia!="printDataTable" && sentencia!="help" )
        cout << "\t¡EL comando '" << comando <<"' no es valido!" << '\n';
}

Parametro getParametros(string argumentos, int n){
    Parametro aux, res = new nodoListaParamentro;
    res->sig = NULL;
    aux = res;
    int posicion= 0;

    if( !argumentos.empty() && n==0 ){//Si no tiene coma pero no esta vacio hay un argumento
        aux->sig = new nodoListaParamentro;
        aux = aux->sig;
        aus->sig = NULL;
        aux->info = argumentos;
    }
    if( n > 0 ){
        for(int i=0; i<=n; i++){
            aux->sig = new nodoListaParamentro;
            aux = aux->sig;
            aux->info = argumentos.find()
        }
    }

    return res;
}

bool addTabla( Tabla &T, string nombreTabla){

}
