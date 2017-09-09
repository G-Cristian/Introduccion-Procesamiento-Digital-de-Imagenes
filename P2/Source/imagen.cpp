#include "../Include/imagen.h"
#include <algorithm>
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

	Histograma Imagen::histograma() const {
		return Histograma(*this);
	}

	//histograma debe tener algun elemento.
	Imagen Imagen::histogramaAImagen(const Histograma &histograma) {
		return histograma.aImagen();
	}
	
	Imagen Imagen::contraste() const{
		
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();
		int minimo = _matriz.minimo();
		int maximo = _matriz.maximo();

		const int unQuinto = minimo + (maximo - minimo) / 5;
		const int dosQuintos = minimo + 2*(maximo - minimo) / 5;
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
		assert (histograma.cantidadDeElementos() == _cantidadDeNivelesDeGris);
		int alto = _matriz.alto();
		int ancho = _matriz.ancho();
		
		MatrizUChar m = MatrizUChar(alto, ancho, (unsigned char)0);
		
		for(int i = 0; i < alto; i++){
			for(int j = 0; j < ancho; j++){
				//cout << "i " << i << " j " << j << " m[i][j]] " << m[i][j] << " histograma[m[i][j]]] " << (unsigned char)histograma[m[i][j]] << endl;
				m[i][j] = (unsigned char)histograma[_matriz[i][j]];
			}
		}
		
		return Imagen(m, _cantidadDeNivelesDeGris, _canales);
	}
	

	//Multiplicacion con saturación en 0 y cantidadDeNivelesDeGris-1
	Imagen operator*(double escalar, const Imagen& imagen) {
		return imagen * escalar;
	}
}
