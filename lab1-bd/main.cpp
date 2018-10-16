#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>

using namespace std;

enum _retorno{OK, ERROR, NO_IMPLEMENTADA};
typedef _retorno TipoRet;


struct nodoCelda{
    int nroCelda;
    string info;
    nodoCelda * sig;
};
typedef nodoCelda * Celda;

struct nodoCampo{
	string nombreCampo;
	string tipoCampo;
	int nroCampo;
	nodoCampo * sig;
};
typedef nodoCampo * Campo;

struct  nodoTupla{
	int indice;
	Celda celda;
};
typedef nodoTupla * Tupla;

struct nodoTabla{
    string nombre;
    int nroCampos = 0;
    Campo campo;
    Tupla tupla;
    nodoTabla * sig;
};
typedef nodoTabla * Tabla;

/** FUNCIONES AUXILIARES */
void readInput( string comando ); //Interpreta el comando de entrada
void printHelp(); // Imprime la Ayuda con los comandos validos
bool addTabla( Tabla &T, string nombreTabla );


int main(){
    Tabla T = new nodoTabla; //dummy
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

TipoRet createTable(string nombreTabla){
    TipoRet salida = NO_IMPLEMENTADA;
    return salida;
}

TipoRet dropTable(string nombreTabla){

    TipoRet res = NO_IMPLEMENTADA;
    cout << "estas en dropTable, parmetros: "<<nombreTabla <<endl;
    return res;
}

TipoRet addCol(string nombreTabla, string nombreCol){
    TipoRet res = NO_IMPLEMENTADA;
    cout << "estas en addCol, parmetros: "<<nombreTabla<<", "<<nombreCol <<endl;
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
    for (int i=0; i<comando.length(); ++i){ // Borro todos los espacios en blanco
        actual = comando[i];
        if( blanco == actual ){
           // cout <<"posicion "<< i << " caracter " << actual << endl;
            comando.erase(i, 1);
            i--;
        }
    }
    size_t posApertura = comando.find("(");      // posicion del parentesis de apertura
    string sentencia = comando.substr(0,posApertura); //setencia ingresada con espacios
    size_t posCierre = comando.find(")");        // posicion de parentesis de cierre
    string argALL = comando.substr(posApertura+1 ,(posCierre - posApertura -1)); // obtiene el contenido de los argumentos
    cout << "El comando ingresado es: " << sentencia <<endl;
    cout << "Y los argumentos son: " << argALL << endl;
    cout << "el largo del comando es: " << comando.length()<<endl;

    if( !argALL.empty() ){
        size_t fin = argALL.find(":")
    }else{
        cout << "NO HAY ARGUMENTOS" <<endl;
    }

    if( sentencia == "createTable" )
        createTable("nombreTabla");
    if( sentencia == "dropTable" )
        dropTable("nombreTabla");
    if( sentencia == "addCol" )
        addCol("nombreTabla","nombreCol");
    if( sentencia == "dropCol" )
        dropCol("nombreTavoid printHelp()bla","nombreCol");
    if( sentencia == "insertInto" )
        insertInto("nombreTabla","valoresTupla");
    if( sentencia == "deleteFrom")
        deleteFrom("nombreTabla", "condicionEliminar");
    if( sentencia == "update" )
        update("nombreTabla", "condicionModificar", "columnaModificar", "valorModificar");
    if( sentencia == "printDataTable" )
        printDataTable( "nombreTabla");
    if( sentencia == "help" )
        printHelp();
    if(sentencia!="createTable" && sentencia!="dropTable" && sentencia!="addCol" && sentencia!="dropCol" && sentencia!="insertInto" && sentencia!="deleteFrom" && sentencia!="update" && sentencia!="printDataTable" && sentencia!="help" )
        cout << "\t¡EL comando '" << comando <<"' no es valido!" << '\n';


}

bool addTabla( Tabla &T, string nombreTabla){
    p
}
