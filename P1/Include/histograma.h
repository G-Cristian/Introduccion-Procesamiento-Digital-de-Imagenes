#ifndef _HISTOGRAMA_H_
#define _HISTOGRAMA_H_

#include "imagen.h"
#include <vector>

using namespace std;

namespace IPDI {
	class Imagen;
	
	class Histograma{
	public:
		Histograma(const vector<double> &datos);
		Histograma(const Imagen &imagen);
		~Histograma();
	
		inline int cantidadDeElementos() const { return _datos.size(); }
	
		const double operator[](int indice) const;
	
		Imagen aImagen() const;
		Histograma acumulado() const;
		
		//Calcula el acumulado y lo lleva al intervalo [0-cantidadDeElementos - 1]
		Histograma acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno() const;
	private:
		//Lleva el histograma al intervalo [0-L - 1]
		//El histograma representa una función de distribución acumulada (creciente).
		Histograma llevarAIntervaloCeroLMenosUno(double L) const;
		
		vector<double> _datos;
	};
}

#endif
