#define _USE_MATH_DEFINES
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
	
	Imagen compresionRangoDinamico = img1.compresionDelRangoDinamico();
	oglw.escribirImagen(nombreImagenSalida + "(compresionRangoDinamico).png", compresionRangoDinamico);

	Imagen negativo = img1.negativo();
	oglw.escribirImagen(nombreImagenSalida + "(negativo).png", negativo);

	Imagen umbral = img1.umbral(100);
	oglw.escribirImagen(nombreImagenSalida + "(umbral).png", umbral);

	vector<Imagen> planos = img1.planosDeBits();
	for (int i = 0; i < 8; i++) {
		stringstream ss;
		ss << nombreImagenSalida << "(planos - bit " << i << ").png";
		oglw.escribirImagen(ss.str(), planos[i]);
	}

	Histograma h = img1.histograma();
	oglw.escribirImagen(nombreImagenSalida + "(histograma).png", h.aImagen());
	
	Histograma acum = h.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno();
	oglw.escribirImagen(nombreImagenSalida + "(histograma - acum).png", acum.aImagen());
	
	
	Imagen contraste = img1.contraste();
	oglw.escribirImagen(nombreImagenSalida + "(contraste).png", contraste);
	
	Histograma histoConstraste = contraste.histograma();
	oglw.escribirImagen(nombreImagenSalida + "(contraste - histo).png", histoConstraste.aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(contraste - histo - acum).png", histoConstraste.acumulado().aImagen());
	
	
	Imagen ecualizada = img1.ecualizarConHistograma(h.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno());
	oglw.escribirImagen(nombreImagenSalida + "(ecualizada).png", ecualizada);
	
	Histograma histoEcu = ecualizada.histograma();
	oglw.escribirImagen(nombreImagenSalida + "(ecualizada - histo).png", histoEcu.aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(ecualizada - histo - acum).png", histoEcu.acumulado().aImagen());

	Imagen dobleEcuImg = ecualizada.ecualizarConHistograma(histoEcu.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno());
	oglw.escribirImagen(nombreImagenSalida + "(doble ecu).png", dobleEcuImg);
	
	Histograma histoDobleEcu = dobleEcuImg.histograma();
	oglw.escribirImagen(nombreImagenSalida + "(doble ecu - histo).png", histoDobleEcu.aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(doble ecu - histo - acum).png", histoDobleEcu.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno().aImagen());
	
	Histograma normalHisto = Histograma(256, DistribucionNormalFunctor(256.0 / 2.0, 256.0 / 4.0));
	oglw.escribirImagen(nombreImagenSalida + "(normal - histo).png", normalHisto.aImagen());
	
	Histograma normalHistoAcum = normalHisto.acumulado();
	oglw.escribirImagen(nombreImagenSalida + "(normal - histo - acum).png", normalHistoAcum.aImagen());
	
	Histograma especificadoConNormal = h.especificarParaObtenerDistribucionAcumulada(normalHistoAcum);
	//oglw.escribirImagen(nombreImagenSalida + "(histo - especificado con normal).png", especificadoConNormal.aImagen());
	//oglw.escribirImagen(nombreImagenSalida + "(histo - especificado con normal - acum).png", especificadoConNormal.acumulado().aImagen());

	Imagen normalEcuImg = img1.ecualizarConHistograma(especificadoConNormal);
	oglw.escribirImagen(nombreImagenSalida + "(normal - ecu - img).png", normalEcuImg);
	oglw.escribirImagen(nombreImagenSalida + "(normal - ecu - img - histo).png", normalEcuImg.histograma().aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(normal - ecu - img - histo - acum).png", normalEcuImg.histograma().acumulado().aImagen());
	
	Histograma modificadoHist1 = h.histogramaModificadoComoUniforme(1);
	Imagen modificadoImg1 = img1.ecualizarConHistograma(modificadoHist1.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno());
	//oglw.escribirImagen(nombreImagenSalida + "(modificado - hist1).png", modificadoHist1.aImagen());
	//oglw.escribirImagen(nombreImagenSalida + "(modificado - hist1 - acum).png", modificadoHist1.acumulado().aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(modificado - img1).png", modificadoImg1);
	oglw.escribirImagen(nombreImagenSalida + "(modificado - img1 - histo).png", modificadoImg1.histograma().aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(modificado - img1 - histo - acum).png", modificadoImg1.histograma().acumulado().aImagen());

	Histograma modificadoHist2 = h.histogramaModificadoComoUniforme(2);
	Imagen modificadoImg2 = img1.ecualizarConHistograma(modificadoHist2.acumuladoEnIntervaloCeroYCantidadDeElementosMenosUno());
	//oglw.escribirImagen(nombreImagenSalida + "(modificado - hist2).png", modificadoHist2.aImagen());
	//oglw.escribirImagen(nombreImagenSalida + "(modificado - hist2 - acum).png", modificadoHist2.acumulado().aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(modificado - img2).png", modificadoImg2);
	oglw.escribirImagen(nombreImagenSalida + "(modificado - img2 - histo).png", modificadoImg2.histograma().aImagen());
	oglw.escribirImagen(nombreImagenSalida + "(modificado - img2 - histo - acum).png", modificadoImg2.histograma().acumulado().aImagen());
	
}
