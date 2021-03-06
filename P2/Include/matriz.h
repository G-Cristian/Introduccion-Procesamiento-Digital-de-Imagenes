#ifndef _MATRIZ_H_
#define _MATRIZ_H_

#include <vector>
#include <exception>
#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;

template <class T> class Matriz;

template<class T> class SumarMatrizFunctor{
public:
	SumarMatrizFunctor(const Matriz<T> &sumando):_sumando(sumando){
		
	}
	
	T operator()(int fila, int columna, T elemento){
		return elemento + _sumando[fila][columna];
	}
private:
	Matriz<T> _sumando;
};

template<class tipoMatriz, class tipoEscalar> class MultiplicarPorEscalarMatrizFunctor{
public:
	MultiplicarPorEscalarMatrizFunctor(const tipoEscalar &escalar){
		_escalar = escalar;
	}
	
	tipoMatriz operator()(int fila, int columna, tipoMatriz elemento){
		return (tipoMatriz)(elemento * _escalar);
	}
private:
	tipoEscalar _escalar;
};

template<class T> class SaturarMatrizAMatrizCharFunctor{
public:
	SaturarMatrizAMatrizCharFunctor(T menor, T mayor):_menor(menor),_mayor(mayor){
	}
	
	T operator()(int fila, int columna, T elemento){
		if (elemento < _menor) return _menor;
		if (elemento > _mayor) return _mayor;
		return elemento;
	}
private:
	T _menor;
	T _mayor;
};

template <class T> class Matriz{
public:
	Matriz(int alto, int ancho) :Matriz<T>(alto, ancho, T()) {
	}

	Matriz(int alto, int ancho, T valorInicial) {
		_ancho = ancho;
		_alto = alto;
	
		_matriz = vector<vector<T> >(alto, vector<T>(ancho,valorInicial));
	}

	Matriz(int alto, int ancho, T *m) {
		_alto = alto;
		_ancho = ancho;
		_matriz = vector<vector<T> >(alto, vector<T>(ancho, 0));

		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				_matriz[i][j] = m[i*ancho + j];
			}
		}
	}
	
	~Matriz(){
	}
	
	//operador para castear
	template<class tipoACastear> operator Matriz<tipoACastear>()const {
		Matriz<tipoACastear> r = Matriz<tipoACastear>(_alto, _ancho, (tipoACastear)0);
		
		for(int i = 0; i < _alto; i++){
			for(int j = 0; j < _ancho; j++){
				r[i][j] = (tipoACastear)_matriz[i][j];
			}
		}
		
		return r;
	}
	
	const vector<T> &operator[](int indice) const{
		assert(indiceEnRangoAlto(indice));
		return _matriz[indice];
	}

	vector<T> &operator[](int indice){
		assert(indiceEnRangoAlto(indice));
		return _matriz[indice];
	}
	
	inline bool indiceEnRangoAlto(int indice) const {
		return 0 <= indice && indice < _alto;
	}

	inline bool indiceEnRangoAncho(int indice) const {
		return 0 <= indice && indice < _ancho;
	}

	inline int ancho() const{
		return _ancho;
	}
	
	inline int alto() const {
		return _alto;
	}

	//Obtiene los valores desde Y, X hasta Y, X + rango - 1.
	//Si X + rango >= ancho obtiene hasta ancho - 1 y llena con ceros el resto
	vector<T> obtenerEnRangoDesdeYX(int y, int x, int rango)const {
		assert(indiceEnRangoAlto(y) && indiceEnRangoAncho(x));

		vector<T> ret = vector<T>(rango, T());
		int limiteAncho = min(_ancho, x + rango);
		for (int i = x; i < limiteAncho; i++) {
			ret[i - x] = _matriz[y][i];
		}

		return ret;
	}

	//Crea puntero. El usuario debe encargarse de liberar la memoria.
	T* comoPuntero() const {
		T * ret = new T[_ancho*_alto];

		for (int i = 0; i < _alto; i++) {
			for (int j = 0; j < _ancho; j++) {
				ret[i*_ancho + j] = _matriz[i][j];
			}
		}

		return ret;
	}
	
	//Deben tener el mismo tamaño.
	Matriz<T> operator+(const Matriz<T> &otra) const {
		//if(_alto != otra._alto || _ancho != otra._ancho)
		//	throw "Deben tener las mismas dimensiones.";
		assert(_alto == otra._alto && _ancho == otra._ancho);
		return crearMatrizAPartirDeOtraAplicandoFuncAElementos(*this, SumarMatrizFunctor<T>(otra));
	}

	Matriz<T> operator-()const {
		return *this * (-1);
	}
	
	Matriz<T> operator-(const Matriz<T> &otra)const {
		return *this + (-otra);
	}
	
	Matriz<T> operator*(const Matriz<T> &otra) const {
		assert(_ancho == otra._alto);
		int m = _alto;
		int n = _ancho;
		int s = otra._ancho;

		Matriz<T> r = Matriz<T>(m, s, 0);
		T sum = 0;
		for (int f = 0; f < m; f++) {
			for (int c = 0; c < s; c++) {
				sum = 0;
				for (int k = 0; k < n; k++) {
					sum += _matriz[f][k] * otra._matriz[k][c];
				}

				r._matriz[f][c] = sum;
			}
		}

		return r;
	}

	template <class U>
	Matriz<T> operator*(U escalar) const {
		return crearMatrizAPartirDeOtraAplicandoFuncAElementos(*this, MultiplicarPorEscalarMatrizFunctor<T, U>(escalar));
	}

	/*
	Matriz<T> operator/(int escalar) const {
		return *this * (1.0/(double)escalar);
	}
	*/
	Matriz<T> operator/(double escalar) const {
		return *this * (1.0/escalar);
	}
	
	//Satura los valores entre 0 y 255
	Matriz<T> saturar(T menor, T mayor) const {
		return crearMatrizAPartirDeOtraAplicandoFuncAElementos(*this, SaturarMatrizAMatrizCharFunctor<T>(menor, mayor));
	}
	
	Matriz<T> filaComoMatriz(int fila) const {
		assert(indiceEnRangoAlto(fila));
		Matriz<T> r = Matriz<T>(1, _ancho, T());
		for (int i = 0; i < _ancho; i++) {
			r[0][i] = _matriz[fila][i];
		}

		return r;
	}

	Matriz<T> & agreagrleAFilaIOtraFila(int filaI, const Matriz<T> &otraFila) {
		assert(_ancho == otraFila._ancho && otraFila._alto == 1);

		return agreagrleAFilaIOtraFila(filaI, otraFila[0]);
	}

	Matriz<T> & agreagrleAFilaIOtraFila(int filaI, const vector<T> &otraFila) {
		assert(_ancho == otraFila.size());
		
		for (int c = 0; c < _ancho; c++) {
			_matriz[filaI][c] += otraFila[c];
		}

		return *this;
	}

	Matriz<T> submatriz(int filaSup, int colIzq, int filaInf, int colDer) const {
		assert(indiceEnRangoAlto(filaSup) && indiceEnRangoAlto(filaInf) && indiceEnRangoAncho(colIzq) && indiceEnRangoAncho(colDer) &&
			filaSup <= filaInf && colIzq <= colDer);
		Matriz<T> r = Matriz<T>(filaInf - filaSup + 1, colDer - colIzq + 1, T());

		for (int i = 0; i < r._alto; i++) {
			for (int j = 0; j < r._ancho; j++) {
				r[i][j] = _matriz[filaSup + i][colIzq + j];
			}
		}

		return r;
	}

	T minimo()const {
		T min = 0;
		if (_alto * _ancho > 0) {
			min = _matriz[0][0];
			for (int i = 0; i < _alto; i++) {
				for (int j = 0; j < _ancho; j++) {
					if (min > _matriz[i][j]) {
						min = _matriz[i][j];
					}
				}
			}
		}

		return min;
	}

	T maximo()const {
		T max = 0;
		if (_alto * _ancho > 0) {
			max = _matriz[0][0];
			for (int i = 0; i < _alto; i++) {
				for (int j = 0; j < _ancho; j++) {
					if (max < _matriz[i][j]) {
						max = _matriz[i][j];
					}
				}
			}
		}

		return max;
	}

	//Modifica la matriz original.
	//Func es de tipo T fun(int fila, int columna, T elementoActual) donde fila, columna es la posicion de la matriz que se recorre y elementoActual es el elemento actual de la matriz que se recorre.
	template<class Func> Matriz<T>& aplicarFuncAElementos(Func func) {
		for(int i = 0; i < _alto; i++){
			for(int j = 0; j < _ancho; j++){
				_matriz[i][j] = func(i,j,_matriz[i][j]);
			}
		}
		return *this;
	}

	void mostrar() const {
		for (int i = 0; i < _alto; i++) {
			for (int j = 0; j < _ancho; j++) {
				cout << _matriz[i][j] << " ";
			}
			cout << endl;
		}
	}

private:
	vector<vector<T> > _matriz;
	int _ancho;
	int _alto;
};

template<class T> Matriz<T> operator*(int escalar, const Matriz<T> &otra) {
	return otra * escalar;
}

template<class T> Matriz<T> operator*(double escalar, const Matriz<T> &otra) {
	return otra * escalar;
}

template<class T, class Func> Matriz<T> crearMatrizAPartirDeOtraAplicandoFuncAElementos(const Matriz<T> &otra, Func func) {
	Matriz<T> r = otra;
	r.aplicarFuncAElementos(func);
	
	return r;
}

typedef Matriz<char> MatrizChar;
typedef Matriz<unsigned char> MatrizUChar;
typedef Matriz<int> MatrizInt;
typedef Matriz<double> MatrizDouble;

#endif
