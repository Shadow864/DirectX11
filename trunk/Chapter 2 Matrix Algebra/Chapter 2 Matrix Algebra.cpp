// Chapter 2 Matrix Algebra.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <DirectXMath.h>

using namespace DirectX;

std::ostream& operator<<(std::ostream& out, CXMMATRIX matrix)
{

	XMFLOAT4X4 dest;

	XMStoreFloat4x4(&dest, matrix);

	for (auto row = 0; row < 4; ++row)
	{
		for (auto column = 0; column < 4; ++column)
		{
			out << dest(row, column) << "\t";
		}

		out << std::endl;
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, FXMVECTOR vector)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, vector);

	out << dest.x << " " << dest.y << " " << dest.z << " " << dest.w << std::endl;
	return out;
}

int _tmain(int argc, _TCHAR* argv[])
{
	XMMATRIX A(	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 4.0f, 0.0f,
				1.0f, 2.0f, 3.0f, 1.0f);

	XMMATRIX B = XMMatrixIdentity();


	XMMATRIX C = A * B;

	XMMATRIX D = XMMatrixTranspose(A);

	XMVECTOR det  = XMMatrixDeterminant(A);

	XMMATRIX E = XMMatrixInverse(&det, A);

	XMMATRIX F = A * E;


	std::cout << "A = " << std::endl << A << std::endl;
	std::cout << "B = " << std::endl << B << std::endl;
	std::cout << "C = A*B = " << std::endl << C << std::endl;
	std::cout << "D = transpose(A) = " << std::endl << D << std::endl;
	std::cout << "det = determinant(A) = " << det << std::endl << std::endl;
	std::cout << "E = inverse(A) = " << std::endl << E << std::endl;
	std::cout << "F = A*E = " << std::endl << F << std::endl;
	

	char c; 

	std::cin >> c;

	return 0;
}

