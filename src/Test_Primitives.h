#pragma once

float triangle[] = {
	-0.5f, -0.5f, 0.0f,
	0.0f, 0.5, 0.0f,
	0.5f, -0.5f, 0.0f
};

float twoTriangles[] = {
	-0.5f, -0.5f, 0.0f,
	0.0f, 0.5, 0.0f,
	0.5f, -0.5f, 0.0f,

	-1.0f, -0.5f, 0.0f,
	-0.5f, 0.5, 0.0f,
	0.0f, -0.5f, 0.0f
};

float square[] = {
	// x, y, z, r, g, b, u, v
	-1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
	 1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
	 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
	-1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f,0.0f,0.0f,0.0f,0.0f,0.0f
};

float cube[] = {
	// Face 1 ABCD
	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.66f, // A
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.66f,  // B
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,    // C
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,   // D

	// Face 2 BEFC
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.33f,  // B
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.33f, // E
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.66f,  // F
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.66f,   // C

	// Face 3 EGHF
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,  // E
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // G
	-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.33f, // H
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.33f,  // F

	// Face 4 GADH
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.5f, 0.33f,// G
	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.33f, // A
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.66f,  // D
	-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.5f, 0.66f, // H

	// Face 5 DCFH
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // D
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f,    // C
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.33f,  // F
	-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.33f, // H

	// Face 6 GEBA
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.5f, 0.66f,// G
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.66f, // E
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // B
	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f   // A
};

unsigned int square_index[] = {
	0, 1, 2,
	3, 2, 0
};

unsigned int cube_index[] = {
	0, 1, 2,
	3, 2, 0,

	4, 5, 6,
	7, 6, 4,

	8, 9, 10,
	11, 10, 8,

	12, 13, 14,
	15, 14, 12,

	16, 17, 18,
	19, 18, 16,

	20, 21, 22,
	23, 22, 20
};