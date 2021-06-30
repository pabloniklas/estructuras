/*

 Universidad de Palermo.
 Materia: Estructura de datos - Programacion II.
 Alumno: 81050 - Pablo NIKLAS.
 Titulo: TP Final - Sistema de reservas en salas de Cine.

 Modulo de funciones de manejo de listas.

 20140404 - PSRN - Version Inicial.
 20140518 - PSRN - Fix lectura de cadenas de caracteres con blancos en fscan(%s).
 20140519 - PSRN - Se reemplaza CSV por TSV.-
 20140524 - PSRN - Se incorpora carga de las peliculas por sala (lista enlazada).
 20140526 - PSRN - Se incorpora purgado de listas.
 20140527 - PSRN - Se incorpora volcado de archivos.

*/

#define DEBUG_CARGA_PELICULAS 0

#define ARCHIVO_SALA "C:/Users/pablo/Dropbox/Facultad/Programación 2/TPFinal/archivos/salas.txt"
#define ARCHIVO_PELICULAS "C:/Users/pablo/Dropbox/Facultad/Programación 2/TPFinal/archivos/peliculas.txt"
#define ARCHIVO_USUARIOS "C:/Users/pablo/Dropbox/Facultad/Programación 2/TPFinal/archivos/usuarios.txt"

typedef struct sala {
    int nro_sala;
    int cantidad;
    struct peliculas * sigpeli;
    struct sala * sig;
} nodo_sala;

//cantidad de asientos totales de la sala
typedef struct peliculas {
    char nombre [30];
    int cant_disponibles;
    struct peliculas * sig;
} nodo_peliculas;

typedef struct usuarios {
    int nro_usuario;
    char identificador [20];
    char tipo[2];
    struct usuarios * sig;
} nodo_usuarios;

///Declaracion de funciones
nodo_sala * localizaSala ( nodo_sala * registro, int objetivo );
void altaPeliculas();
void volcadoArchivos();
void purgarListasSalasyPeliculas();
void purgarListaUsuarios();
void mostrarUsuarios();
void mostrarSalasyPeliculas();
int cargarArchivosCreados();
void agregarPelicula ( int nro_sala, char nombre[20], int cant_disponibles );
void eliminaPeliculasSinReservas();

// tipo de usuario (a. administrador b. usuario)
///////////////////////////////////////
//Variables globales
///////////////////////////////////////

nodo_sala    *   ptrInicioListaSala = NULL;
nodo_usuarios  * ptrInicioListaUsuarios = NULL;

///////////////////////////////////////
void eliminaPeliculasSinReservas() {       ///Menu Administrador - Punto 5.

    WINDOW   *   v;

    nodo_peliculas * ptrNP = NULL, *ptrAuxNP = NULL;
    nodo_sala * ptrNS = ptrInicioListaSala;

    int fila = 3;

    v = abrirVentana ( 3, 2, 73, 19, "Eliminar peliculas sin reservar" );
    msgStatus ( "Mostrando contenido." );

    while ( ptrNS->sig != NULL ) {
        ptrNP = ptrNS->sigpeli;

        if ( ptrNP->sig != NULL && ptrNP->sig->sig == NULL ) {

            ///Un solo nodo.
            free ( ptrNS->sigpeli );
            ptrNS->sigpeli = NULL;

        } else {
            while ( ptrNP->sig->sig != NULL ) {

                if ( ptrNP->sig->cant_disponibles == ptrNS->cantidad )  {    ///Borro las que nadie les compro
                    mvwprintw ( v, fila++, 4, "Eliminando: %s.", ptrNP->sig->nombre );

                    ptrAuxNP = ptrNP->sig->sig;
                    free ( ptrNP->sig ); ///Libero el espacio de memoria.
                    ptrNP->sig = ptrAuxNP;

                } else {
                    ptrNP = ptrNP->sig;    ///Avanzo al siguiente nodo.
                }

            }
        }

        ptrNS = ptrNS->sig;
    }

    refresh();
    wrefresh ( v );

    presionaTecla();
    cerrarVentana ( v );

    return;
}


void altaPeliculas() {

    WINDOW * v;

    nodo_sala  * ptrNS;

    int nro_sala = 0, cant_disponible = 0;
    char pelicula[30];

    v = abrirVentana ( 4, 10, 50, 10, "Carga de datos" );
    mvwprintw ( v, 2, 3, "# Sala:" );
    mvwprintw ( v, 3, 3, "Nombre pelicula:" );

    touchwin ( v );
    refresh();
    wrefresh ( v );

    curs_set ( 1 );
    cbreak();
    echo();

    mvwscanw ( v, 2, 20, "%d", &nro_sala );
    mvwscanw ( v, 3, 20, "%s", pelicula );

    ///Busco la sala para cargar la cantidad de asientos de la sala.
    ptrNS = localizaSala ( ptrInicioListaSala, nro_sala );      ///TODO.
    agregarPelicula ( nro_sala, pelicula, ptrNS->cantidad );

    mvwprintw ( v, 5, 3, "Se dio de alta: %s.", pelicula );
    mvwprintw ( v, 6, 3, "con %d asientos en la sala #%d.", ptrNS->cantidad, nro_sala );

    refresh();
    wrefresh ( v );
    curs_set ( 0 );
    noecho();
    presionaTecla();
    cerrarVentana ( v );

    return;
}


///Volcado a archivos - Al salir del sistema.
void volcadoArchivos() {

    FILE     *    salas = NULL, *peliculas = NULL, *usuarios = NULL;

    nodo_sala  *  ptrNS = ptrInicioListaSala;
    nodo_peliculas * ptrNP = NULL;

    ///Abro AMBOS archivos, ante error de alguno de ellos, se cancela el procedimiento.
    if ( ( peliculas = fopen ( ARCHIVO_PELICULAS, "w" ) ) == NULL || ( salas = fopen ( ARCHIVO_SALA, "w" ) ) == NULL ) {
        msgError ( "cargarArchivosCreados", "El archivo no pudo ser abierto." );
    } else {

        /// Recorro la lista de salas.
        while ( ptrNS->sig != NULL ) {
            ptrNP = ptrNS->sigpeli;

            if ( ptrNS->sig->sig != NULL ) {   /// No quiero la ultima linea del archivo en blanco.-
                fprintf ( salas, "%d\t%d\n", ptrNS->nro_sala, ptrNS->cantidad );
            } else {
                fprintf ( salas, "%d\t%d", ptrNS->nro_sala, ptrNS->cantidad );
            }

            /// Recorro la lista de peliculas de cuelga de cada nodo de salas.-
            while ( ptrNP->sig != NULL ) {

                if ( ptrNS->sig->sig == NULL && ptrNP->sig->sig == NULL ) {  /// No quiero la ultima linea del archivo en blanco.-
                    fprintf ( peliculas, "%d\t%s\t%d", ptrNS->nro_sala, ptrNP->nombre, ptrNP->cant_disponibles );
                } else {
                    fprintf ( peliculas, "%d\t%s\t%d\n", ptrNS->nro_sala, ptrNP->nombre, ptrNP->cant_disponibles );
                }

                ptrNP = ptrNP->sig; ///Avanzo al siguiente nodo de peliculas.
            }

            ptrNS = ptrNS->sig;    ///Avanzo al siguiente nodo de salas.
        }

        ///Cierro los archivos.
        fclose ( peliculas );
        fclose ( salas );

    }

    return;
}


///Purgar listas - Despues del volcado al archivo.
void purgarListasSalasyPeliculas() {

    nodo_sala  * ptrNS = ptrInicioListaSala, *ptrAuxNS;
    nodo_peliculas * ptrNP, *ptrAuxNP;

    while ( ptrNS->sig != NULL ) {

        ptrNP = ptrNS->sigpeli;

        while ( ptrNP->sig != NULL ) {

            ptrAuxNP = ptrNP;
            ptrNP = ptrNP->sig; ///Avanzo al siguiente nodo.
            free ( ptrAuxNP ); ///Libero el espacio de memoria.
        }

        free ( ptrNP ); ///El que quedo pendiente.

        ptrAuxNS = ptrNS;
        ptrNS = ptrNS->sig; ///Avanzo al siguiente nodo.
        free ( ptrAuxNS );  ///Libero el espacio de memoria.
    }

    free ( ptrNS );     ///El que quedo pendiente.
}

void purgarListaUsuarios() {

    nodo_usuarios  * ptrUS = ptrInicioListaUsuarios, *ptrAuxUS;

    while ( ptrUS -> sig != NULL ) {

        ptrAuxUS = ptrUS;
        ptrUS = ptrUS->sig; ///Avanzo al siguiente nodo.
        free ( ptrAuxUS ); ///Libero el espacio de memoria.
    }

    free ( ptrUS ); ///El que quedo pendiente.

    return;
}

///Menu Administrador - Punto 2.
void mostrarUsuarios() {

    nodo_usuarios  * ptrNU = ptrInicioListaUsuarios;

    WINDOW     *     v;
    int             fila = 2;

    v = abrirVentana ( 3, 2, 73, 19, "Listado de Usuarios" );

    msgStatus ( "Mostrando contenido." );

    while ( ptrNU->sig != NULL ) {
        mvwprintw ( v, fila++, 4, "Usuario: %3d - %s    | Tipo: %2s", ptrNU->nro_usuario, ptrNU->identificador, ptrNU->tipo );
        ptrNU = ptrNU->sig;
    }

    wrefresh ( v );
    refresh();

    presionaTecla();
    cerrarVentana ( v );
    refresh();

    return;
}

///Menu Administrador - Punto 1.
void mostrarSalasyPeliculas() {

    nodo_peliculas * ptrNP;
    nodo_sala    *   ptrNS = ptrInicioListaSala;
    WINDOW     *     v;
    int              fila = 2;

    v = abrirVentana ( 3, 2, 73, 19, "Listado de Salas y Peliculas" );

    msgStatus ( "Mostrando contenido." );

    while ( ptrNS->sig != NULL ) {
        mvwprintw ( v, fila++, 2, "Sala: %d | Capacidad: %3d", ptrNS->nro_sala, ptrNS->cantidad );
        ptrNP = ptrNS->sigpeli; ///Obtengo el nodo de inicio de la lista de peliculas.

        ///Recorro la lista de peliculas.
        while ( ptrNP->sig != NULL ) {
            mvwprintw ( v, fila++, 2, "        | Pelicula: %-30s | Disponibles: %3d", ptrNP->nombre, ptrNP->cant_disponibles );
            ptrNP = ptrNP->sig;     ///Avanzo en la lista de peliculas.
        }

        ptrNS = ptrNS->sig; ///Avanzo en la lista de salas.
        fila++;
    }

    wrefresh ( v );
    refresh();

    presionaTecla();
    cerrarVentana ( v );
    refresh();

    return;
}

///Localizar el nodo de sala en la lista. La lista no debe tener elementos repetidos
nodo_sala * localizaSala ( nodo_sala * registro, int objetivo ) {

    while ( registro->sig != NULL && registro->nro_sala != objetivo ) {
        registro = registro->sig;
    }

    if ( registro->nro_sala == objetivo ) {
        return registro;
    } else {
        return NULL;
    }
}

/*

Cargar listas

%s se detiene cuando lee los espacios. Se arregla reemplazando %s con %[^\n\t]
http://stackoverflow.com/questions/20803245/how-to-write-and-read-including-spaces-from-text-file

*/
int cargarArchivosCreados() {
    FILE      *      salas = NULL, *peliculas = NULL, *usuarios = NULL;
    nodo_peliculas * ptrAuxListaPeliculas = NULL;
    nodo_sala    *   ptrNS = NULL,    *ptrSalaEncontrada = NULL;
    nodo_usuarios  * ptrNU = NULL;

    ///Archivo usuarios
    int     nro_usu;
    char    tipo[2];

    ///Archivo salas
    int     nro_sala, cantidad;

    ///Archivo peliculas
    char    nombre [20];
    int     cant_disponibles;
//    char    estado [40];

#if DEBUG_CARGA_PELICULAS
    int fila = 3;
#endif // DEBUG_CARGA_PELICULAS

    ///Cargo los usuarios
    msgStatus ( "Cargando usuarios en memoria." );    //Trazabilidad

    if ( ( usuarios = fopen ( ARCHIVO_USUARIOS, "r" ) ) == NULL ) {
        msgError ( "cargarArchivosCreados", "El archivo no pudo ser abierto." );
    } else {
        ptrNU = ( nodo_usuarios * ) malloc ( sizeof ( nodo_usuarios ) );
        ptrInicioListaUsuarios = ptrNU;

        while ( !feof ( usuarios ) ) {
            fscanf ( usuarios, "%d\t%[^\n\t]\t%[^\n\t]", &nro_usu, nombre, tipo ); ///TODO: nro_usu no levanta el valor.
            strcpy ( ptrNU->tipo, tipo );
            strcpy ( ptrNU->identificador, nombre );
            ptrNU->nro_usuario = nro_usu;

            ptrNU->sig = ( nodo_usuarios * ) malloc ( sizeof ( nodo_usuarios ) );
            ptrNU = ptrNU->sig;
        }

        ptrNU->sig = NULL;
        fclose ( usuarios );
    }

    ///Cargo las salas.
    msgStatus ( "Cargando salas en memoria." );    ///Trazabilidad

    if ( ( salas = fopen ( ARCHIVO_SALA, "r" ) ) == NULL ) {
        msgError ( "cargarArchivosCreados", "El archivo no pudo ser abierto." );
    } else {
        ptrNS = ( nodo_sala * ) malloc ( sizeof ( nodo_sala ) );
        ptrInicioListaSala = ptrNS;

        while ( !feof ( salas ) ) {
            fscanf ( salas, "%d\t%d", &nro_sala, &cantidad );
            ptrNS->nro_sala = nro_sala;
            ptrNS->cantidad = cantidad;
            ptrNS->sigpeli = NULL; ///Cargo NULL
            ptrNS->sig = ( nodo_sala * ) malloc ( sizeof ( nodo_sala ) ); ///Creo nuevo nodo y lo relaciono.
            ptrNS = ptrNS->sig;
        }

        ptrNS->sig = NULL; ///Cierro la lista.
        fclose ( salas );
    }

    ///Cargo las peliculas, insertando los nodos en las salas correspondientes.
    msgStatus ( "Cargando peliculas en memoria." );    ///Trazabilidad

    if ( ( peliculas = fopen ( ARCHIVO_PELICULAS, "r" ) ) == NULL ) {
        msgError ( "cargarArchivosCreados", "El archivo no pudo ser abierto." );
    } else {

#if DEBUG_CARGA_PELICULAS
        mvprintw ( --fila, 0, "" );
#endif // DEBUG_CARGA_PELICULAS

        while ( !feof ( peliculas ) ) {
            fscanf ( peliculas, "%d\t%[^\n\t]\t%d", &nro_sala, nombre, &cant_disponibles ); ///Leo el archivo.

            agregarPelicula ( nro_sala, nombre, cant_disponibles );
        }

        fclose ( peliculas );
    }

#if DEBUG_CARGA_PELICULAS
    presionaTecla();
    exit ( EXIT_FAILURE );
#endif // DEBUG_CARGA_PELICULAS

    return EXIT_SUCCESS;
}


/*

  Multilistas

*/

void agregarPelicula ( int nro_sala, char nombre[20], int cant_disponibles ) {

    nodo_sala    *    ptrSalaEncontrada = NULL;
    nodo_peliculas  * ptrAuxListaPeliculas = NULL, *ptrNP = NULL;

    ///Buscar el nodo de sala.
    ptrSalaEncontrada = localizaSala ( ptrInicioListaSala, nro_sala );
#if DEBUG_CARGA_PELICULAS
    printw ( " -> Nodo sala %d: 0x%x", nro_sala, ptrSalaEncontrada );
    refresh();
#endif // DEBUG_CARGA_PELICULAS


    ///La sala existe, agrego el nodo de pelicula.
    if ( ptrSalaEncontrada != NULL ) {
        ptrNP = ( nodo_peliculas * ) malloc ( sizeof ( nodo_peliculas ) ); ///Creo el nodo de peliculas.

        ///Completo los datos del nodo.
        strcpy ( ptrNP->nombre, nombre );
        ptrNP->cant_disponibles = cant_disponibles;

        ///Creo nodo basura para fin de lista.
        ptrNP->sig = ( nodo_peliculas * ) malloc ( sizeof ( nodo_peliculas ) );
        ptrNP->sig->cant_disponibles = 0;
        strcpy ( ptrNP->sig->nombre, "----FIN DE LISTA----" );
        ptrNP->sig->sig = NULL;

        ///Es el primer nodo de la lista de pelicula?
        if ( ptrSalaEncontrada->sigpeli == NULL ) {
            ptrSalaEncontrada->sigpeli = ptrNP;
        } else {
            ///Posiciono el puntero auxiliar en el comienzo de la lista de peliculas.
            ptrAuxListaPeliculas = ptrSalaEncontrada->sigpeli;

#if DEBUG_CARGA_PELICULAS

            if ( ptrAuxListaPeliculas->sig != NULL ) {
                printw ( "\n -> Recorro lista a partir de nodo 0x%x: \n", ptrAuxListaPeliculas );
                refresh();
                presionaTecla();
            }

#endif // DEBUG_CARGA_PELICULAS

            ///Hay mas nodos en la lista de peliculas?
            while ( ptrAuxListaPeliculas->sig->sig != NULL ) { ///Me posiciono en el ultimo nodo de la lista de peliculas.
#if DEBUG_CARGA_PELICULAS
                printw ( "\t--> 0x%x ", ptrAuxListaPeliculas );
                refresh();
#endif // DEBUG_CARGA_PELICULAS
                ptrAuxListaPeliculas = ptrAuxListaPeliculas->sig;
            }

            ptrAuxListaPeliculas->sig = ptrNP;  ///Engancho el nodo de peliculas NP
        }
    } else {    ///La sala no existe !!!!
        msgError ( "cargarArchivosCreados()", "Sala no encontrada. Revisar archivos de entrada" );
        refresh();
        exit ( EXIT_FAILURE );
    }

    return;
}


