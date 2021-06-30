/*
 Universidad de Palermo.
 Materia: Estructura de datos - Programacion II.
 Alumno: 81050 - Pablo NIKLAS.
 Titulo: TP Final - Sistema de reservas en salas de Cine.

 Mis funciones.

 20140308 - PSRN - Version Inicial.
 20140309 - PSRN - Incorporo ncurses.

*/

#define TITULO_SISTEMA "Sistema de Gestion para Cines"
#define VERSION_SISTEMA "V1.0"
#define UNIVERSIDAD "Universidad de Palermo"
#define MATERIA "Estructura de Datos"

//Cerrar la ventana
void cerrarVentana ( WINDOW * v ) {

    wborder ( v, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
    wbkgd ( v, COLOR_PAIR ( 1 ) );
    wclear ( v );
    wrefresh ( v );
    delwin ( v );
    refresh();

    return;
}

//Funcion para crear ventanas con titulo.
WINDOW * abrirVentana ( int fila, int col, int ancho , int largo, char * titulo ) {

    WINDOW * v;
    int largo_titulo = strlen ( titulo ) + 4;
    char t[largo_titulo];

    //Creo la ventana
    if ( ( v = newwin ( largo, ancho, fila, col ) ) == NULL ) {
        msgError ( "abrir_ventana", "No hay memoria suficiente" );

        endwin();
        exit ( EXIT_FAILURE );
    }

    wbkgd ( v, COLOR_PAIR ( 2 ) );

    //Adorno el titulo
    strcpy ( t, "[ " );
    strcat ( t, titulo );
    strcat ( t, " ]" );

    //Macro para el borde.
    box ( v, ACS_VLINE, ACS_HLINE );

    //Lo centro.
    mvwprintw ( v, 0, ( ( ancho - largo_titulo ) / 2 ), t );
    mvwchgat ( v, 0, ( ( ancho - largo_titulo ) / 2 ), largo_titulo , A_REVERSE , 3 , NULL );

    wrefresh ( v );

    return v;
}

void limpiar_terminal ( char titulo[40], char subtitulo[40] ) {
    static const char titulo2[] = UNIVERSIDAD;
    static const char subtitulo2[] = MATERIA;

    //Defino los pares de colores
    init_pair ( 1, COLOR_YELLOW, COLOR_BLUE );  //Pantalla
    init_pair ( 2, COLOR_BLACK, COLOR_WHITE );  //Para ventanas fondo
    init_pair ( 3, COLOR_RED, COLOR_WHITE );    //Para ventanas titulo

    //Activo la definicion
    bkgd ( COLOR_PAIR ( 1 ) );

    //Cabecera de todas las pantallas.
    mvprintw ( 0, 0, "%-40s", titulo );
    printw ( "%40s", titulo2 );

    mvprintw ( 1, 0, "%-40s", subtitulo );
    printw ( "%40s", subtitulo2 );

    mvhline ( 2, 0, ACS_HLINE, 80 );
    mvhline ( 22, 0, ACS_HLINE, 80 );

    refresh();
}

//Linea de estado/operacion
void msgStatus ( char * estado ) {
    mvprintw ( 23, 0, "                                                                                " );
    mvprintw ( 23, 0, estado, 80 );
    refresh();

    return;
}

//Presiona una tecla
void presionaTecla ( void ) {
    msgStatus ( "Presiona una tecla para continuar..." );
    getch();

    return;
}

//Agrupo las funciones de inicializacion de ncurses.
void ncurses_init() {
    if ( ( initscr() ) == NULL ) { //Si la terminal detectada no esta en termcap, cancela.
        msgError ( "ncurses_init", "No hay memoria suficiente" );
        exit ( EXIT_FAILURE );
    }

    start_color();
    cbreak();
    noecho();

    return;
}

//Agrupo las funciones de terminacion de ncurses.
void ncurses_terminate() {
    endwin();
}

void msgError ( char * funcion, char * txt ) {
    printf ( "%s(): %s.\n", txt, funcion );
}


