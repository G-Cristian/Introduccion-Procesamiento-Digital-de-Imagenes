#define _USE_MATH_DEFINES
#include "../Include/opencvWrapper.h"
#include "../Include/imagen.h"
#include "../Include/histograma.h"

#include <iostream>
#include <string>
#include <math.h>

using namespace std;
using namespace IPDI;

int main() {
	string nombreImagenEntrada = "";
	string nombreImagenSalida = "";

	cout << "Imagen entrada: ";
	getline(cin, nombreImagenEntrada);
	cout << endl;
	cout << "Imagen salida: ";
	getline(cin, nombreImagenSalida);

	OpenCVWrapper oglw = OpenCVWrapper();
	Imagen img1 = oglw.abrirImagen(nombreImagenEntrada);

	vector<Imagen> canales = img1.separarEnCanalesEscalaDeGrises();
	for (int i = 0; i < canales.size(); i++) {
		stringstream ss;
		ss << nombreImagenSalida << "(canal " << i << ").png";
		oglw.escribirImagen(ss.str(), canales[i]);
	}

	vector<Histograma> histogramasDeCanales = img1.histogramasDeCanales();
	for (int i = 0; i < histogramasDeCanales.size(); i++) {
		stringstream ss;
		ss << nombreImagenSalida << "(histograma de canal " << i << ").png";
		oglw.escribirImagen(ss.str(), histogramasDeCanales[i].aImagen());
	}

	for (int i = 0; i < canales.size(); i++) {
		stringstream ss;
		ss << nombreImagenSalida << "(ecualizacion de canal " << i << ").png";
		oglw.escribirImagen(ss.str(), canales[i].ecualizarConHistograma(histogramasDeCanales[i].acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno()));
	}

	ImagenHSI hsi = img1.aImagenHSI();
	oglw.escribirImagen(nombreImagenSalida + "(canal H).png", hsi.HComoImagen());
	oglw.escribirImagen(nombreImagenSalida + "(canal S).png", hsi.SComoImagen());
	oglw.escribirImagen(nombreImagenSalida + "(canal I).png", hsi.IComoImagen());

	Imagen imagenI = hsi.IComoImagen();
	Histograma histogramaI = imagenI.histograma();
	Imagen imagneIEcualizada = imagenI.ecualizarConHistograma(histogramaI.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno());
	Imagen ecualizadaEnI = Imagen::aPartirDeHSI(hsi.H_Referencia(), hsi.S_Referencia(), (MatrizDouble)(imagneIEcualizada.matriz()));
	oglw.escribirImagen(nombreImagenSalida + "(histograma I).png", histogramaI.aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(histograma I despues de ecualizar).png", imagneIEcualizada.histograma().aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(I ecualizado).png", imagneIEcualizada);
	oglw.escribirImagen(nombreImagenSalida + "(ecualizada en I).png", ecualizadaEnI);

	return 0;
}
