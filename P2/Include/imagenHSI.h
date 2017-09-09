#ifndef _IMAGEN_HSI_H_
#define _IMAGEN_HSI_H_

#include "matriz.h"
#include "imagen.h"
namespace IPDI {
	class Imagen;
	class ImagenHSI {
	public:
		ImagenHSI(const MatrizDouble &h, const MatrizDouble &s, const MatrizDouble &i);
		~ImagenHSI();

		inline MatrizDouble H() const { return _h; }
		inline MatrizDouble S() const { return _s; }
		inline MatrizDouble I() const { return _i; }

		inline const MatrizDouble & H_Referencia() const { return _h; }
		inline const MatrizDouble & S_Referencia() const { return _s; }
		inline const MatrizDouble & I_Referencia() const { return _i; }

		Imagen HComoImagen() const;
		Imagen SComoImagen() const;
		Imagen IComoImagen() const;
	private:
		MatrizDouble _h;
		MatrizDouble _s;
		MatrizDouble _i;
	};
}
#endif // !_IMAGEN_HSI_H_
