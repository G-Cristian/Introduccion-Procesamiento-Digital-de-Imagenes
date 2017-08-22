#include "../Include/imagen.h"
#include <cmath>

using namespace std;

namespace IPDI {
	Imagen::Imagen(const MatrizChar &matriz, char cantidadDeNivelesDeGris) {
		_matriz = matriz;
		_cantidadDeNivelesDeGris = cantidadDeNivelesDeGris;
	}

	Imagen::~Imagen() {

	}

	//Suma con saturación en 0 y cantidadDeNivelesDeGris
	Imagen Imagen::operator+(const Imagen &otra) const {
		MatrizChar r = _matriz.sumaConSaturacion(otra._matriz, 0, _cantidadDeNivelesDeGris);

		return Imagen(r, _cantidadDeNivelesDeGris);
	}

	//Resta con saturación en 0 y cantidadDeNivelesDeGris
	Imagen Imagen::operator-(const Imagen &otra) const {
		MatrizChar r = _matriz.restaConSaturacion(otra._matriz, 0, _cantidadDeNivelesDeGris);

		return Imagen(r, _cantidadDeNivelesDeGris);
	}

	//TODO: No se como es
	//Imagen operator*(const Imagen &otra) const;
	
	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris
	Imagen Imagen::operator*(double escalar) const {
		MatrizChar r = _matriz.productoPorEscalarConSaturacion(escalar, 0, _cantidadDeNivelesDeGris);

		return Imagen(r, _cantidadDeNivelesDeGris);
	}

	Imagen Imagen::compresionDelRangoDinamico(char nivelDeGrisDeimagenDeSalida) const {
		MatrizChar r = MatrizChar(_matriz.alto(), _matriz.ancho);

		//TODO: Esta bien usar 'nivelDeGrisDeimagenDeSalida' asi o tendría que restarle 1
		//TODO: Esta bien log10
		double c = nivelDeGrisDeimagenDeSalida / log10(1 + _cantidadDeNivelesDeGris);
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();
		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				r[i][j] = c * log10(1 + _matriz[i][j]);
			}
		}

		return Imagen(r, nivelDeGrisDeimagenDeSalida);
	}

	Imagen Imagen::negativo() const {
		MatrizChar r = MatrizChar(_matriz.alto(), _matriz.ancho);
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();

		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				r[i][j] = -_matriz[i][j] + _cantidadDeNivelesDeGris;
			}
		}

		return Imagen(r, _cantidadDeNivelesDeGris);
	}

	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris
	Imagen operator*(double escalar, const Imagen& imagen) {
		return imagen * escalar;
	}
}