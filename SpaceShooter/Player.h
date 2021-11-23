#include <vector>
#include "Main.h"
class Player
{
private:
	std::vector<std::vector<char>> player_ship = { {'@','@',' '},
	{ '@','@','@'}, {'@','@',' '} };

	void Move();

public:

	void Shoot();

	void Spawn(int x, int y);

};
