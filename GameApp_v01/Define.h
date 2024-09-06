#pragma once
#include <directxtk/simplemath.h>
using namespace DirectX::SimpleMath;

// ���� ����.
struct Vertex
{
	Vector3 position;		// ���� ��ġ ����.
	Vector4 color;			// ���� ���� ����.

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	Vertex(Vector3 position, Vector4 color)
		: position(position), color(color) { }
};
