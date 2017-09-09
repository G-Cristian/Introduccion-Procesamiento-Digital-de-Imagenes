#define _USE_MATH_DEFINES
#include "../Include/imagen.h"
#include <algorithm>
//#include <cmath>
#include <math.h>

using namespace std;

namespace IPDI {
	Imagen::Imagen(const MatrizUChar &matriz, int cantidadDeNivelesDeGris, int canales) :_matriz(matriz) {
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
		MatrizInt r = ((MatrizInt)(_matriz)+(MatrizInt)(otra._matriz)).saturar(0, _cantidadDeNivelesDeGris - 1);

		return Imagen((MatrizUChar)r, _cantidadDeNivelesDeGris, _canales);
	}

	//Resta con saturación en 0 y cantidadDeNivelesDeGris - 1
	Imagen Imagen::operator-(const Imagen &otra) const {
		MatrizInt r = ((MatrizInt)(_matriz)-(MatrizInt)(otra._matriz)).saturar(0, _cantidadDeNivelesDeGris - 1);

		return Imagen((MatrizUChar)r, _cantidadDeNivelesDeGris, _canales);
	}

	//TODO: No se como es
	//Imagen operator*(const Imagen &otra) const;

	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris-1
	Imagen Imagen::operator*(double escalar) const {
		MatrizDouble r = ((MatrizDouble)(_matriz)*escalar).saturar(0, _cantidadDeNivelesDeGris - 1);

		return Imagen((MatrizUChar)r, _cantidadDeNivelesDeGris, _canales);
	}

	//'nivelDeGrisDeimagenDeSalida' es tal que la imagen de salida va a estar entre 0 y 'nivelDeGrisDeimagenDeSalida' - 1.
	Imagen Imagen::compresionDelRangoDinamico(int nivelDeGrisDeimagenDeSalida) const {
		MatrizUChar r = MatrizUChar(_matriz.alto(), _matriz.ancho(), (unsigned char)0);

		//TODO: Esta bien usar '_cantidadDeNivelesDeGris - 1' o deberia ser '_cantidadDeNivelesDeGris' (notar que la imagen va de 0 a _cantidadDeNivelesDeGris - 1)
		//TODO: Esta bien log10
		double c = (nivelDeGrisDeimagenDeSalida - 1) / log10(1 + (_cantidadDeNivelesDeGris - 1));
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
		MatrizInt aux = MatrizInt(_matriz.alto(), _matriz.ancho(), _cantidadDeNivelesDeGris - 1);

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

	Histograma Imagen::histograma() const {
		return Histograma(*this);
	}

	//histograma debe tener algun elemento.
	Imagen Imagen::histogramaAImagen(const Histograma &histograma) {
		return histograma.aImagen();
	}

	Imagen Imagen::contraste() const {

		int alto = _matriz.alto();
		int ancho = _matriz.ancho();
		int minimo = _matriz.minimo();
		int maximo = _matriz.maximo();

		const int unQuinto = minimo + (maximo - minimo) / 5;
		const int dosQuintos = minimo + 2 * (maximo - minimo) / 5;
		const int tresQuintos = minimo + 3 * (maximo - minimo) / 5;
		const int cuatroQuintos = minimo + 4 * (maximo - minimo) / 5;

		MatrizUChar m = MatrizUChar(alto, ancho, (unsigned char)0);

		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				m[i][j] = _matriz[i][j];
				if (_matriz[i][j] < unQuinto)
					m[i][j] = _matriz[i][j] / 5;
				else if (_matriz[i][j] < dosQuintos)
					m[i][j] = (2 * _matriz[i][j] / 5);
				else if (_matriz[i][j] < tresQuintos)
					m[i][j] = (5 * _matriz[i][j] / 5);
				else if (_matriz[i][j] < cuatroQuintos)
					m[i][j] = min(7 * (int)(_matriz[i][j]) / 5, (int)255);
				else
					m[i][j] = min(6 * (int)(_matriz[i][j]) / 5, 255);

			}
		}

		return Imagen(m, _cantidadDeNivelesDeGris, _canales);
	}

	//'histograma' debe tener valores entre 0 y _cantidadDeNivelesDeGris - 1
	Imagen Imagen::ecualizarConHistograma(const Histograma &histograma)const {
		assert(histograma.cantidadDeElementos() == _cantidadDeNivelesDeGris);
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();

		MatrizUChar m = MatrizUChar(alto, ancho, (unsigned char)0);

		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				//cout << "i " << i << " j " << j << " m[i][j]] " << m[i][j] << " histograma[m[i][j]]] " << (unsigned char)histograma[m[i][j]] << endl;
				m[i][j] = (unsigned char)histograma[_matriz[i][j]];
			}
		}

		return Imagen(m, _cantidadDeNivelesDeGris, _canales);
	}

	vector<Imagen> Imagen::separarEnCanalesEscalaDeGrises()const {
		int ancho = this->ancho();
		int alto = this->alto();

		vector<MatrizUChar> matrices = vector<MatrizUChar>(_canales, MatrizUChar(alto, ancho, (unsigned char)0));
		vector<Imagen> imagenes = vector<Imagen>();

		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				for (int c = 0; c < _canales; c++) {
					matrices[c][i][j] = _matriz[i][j * _canales + c];
				}
			}
		}

		for (int i = 0; i < _canales; i++) {
			imagenes.push_back(Imagen(matrices[i], _cantidadDeNivelesDeGris, 1));
		}

		return imagenes;
	}

	vector<Histograma> Imagen::histogramasDeCanales() const {
		vector<Imagen> canales = this->separarEnCanalesEscalaDeGrises();
		int n = canales.size();

		vector<Histograma> histogramas = vector<Histograma>();
		for (int i = 0; i < n; i++) {
			histogramas.push_back(canales[i].histograma());
		}

		return histogramas;
	}

	//Se espera que la imagen tenga 3 canales (BGR)
	ImagenHSI Imagen::aImagenHSI()const {
		int ancho = this->ancho();
		int alto = this->alto();
		MatrizDouble H = MatrizDouble(alto, ancho);
		MatrizDouble S = MatrizDouble(alto, ancho);
		MatrizDouble I = MatrizDouble(alto, ancho);

		Vector3UChar pixel;
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				pixel = pixelBGREnXY(j, i);
				r = pixel.Z();
				g = pixel.Y();
				b = pixel.X();
				H[i][j] = hDeHSI(r, g, b);
				S[i][j] = sDeHSI(r, g, b);
				I[i][j] = iDeHSI(r, g, b);
			}
		}

		return ImagenHSI(H, S, I);
	}

	double Imagen::hDeHSI(unsigned char r, unsigned char g, unsigned char b) const {
		double h;
		double RMenosGMasRMenosBSobreDos = 0.5*((r - g) + (r - b));
		double RMenosGCuadrado = (r - g)*(r - g);
		double aux = RMenosGMasRMenosBSobreDos / sqrt(RMenosGCuadrado + (r - b)*(g - b));
		h = acos(aux);
		if (b > g) {
			h = 2 * M_PI - h;
		}

		return h;
	}

	double Imagen::sDeHSI(unsigned char r, unsigned char g, unsigned char b) const {
		double minRGB = min(r, min(g, b));
		return 1.0 - 3.0*minRGB / (double)(r + g + b);
	}

	double Imagen::iDeHSI(unsigned char r, unsigned char g, unsigned char b) const {
		return (double)(r + g + b) / 3.0;
	}

	//'H' debe pertenecer [0, 2Pi], 'S' debe pertenecer [0, 1], 'I' debe pertenecer [0, 255].
	Imagen Imagen::aPartirDeHSI(const MatrizDouble &H, const MatrizDouble &S, const MatrizDouble &I) {
		assert(H.alto() == S.alto() && S.alto() == I.alto() && H.ancho() == S.ancho() && S.ancho() == I.ancho());
		int alto = H.alto();
		int ancho = H.ancho();
		MatrizUChar m = MatrizUChar(alto, ancho*3);

		double r = 0.0;
		double g = 0.0;
		double b = 0.0;
		for (int i = 0; i < alto; i++) {
			for (int j = 0; j < ancho; j++) {
				if (H[i][j] < 2 * M_PI / 3) {
					r = I[i][j] * (1 + (S[i][j] * cos(H[i][j])) / cos(M_PI / 3 - H[i][j]));
					b = I[i][j] * (1 - S[i][j]);
					g = 3 * I[i][j] - (r + b);
				}
				else if (H[i][j] < 4 * M_PI / 3) {
					g = I[i][j] * (1 + (S[i][j] * cos(H[i][j] - 2 * M_PI / 3)) / cos(M_PI / 3 - (H[i][j] - 2 * M_PI / 3)));
					r = I[i][j] * (1 - S[i][j]);
					b = 3 * I[i][j] - (r + g);
				}
				else{
					b = I[i][j] * (1 + (S[i][j] * cos(H[i][j] - 4 * M_PI / 3)) / cos(M_PI / 3 - (H[i][j] - 4 * M_PI / 3)));
					g = I[i][j] * (1 - S[i][j]);
					r = 3 * I[i][j] - (g + b);
				}

				m[i][j*3] = b;
				m[i][j * 3 +1] = g;
				m[i][j * 3 +2] = r;
			}
		}
		return Imagen(m, 256, 3);
	}


	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris-1
	Imagen operator*(double escalar, const Imagen& imagen) {
		return imagen * escalar;
	}
}
