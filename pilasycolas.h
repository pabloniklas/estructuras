/*

 Universidad de Palermo.
 Materia: Estructura de datos - Programacion II.
 Alumno: 81050 - Pablo NIKLAS.
 Titulo: TP Final - Sistema de reservas en salas de Cine.

 Modulo de funciones de manejo de pilas y colas.

 20140529 - PSRN - Version Inicial.

*/

typedef struct cola_compra {
    char nombre[20];        //nombre de la pelicula.
    int cantidad;   // cantidad de entradas
    struct cola_compra * sigcola;
} nodo_cola;

typedef struct pila_reserva {
    int nro_usuario;
    int nro_auto;
    char nombre[30];    //nombre de la película
    int cantidad;       // cantidad de entradas
    struct pila_reserva * sigpila;
} nodo_pila;


///////////////////////////////////////
//Variables globales
///////////////////////////////////////

nodo_cola    *   ptrInicioColaCompra = NULL;
nodo_cola    *   ptrFinColaCompra = NULL;

nodo_pila    *   ptrInicioPilaReserva = NULL;

///////////////////////////////////////

void agregarPila ( int nro_usuario, int nro_auto, char nombre[30], int cantidad );
void agregarCola ( char nombre[20], int cantidad );
nodo_pila sacarPila();
nodo_cola sacarCola();
nodo_peliculas * buscarPelicula ( char nombre_pelicula[] ) ;
void procesarReservasyCompras() ;

///Compra de entradas
void comprarEntradas() {

    WINDOW * v;

    nodo_sala  * ptrNS;
    nodo_peliculas * ptrNP;
    int r = 2, cont = 0, i;
    int nro_pelicula = 0, nro_sala, nro_usuario, nro_auto, cantidad;

    v = abrirVentana ( 4, 10, 43, 17, "Compra de entradas" );

    mvwprintw ( v, r++, 2, "Sala:" );
    mvwhline ( v, r++, 1, ACS_HLINE, 40 );
    touchwin ( v );
    refresh();
    wrefresh ( v );

    curs_set ( 1 );
    cbreak();
    echo();

    mvwscanw ( v, 2, 10, "%d", &nro_sala );

    ///Busco la sala para cargar la cantidad de asientos de la sala.
    ptrNS = localizaSala ( ptrInicioListaSala, nro_sala );
    ptrNP = ptrNS->sigpeli;

    r++;

    ///Muestro la lista de peliculas de esa sala.
    while ( ptrNP->sig != NULL ) {
        cont++;
        mvwprintw ( v, r++, 2, "%d. %-30s", cont, ptrNP->nombre );
        ptrNP = ptrNP->sig;     ///Avanzo en la lista de peliculas.
    }

    refresh();
    wrefresh ( v );

    mvwprintw ( v, ++r, 2, "Pelicula:" );
    mvwscanw ( v, r, 13, "%d", &nro_pelicula );
    refresh();
    wrefresh ( v );

    mvwprintw ( v, ++r, 2, "Cantidad:" );
    mvwscanw ( v, r, 13, "%d", &cantidad );
    refresh();
    wrefresh ( v );

    ptrNP = ptrNS->sigpeli;

    for ( i = 1; i < nro_pelicula ; i++ ) { ptrNP = ptrNP->sig; } ///Avanzo hasta el nodo de pelicula elegido.

    agregarCola ( ptrNP->nombre, cantidad );

    r++;
    mvwprintw ( v, ++r, 2, "Compra exitosa." );

    refresh();
    wrefresh ( v );

    curs_set ( 0 );
    noecho();
    presionaTecla();
    cerrarVentana ( v );

    return;
}

void reservarEntradas() {

    WINDOW * v;

    nodo_sala  * ptrNS;
    nodo_peliculas * ptrNP;
    int r = 2, cont = 0, i;
    int nro_pelicula = 0, nro_sala, nro_usuario, nro_auto, cantidad;

    v = abrirVentana ( 4, 10, 43, 17, "Reserva de entradas" );

    mvwprintw ( v, r++, 2, "Sala:" );
    mvwhline ( v, r++, 1, ACS_HLINE, 40 );
    touchwin ( v );
    refresh();
    wrefresh ( v );

    curs_set ( 1 );
    cbreak();
    echo();

    mvwscanw ( v, 2, 10, "%d", &nro_sala );

    ///Busco la sala para cargar la cantidad de asientos de la sala.
    ptrNS = localizaSala ( ptrInicioListaSala, nro_sala );
    ptrNP = ptrNS->sigpeli;

    r++;

    ///Muestro la lista de peliculas de esa sala.
    while ( ptrNP->sig != NULL ) {
        cont++;
        mvwprintw ( v, r++, 2, "%d. %-30s", cont, ptrNP->nombre );
        ptrNP = ptrNP->sig;     ///Avanzo en la lista de peliculas.
    }

    refresh();
    wrefresh ( v );

    mvwprintw ( v, ++r, 2, "Pelicula:" );
    mvwscanw ( v, r, 13, "%d", &nro_pelicula );
    refresh();
    wrefresh ( v );

    mvprintw ( v, ++r, 2, "Nro. Usuario: " );
    mvwscanw ( v, r, 20, "%d", &nro_usuario );
    refresh();
    wrefresh ( v );

    mvwprintw ( v, ++r, 2, "Nro. Auto:" );
    mvwscanw ( v, r, 13, "%d", &nro_auto );
    refresh();
    wrefresh ( v );

    mvwprintw ( v, ++r, 2, "Cantidad:" );
    mvwscanw ( v, r, 13, "%d", &cantidad );
    refresh();
    wrefresh ( v );

    ptrNP = ptrNS->sigpeli;

    for ( i = 1; i < nro_pelicula ; i++ ) { ptrNP = ptrNP->sig; } ///Avanzo hasta el nodo de pelicula elegido.

    agregarPila ( nro_usuario, nro_auto, ptrNP->nombre, cantidad );

    r++;
    mvwprintw ( v, ++r, 2, "Pelicula reservada satisfactoriamente." );

    refresh();
    wrefresh ( v );

    curs_set ( 0 );
    noecho();
    presionaTecla();
    cerrarVentana ( v );

    return;
}


///Proceso la data de las pilas y las colas.
void procesarReservasyCompras() {

    WINDOW * v;

    nodo_cola  * ptrNodoCola = ptrInicioColaCompra;
    nodo_pila  * ptrNodoPila = ptrInicioPilaReserva;

    nodo_peliculas * ptrNodoPeliculaEncontrada = NULL;

    int         fila = 2;

    v = abrirVentana ( 3, 2, 73, 19, "Procesando Reservas y Compras" );

    msgStatus ( "Mostrando contenido." );

    ///Pila
    mvwprintw ( v, fila++, 2, "Reservas (Pila) --------------" );

    while ( ptrNodoPila != NULL ) {
        mvwprintw ( v, fila++, 2, "==> Cant: %d - Nombre: %s - # Auto: %d - # Usuario: %d",
                    ptrNodoPila->cantidad, ptrNodoPila->nombre, ptrNodoPila->nro_auto, ptrNodoPila->nro_usuario );

        ptrNodoPeliculaEncontrada = buscarPelicula ( ptrNodoPila->nombre );

        ptrNodoPeliculaEncontrada->cant_disponibles = ptrNodoPeliculaEncontrada->cant_disponibles - ptrNodoPila->cantidad;
        mvwprintw ( v, fila++, 5, "==> Encontrada: %s - Antes: %d - Ahora: %d",
                    ptrNodoPeliculaEncontrada->nombre, ptrNodoPeliculaEncontrada->cant_disponibles, ptrNodoPeliculaEncontrada->cant_disponibles - ptrNodoPila->cantidad );
        ptrNodoPila = ptrNodoPila->sigpila;
    }

    refresh();
    wrefresh ( v );

    fila++;

    ///Cola
    mvwprintw ( v, fila++, 2, "Compras (Cola) --------------" );

    while ( ptrNodoCola != NULL ) {
        mvwprintw ( v, fila++, 2, "==> Cant: %d - Nombre: %s", ptrNodoCola->cantidad, ptrNodoCola->nombre );

        ptrNodoPeliculaEncontrada = buscarPelicula ( ptrNodoCola->nombre );
        ptrNodoPeliculaEncontrada->cant_disponibles = ptrNodoPeliculaEncontrada->cant_disponibles - ptrNodoCola->cantidad;
        mvwprintw ( v, fila++, 5, "==> Encontrada: %s - Antes: %d - Ahora: %d",
                    ptrNodoPeliculaEncontrada->nombre, ptrNodoPeliculaEncontrada->cant_disponibles, ptrNodoPeliculaEncontrada->cant_disponibles - ptrNodoPila->cantidad );

        ptrNodoCola = ptrNodoCola->sigcola;
    }

    refresh();
    wrefresh ( v );

    presionaTecla();
    cerrarVentana ( v );
}

/// Busca nodo de pelicula por nombre.
/// Las peliculas no se repiten.
nodo_peliculas * buscarPelicula ( char nombre_pelicula[20] ) {

    nodo_sala  * ptrNS = ptrInicioListaSala;
    nodo_peliculas * ptrNP = NULL, *ptrAuxNP=NULL;

    while ( ptrNS->sig != NULL) {
        ptrNP = ptrNS->sigpeli;

        while ( ptrNP->sig != NULL  ) {
            if ( strcmp ( ptrNP->nombre, nombre_pelicula ) == 0 ) ptrAuxNP=ptrNP;

            ptrNP = ptrNP->sig;
        }

        ptrNS = ptrNS->sig;
    }


    return ptrAuxNP;
}


///Manejo de pilas
/*
    ptrInicioColaReserva: Tiene el primer elemento ingresado.
    ptrFinColaReserva: Tiene el ultimo elemento ingresado.
*/
void agregarCola ( char nombre[20], int cantidad ) {

    nodo_cola  * ptrNC = ( nodo_cola * ) malloc ( sizeof ( nodo_cola ) );
    nodo_cola  * ptrAuxNC = NULL;

    ptrNC->cantidad = cantidad;
    strcpy ( ptrNC->nombre, nombre );
    ptrNC->sigcola = NULL;

    if ( ptrInicioColaCompra == NULL  || ptrFinColaCompra == NULL ) {
        ptrInicioColaCompra = ptrNC;
        ptrFinColaCompra = ptrNC;
    } else {

        ptrAuxNC = ptrInicioColaCompra;

        while ( ptrAuxNC->sigcola != NULL ) {
            ptrAuxNC = ptrAuxNC->sigcola;
        }

        ptrAuxNC->sigcola = ptrNC;
        ptrFinColaCompra = ptrNC;
    }

    return;
}

nodo_cola sacarCola() {

    nodo_cola  * ptrSig = NULL;
    nodo_cola   NC;

    if ( ptrInicioColaCompra != NULL ) {

        ///Cargo el struct.
        NC.cantidad = ptrInicioColaCompra->cantidad;
        strcpy ( NC.nombre, ptrInicioColaCompra->nombre );
        NC.sigcola = NULL;  ///Buena practica.

        ///Corro el puntero maestro y elimino el primer elemento.
        ptrSig = ptrInicioColaCompra->sigcola;
        free ( ptrInicioColaCompra );
        ptrInicioColaCompra = ptrSig;

    } else {

        ///Devuelvo NULL en caso de no haber mas elementos.
        NC.cantidad = -1;
        strcpy ( NC.nombre, "" );
        NC.sigcola = NULL;
    }

    return NC;
}


void agregarPila ( int nro_usuario, int nro_auto, char nombre[30], int cantidad ) {

    nodo_pila  *  ptrNP = ( nodo_pila * ) malloc ( sizeof ( nodo_pila ) );

    ///Cargo el nodo.
    ptrNP->nro_usuario = nro_usuario;
    ptrNP->nro_auto = nro_auto;
    strcpy ( ptrNP->nombre, nombre );
    ptrNP->cantidad = cantidad;

    ///Comienzo algoritmo.
    if ( ptrInicioPilaReserva == NULL ) {
        ptrInicioPilaReserva = ptrNP;
        ptrNP->sigpila = NULL;  ///Marca del primer nodo en ingresar.
    } else {
        ptrNP->sigpila = ptrInicioPilaReserva;
        ptrInicioPilaReserva = ptrNP;
    }

    return;
}

nodo_pila sacarPila() {

    nodo_pila * ptrSig = NULL;
    nodo_pila   NP;

    if ( ptrInicioPilaReserva != NULL ) {

        ///Cargo el struct.
        NP.cantidad = ptrInicioPilaReserva->cantidad;
        NP.nro_auto = ptrInicioPilaReserva->nro_auto;
        NP.nro_usuario = ptrInicioPilaReserva->nro_usuario;
        strcpy ( NP.nombre, ptrInicioPilaReserva->nombre );
        NP.sigpila = NULL;

        ///Corro el puntero maestro y elimino el primer elemento.
        ptrSig = ptrInicioPilaReserva->sigpila;
        free ( ptrInicioPilaReserva );
        ptrInicioPilaReserva = ptrSig;

    } else {

        ///Devuelvo NULL en caso de no haber mas elementos.
        NP.cantidad = -1;
        NP.nro_auto = -1;
        NP.nro_usuario = -1;
        NP.sigpila = NULL;
        strcpy ( NP.nombre, "" );
    }

    return NP;
}

