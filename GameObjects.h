#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


//====================== GameObjects Class =====================//
enum Type{ PlayerObj, CropObj, ToolObj, AnimalObj };

enum FoodType { Pumpkin, Corn, Yam, Turkey, NoFood };

enum ToolType { Sickle, Trowel, Hatchet, NoTool };

struct InventoryItem
{
	Type item;
	FoodType food;
	ToolType tool;
	bool acceptable;
};

struct Food
{
	FoodType edibleType;
	int health;
	sf::Texture* foodTexture;
};

class GameObjects
{
public:
	GameObjects();
	~GameObjects();
	//float gravity(sf::RectangleShape* obj, sf::Time dt);
	virtual void Update(sf::RenderWindow* window, sf::Time dt);
	virtual void Draw(sf::RenderWindow* window);
	bool colliding;
	bool pickedUp;
	bool toolPickedUp;
	bool hasTool;
	bool inRange;
	bool useTool;
	bool cropPicked;
	bool pickAlert;
	Type objectType;
	Food m_feastItem;
	ToolType toolInInventory;
	sf::RectangleShape* m_object;
protected:

};


//===================== Player Class ======================//
#define ACCEL 200
#define MAX_VELOCITY 500
#define INVENTORY_SIZE 7
#define TOOL_INVENTORY_SIZE 3

class Player : public GameObjects
{
public:
	Player();
	void keyUp();
	void keyDown();
	void Update(sf::RenderWindow* window, sf::Time dt);
	void Draw(sf::RenderWindow* window);

private:
	sf::RectangleShape* m_player;
	sf::Vector2f m_playerPos;
	//sf::Vector2f m_vel;
	sf::Clock m_Time;
	sf::Time m_Timer;
	InventoryItem m_Inventory[INVENTORY_SIZE];
	//InventoryItem m_ToolInventory[TOOL_INVENTORY_SIZE];
	bool mRight;
	bool mLeft;
	bool mUp;
	bool mDown;
	bool mSpaceReset;
	int m_inventoryCount;
	int m_toolCount;
	float m_vel;
};


//===================== Crop Class ======================//

class Crop : public GameObjects
{
public:
	Crop(sf::Texture& texture, FoodType edibleType, sf::Vector2f pos);
	void DestroyCrop();
	void Update();
	void Draw(sf::RenderWindow* window);
	
private:
	sf::RectangleShape* m_foodItem;
};


//====================== Tool Class ==================//

class Tool : public GameObjects
{
public:
	Tool(ToolType thisTool);
	void DestroyTool();
	void Update(sf::RenderWindow* window, sf::Time dt);
	void Draw(sf::RenderWindow* window);
	ToolType tool;
private:
	sf::RectangleShape* m_tool;
	sf::Texture* m_toolTexture;
};


//====================== Animal Base Class =====================//
class Animal : public GameObjects
{
public:
	Animal();
	virtual void Update(sf::RenderWindow* window, sf::Time dt);
	virtual void Draw(sf::RenderWindow* window);
	sf::RectangleShape* m_animalObject;
private:

};

//===================== Turkey Class ===================//
class TurkeyUnit : public Animal
{
public:
	TurkeyUnit(sf::Vector2f pos);
	void Update(sf::RenderWindow* window, sf::Time dt);
	void Draw(sf::RenderWindow* window);
private:
	sf::RectangleShape* m_animal;
	sf::Vector2f m_animalPos;
	sf::Clock m_Time;
	sf::Time m_Timer;
	float m_vel;
};