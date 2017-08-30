#include "../Include/histograma.h"
#include "../Include/matriz.h"
#include <assert.h>

namespace IPDI {
	class Imagen;
	
	Histograma::Histograma(const vector<double> &datos){
		assert(datos.size() > 0);
		_datos = datos;
	}

	Histograma::Histograma(const Imagen &imagen){
		_datos = vector<double>(imagen.cantidadDeNivelesDeGris(), 0.0);
		MatrizUChar matriz = imagen.matriz();
		int alto = matriz.alto();
		int ancho = matriz.ancho();
		double anchoXAlto = ancho * alto;
		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				_datos[matriz[i][j]] += 1.0 / anchoXAlto;
			}
		}
	}

	Histograma::~Histograma(){
	}
		
	const double Histograma::operator[](int indice) const{
		return _datos[indice];
	}
		
	Imagen Histograma::aImagen() const{
		double max = -1.0;
		for (int i = 0; i < _datos.size(); i++) {
			if (max < _datos[i]) {
				max = _datos[i];
			}
		}
		
		const int alto = _datos.size();
		const int ancho = alto;

		MatrizUChar m = MatrizUChar(alto, ancho, (unsigned char)0);

		for (int j = 0; j < ancho; j++) {
			int u = (int)((alto-1) * (_datos[j] / max));
			for (int i = 0; i < alto; i++) {
				if (i <= u) {
					m[alto - i - 1][j] = 255;
				}
			}
		}

		return Imagen(m, _datos.size(), 1);
	}

	Histograma Histograma::acumulado() const {
		int n = _datos.size();
		vector<double> acum = vector<double>(n,0.0);
		
		acum[0] = _datos[0];
		
		for(int i = 1; i < n; i++){
			acum[i] = _datos[i] + acum[i-1];
		}
		
		return Histograma(acum);
	}
	
	//Calcula el acumulado y lo lleva al intervalo [0-cantidadDeElementos - 1]
	Histograma Histograma::acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno() const {
		return acumulado().llevarAIntervaloCeroLMenosUno(_datos.size());
	}
	
	//Lleva el histograma al intervalo [0-L - 1]
	//El histograma representa una función de distribución acumulada (creciente).
	Histograma Histograma::llevarAIntervaloCeroLMenosUno(double L) const {
		assert(_datos.size() > 1);
		int n = _datos.size();
		double min = _datos[0];
		double max = _datos[n - 1];
		vector<double> s = vector<double>(n,0.0);
		
		for(int i = 0; i < n; i++){
			s[i] = (int)(((_datos[i]-min)*(L - 1)/(max - min)) + 0.5);
		}
		
		return Histograma(s);
	}
}
