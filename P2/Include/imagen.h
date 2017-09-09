#ifndef _IMAGEN_H_
#define _IMAGEN_H_

#include "matriz.h"
#include "histograma.h"
#include "vector3.h"
#include "imagenHSI.h"

namespace IPDI {
	class Histograma;
	class ImagenHSI;
	
	class Imagen {
	public:
		Imagen(const MatrizUChar &matriz, int cantidadDeNivelesDeGris, int canales);
		~Imagen();

		inline int alto() const { return _matriz.alto(); }
		inline int ancho() const { return (int)(_matriz.ancho() / _canales); }
		inline int cantidadDeNivelesDeGris() const { return _cantidadDeNivelesDeGris; }
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

		//Si la cantidad de canales es menor a 3 los ultimos elementos valen 0.
		inline Vector3UChar pixelBGREnXY(int x, int y) const { return Vector3UChar(_matriz.obtenerEnRangoDesdeYX(y, x * _canales, _canales)); }
		Imagen compresionDelRangoDinamico(int nivelDeGrisDeimagenDeSalida = 256) const;
		Imagen negativo() const;
		Imagen umbral(int u) const;
		vector<Imagen> planosDeBits() const;
		Histograma histograma() const;
		static Imagen histogramaAImagen(const Histograma &histograma);
		Imagen contraste() const;
		Imagen ecualizarConHistograma(const Histograma &histograma)const;
		
		vector<Imagen> separarEnCanalesEscalaDeGrises()const;
		vector<Histograma> histogramasDeCanales() const;
		//Se espera que la imagen tenga 3 canales (BGR)
		ImagenHSI aImagenHSI()const;
		double hDeHSI(unsigned char r, unsigned char g, unsigned char b) const;
		double sDeHSI(unsigned char r, unsigned char g, unsigned char b) const;
		double iDeHSI(unsigned char r, unsigned char g, unsigned char b) const;

		//'H' debe pertenecer [0, 2Pi], 'S' debe pertenecer [0, 1], 'I' debe pertenecer [0, 255].
		static Imagen aPartirDeHSI(const MatrizDouble &H, const MatrizDouble &S, const MatrizDouble &I);
	private:
		int _cantidadDeNivelesDeGris;
		MatrizUChar _matriz;
		int _canales;
	};
	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris
	Imagen operator*(double escalar, const Imagen& imagen);
}

#endif // !_IMAGEN_H_
