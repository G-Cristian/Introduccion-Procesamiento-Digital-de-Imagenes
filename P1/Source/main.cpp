#include "../Include/opencvWrapper.h"
#include "../Include/imagen.h"
#include "../Include/histograma.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>
#include <math.h>

using namespace std;
using namespace IPDI;
using namespace cv;

class DistribucionNormalFunctor{
public:
	//mu y sigma (no sigma ^ 2)
	DistribucionNormalFunctor(double u, double sigma){
		_u = u;
		_sigma = sigma;
	}
	~DistribucionNormalFunctor(){}
	
	double operator()(double x){
		const double e = M_E;
		const double pi = M_PI;
		double aux = ((double)x - _u)/_sigma;
		double potencia = -0.5*(aux*aux);
		//cout << "x: "<< x << " _u: " << _u << " e: " << e << " potencia: " << potencia << " pow(e, potencia): " << pow(e, potencia) << " _sigma: "<< _sigma << " pi: " << pi << " sqrt(2*pi): " << sqrt(2*pi) << " (_sigma * sqrt(2*pi): " << _sigma * sqrt(2*pi) << endl;
		return (pow(e, potencia))/(_sigma * sqrt(2*pi));
	}
private:
	double _u;
	double _sigma;
};

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

//	Imagen compresionRangoDinamico = img1.compresionDelRangoDinamico();
//	oglw.escribirImagen("(compresionRangoDinamico)"+ nombreImagenSalida, compresionRangoDinamico);

//	Imagen negativo = img1.negativo();
//	oglw.escribirImagen("(negativo)"+ nombreImagenSalida, negativo);

//	Imagen umbral = img1.umbral(100);
//	oglw.escribirImagen("(umbral)" + nombreImagenSalida, umbral);

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
	oglw.escribirImagen("(histograma - acum)" + nombreImagenSalida, acumImg);
	/*
	Imagen contraste = img1.constraste();
	oglw.escribirImagen("(contraste)" + nombreImagenSalida, contraste);
	
	Histograma histoEcu = contraste.histograma();
	Imagen histoEcuImg = histoEcu.aImagen();
	oglw.escribirImagen("(contraste - histo)" + nombreImagenSalida, histoEcuImg);
	
	oglw.escribirImagen("(contraste - histo - ecu)" + nombreImagenSalida, histoEcu.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno().aImagen());
	
	Imagen dobleEcuImg = contraste.ecualizarConHistograma(histoEcu);
	oglw.escribirImagen("(doble ecu)" + nombreImagenSalida, dobleEcuImg);
	
	Histograma histoDobleEcu = dobleEcuImg.histograma();
	oglw.escribirImagen("(doble ecu - histo)" + nombreImagenSalida, histoDobleEcu.aImagen());
	oglw.escribirImagen("(doble ecu - histo - acum)" + nombreImagenSalida, histoDobleEcu.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno().aImagen());
	
	*/
	/*
	Histograma normalHisto = Histograma(256, DistribucionNormalFunctor(256.0 / 2.0, 256.0 / 4.0));
	oglw.escribirImagen("(normal - histo)" + nombreImagenSalida, normalHisto.aImagen());
	
	Histograma normalHistoAcum = normalHisto.acumulado();
	oglw.escribirImagen("(normal - histo - acum)" + nombreImagenSalida, normalHistoAcum.aImagen());
	
	Histograma especificadoConNormal = h.especificarParaObtenerDistribucionAcumulada(normalHistoAcum);
	
	Imagen normalEcuImg = img1.ecualizarConHistograma(especificadoConNormal);
	oglw.escribirImagen("(normal - ecu - img)" + nombreImagenSalida, normalEcuImg);
	oglw.escribirImagen("(normal - ecu - img - histo)" + nombreImagenSalida, normalEcuImg.histograma().aImagen());
	oglw.escribirImagen("(normal - ecu - img - histo - acum)" + nombreImagenSalida, normalEcuImg.histograma().acumulado().aImagen());
	*/
	Histograma modificadoHist = h.histogramaModificadoComoUniforme(2);
	Imagen modificadoImg = img1.ecualizarConHistograma(modificadoHist.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno());
	oglw.escribirImagen("(modificado - hist)" + nombreImagenSalida, modificadoHist.aImagen());
	oglw.escribirImagen("(modificado - hist - acum)" + nombreImagenSalida, modificadoHist.acumulado().aImagen());
	oglw.escribirImagen("(modificado - img)" + nombreImagenSalida, modificadoImg);
	oglw.escribirImagen("(modificado - img - histo)" + nombreImagenSalida, modificadoImg.histograma().aImagen());
	oglw.escribirImagen("(modificado - img - histo - acum)" + nombreImagenSalida, modificadoImg.histograma().acumulado().aImagen());
}
