#ifndef _HISTOGRAMA_H_
#define _HISTOGRAMA_H_

#include "imagen.h"
#include <vector>

using namespace std;

namespace IPDI {
	class Imagen;
	
	class DistribucionUniformeFunctor{
	public:
		DistribucionUniformeFunctor(double cantidad)
		{
			_cantidad = cantidad;
		}
		~DistribucionUniformeFunctor()
		{
		}
		double operator()(double x){
			return 1.0/_cantidad;
		}
	private:
		double _cantidad;
	};
		
	
	class Histograma{
	public:
		Histograma(const vector<double> &datos);
		Histograma(const Imagen &imagen);
		
		template<class Func> Histograma(int cantidadDeElementos, Func funcionDeProbabilidad)
		{
			_datos = vector<double>(cantidadDeElementos, 0.0);
			
			for(int i = 0; i < cantidadDeElementos; i++){
				_datos[i] = funcionDeProbabilidad(i);
		//		cout << "i: " << i << " _datos[i]: " << _datos[i] << " funcionDeProbabilidad(i): " << funcionDeProbabilidad(i) << endl;
			}
		}
		
		~Histograma();
	
		inline int cantidadDeElementos() const { return _datos.size(); }
	
		const double operator[](int indice) const;
		
		Histograma operator+(const Histograma &otro) const;
		Histograma operator*(double escalar) const;
		Histograma operator/(double escalar) const;
	
		Imagen aImagen() const;
		Histograma acumulado() const;
		
		//Calcula el acumulado y lo lleva al intervalo [0-cantidadDeElementos - 1]
		Histograma acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno() const;
		
		//'otroAcumulado' debe representar una función de distrbución acumulada (creciente con valores entre 0 y 1).
		//Este histograma no debe representar una distribucion acumulada (debe representar probabilidades entre 0 y 1).
		Histograma especificarParaObtenerDistribucionAcumulada(const Histograma &otroAcumulado) const;
		
		Histograma histogramaModificadoComoUniforme(double lambda)const;
	private:
		//Lleva el histograma al intervalo [0-L - 1]
		//El histograma representa una función de distribución acumulada (creciente).
		Histograma llevarAIntervaloCeroLMenosUno(double L) const;
		
		vector<double> _datos;
	};
	
	Histograma operator*(double escalar, const Histograma &hitograma);
}

#endif
