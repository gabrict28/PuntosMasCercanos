/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

struct Punto2D{
    double x,y;
};

ostream& operator<<(ostream& os, const Punto2D& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

//Podríamos usarlo en el cálculo de la distancia en la franja para optimizar
bool compararY(const Punto2D& p1, const Punto2D& p2) {
    return p1.y < p2.y;
}

template <typename T>
void imprimeVector(const vector<T>& vec) {
    cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int partition(vector<Punto2D>& puntos, int low, int high) {
    double pivot = puntos[high].x;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (puntos[j].x < pivot) {
            i++;
            std::swap(puntos[i], puntos[j]);
        }
    }
    std::swap(puntos[i + 1], puntos[high]);
    return i + 1;
}

void quicksort(vector<Punto2D>& puntos, int low, int high) {
    if (low < high) {
        int pi = partition(puntos, low, high);
        quicksort(puntos, low, pi - 1);
        quicksort(puntos, pi + 1, high);
    }
}

//Función para calcular la distancia euclídea entre dos puntos -> O(1)
double distanciaEuclidea(Punto2D punto1, Punto2D punto2){
    double d = 0;
    double x_diff = punto2.x-punto1.x;
    double y_diff = punto2.y-punto1.y;
    d += sqrt(x_diff*x_diff+y_diff*y_diff);
    return d;
}

Punto2D puntoMedio(Punto2D punto1, Punto2D punto2){
    Punto2D M;
    M.x = (punto1.x + punto2.x)/2;
    M.y = (punto1.y + punto2.y)/2;
    return M;
}

pair<Punto2D, Punto2D> masCercanosFB(vector<Punto2D> P){
    double min_d = distanciaEuclidea(P.at(0), P.at(1));
    double aux;
    pair<Punto2D,Punto2D> puntos = {P.at(0), P.at(1)};
    for(int i =0; i < P.size()-1; i++){
        for(int j = i+1; j < P.size(); j++){
            aux = distanciaEuclidea(P.at(i), P.at(j));
            if(aux < min_d){
                min_d = aux;
                puntos = {P.at(i), P.at(j)};
            }
        }
    }
    return puntos;
}

pair<Punto2D, Punto2D> masCercanosFBIndices(vector<Punto2D> P, int inicio, int fin){
    double min_d = distanciaEuclidea(P.at(inicio), P.at(fin));
    double aux;
    pair<Punto2D,Punto2D> puntos = {P.at(inicio), P.at(fin)};
    for(int i =inicio; i < fin; i++){
        for(int j = i+1; j <= fin; j++){
            aux = distanciaEuclidea(P.at(i), P.at(j));
            if(aux < min_d){
                min_d = aux;
                puntos = {P.at(i), P.at(j)};
            }
        }
    }
    return puntos;
}

pair<Punto2D, Punto2D> masCercanosDV(vector<Punto2D> P, int inicio, int fin){
    
    pair <Punto2D, Punto2D> mas_cercanos;
    //Caso base
    if((fin-inicio)<=2){
        //Misma función de fuerza bruta pero con índices
        return masCercanosFBIndices(P,inicio,fin);
    }
    
    //Establecemos lo que consideraremos la mitad de nuestro vector
    int mitad = (inicio+fin)/2;
    //Llamamos recursivamente a la función en la primera mitad del vector
    pair<Punto2D, Punto2D> puntosIzq = masCercanosDV(P, inicio, mitad);
    //Establecemos la solución y la mínima distancia para compararlas después
    mas_cercanos = puntosIzq;
    double epsilon = distanciaEuclidea(puntosIzq.first, puntosIzq.second);
    //Llamamos recursivamente a la función en la segunda mitad del vector
    pair<Punto2D, Punto2D> puntosDer = masCercanosDV(P, mitad + 1, fin);
    double d2 = distanciaEuclidea(puntosDer.first, puntosDer.second);
    //Si la mínima distancia de la segunda mitad es menor que la de la primera
    //reestablecemos la solución
    if(d2 < epsilon){
        mas_cercanos = puntosDer;
        epsilon = d2;
    }
    
    //Creamos F tomando como la mitad del vector la coordenada x del punto en la
    //mitad de este
    vector<Punto2D> franja;
    double mitad_franja = P.at(mitad).x;
    double izquierda = mitad_franja - (epsilon/2);
    double derecha = mitad_franja + (epsilon/2);
    for(int i = inicio; i < fin; i++){
        if((izquierda < P[i].x) &&  (P[i].x < derecha)){
            franja.push_back(P[i]);
        }
    }
    
    //Comprobamos que haya más de un punto en la franja ya que en caso contrario
    //no tiene sentido tenerla en cuenta
    if(franja.size() > 1){
        pair<Punto2D, Punto2D> cercanos_franja = masCercanosFB(franja);
        double dist_franja = distanciaEuclidea(cercanos_franja.first, cercanos_franja.second);
        if(dist_franja < epsilon){
            mas_cercanos = cercanos_franja;
        }
    }
    
    return (mas_cercanos);
}
 
pair<Punto2D, Punto2D> solFinalDV(vector<Punto2D>& P, int inicio, int fin){
    quicksort(P, inicio, fin);
    //cout << "VECTOR ORDENADO: ";
    //imprimeVector(P);
    pair<Punto2D, Punto2D> solFinal = masCercanosDV(P,inicio,fin);
    return solFinal;
}

// Función para generar puntos aleatorios
vector<Punto2D> generarPuntos(int n) {
    vector<Punto2D> puntos;
    for (int i = 0; i < n; ++i) {
        Punto2D p = {static_cast<double>(rand()) / RAND_MAX * 1000,
                     static_cast<double>(rand()) / RAND_MAX * 1000};
        puntos.push_back(p);
    }
    return puntos;
}

int main() {
    srand(time(0));

    //Encabezado
    cout << left << setw(8) << "TAM"
         << "| " << setw(15) << "Tiempo FB"
         << "| " << setw(15) << "Tiempo DV" << "|" << endl;
    cout << string(45, '-') << endl;
    //Cambiar el tamaño de n y el número de incremento para obtener otras ejecuciones
    for (int n = 10; n <= 10000; n += 100) {
        vector<Punto2D> puntos = generarPuntos(n);
        vector<Punto2D> puntosFB = puntos;
        vector<Punto2D> puntosDV = puntos;

        auto inicioFB = high_resolution_clock::now();
        pair<Punto2D, Punto2D> resFB = masCercanosFB(puntosFB);
        auto finFB = high_resolution_clock::now();
        auto duracionFB = duration_cast<milliseconds>(finFB - inicioFB);

        auto inicioDV = high_resolution_clock::now();
        pair<Punto2D, Punto2D> resDV = solFinalDV(puntosDV, 0, puntosDV.size() - 1);
        auto finDV = high_resolution_clock::now();
        auto duracionDV = duration_cast<milliseconds>(finDV - inicioDV);

        cout << left << setw(8) << n
             << "| " << setw(15) << (to_string(duracionFB.count()) + " ms")
             << "| " << setw(15) << (to_string(duracionDV.count()) + " ms") << "|" << endl;
    }

    return 0;
}

