/*

 Universidad de Palermo.
 Materia: Estructura de datos - Programacion II.
 Alumno: 81050 - Pablo NIKLAS.
 Titulo: TP Final - Sistema de reservas en salas de Cine.

 Modulo de listas.

 20140404 - PSRN - Version Inicial.

*/

void mostrarSala(nodo_sala *h) {
    if (h->sig != NULL) {
        printf("%d", h->nro_sala);
        printf("%d", h->cantidad);
        mostrarpeliculas(h->sigpeli);
        mostrarsala(h->sig);
    }
}



