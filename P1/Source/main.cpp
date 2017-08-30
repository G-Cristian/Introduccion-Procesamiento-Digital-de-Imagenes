#include "../Include/opencvWrapper.h"
#include "../Include/imagen.h"
#include "../Include/histograma.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace IPDI;
using namespace cv;

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

//	oglw.escribirImagen("(original)" + nombreImagenSalida, img1);

	Imagen compresionRangoDinamico = img1.compresionDelRangoDinamico();
	oglw.escribirImagen("(compresionRangoDinamico)"+ nombreImagenSalida, compresionRangoDinamico);

	Imagen negativo = img1.negativo();
	oglw.escribirImagen("(negativo)"+ nombreImagenSalida, negativo);

	Imagen umbral = img1.umbral(100);
	oglw.escribirImagen("(umbral)" + nombreImagenSalida, umbral);

//	vector<Imagen> planos = img1.planosDeBits();
//	for (int i = 0; i < 8; i++) {
//		stringstream ss;
//		ss << "(planos - bit " << i << ") " << nombreImagenSalida;
//		oglw.escribirImagen(ss.str(), planos[i]);
//	}

	Histograma h = img1.histograma();
	Imagen histo = Imagen::histogramaAImagen(h);
	oglw.escribirImagen("(histograma)" + nombreImagenSalida, histo);
	
	Histograma acum = h.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno();
	Imagen acumImg = acum.aImagen();
	oglw.escribirImagen("(acum)" + nombreImagenSalida, acumImg);
	
	Imagen contraste = img1.constraste();
	oglw.escribirImagen("(contraste)" + nombreImagenSalida, contraste);
	
	Histograma histoEcu = contraste.histograma();
	Imagen histoEcuImg = histoEcu.aImagen();
	oglw.escribirImagen("(histo de imagen ecualizada)" + nombreImagenSalida, histoEcuImg);
	
	oglw.escribirImagen("(acum de histo de imagen ecualizada)" + nombreImagenSalida, histoEcu.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno().aImagen());
}
