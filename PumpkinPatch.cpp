#include "stdafx.h"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObjects.h"

//==================== Game Class ==================//
class Game
{
public:
	Game();
	void InitTextures();
	bool collision(sf::RectangleShape* obj1, sf::RectangleShape* obj2);
	void Update(sf::RenderWindow* window, sf::Time dt);
	void Draw(sf::RenderWindow* window);
	void Exit();

private:
	std::vector <GameObjects*> gameManager;
	sf::Texture* m_pumpkinTexture;
};


Game::Game()
{
	InitTextures();

	gameManager.push_back(new Player);
	gameManager.push_back(new Crop(*m_pumpkinTexture, Pumpkin, sf::Vector2f(100, 300)));
	gameManager.push_back(new Crop(*m_pumpkinTexture, Pumpkin, sf::Vector2f(250, 100)));
	gameManager.push_back(new Crop(*m_pumpkinTexture, Yam, sf::Vector2f(650, 45)));
	gameManager.push_back(new Tool(Sickle));
	gameManager.push_back(new TurkeyUnit(sf::Vector2f(600, 350)));
}

void Game::InitTextures()
{
	m_pumpkinTexture = new sf::Texture;

	m_pumpkinTexture->loadFromFile("RU4By.png", sf::IntRect(0, 0, 100, 100));
}

bool Game::collision(sf::RectangleShape* obj1, sf::RectangleShape* obj2)
{

	return obj1->getGlobalBounds().intersects(obj2->getGlobalBounds());
}

void Game::Update(sf::RenderWindow* window, sf::Time dt)
{
	float dx;
	float dy;
	float dist;

	for (int i = 0; i < gameManager.size(); i++)
	{
		gameManager[i]->Update(window, dt);

		if (gameManager[i]->objectType == PlayerObj)
		{
			for (int j = 0; j < gameManager.size(); j++)
			{
				if (gameManager[j]->objectType != PlayerObj && collision(gameManager[i]->m_object, gameManager[j]->m_object))     // Player collissions with other game objects
				{
					gameManager[i]->colliding = true;
					if (gameManager[j]->objectType == ToolObj)
					{
						gameManager[j]->pickedUp = true;
						gameManager[i]->toolPickedUp = true;
						gameManager[i]->hasTool = true;
						gameManager[i]->toolInInventory = gameManager[j]->toolInInventory;
						//gameManager.erase(gameManager.begin() + j);
					}

					// ADD HERE any more desired collision with player
					
				}

				if (gameManager[j]->objectType == CropObj && gameManager[i]->hasTool)               // Handling of player interaction with crops after player acquires tool for harvesting
				{
					// TODO: Make vector for invectory storage. Make bool to destroy crop when hit with tool. GIVE CROPS HEALTH SO TAKES MORE THAN ONE HIT
					dx = abs(gameManager[i]->m_object->getPosition().x - gameManager[j]->m_object->getPosition().x);
					dy = abs(gameManager[i]->m_object->getPosition().y - gameManager[j]->m_object->getPosition().y);
					dist = sqrtf((dx * dx) + (dy * dy));
					//printf("\ndist: %f", dist);
					if (dist <= gameManager[j]->m_object->getSize().x + gameManager[i]->m_object->getSize().x + 7 && dist <= gameManager[j]->m_object->getSize().y + gameManager[i]->m_object->getSize().y + 7)
					{
						// TODO: Make Collider box for crops to make this easier
						printf("\nIN RANGE");
						//gameManager[i]->inRange = true;
						gameManager[j]->pickAlert = true;
					}
					else
					{
						//gameManager[i]->inRange = false;
						gameManager[j]->pickAlert = false;
					}

					// Harvesting of crops once player has acquired appropriate tool
					// ONLY DESTROYS CROPS IN REVERSE ORDER
					if (gameManager[i]->useTool && gameManager[j]->pickAlert)
					{
						printf("\nTOOL USED");
						// TODO: Any possible differences in harvesting for crops
						gameManager[j]->m_feastItem.health -= 5;
						if (gameManager[j]->m_feastItem.health <= 0)
						{
							// Use these variables to manipulate inventory
							gameManager[j]->pickedUp = true;
							gameManager[i]->cropPicked = true;
							gameManager[i]->m_feastItem = gameManager[j]->m_feastItem;
							//gameManager.erase(gameManager.begin() + j);
						}

						gameManager[i]->useTool = false;
					}
				}
				//else
				//{
				//	gameManager[i]->colliding = false;
				//}
			}
		}

		for (int i = 0; i < gameManager.size(); i++)
		{
			if (gameManager[i]->pickedUp)
			{
				gameManager.erase(gameManager.begin() + i);
				i--;
				printf("\nGame Object Erased.");
			}
		}
	}
}

void Game::Draw(sf::RenderWindow* window)
{
	for (int i = 0; i < gameManager.size(); i++)
	{
		gameManager[i]->Draw(window);
	}
}

void Game::Exit()
{
	for (int i = 0; i < gameManager.size(); i++)
	{
		delete gameManager[i];
	}
}

// TODO: Build and test enemy class (turkey, crow, gopher, etc).  


int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window(sf::VideoMode(1280, 840), "The Perfect Pumpkin");
	sf::Clock clock;
	sf::Clock time;
	sf::Time timer;
	Game myGame;


	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		window.clear();
		sf::Event event;
		// Check to see if close-window button has been pushed
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		myGame.Update(&window, dt);
		myGame.Draw(&window);

		window.display();

	}

	myGame.Exit();
	
	return 0;
}