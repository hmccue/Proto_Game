
#define OLC_PGE_APPLICATION
#define OLC_PGEX_ANIMSPR 
#include "olcPixelGameEngine.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <dos.h>
#include <string>
#include "olcPGEX_AnimatedSprite.h"

// THis will replace the main, adding structure to the game 
using namespace std;

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Proto Game";
	}
	struct shape
	{
		float x;
		float y;
		float dx;
		float dy;
		float angle;
		int nSize;
	};
	struct stats
	{
		int health;
		int points;
	};

	// intializing vectors: bullets, minions, tiles? 
	vector<shape> bullet;
	vector<shape> minion;
	vector<shape> tiles;
	// initalizing main characters 
	shape keith;
	shape player;
	// main character stats
	stats keithstat;
	stats playerstat;
	// Sprite stuff 
	olc::Sprite* sprPlayer = new olc::Sprite("./Gnome2.png");
	//olc::Decal* playerDecal = new olc::Decal(sprPlayer)
	olc::Sprite* sprMinion = new olc::Sprite("./monster.png");
	olc::Sprite* sprKeith = new olc::Sprite("./Keith.png");
	olc::Sprite* sprMap = new olc::Sprite("./map2.png"); // changed the map here 
	olc::Sprite* sprBrick = new olc::Sprite("./brick.png");

	int mode;

public:

	bool OnUserCreate() override
	{
		// Clean screen 
		Clear(olc::BLANK); 
		// set up start screen here??? Idea play, leave, tutorial Cases????  
		// initial plaayer spot 
		player.x = 150.0f; player.y = 120.0f; player.dx = 0.0f; player.dy = 0.0f; player.nSize = 16;
		keith.x = 200.0f; keith.y = 200.0f; keith.dx = 10.0f; keith.dy =10.0f; keith.nSize = 32;
		// intializing 2 minions
		minion.push_back({ 130.0f,100.0f,45.0f,30.0f,0.0f,(int)16 });
		minion.push_back({ 120.0f,120.0f,-15.0f,28.0f,0.0f,(int)16 });
		minion.push_back({ 150.0f,20.0f,10.0f,-14.0f,0.0f,(int)16 });
		// initial tiles 
		tiles.push_back({ 0.0f,0.0f,0.0f,0.0f,0.0f,int(4) });
		// initalizing screen set up
		int nHeight = 200;
		int nWidth = 200;
		SetScreenSize(nHeight, nWidth);
		// initializing the player statitics
		keithstat.health = 50; 
		playerstat.health = 100;
		playerstat.points = 0; 
		mode = 3;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Set up start screen, use switch cases to decide if it tutorial, case one: start screen case 2: game case 3: keiths rooms?? 
		Clear(olc::BLANK);
		float speed = 75.0f;
		// set up start screen here 
		// enter to start game 
		// escape to leave

		// Draw map 
		switch (mode) {
		case 3: {
			std::stringstream start;
			start << "START GAME ";
			DrawString(20, 80, start.str(), olc::CYAN);
			start.str("Options: ");
			DrawString(20, 90, start.str(), olc::YELLOW);
			start.str("Press enter to play");
			DrawString(20, 100, start.str(), olc::YELLOW);
			start.str("Esc. to exit");
			DrawString(20, 110, start.str(), olc::GREEN);
			start.str("Space for instructions");
			DrawString(20, 120, start.str(), olc::GREEN);
			DrawPartialSprite(170, 170, sprKeith, 0, 0, 32, 32);
			DrawPartialSprite(10, 10, sprPlayer, 0, 0, 16, 16);
			if (GetKey(olc::Key::ENTER).bHeld) { mode = 1; } // has to re update every on user update, find a way for it to stay persistent 
			if (GetKey(olc::Key::SPACE).bHeld) { mode = 2; }
			break;
		}
		case 1: {
			DrawSprite(0, 0, sprMap);
			// player movements 
			if (GetKey(olc::Key::LEFT).bHeld) { player.angle = 4.71f; player.dx = speed; player.dy = speed; }
			if (GetKey(olc::Key::RIGHT).bHeld) { player.angle = 1.57f; player.dx = speed; player.dy = speed; }
			if (GetKey(olc::Key::UP).bHeld) { player.angle = 3.14f; player.dx = speed; player.dy = speed; }
			if (GetKey(olc::Key::DOWN).bHeld) { player.angle = 0.0f; player.dx = speed; player.dy = speed; }
			if (GetKey(olc::Key::UP).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
			if (GetKey(olc::Key::DOWN).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
			if (GetKey(olc::Key::LEFT).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
			if (GetKey(olc::Key::RIGHT).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
			player.x += player.dx * sin(player.angle) * fElapsedTime;
			player.y += player.dy * cos(player.angle) * fElapsedTime;
			// keeping player in the bounds of the screen
			if (player.x > 182 || player.x < 0) { player.x = player.x - player.dx * sin(player.angle) * fElapsedTime; }
			if (player.y > 182 || player.y < 0) { player.y = player.y - player.dy * cos(player.angle) * fElapsedTime; }
			// random generation of minion movement


			// drawing the 2 minions 
			for (auto& m : minion)
			{
				m.x += m.dx * fElapsedTime;
				m.y += m.dy * fElapsedTime;
				m.nSize = 4;
				//DrawCircle(m.x, m.y, m.nSize, olc::DARK_CYAN);
				// minion sprite 
				DrawSprite(m.x, m.y, sprMinion);

				// minion bouncing off the walls 
				if (m.x > 192 || m.x < 0) { m.dx = -1 * m.dx; } // vertical wall 
				if (m.y > 192 || m.y < 0) { m.dy = -1 * m.dy; } // horizantal wall

				// minion collision with Player , right now just bounces off 
				if (IsPointInsideRect(player.x, player.y, player.x + 16, player.y + 16, m.x, m.y))
				{
					m.dx = -1 * m.dx;
					m.dy = -1 * m.dy;
					playerstat.health = playerstat.health - 1;
				}
			}

			// drawing bullets 
			if (GetKey(olc::Key::SPACE).bPressed) { bullet.push_back({ player.x + 8,player.y + 8, 100.0f * sin(player.angle),100.0f * cos(player.angle), 0.0f, (int)4 }); }
			for (auto& b : bullet)
			{
				b.x += b.dx * fElapsedTime;
				b.y += b.dy * fElapsedTime;
				DrawCircle(b.x, b.y, 2, olc::WHITE);
				// checking collision with minions 
				for (auto& m : minion)
				{
					if (IsPointInsideCircle(m.x, m.y, m.nSize, b.x, b.y))
					{
						//remove bullet 
						b.x = -100; b.dx = 0; b.dy = 0;
						// remove minion 
						m.y = -100; m.dy = 0; m.dx = 0;
						playerstat.points += 1;
					}
				}
				// collision with keith 
				if (IsPointInsideRect(keith.x, keith.y, keith.x + 32, keith.y + 32, b.x, b.y))
				{
					//remove bullet 
					b.x = -100; b.dx = 0; b.dy = 0;
					keithstat.health = keithstat.health - 1;
					playerstat.points += 1;

				}
			}

			// placing tiles
			for (auto& t : tiles)
			{
				// drawing initial targets 
				for (int k = 0; k < 80; k = k + 10) {
					t.x = (float)k;
					t.y = 50;
					DrawSprite(t.x, t.y, sprBrick);
					// adding another tile set 

					// Check player collision with tiles 
					if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y + player.nSize) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y + player.nSize)) // fix for all sides of circles?? 
					{
						player.x = player.x - player.dx * sin(player.angle) * fElapsedTime;
						player.y = player.y - player.dy * cos(player.angle) * fElapsedTime;
					}
					// bullet collision with tile 
					for (auto& b : bullet)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, b.x, b.y))
						{
							b.dx = -1 * b.dx;
							b.dy = -1 * b.dy;
						}
					}
					// minion collision with tile 
					for (auto& m : minion)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, m.x, m.y))
						{
							m.dx = -1 * m.dx;
							m.dy = -1 * m.dy;
						}
					}
				}
				// New set of tiles/wall 
				for (int k = 0; k < 80; k = k + 10) {
					t.x = (float)k + 50;
					t.y = 100;
					//DrawRect(t.x, t.y, 10, 10, olc::WHITE);
					DrawSprite(t.x, t.y, sprBrick);
					// adding another tile set 

					// Check player collision with tiles 
					if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y + player.nSize) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y + player.nSize)) // fix for all sides of circles?? 
					{
						player.x = player.x - player.dx * sin(player.angle) * fElapsedTime;
						player.y = player.y - player.dy * cos(player.angle) * fElapsedTime;
					}
					// bullet collision with tile 
					for (auto& b : bullet)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, b.x, b.y))
						{
							b.dx = -1 * b.dx;
							b.dy = -1 * b.dy;
						}
					}
					// minion collision with tile 
					for (auto& m : minion)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, m.x, m.y))
						{
							m.dx = -1 * m.dx;
							m.dy = -1 * m.dy;
						}
					}
				}
				for (int k = 0; k < 80; k = k + 10) {
					t.x = (float)k + 10;
					t.y = 160;
					//DrawRect(t.x, t.y, 10, 10, olc::WHITE);
					DrawSprite(t.x, t.y, sprBrick);
					// adding another tile set 

					// Check player collision with tiles 
					if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y + player.nSize) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y + player.nSize)) // fix for all sides of circles?? 
					{
						player.x = player.x - player.dx * sin(player.angle) * fElapsedTime;
						player.y = player.y - player.dy * cos(player.angle) * fElapsedTime;
					}
					// bullet collision with tile 
					for (auto& b : bullet)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, b.x, b.y))
						{
							b.dx = -1 * b.dx;
							b.dy = -1 * b.dy;
						}
					}
					// minion collision with tile 
					for (auto& m : minion)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, m.x, m.y))
						{
							m.dx = -1 * m.dx;
							m.dy = -1 * m.dy;
						}
					}
				}
				for (int k = 0; k < 80; k = k + 10) {
					t.x = (float)k + 150;
					t.y = 180;
					//DrawRect(t.x, t.y, 10, 10, olc::WHITE);
					DrawSprite(t.x, t.y, sprBrick);
					// adding another tile set 

					// Check player collision with tiles 
					if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y + player.nSize) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x + player.nSize, player.y) || IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, player.x, player.y + player.nSize)) // fix for all sides of circles?? 
					{
						player.x = player.x - player.dx * sin(player.angle) * fElapsedTime;
						player.y = player.y - player.dy * cos(player.angle) * fElapsedTime;
					}
					// bullet collision with tile 
					for (auto& b : bullet)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, b.x, b.y))
						{
							b.dx = -1 * b.dx;
							b.dy = -1 * b.dy;
						}
					}
					// minion collision with tile 
					for (auto& m : minion)
					{
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, m.x, m.y))
						{
							m.dx = -1 * m.dx;
							m.dy = -1 * m.dy;
						}
					}
				}
			}



			// Keiths movement when player gets all the minions 
			if (playerstat.points > 2) {
				keith.angle = (player.y - keith.y) / (player.x - keith.x);
				if (player.x > keith.x)
				{
					// player collision with Keith 
					if (IsPointInsideRect(keith.x, keith.y, keith.x + 32, keith.y + 32, player.x, player.y))
					{
						playerstat.health = playerstat.health - 1;
					};
					keith.x += keith.dx * cos(keith.angle) * fElapsedTime;
					keith.y += keith.dy * sin(keith.angle) * fElapsedTime;
				}
				else
				{
					// collision with player 
					if (IsPointInsideRect(player.x, player.y, player.x + 16, player.y + 16, keith.x, keith.y))
					{
						playerstat.health = playerstat.health - 1;
					};
					keith.x += -1 * keith.dx * cos(keith.angle) * fElapsedTime;
					keith.y += -1 * keith.dy * sin(keith.angle) * fElapsedTime;
				}

				// ADD keith collision and health tracking 
			}

			// draw player
			// DrawCircle(player.x, player.y, player.nSize, olc::WHITE);
			DrawPartialSprite(player.x, player.y, sprPlayer, 0, 0, 16, 16);

			// Drawing keith if comes to that point 
			if (playerstat.points > 2) {
				DrawPartialSprite(keith.x, keith.y, sprKeith, 0, 0, 32, 32);
				// drawing keiths helath bar 
				FillRect(168, 25, keithstat.health / 2, 5, olc::RED);
			}

			// score and health box 
			DrawRect(165, 8, 24, 15, olc::WHITE);
			// update health and score stats for player and keith 
			std::stringstream ss;
			ss << playerstat.health;
			if (playerstat.health >=50){ FillRect(168, 10, playerstat.health / 5, 5, olc::GREEN); }
			if (playerstat.health < 50) { FillRect(168, 10, playerstat.health / 5, 5, olc::RED); }
			// DrawString(170, 10, ss.str(), olc::WHITE, 1);
			std::stringstream sc;
			sc << playerstat.points;
			DrawString(168, 15, sc.str(), olc::WHITE);

			if (playerstat.health <= 0) {
				// Clean screen and display results does not work RN 
				Clear(olc::BLACK);
				std::stringstream game;
				game << "GAME OVER";
				DrawString(100, 100, game.str(), olc::WHITE);
			}

			if (keithstat.health <= 0) {
				Clear(olc::BLACK);
				std::stringstream game;
				game << "You WIN!! ";
				DrawString(100, 100, game.str(), olc::WHITE);
			}
			// escape for game exit 
			if (GetKey(olc::Key::ESCAPE).bHeld) {
				return false;
			}
			break;
		}
			// return true;
		case 2: 
		{
			Clear(olc::BLACK);
			std::stringstream game;
			game << "instructions:  ";
			DrawString(30, 80, game.str(), olc::DARK_YELLOW);
			game.str("Use arrows to move");
			DrawString(30, 90, game.str(), olc::WHITE);
			game.str("Space to shoot");
			DrawString(30, 100, game.str(), olc::WHITE);
			game.str("Beat all minions");
			DrawString(30, 110, game.str(), olc::WHITE);
			game.str("esc. to exit");
			DrawString(30, 120, game.str(), olc::DARK_RED);
			game.str("enter to play!");
			DrawString(30, 130, game.str(), olc::DARK_GREEN);
			if (GetKey(olc::Key::ENTER).bHeld) { mode = 1; }
			if (GetKey(olc::Key::ESCAPE).bHeld) { return false; }
			break;
		}

		} // parenthese for switch 
		return true;
	}

	bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
	{
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
	}

	bool IsPointInsideRect(float x, float y, float xplus, float yplus, float xcheck, float ycheck)
	{
		return (xcheck > x && xcheck <xplus && ycheck>y && ycheck < yplus);
	}
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4,false, true))
		demo.Start();
	return 0;
}