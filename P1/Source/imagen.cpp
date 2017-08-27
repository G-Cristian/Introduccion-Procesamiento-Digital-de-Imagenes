#include "../Include/imagen.h"
#include <cmath>

using namespace std;

namespace IPDI {
	Imagen::Imagen(const MatrizUChar &matriz, int cantidadDeNivelesDeGris, int canales):_matriz(matriz){
		assert(cantidadDeNivelesDeGris >= 1);
		_cantidadDeNivelesDeGris = cantidadDeNivelesDeGris;
		_canales = canales;
	}

	Imagen::~Imagen() {

	}

	const vector<unsigned char> & Imagen::operator[](int indice)const {
		return _matriz[indice];
	}

	vector<unsigned char> & Imagen::operator[](int indice) {
		return _matriz[indice];
	}

	//Suma con saturación en 0 y cantidadDeNivelesDeGris - 1
	Imagen Imagen::operator+(const Imagen &otra) const {
		MatrizInt r = ((MatrizInt)(_matriz) + (MatrizInt)(otra._matriz)).saturar(0, _cantidadDeNivelesDeGris-1);

		return Imagen((MatrizUChar)r, _cantidadDeNivelesDeGris, _canales);
	}

	//Resta con saturación en 0 y cantidadDeNivelesDeGris - 1
	Imagen Imagen::operator-(const Imagen &otra) const {
		MatrizInt r = ((MatrizInt)(_matriz) - (MatrizInt)(otra._matriz)).saturar(0, _cantidadDeNivelesDeGris-1);

		return Imagen((MatrizUChar)r, _cantidadDeNivelesDeGris, _canales);
	}

	//TODO: No se como es
	//Imagen operator*(const Imagen &otra) const;
	
	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris-1
	Imagen Imagen::operator*(double escalar) const {
		MatrizDouble r = ((MatrizDouble)(_matriz)*escalar).saturar(0, _cantidadDeNivelesDeGris-1);

		return Imagen((MatrizUChar)r, _cantidadDeNivelesDeGris, _canales);
	}

	//'nivelDeGrisDeimagenDeSalida' es tal que la imagen de salida va a estar entre 0 y 'nivelDeGrisDeimagenDeSalida' - 1.
	Imagen Imagen::compresionDelRangoDinamico(int nivelDeGrisDeimagenDeSalida) const {
		MatrizUChar r = MatrizUChar(_matriz.alto(), _matriz.ancho(), (unsigned char)0);

		//TODO: Esta bien usar '_cantidadDeNivelesDeGris - 1' o deberia ser '_cantidadDeNivelesDeGris' (notar que la imagen va de 0 a _cantidadDeNivelesDeGris - 1)
		//TODO: Esta bien log10
		double c = (nivelDeGrisDeimagenDeSalida-1) / log10(1 + (_cantidadDeNivelesDeGris-1));
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();
		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				r[i][j] = (unsigned char)(c * log10(1 + _matriz[i][j]));
			}
		}

		return Imagen(r, nivelDeGrisDeimagenDeSalida, _canales);
	}

	Imagen Imagen::negativo() const {
		MatrizInt aux = MatrizInt(_matriz.alto(), _matriz.ancho(), _cantidadDeNivelesDeGris-1);

		MatrizInt r = aux - (MatrizInt)(_matriz);

		return Imagen((MatrizUChar)r, _cantidadDeNivelesDeGris, _canales);
	}

	Imagen Imagen::umbral(int u) const {
		MatrizUChar aux = MatrizUChar(_matriz.alto(), _matriz.ancho(), (unsigned char)0);

		int alto = _matriz.alto();
		int ancho = _matriz.ancho();

		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				aux[i][j] = (_matriz[i][j] < u) ? 0 : 255;
			}
		}

		return Imagen(aux, _cantidadDeNivelesDeGris, _canales);
	}

	vector<Imagen> Imagen::planosDeBits() const {
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();
		vector<MatrizUChar> matricesPlanos = vector<MatrizUChar>(8, MatrizUChar(alto, ancho, (unsigned char)0));

		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				matricesPlanos[0][i][j] = _matriz[i][j] & 1;
				matricesPlanos[1][i][j] = _matriz[i][j] & 2;
				matricesPlanos[2][i][j] = _matriz[i][j] & 4;
				matricesPlanos[3][i][j] = _matriz[i][j] & 8;
				matricesPlanos[4][i][j] = _matriz[i][j] & 16;
				matricesPlanos[5][i][j] = _matriz[i][j] & 32;
				matricesPlanos[6][i][j] = _matriz[i][j] & 64;
				matricesPlanos[7][i][j] = _matriz[i][j] & 128;
			}
		}

		vector<Imagen> imagenesPlanos = vector<Imagen>();

		for (int i = 0; i < 8; i++) {
			imagenesPlanos.push_back(Imagen(matricesPlanos[i], _cantidadDeNivelesDeGris, _canales));
		}

		return imagenesPlanos;
	}

	vector<double> Imagen::histograma() const {
		vector<double> h = vector<double>(_cantidadDeNivelesDeGris, 0.0);
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();
		double anchoXAlto = ancho * alto;
		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				h[_matriz[i][j]] += 1.0 / anchoXAlto;
			}
		}

		return h;
	}

	//histograma debe tener algun elemento.
	Imagen Imagen::histogramaAImagen(const vector<double> &histograma) {
		assert(histograma.size() > 0);

		double max = -1.0;
		for (int i = 0; i < histograma.size(); i++) {
			if (max < histograma[i]) {
				max = histograma[i];
			}
		}

		const int alto = histograma.size();
		const int ancho = histograma.size();

		MatrizUChar m = MatrizUChar(alto, ancho, (unsigned char)0);

		for (int j = 0; j < ancho; j++) {
			int u = (int)((alto-1) * (histograma[j] / max));
			for (int i = 0; i < alto; i++) {
				if (i <= u) {
					m[alto - i - 1][j] = 255;
				}
			}
		}

		return Imagen(m, histograma.size(), 1);
	}

	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris-1
	Imagen operator*(double escalar, const Imagen& imagen) {
		return imagen * escalar;
	}
}