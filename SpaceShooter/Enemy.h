#include <vector>
#include "Main.h"

class Enemy
{
private:

	std::vector<std::vector<char>> enemy_ship = { {'#','#','#'},
	{ '#','#',' '}, {'#','#','#'} };

	Vector2 enemy_coordinates;
	const Vector2 move_vector = Vector2(-1, 0);

	bool Enemy_Is_End_Field();
	bool Enemy_Is_Collide();
	void Enemy_Move();

public:


	Enemy()
	{

	}

	Enemy(Vector2 spawn_coordinates)
	{
		enemy_coordinates = spawn_coordinates;
	}

	void Enemy_Spawn(Vector2 spawn_coordinates);
};
