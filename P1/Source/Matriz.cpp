#include "../Include/Matriz.h"
#include <iostream>
#include <algorithm>
//#include <exception>
#include <assert.h>

using namespace std;

namespace IPDI {
	template<class T> Matriz<T>::Matriz(int alto, int ancho, T valorInicial) {
		_matriz = vector<vector<T> >(alto, vector<T>(ancho, valorInicial));
		_alto = alto;
		_ancho = ancho;
	}

	template<class T> Matriz<T>::~Matriz() {

	}

	template<class T> const vector<T> & Matriz<T>::operator[](int indice) const {
		return _matriz[indice];
	}

	template<class T> vector<T> & Matriz<T>::operator[](int indice) {
		return _matriz[indice];
	}

	template<class T>Matriz<T> Matriz<T>::sumaConSaturacion(const Matriz<T> &otra, T nivelDeSaturacionInferior, T nivelDeSaturacionSuperior) const {
		//TODO: cambiar por excepcion
		assert(_alto == otra.alto && _ancho == otra._ancho);

		Matriz<T> resultado = Matriz<T>(_alto, _ancho, 0);

		for (int i = 0; i < _alto; i++) {
			for (int j = 0; j < _ancho; j++) {
				double r = _matriz[i][j] + otra[i][j];
				resultado[i][j] = min(nivelDeSaturacionSuperior, max(nivelDeSaturacionInferior, static_cast<T>(r)));
			}
		}

		return resultado;
	}

	template<class T> Matriz<T> Matriz<T>::restaConSaturacion(const Matriz<T> &otra, T nivelDeSaturacionInferior, T nivelDeSaturacionSuperior) const {
		//TODO: cambiar por excepcion
		assert(_alto == otra.alto && _ancho == otra._ancho);

		Matriz<T> resultado = Matriz<T>(_alto, _ancho, 0);

		for (int i = 0; i < _alto; i++) {
			for (int j = 0; j < _ancho; j++) {
				double r = _matriz[i][j] - otra[i][j];
				resultado[i][j] = min(nivelDeSaturacionSuperior, max(nivelDeSaturacionInferior, static_cast<T>(r)));
			}
		}

		return resultado;
	}
	template<class T> Matriz<T> Matriz<T>::productoPorEscalarConSaturacion(double escalar, T nivelDeSaturacionInferior, T nivelDeSaturacionSuperior) const {
		//TODO: cambiar por excepcion
		assert(_alto == otra.alto && _ancho == otra._ancho);

		Matriz<T> resultado = Matriz<T>(_alto, _ancho, 0);

		for (int i = 0; i < _alto; i++) {
			for (int j = 0; j < _ancho; j++) {
				double r = _matriz[i][j] * escalar;
				resultado[i][j] = min(nivelDeSaturacionSuperior, max(nivelDeSaturacionInferior, static_cast<T>(r)));
			}
		}

		return resultado;
	}
}