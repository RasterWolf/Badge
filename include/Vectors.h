#pragma once

template<typename t>
inline bool VectorElementCompare(const t& a, const t& b)
{
	return a == b;
}

template<>
inline bool VectorElementCompare<float>(const float& a, const float& b)
{
	return abs(a-b) < 0.0001;
}

template<typename type>
class Vector2D
{
public:
	type X;
	type Y;

	Vector2D(type _x, type _y)
		: X(_x)
		, Y(_y)
	{
		;
	}

	Vector2D() : Vector2D(type(0), type(0))
	{
		;
	}

	bool operator==(const Vector2D& other) const
	{
		return VectorElementCompare<type>(X, other.X) && VectorElementCompare(Y, other.Y);
	}

	bool operator!=(const Vector2D& other) const
	{
		return !(*this == other);
	}
};

typedef Vector2D<float> FVector2D;
typedef Vector2D<int> IVector2D;

template<typename type>
class Vector3D 
{
public:
	type X;
	type Y;
	type Z;

	Vector3D(type _X, type _Y, type _Z) :
		X(_X)
		, Y(_Y)
		, Z(_Z)
	{
		;
	}

	Vector3D() :
		 X(type(0))
		, Y(type(0))
		, Z(type(0))
	{
		;
	}

	Vector3D<type> operator-(const Vector3D<type>& other) const
	{
		return Vector3D<type>(X - other.X, Y - other.Y, Z - other.Z);
	}

	void operator-=(const Vector3D<type>& other)
	{
		return *this = *this - other;
	}

	bool operator==(const Vector3D<type>& other) const
	{
		return VectorElementCompare<type>(X, other.X) && VectorElementCompare(Y, other.Y) && VectorElementCompare(Z, other.Z);
	}

	bool operator!=(const Vector3D<type>& other) const
	{
		return !(*this == other);
	}

	void operator=(const Vector3D<type>& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}

};

typedef Vector3D<float> FVector3D;
typedef Vector3D<int> IVector3D;
