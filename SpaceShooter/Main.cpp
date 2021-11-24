#include "Player.h"
#include "Enemy.h"
#include "Main.h"
#include "Projectile.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <conio.h>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <iterator>
#include <list>
#include <time.h>

Player* player;

std::list<Projectile*> projectiles;
std::vector<std::vector<char>> main_field;
Vector2 player_coordinates;
Vector2 gun_relative_coordinates = Vector2(3, 1);
std::queue<Projectile> projectile_queue;

std::mutex mtx;

static int moveX[] = { 0, 0, 0, 1, 1, 1, -1, -1, -1 };
static int moveY[] = { 0, 1, -1, 0, 1, -1, 1, 0, -1 };
Vector2 moveVec[8];
static int check_collide[] = { 0, 1, -1 };

int count = 0;
int Life = 5;


bool isGameOver = false;
void Draw_Start_Field()
{
	player = new Player();

	for (int i = 0; i < 8; i++)
	{
		moveVec[i] = Vector2(moveX[i], moveY[i]);
	}

	main_field.resize(HEIGHT);

	for (int i = 0; i < HEIGHT; i++)
	{

		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0 || i == HEIGHT - 1)
				main_field[i].push_back('-');
			else
				main_field[i].push_back(' ');

		}
	}
}

void Re_Draw_Field()
{
	mtx.lock();
	system("cls");
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printf("%c", main_field[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("Lives: %d\n", Life);
	mtx.unlock();
}

int main()
{

	gh:
	Draw_Start_Field();

	//std::thread player_thread(&Player::Spawn, player,4,13);
	//player_thread.detach();
	player->Spawn(4, 13);
	std::srand(time(NULL));
	while (isGameOver == false)
	{

	
	//while (isGameOver == false)
	//{

		Enemy* enemy = new Enemy();
		int yEnemyCoord = std::rand() % HEIGHT;
		if (yEnemyCoord == 0)
		{
			yEnemyCoord = 1;
		}
		else
		{
			if (yEnemyCoord == HEIGHT - 1 || yEnemyCoord == HEIGHT - 2 || yEnemyCoord == HEIGHT - 3)
			{
				yEnemyCoord = HEIGHT - 4;
			}
		}

		//std::thread enemy_thread(&Enemy::Enemy_Spawn, enemy, Vector2(WIDTH - 4, yEnemyCoord));
		//enemy_thread.detach();
		enemy->Enemy_Spawn(Vector2(WIDTH - 4, yEnemyCoord));
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	//	

		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	
	printf("\n ====== GameOver ======\n");
	printf("try retry press 1");
	int f;
	scanf_s("%d", &f);

	//delete player;

	if (f == 1)
	{
		Life = 0;
		isGameOver = false;
		main_field.clear();
		goto gh;
	}
	//enemy->Enemy_Spawn(Vector2(67, 13));
	//playerThread.join();
	exit(0);
	return 0;
}

void Projectile::Projectile_Spawn(Vector2 spawn_coordinates)
{
	main_field[spawn_coordinates.Y][spawn_coordinates.X] = bullet;
	coordinates = spawn_coordinates;
	//std::thread a(&Projectile::Projectile_Move, this);
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//Sleep(DEFDELAY);
	Projectile_Move();
	
	//a.join();
}


bool Projectile::Projectile_Is_Collide()
{
	return main_field[coordinates.Y + move_vector.Y][coordinates.X + move_vector.X] == '#';
}

bool Projectile::Projectile_Is_End_Of_Field()
{
	return coordinates.X + move_vector.X == WIDTH - 1;
}

void Projectile::Projectile_Move()
{
	if (isGameOver == false)
		if (Projectile_Is_Collide() || Projectile_Is_End_Of_Field())
		{
			main_field[coordinates.Y][coordinates.X] = ' ';
			Re_Draw_Field();

			delete this;
			player->Shoot();
		}
		else
		{
			main_field[coordinates.Y + move_vector.Y][coordinates.X + move_vector.X] = bullet;
			main_field[coordinates.Y][coordinates.X] = ' ';
			coordinates.Y += move_vector.Y;
			coordinates.X += move_vector.X;
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			Re_Draw_Field();
			Projectile_Move();
		}

}



void Player::Spawn(int x, int y)
{

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			main_field[y + i][x + j] = player_ship[i][j];

	player_coordinates = Vector2(x, y);

	Re_Draw_Field();
	std::thread playerThread(&Player::Shoot, player);
	//Shoot();
	playerThread.detach();
	std::thread player_Thread(&Player::Move, player);
	player_Thread.detach();
	//Move();
	
}


void Player::Shoot()
{

	Projectile* new_projectile = new Projectile();
	//projectiles.push_back(new_projectile);
	std::thread sb(&Projectile::Projectile_Spawn, new_projectile, player_coordinates + gun_relative_coordinates);
	//new_projectile->Projectile_Spawn(player_coordinates + gun_relative_coordinates);
	//Re_Draw_Field();


	sb.join();

}

void Player::Move()
{
	if (isGameOver == false)
	{
		char input = _getch();
		char arr[4] = { 'w','a','s','d' };
		Vector2 arr2[4] = { Vector2(0,-1), Vector2(-1,0), Vector2(0,1), Vector2(1,0) };
		for (int i = 0; i < 4; i++)
		{
			if (input == arr[i])
			{
				Vector2 Next = player_coordinates + arr2[i];
				if (Next.X >= 0 && Next.X < WIDTH - 2 && main_field[Next.Y][Next.X] != '-' && main_field[Next.Y + 2][Next.X] != '-')
				{

					for (int g = 0; g < 3; g++)
						for (int k = 0; k < 3; k++)
						{
							main_field[player_coordinates.Y + g][player_coordinates.X + k] = ' ';
						}

					player_coordinates.X += arr2[i].X;
					player_coordinates.Y += arr2[i].Y;

					for (int g = 0; g < 3; g++)
						for (int k = 0; k < 3; k++)
						{
							main_field[Next.Y + g][Next.X + k] = player_ship[g][k];
						}
				}
			}
		}



		//Re_Draw_Field();

		Move();
	}

}

void Enemy::Enemy_Move()
{
	if(isGameOver == false)
	if (Enemy_Is_Collide() || Enemy_Is_End_Field())
	{
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				main_field[enemy_coordinates.Y + j][enemy_coordinates.X + k] = ' ';

		//Re_Draw_Field();

		delete this;
	}
	else
	{
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				main_field[enemy_coordinates.Y + j][enemy_coordinates.X + k] = ' ';

		enemy_coordinates.Y += move_vector.Y;
		enemy_coordinates.X += move_vector.X;

		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				main_field[enemy_coordinates.Y + j][enemy_coordinates.X + k] = enemy_ship[j][k];

		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		//Re_Draw_Field();
		Enemy_Move();

	}
}


bool Enemy::Enemy_Is_End_Field()
{
	return enemy_coordinates.X - 1 == 0;
}

bool Enemy::Enemy_Is_Collide()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (main_field[enemy_coordinates.Y + j][enemy_coordinates.X - 1] == '*' ||
				main_field[enemy_coordinates.Y + j][enemy_coordinates.X] == '*')
			{
				return true;
			}
			else
			{
				if (main_field[enemy_coordinates.Y + j][enemy_coordinates.X - 1] == '@')
				{
					if (Life > 0)
						Life--;
					else
					{
						isGameOver = true;
					}

					return true;
				}
			}
		}
	return false;
}

void Enemy::Enemy_Spawn(Vector2 spawn_coordinates)
{
	enemy_coordinates = spawn_coordinates;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			main_field[spawn_coordinates.Y + i][spawn_coordinates.X + j] = enemy_ship[i][j];

	std::thread enemyThread(&Enemy::Enemy_Move, this);
	enemyThread.detach();
	//Enemy_Move();
}