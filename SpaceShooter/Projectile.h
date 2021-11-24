#include "Main.h"
#include <thread>

class Projectile
{
public:
	const char bullet = '*';

	void Projectile_Spawn(Vector2 spawn_coordinates);

	void Projectile_Move();

	bool Projectile_Is_Collide();

	bool Projectile_Is_End_Of_Field();

	Vector2 coordinates;

	const Vector2 move_vector = Vector2(1, 0);

	void Destroy();
};
