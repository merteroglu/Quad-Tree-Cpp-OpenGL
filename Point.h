/*
2 boyutlu düzlemde ki noktalarýn classý
*/

#pragma once

class point {

public:

	long double x;  // Noktanýn x koordinatý
	long double y;  // Noktanýn y koordinatý

	point() { // Constructor parametresiz oluþturulursa 0 ata
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

	// iki noktanýn eþitliðini karþýlaþtýrmak için
	bool operator == (point v) {
		return ((x == v.x) && (y == v.y));
	}

	// Noktanýn koordinatlarýný daha sonra deðiþtirmek için
	void operator =(point v) {
		x = v.x;
		y = v.y;
	}

};
