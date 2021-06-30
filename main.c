/*

 Universidad de Palermo.
 Materia: Estructura de datos - Programacion II.
 Alumno: 81050 - Pablo NIKLAS.
 Titulo: TP Final - Sistema de reservas en salas de Cine.

 Modulo Principal.

 Utiliza las librerias ncurses, las cuales se obtienen de:
   ftp://invisible-island.net/ncurses/win32/mingw32-20140308.zip
   Descomprimir y copiar en: "C:\Program Files (x86)\CodeBlocks\MinGW"

 Linkeditar agreando las librerias: menuw, ncursesw.-

 20140308 - PSRN - Version Inicial.
 20140404 - PSRN - Se comento el pedido de clave.
 20140427 - PSRN - Se incorpora las llamadas a las funciones de volcado de archivos y purgado de listas.

*/

#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include <errno.h>
#include <string.h>
#include <ncursesw/menu.h>

#include "pablo.h"
#include "sistema.h"
#include "listas.h"
#include "pilasycolas.h"

int main() {
    int opcion = 0, opcion_usuario = 0, opcion_administrador = 0;

    //Creo el menu.
    char * opciones_iniciales[] = {
        "Usuario registrado", "Usuario no registrado", "Administrador", "Creditos", "Salir"
    };

    char * opciones_administrador[] = {
        "Informe completo de salas y películas",
        "Listar los usuarios del sistema",
        "Procesar la cola de compra y la pila de reserva de entradas",
        "Agregar películas en una determinada sala",
        "Eliminar aquellas películas que no tienen ventas de entradas",
        "Volver"
    };

    char * opciones_usuario_registrado[] = {
        "Reservar entrada", "Comprar entrada", "Consultar las películas de cada sala y la disponibilidad de asientos", "Volver"
    };

    char * opciones_usuario_noregistrado[] = {
        "Comprar entrada", "Consultar las películas de cada sala y la disponibilidad de asientos", "Volver"
    };
/*
///Area de prueba de funciones

    nodo_cola      auxCola;
    nodo_pila      auxPila;

    printf("Saco cola... ");
    auxCola=sacarCola();
    printf("%s\t%d\n", auxCola.nombre, auxCola.cantidad);

    printf("Agrego uno a la cola...\n");
    agregarCola("uno",1);
    printf("Agrego dos...\n");
    agregarCola("dos",2);

    printf("Saco cola... ");
    auxCola=sacarCola();
    printf("%s\t%d\n", auxCola.nombre, auxCola.cantidad);

    printf("Saco cola... ");
    auxCola=sacarCola();
    printf("%s\t%d\n", auxCola.nombre, auxCola.cantidad);

    printf("Saco cola... ");
    auxCola=sacarCola();
    printf("%s\t%d\n", auxCola.nombre, auxCola.cantidad);

    printf("\n\n");

    printf("Saco pila....");
    auxPila=sacarPila();
    printf("%d\t%d\t%s\t%d\n", auxPila.nro_usuario, auxPila.nro_auto, auxPila.nombre, auxPila.cantidad);

    printf("Pongo uno en la pila....\n");
    agregarPila(1,10,"uno",100);
    printf("Pongo dos en la pila....\n");
    agregarPila(2,20,"dos",200);

    printf("Saco pila....");
    auxPila=sacarPila();
    printf("%d\t%d\t%s\t%d\n", auxPila.nro_usuario, auxPila.nro_auto, auxPila.nombre, auxPila.cantidad);

    printf("Saco pila....");
    auxPila=sacarPila();
    printf("%d\t%d\t%s\t%d\n", auxPila.nro_usuario, auxPila.nro_auto, auxPila.nombre, auxPila.cantidad);

    printf("Saco pila....");
    auxPila=sacarPila();
    printf("%d\t%d\t%s\t%d\n", auxPila.nro_usuario, auxPila.nro_auto, auxPila.nombre, auxPila.cantidad);

    return EXIT_SUCCESS;
///FIN area de prueba de funciones
*/

    ncurses_init();
    limpiar_terminal ( TITULO_SISTEMA, VERSION_SISTEMA );

    //Cargo archivos de datos
    if ( cargarArchivosCreados() == EXIT_FAILURE ) {
        ncurses_terminate();
        return EXIT_FAILURE;
    }

//    limpiar_terminal(TITULO_SISTEMA, VERSION_SISTEMA);

    /*    if (clave()==0) {
                msg_status("Acceso aceptado.");
        } else {
                msg_status("Acceso denegado.");
        }
    */

    do {
        msgStatus ( "Menu general del sistema." );
        opcion = menu ( 5, opciones_iniciales );

        switch ( opcion ) {

        case 1:
            do {
                msgStatus ( "Menu de usuario registrado." );
                opcion_usuario = menu ( 4, opciones_usuario_registrado );

                switch ( opcion_usuario ) {

                case 1:
                    ///Reservar entradas.
                    reservarEntradas();

                    break;

                case 2:
                    ///Comprar entradas.
                    comprarEntradas();

                    break;

                case 3:
                    ///Consultar peliculas y disponibilidad de asientos.
                    mostrarSalasyPeliculas();

                    break;


                }
            } while ( opcion_usuario != 4 );

            break;


        ///Menu Usuario no registrado
        case 2:
            do {
                msgStatus ( "Menu de usuario NO registrado." );
                opcion_usuario = menu ( 3, opciones_usuario_noregistrado );

                switch ( opcion_usuario ) {

                case 1:
                    ///Comprar entradas.
                    comprarEntradas();

                    break;

                case 2:
                    ///Consultar peliculas y disponibilidad de asientos.
                    mostrarSalasyPeliculas();

                    break;

                }
            } while ( opcion_usuario != 3 );

            break;

        ///Menu Administrador
        case 3:
            do {
                msgStatus ( "Menu de administrador." );
                opcion_administrador = menu ( 6, opciones_administrador );

                switch ( opcion_administrador )

                {

                case 1:
                    ///Informe completo de salas y peliculas (disponibles y no disponibles).
                    mostrarSalasyPeliculas();

                    break;

                case 2:
                    ///Listar los usuarios del sistema (sin administrador).
                    mostrarUsuarios();

                    break;

                case 3:
                    ///Procesar la cola de compra y la pila de reserva de entradas.
                    procesarReservasyCompras();

                    break;

                case 4:
                    ///Agregar peliculas en una determinada sala.
                    altaPeliculas();

                    break;

                case 5:
                    ///Eliminar aquellas películas que no tienen ventas de entradas.
                    eliminaPeliculasSinReservas();

                    break;

                }
            } while ( opcion_administrador != 6 );

            break;

        //Creditos
        case 4:
            creditos();
            break;

        }
    } while ( opcion != 5 );

    volcadoArchivos();
    purgarListasSalasyPeliculas();
    purgarListaUsuarios();

    //presiona_tecla();

    ncurses_terminate();
    printf ( "Ejecucion terminada.\n" );
    printf ( "(c) 2014 by PN Software para UP.\n" );
    printf ( "--------------------------------------------------------------------------------\n" );

    return EXIT_SUCCESS;
}

