#include "gfx.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <math.h>

#define PI 3.14159265

#define RADIO_MAXIMO 100
#define RADIO_MINIMO 10

#define MIN_NUM_LADOS 5

#define LARGO_VENTANA 800
#define ANCHO_VENTANA 495

#define MAX_VEL_ANGULAR 30
#define MIN_VEL_ANGULAR 5

#define MAX_VELOCIDAD 10
#define MIN_VELOCIDAD 1

using namespace std;

class Asteroide {
private:
    int num_lados;
    double angulo;
    double vel_angular;
    double velocidad;
    int *direccion = new int[2]();
    int *centro = new int[2]();
    double radio;
    vector<vector<int>> vertices;
    vector<vector<int>> vectores;
    vector<vector <double>> matriz_rotacion;

public:
    Asteroide(void);

    Asteroide(int numero, int x, int y, double radio_asteroide){
        num_lados = numero;
        angulo = 360.0/num_lados;
        centro[0] = x;
        centro[1] = y;
        radio = radio_asteroide;

        vectores.resize(num_lados);
        vertices.resize(num_lados);
        for (int i = 0; i < num_lados; i++){
            vertices[i].resize(2);
            vectores[i].resize(2);
        }

        velocidad = (-0.1*radio) + 11;

        direccion[0] = rand() % (MAX_VELOCIDAD - MIN_VELOCIDAD + 1) + MIN_VELOCIDAD;
        direccion[1] = (velocidad*velocidad) - (direccion[0]*direccion[0]);
        direccion[1] = sqrt(direccion[1]);

        if (rand() % 2)
            direccion[0] *= -1;
        if (rand() % 2)
            direccion[0] *= -1;

        vel_angular = (-0.21*radio) + 22.14;
        if (rand() % 2)
            vel_angular *= -1;

        inicializar_matriz_rotacion();
    }

    void inicializar_matriz_rotacion(void) {
        matriz_rotacion.resize(2);
        matriz_rotacion[0].resize(2);
        matriz_rotacion[1].resize(2);

        matriz_rotacion[0][0] = cos(vel_angular*PI/180);
        matriz_rotacion[0][1] = -sin(vel_angular*PI/180);
        matriz_rotacion[1][0] = sin(vel_angular*PI/180);
        matriz_rotacion[1][1] = cos(vel_angular*PI/180);

        return;
    }

    void dibujar(void) {
        int i = 0;
        for (; i < num_lados-1; i++)
            gfx_line(vertices[i][0], vertices[i][1], vertices[i+1][0], vertices[i+1][1]);
        
        gfx_line(vertices[0][0], vertices[0][1], vertices[i][0], vertices[i][1]);

        return;
    }

    void calcular_coordenadas(void) {
        int *valores_y = new int[2]();
        int *valores_x = new int[2]();

        int magnitud_A = radio;
        int magnitud_B = magnitud_A;
        
        int coord_a = 0;
        int coord_b = 0;

        float a = 0;
        float b = 0;
        float c = 0;

        bool comparacion = false;

        for (int i = 0; i < num_lados-1; i++) {
            //magnitud_B = magnitud_A;
            if (i == 0) {
                coord_a = magnitud_A;
                coord_b = 0;

                vectores[0][0] = coord_a;
                vectores[0][1] = coord_b;
            } else {
                coord_a = vectores[i][0];
                coord_b = vectores[i][1];
            }
                // Genera la mayoria de los errores
                if (coord_a == 0)
                    return;

                a = ((coord_b*coord_b)/(coord_a*coord_a)) + 1;
                b = -(2*magnitud_A*magnitud_B*coord_b*cos(angulo*PI/180.0))/(coord_a*coord_a);
                c = magnitud_A*magnitud_B*cos(angulo*PI/180.0)/coord_a;
                c = c*c;
                c = c - (magnitud_B*magnitud_B);

                // Mas errores
                if (a == 0)
                    return;

                resolver_trinomio(a, b, c, valores_y);

                valores_x[0] = ((magnitud_A*magnitud_B*cos(angulo*PI/180))-(coord_b*valores_y[0])) / coord_a;

                valores_x[1] = ((magnitud_A*magnitud_B*cos(angulo*PI/180))-(coord_b*valores_y[1])) / coord_a;

            if (i == 0)
                comparacion = valores_y[0] < valores_y[1];
            else {
                comparacion = comparar_distancias(vectores[i-1][0], vectores[i-1][1], valores_x[0], valores_y[0], valores_x[1], valores_y[1]);
            }

            if (comparacion) {
                vectores[i+1][0] = valores_x[0];
                vectores[i+1][1] = valores_y[0];
            } else {
                vectores[i+1][0] = valores_x[1];
                vectores[i+1][1] = valores_y[1];
            }

        }

        for (int i = 0; i < num_lados; i++){
            vertices[i][0] = vectores[i][0] + centro[0];
            vertices[i][1] = vectores[i][1] + centro[1];
        }

        return;
    }

    void resolver_trinomio(float a, float b, float c, int *resultados) {
        double dentro = b*b - 4*a*c;
        dentro = sqrt(dentro);
        resultados[0] = (-b + dentro)/(2*a);
        resultados[1] = (-b - dentro)/(2*a);

        return;
    }

    bool comparar_distancias(int x, int y, int x1, int y1, int x2, int y2) {
        double c1 = ((x - x1)*(x - x1)) + ((y - y1)*(y - y1));
        double c2 = ((x - x2)*(x - x2)) + ((y - y2)*(y - y2));

        if (c1 > c2)
            return true;
        return false;
    }

    void rotar(void) {
        vector<vector <double>> resta;
        vector<vector <double>> nuevos_vertices;

        resta.resize(num_lados);
        nuevos_vertices.resize(num_lados);

        for (int i = 0; i < num_lados; i++){
            resta[i].resize(2);
            nuevos_vertices[i].resize(2);

            resta[i][0] = vertices[i][0] - centro[0];
            resta[i][1] = vertices[i][1] - centro[1];
        }

        for (int i = 0; i < num_lados; i++)
            for (int j = 0; j < 2; j++) {
                nuevos_vertices[i][j] = 0;
                for (int k = 0; k < 2; k++)
                    nuevos_vertices[i][j] += matriz_rotacion[j][k] * resta[i][k];
            }

        for (int i = 0; i < num_lados; i++){
            vertices[i][0] = round(nuevos_vertices[i][0] + centro[0]);
            vertices[i][1] = round(nuevos_vertices[i][1] + centro[1]);
        }

        dibujar();

        return;
    }

    void mover(void) {
        centro[0] += direccion[0];
        centro[1] += direccion[1];

        for (int i = 0; i < num_lados; i++) {
            vertices[i][0] += direccion[0];
            vertices[i][1] += direccion[1];
        }

        dibujar();

        return;
    }
};

int main(int argc, char const *argv[]) {
    srand (time(NULL));
    gfx_open(LARGO_VENTANA, ANCHO_VENTANA, "Animacion de asteroides");
    gfx_color(0, 200, 100);
    gfx_clear();

    vector<Asteroide> asteroides;

    asteroides.reserve(5);

    int radio_random = 10;
    int posicion_x = 0;
    int posicion_y = 0;
    int lados = 5;


    for (int i = 0; i < 5; ++i){
        radio_random = rand() % (RADIO_MAXIMO-RADIO_MINIMO + 1) + RADIO_MINIMO;
        lados = rand() % 3 + 5;
        posicion_y = rand() % (ANCHO_VENTANA-50 + 1) + 50;
        posicion_x = rand() % (LARGO_VENTANA-50 + 1) + 50;
        Asteroide aux(lados, posicion_x, posicion_y, radio_random);
        asteroides.push_back(aux);
        asteroides[i].calcular_coordenadas();
        asteroides[i].dibujar();
    }

    gfx_flush();
    usleep(50000);
    for(int t = 0; t < 40; t++){
        gfx_clear();
        asteroides[0].mover();
        asteroides[1].mover();
        asteroides[2].mover();
        asteroides[3].mover();
        asteroides[4].mover();

        gfx_clear();
        asteroides[0].rotar();
        asteroides[1].rotar();
        asteroides[2].rotar();
        asteroides[3].rotar();
        asteroides[4].rotar();

        gfx_flush();
        usleep(50000); //24 por segundo
    }
    /*
    char c;
    while(1) {
        // Wait for the user to press a character.
        c = gfx_wait();
        gfx_flush();
        // Quit if it is the letter q.
        if(c == 'q') break;
    }
    */
    return 0;
}