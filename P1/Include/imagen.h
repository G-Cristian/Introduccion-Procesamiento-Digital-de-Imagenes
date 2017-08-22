#ifndef _IMAGEN_H_
#define _IMAGEN_H_

#include "Matriz.h"

namespace IPDI {
	class Imagen {
	public:
		Imagen(const MatrizChar &matriz, char cantidadDeNivelesDeGris);
		~Imagen();

		//Suma con saturación en 0 y cantidadDeNivelesDeGris
		Imagen operator+(const Imagen &otra) const;
		//Resta con saturación en 0 y cantidadDeNivelesDeGris
		Imagen operator-(const Imagen &otra) const;
		//TODO: No se como es
		//Imagen operator*(const Imagen &otra) const;

		//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris
		Imagen operator*(double escalar) const;

		Imagen compresionDelRangoDinamico(char nivelDeGrisDeimagenDeSalida = 255) const;
		Imagen negativo() const;
	private:
		char _cantidadDeNivelesDeGris;
		MatrizChar _matriz;
	};
	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris
	Imagen operator*(double escalar, const Imagen& imagen);
}

#endif // !_IMAGEN_H_
