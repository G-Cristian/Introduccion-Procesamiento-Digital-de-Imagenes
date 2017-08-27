#include "../Include/opencvWrapper.h"

OpenCVWrapper::OpenCVWrapper() {

}

OpenCVWrapper::~OpenCVWrapper() {

}

Imagen OpenCVWrapper::abrirImagen(string nombreArchivo) const {
	Mat img = imread(nombreArchivo, CV_LOAD_IMAGE_COLOR);
	int alto = img.size().height;
	int ancho = img.size().width;

	MatrizUChar m = MatrizUChar(alto, ancho*img.channels(), img.data);

	/*
	for (int i = 0; i < alto; i++) {
		for (int j = 0; j < ancho; j++) {
			m[i][j*img.channels() + 0] = img.at<Vec3b>(i, j)[0];
			m[i][j*img.channels() + 1] = img.at<Vec3b>(i, j)[1];
			m[i][j*img.channels() + 2] = img.at<Vec3b>(i, j)[2];
		}
	}
	*/

	return Imagen(m, 256, img.channels());
}

void OpenCVWrapper::escribirImagen(string nombreArchivoSalida, const Imagen &imagen) const {
	assert(imagen.canales() == 1 || imagen.canales() == 2 || imagen.canales() == 3 || imagen.canales() == 4);
	int alto = imagen.alto();
	int ancho = imagen.ancho();
	int canales = imagen.canales();
	Mat img;

	unsigned char *m = imagen.matriz().comoPuntero();
	if (imagen.canales() == 1) {
		img = Mat(alto, ancho, CV_8UC1, (void *)m);
	}
	else if (imagen.canales() == 2) {
		img = Mat(alto, ancho, CV_8UC2, (void *)m);
	}
	else if (imagen.canales() == 3) {
		img = Mat(alto, ancho, CV_8UC3, (void *)m);
	}
	else {
		img = Mat(alto, ancho, CV_8UC4, (void *)m);
	}
	/*
	for (int i = 0; i < alto; i++) {
		for (int j = 0; j < ancho; j++) {
			img.at<Vec3b>(i, j)[0] = imagen[i][j*canales + 0];
			img.at<Vec3b>(i, j)[1] = imagen[i][j*canales + 1];
			img.at<Vec3b>(i, j)[2] = imagen[i][j*canales + 2];
		}
	}
	*/
	
	imwrite(nombreArchivoSalida, img);

	delete[]m;
}