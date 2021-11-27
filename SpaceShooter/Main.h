#ifndef MAIN_H
#define MAIN_H
#include <vector>

#define WIDTH 70
#define HEIGHT 20
#define DEFDELAY 100
#define STARTLIFE 0


int g_Life = STARTLIFE;

struct Vector2
{
	int X, Y;

	Vector2()
	{

	}

	Vector2(int x, int y)
	{
		X = x;
		Y = y;
	}

	bool operator==(const Vector2& other)
	{
		return X == other.X && Y == other.Y;
	}

	Vector2 operator+=(const Vector2& other)
	{
		return Vector2(X + other.X, Y + other.Y);
	}

	Vector2 operator+(const Vector2& other)
	{
		return Vector2(X + other.X, Y + other.Y);
	}
};
#endif // !PLAYER_H