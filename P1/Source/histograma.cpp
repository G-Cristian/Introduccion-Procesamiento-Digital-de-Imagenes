#include "../Include/histograma.h"
#include "../Include/matriz.h"
#include <assert.h>
#include <sstream>

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
	
	Histograma Histograma::operator+(const Histograma &otro) const{
		int n = _datos.size();
		vector<double> r = vector<double>(n, 0.0);
		for (int i = 0; i < n; i++){
			r[i] = _datos[i] + otro._datos[i];
		}
		
		return Histograma(r);
	}
	
	Histograma Histograma::operator*(double escalar) const{
		int n = _datos.size();
		vector<double> r = vector<double>(n, 0.0);
		for (int i = 0; i < n; i++){
			r[i] = _datos[i] * escalar;
		}
		
		return Histograma(r);
	}
	
	Histograma Histograma::operator/(double escalar) const{
		return (*this) * (1.0/escalar);
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
		//cout << "min: "<<min <<" max: "<<max << endl;
		for(int i = 0; i < n; i++){
			//cout << "_datos[i]: " << _datos[i] << " ";
			//s[i] = (int)(((_datos[i]-min)*(L - 1)/(max - min)) + 0.5);
			s[i] = (((_datos[i]-min)*(L - 1)/(max - min)));
			//cout << "s[i] posterior: " << s[i] << endl;
		}
		
		return Histograma(s);
	}
	
	//'otroAcumulado' debe representar una función de distrbución acumulada (creciente con valores entre 0 y 1).
	//Este histograma no debe representar una distribucion acumulada (debe representar probabilidades entre 0 y 1).
	Histograma Histograma::especificarParaObtenerDistribucionAcumulada(const Histograma &otroAcumulado) const{
		assert(_datos.size() == otroAcumulado._datos.size());
		int n = _datos.size();
		vector<double> datos = vector<double>(n, 0.0);
		Histograma w = this->acumulado().llevarAIntervaloCeroLMenosUno(2);
		Histograma w_m = otroAcumulado.llevarAIntervaloCeroLMenosUno(2);
		
		int j = 0;
		for(int i = 0; i < n; i++){
			while(j < n && w_m._datos[j] < w._datos[i]){
			//	cout<<"estaAcum._datos[j]: " << estaAcum._datos[j] << " otroAcumulado._datos[i]: " << otroAcumulado._datos[i] << endl;
				j++;
			}
			if(j >= n){
				stringstream ss;
				ss << "Error in file: " << __FILE__ << " line: " << __LINE__ <<". 'j' deberia ser menor a 'n'.";
				throw ss.str();
			}
			
			datos[i] = j;
			
			//cout<<"datos[i]: " << datos[i] << endl;
		}
		
		return Histograma(datos);
	}
	
	Histograma Histograma::histogramaModificadoComoUniforme(double lambda) const {
		int n = _datos.size();
		Histograma uniforme = Histograma(n, DistribucionUniformeFunctor(n));
		
		double lambdaMasUno = lambda + 1.0;
		
		return ((*this) + lambda * uniforme) / lambdaMasUno;
	}
	
	Histograma operator*(double escalar, const Histograma &histograma){
		return histograma * escalar;
	}
}
