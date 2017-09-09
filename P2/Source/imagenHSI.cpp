#define _USE_MATH_DEFINES
#include "../Include/imagenHSI.h"
#include <math.h>
namespace IPDI {
	ImagenHSI::ImagenHSI(const MatrizDouble &h, const MatrizDouble &s, const MatrizDouble &i) :
		_h(h), _s(s), _i(i) {

	}

	ImagenHSI::~ImagenHSI() {

	}

	Imagen ImagenHSI::HComoImagen() const {
		MatrizUChar m = (MatrizUChar)((255 / (2 * M_PI)) * _h);
		return Imagen(m, 256, 1);
	}

	Imagen ImagenHSI::SComoImagen() const {
		MatrizUChar m = (MatrizUChar)(255 * _s);
		return Imagen(m, 256, 1);
	}

	Imagen ImagenHSI::IComoImagen() const {
		MatrizUChar m = (MatrizUChar)(_i);
		return Imagen(m, 256, 1);
	}
}