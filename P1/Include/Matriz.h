#ifndef _MATRIZ_H_
#define _MATRIZ_H_

#include <vector>

using namespace std;

namespace IPDI {
	template <class T> class Matriz {
	public:
		Matriz(int altura, int ancho, T valorInicial = 0);
		~Matriz();

		inline int alto() const {
			return _alto;
		}

		inline int ancho() const {
			return _ancho;
		}

		const vector<T> &operator[](int indice) const;
		vector<T> &operator[](int indice);

		Matriz<T> sumaConSaturacion(const Matriz<T> &otra, T nivelDeSaturacionInferior, T nivelDeSaturacionSuperior) const;
		Matriz<T> restaConSaturacion(const Matriz<T> &otra, T nivelDeSaturacionInferior, T nivelDeSaturacionSuperior) const;
		Matriz<T> productoPorEscalarConSaturacion(double escalar, T nivelDeSaturacionInferior, T nivelDeSaturacionSuperior) const;
	private:
		vector<vector<T> > _matriz;
		int _alto;
		int _ancho;
	};

	typedef Matriz<char> MatrizChar;
}

#endif // !_MATRIZ_H_

