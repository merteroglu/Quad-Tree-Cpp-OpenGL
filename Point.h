/*
2 boyutlu d�zlemde ki noktalar�n class�
*/

#pragma once

class point {

public:

	long double x;  // Noktan�n x koordinat�
	long double y;  // Noktan�n y koordinat�

	point() { // Constructor parametresiz olu�turulursa 0 ata
		x = 0;
		y = 0;
	}

	// Noktaya degerleri ata
	point(long double yeniX, long double yeniY) {
		x = yeniX;
		y = yeniY;
	}

	~point()
	{

	}

	// iki noktan�n e�itli�ini kar��la�t�rmak i�in
	bool operator == (point v) {
		return ((x == v.x) && (y == v.y));
	}

	// Noktan�n koordinatlar�n� daha sonra de�i�tirmek i�in
	void operator =(point v) {
		x = v.x;
		y = v.y;
	}

};
