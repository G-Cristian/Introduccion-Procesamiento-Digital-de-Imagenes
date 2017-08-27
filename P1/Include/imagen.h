#ifndef _IMAGEN_H_
#define _IMAGEN_H_

#include "matriz.h"

namespace IPDI {
	class Imagen {
	public:
		Imagen(const MatrizUChar &matriz, int cantidadDeNivelesDeGris, int canales);
		~Imagen();

		inline int alto() const { return _matriz.alto(); }
		inline int ancho() const { return (int)(_matriz.ancho() / _canales); }
		inline int canales() const { return _canales; }
		inline const MatrizUChar & matriz() const { return _matriz; }

		const vector<unsigned char> &operator[](int indice)const;
		vector<unsigned char> &operator[](int indice);

		//Suma con saturación en 0 y cantidadDeNivelesDeGris
		Imagen operator+(const Imagen &otra) const;
		//Resta con saturación en 0 y cantidadDeNivelesDeGris
		Imagen operator-(const Imagen &otra) const;
		//TODO: No se como es
		//Imagen operator*(const Imagen &otra) const;

		//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris
		Imagen operator*(double escalar) const;

		Imagen compresionDelRangoDinamico(int nivelDeGrisDeimagenDeSalida = 256) const;
		Imagen negativo() const;
		Imagen umbral(int u) const;
		vector<Imagen> planosDeBits() const;
		vector<double> histograma() const;
		static Imagen histogramaAImagen(const vector<double> &histograma);
	private:
		int _cantidadDeNivelesDeGris;
		MatrizUChar _matriz;
		int _canales;
	};
	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris
	Imagen operator*(double escalar, const Imagen& imagen);
}

#endif // !_IMAGEN_H_
