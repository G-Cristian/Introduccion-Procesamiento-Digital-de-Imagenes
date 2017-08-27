#include "../Include/openglWrapper.h"

OpenGLWrapper::OpenGLWrapper() {

}

OpenGLWrapper::~OpenGLWrapper() {

}

Imagen OpenGLWrapper::abrirImagen(string nombreArchivo) const {
	Mat img = imread(nombreArchivo, CV_LOAD_IMAGE_COLOR);
	int alto = img.size().height;
	int ancho = img.size().width;

	MatrizUChar m = MatrizUChar(alto, ancho, 0);

	for (int i = 0; i < alto; i++) {
		for (int j = 0; j < ancho; j++) {
			m[i][j] = img.at<unsigned char>(i,j);
		}
	}

	return Imagen(m, 255, img.channels());
}

void OpenGLWrapper::escribirImagen(string nombreArchivoSalida, const Imagen &imagen) const {
	int alto = imagen.alto();
	int ancho = imagen.ancho();
	int canales = imagen.canales();
	Mat img;
	if(canales == 1)
		img = Mat::zeros(alto, ancho, CV_8UC1);
	else if (canales == 3)
		img = Mat::zeros(alto, ancho, CV_8UC3);
	else
		throw "No se puede con esta cantidad de canales.";

	for (int i = 0; i < alto; i++) {
		for (int j = 0; j < ancho; j++) {
			img.at<unsigned char>(i, j) = imagen[i][j];
		}
	}

	imwrite(nombreArchivoSalida, img);
}