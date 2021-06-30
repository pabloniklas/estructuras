/*

 Universidad de Palermo.
 Materia: Estructura de datos - Programacion II.
 Alumno: 81050 - Pablo NIKLAS.
 Titulo: TP Final - Sistema de reservas en salas de Cine.

 Modulo con funciones del sistema.

 20140308 - PSRN - Version Inicial.
 20140311 - PSRN - Se integra el menu a una ventana de ncurses.
 20140325 - PSRN - Se reescribio rutina de menu, debido a core's no resueltos.
 20140513 - PSRN - Funcion de ingreso de enteros.

*/


#include <ncursesw/menu.h>
#include <ctype.h>

//    #define ARCHIVO_SALA "/home/pablo/Dropbox/Facultad/Programación 2/TPFinal/archivos/salas.txt"
//    #define ARCHIVO_PELICULAS "/home/pablo/Dropbox/Facultad/Programación 2/TPFinal/archivos/peliculas.txt"


/*

    Clave

*/
int clave() {

    int cont = 1;

    char c;

    ///En posiciones 3,6,9 y 12: palermofacultad
    while ( ( c = getchar() ) != 13 ) {

        if ( cont == 3 && c != 'l' ) {
            return 1;
        }

        if ( cont == 6 && c != 'm' ) {
            return 1;
        }

        if ( cont == 9 && c != 'a' ) {
            return 1;
        }

        if ( cont == 12 && c != 'l' ) {
            return 1;
        }

        cont++;
    }

    return 0;
}

/*
    Creditos
*/
void creditos() {

    WINDOW * v;

    int r = 3;
    int c = 3;

    v = abrirVentana ( 4, 10, 50, 15, "Creditos" );

    mvwprintw ( v, r++, c, TITULO_SISTEMA );
    mvwprintw ( v, r++, c, "Alumno: Pablo Niklas." );
    r++;
    mvwhline ( v, r++, 1, ACS_HLINE, 49 );
    r++;
    mvwprintw ( v, r++, c, "Docente: Fabiana DAIAN." );
    mvwprintw ( v, r++, c, "Ayudante: Adrian." );
    r++;
    mvwprintw ( v, r++, c, MATERIA );
    mvwprintw ( v, r++, c, UNIVERSIDAD );
    wrefresh ( v );

    presionaTecla();
    cerrarVentana ( v );
}

/*
   Funcion int menu(opciones).

   Referencias consultadas:

   http://www.mkssoftware.com/docs/man3/menu.3.asp
   http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html
   http://docs.oracle.com/cd/E19455-01/806-0629/6j9vjcnv3/index.html
   http://lists.gnu.org/archive/html/bug-ncurses/2014-03/msg00001.html
   http://www.troubleshooters.com/lpm/200405/200405.htm

   TODO: Calculo de las coordenadas.
*/

int menu ( int cant_opciones, char * opciones[] ) {

    WINDOW * ventana_menu;

    int cur_idx = 0;
    int i, maslargo = 0;
    int x, y, xx, yy, padding;

    ///Averiguo el mas largo.
    for ( i = 0; i < cant_opciones; i++ ) if ( strlen ( opciones[i] ) > maslargo ) {
            maslargo = strlen ( opciones[i] );
        }

    ///Calculo coordenadas
    padding = 3;
    x = ( 25 - ( cant_opciones + ( padding * 2 ) ) ) / 2;
    y = ( 80 - ( maslargo + 2 + ( padding * 2 ) ) ) / 2;
    xx = cant_opciones + ( padding * 2 );
    yy = maslargo + ( padding * 2 ) + 3;

    ///Creo la ventana asociada al menu
    ventana_menu = abrirVentana ( x, y, yy, xx, "Menu de opciones" );

    ///Permite interpretar pulsaciones de tecla.
    //keypad ( ventana_menu, TRUE );
    //curs_set ( 0 );

    ///Creo el menu y establecer el formato.
    for ( i = 1; i <= cant_opciones; i++ )  {
        mvwprintw ( ventana_menu, padding + i - 1, padding, "%d - %s.", i, opciones[i - 1] );
    }

    msgStatus ( "Esperando ingreso del usuario." );

    curs_set ( 1 );
    cbreak();
    echo();

    mvwprintw ( ventana_menu, padding + cant_opciones + 1, padding, "=>" );
    mvwscanw ( ventana_menu, padding + cant_opciones + 1, padding + 3, "%d", &cur_idx );

    refresh();
    wrefresh ( ventana_menu );

    keypad ( ventana_menu, FALSE );
    cerrarVentana ( ventana_menu );

    return ( cur_idx );
}


