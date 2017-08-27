#ifndef _OPENCVWRAPPER_H_
#define _OPENCVWRAPPER_H_

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "imagen.h"

using namespace std;
using namespace cv;
using namespace IPDI;

class OpenCVWrapper {
public:
	OpenCVWrapper();
	~OpenCVWrapper();

	Imagen abrirImagen(string nombreArchivo) const;
	void escribirImagen(string nombreArchivoSalida, const Imagen &imagen) const;
};

#endif // !_OPENCVWRAPPER_H_
