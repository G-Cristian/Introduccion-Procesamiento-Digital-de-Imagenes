#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <vector>

using namespace std;

template<class T>
class Vector3 {
public:
	Vector3():Vector3<T>(T(), T(), T()) {

	}

	Vector3(T x, T y, T z) :_x(x), _y(y), _z(z) {

	}

	Vector3(const vector<T> &valores){
		assert(valores.size() == 3);

		_x = valores[0];
		_y = valores[1];
		_z = valores[2];
	}

	~Vector3() {}

	inline T X() const { return _x; }
	inline T Y() const { return _y; }
	inline T Z() const { return _z; }

private:
	T _x;
	T _y;
	T _z;
};

typedef Vector3<unsigned char> Vector3UChar;
typedef Vector3<double> Vector3Double;

#endif // !_VECTOR3_H_
