#include "stdafx.h"
#include "GameObjects.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//================== GameObjects Class ======================//
GameObjects::GameObjects()
{

}


GameObjects::~GameObjects()
{
}

//float GameObjects::gravity(sf::RectangleShape* obj, sf::Time dt)
//{
//	sf::Vector2f pos = obj->getPosition();
//	float time = dt.asSeconds();
//
//	gravm_vel += GRAVITY * time;
//
//	pos.y += gravm_vel * time;
//
//	return pos.y;
//}

void GameObjects::Update(sf::RenderWindow* window, sf::Time dt)
{

}

void GameObjects::Draw(sf::RenderWindow* window)
{

}

//======================= Player Class ======================//
Player::Player()
{
	objectType = PlayerObj;
	m_player = new sf::RectangleShape;
	m_object = m_player;
	colliding = false;
	hasTool = false;
	toolPickedUp = false;
	inRange = false;
	cropPicked = false;
	m_inventoryCount = 0;
	m_toolCount = 0;

	m_player->setPosition(sf::Vector2f(500, 500));
	m_player->setFillColor(sf::Color::Blue);
	m_player->setSize(sf::Vector2f(20, 20));
	m_player->setOrigin(10, 10);

	m_playerPos = m_player->getPosition();
}

void Player::keyUp()
{
	// Check to see if Left arrow key, Right arrow key, or Space bar are NOT being pressed
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		mLeft = false;
		m_vel = 0;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		mRight = false;
		m_vel = 0;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		mUp = false;
		m_vel = 0;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		mDown = false;
		m_vel = 0;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		useTool = false;
		mSpaceReset = true;
		//jumpReset = true;
		//spaceBar = false;
	}
}

void Player::keyDown()
{
	// Check to see if Left arrow key, Right arrow key, and Space bar ARE being pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		mLeft = true;
		//m_facingRight = false;
		//m_justReleasedLeft = false;
		//if (!m_facingLeft)
		//{
		//	mouseSprite->scale(-1, 1);
		//	m_facingLeft = true;
		//}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		mRight = true;
		//m_facingLeft = false;
		//m_justReleasedRight = false;
		//if (!m_facingRight)
		//{
		//	mouseSprite->scale(-1, 1);
		//	m_facingRight = true;
		//}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		mUp = true;
		//m_facingLeft = false;
		//m_justReleasedRight = false;
		//if (!m_facingRight)
		//{
		//	mouseSprite->scale(-1, 1);
		//	m_facingRight = true;
		//}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		mDown = true;
		//m_facingLeft = false;
		//m_justReleasedRight = false;
		//if (!m_facingRight)
		//{
		//	mouseSprite->scale(-1, 1);
		//	m_facingRight = true;
		//}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && hasTool /*&& inRange*/ && mSpaceReset)
	{
		useTool = true;
		mSpaceReset = false;
  		printf("\nUSING TOOL");
		// TODO: Activate SFX for tool use
	}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !mSpace && jumpReset)
	//{
	//	// Set values for bool variables used in jumping behavior
	//	spaceBar = true;
	//	mSpace = true;
	//	jumpReset = false;
	//	onPlatform = false;
	//	printf("\nSpace Bar Pressed");
	//}
}

void Player::Update(sf::RenderWindow* window, sf::Time dt)
{
	//m_playerPos = m_player->getPosition();
	sf::Vector2f prevPos = m_player->getPosition();
	float time = dt.asSeconds();
	int acceptableNum;
	//colliding = false;

	// Check for Keyboard events
	keyUp();
	keyDown();

	if (toolPickedUp)
	{
		// TODO: If hasTool is true, change texture to character with tool
		m_Inventory[m_inventoryCount].item = ToolObj;
		m_Inventory[m_inventoryCount].tool = toolInInventory;
		m_Inventory[m_inventoryCount].food = NoFood;
		m_inventoryCount++;
		toolPickedUp = false;
		printf("\nTool Picked Up. Inventory space %d.", m_inventoryCount);
	}

	if (cropPicked)
	{
		// TODO: get random num to find out if food is acceptable. 
		acceptableNum = (rand() % 100) + 1;
		if (acceptableNum < 50)
		{
			m_Inventory[m_inventoryCount].acceptable = false;
		}
		else
		{
			m_Inventory[m_inventoryCount].acceptable = true;
		}
		m_Inventory[m_inventoryCount].tool = NoTool;
		m_Inventory[m_inventoryCount].food = m_feastItem.edibleType;
		m_Inventory[m_inventoryCount].item = CropObj;
		m_inventoryCount++;
		cropPicked = false;
		printf("\nCrop Picked. Inventory space %d. acceptableNum: %d", m_inventoryCount, acceptableNum);
		// TODO: adjust inventory according to which food item player has and whether or not the specific food item is acceptable
	}

	if (mLeft && !mRight)
	{
		m_vel += ACCEL * time;
		if (m_vel > MAX_VELOCITY)
		{
			m_vel = MAX_VELOCITY;
		}
		m_playerPos.x -= m_vel;

		// Apply collsion so player stops at edge of screen and does not go off screen
		if (m_playerPos.x < 5 || colliding)
		{
			m_playerPos.x = prevPos.x + 1;
			colliding = false;
		}
	}
	else if (mRight && !mLeft)
	{
		m_vel += ACCEL * time;
		if (m_vel > MAX_VELOCITY)
		{
			m_vel = MAX_VELOCITY;
		}
		m_playerPos.x += m_vel;

		// Apply collsion so player stops at edge of screen and does not go off screen
		if (m_playerPos.x > 1250 || colliding)
		{
			m_playerPos.x = prevPos.x - 1;
			colliding = false;
		}
	}
	else if (mUp && !mDown)
	{
		m_vel += ACCEL * time;
		if (m_vel > MAX_VELOCITY)
		{
			m_vel = MAX_VELOCITY;
		}
		m_playerPos.y -= m_vel;

		// Apply collsion so player stops at edge of screen and does not go off screen
		if (m_playerPos.y < 10 || colliding)
		{
			m_playerPos.y = prevPos.y + 1;
			colliding = false;
		}
	}
	else if (mDown && !mUp)
	{
		m_vel += ACCEL * time;
		if (m_vel > MAX_VELOCITY)
		{
			m_vel = MAX_VELOCITY;
		}
		m_playerPos.y += m_vel;

		// Apply collsion so player stops at edge of screen and does not go off screen
		if (m_playerPos.y > 820 || colliding)
		{
			m_playerPos.y = prevPos.y - 1;
			colliding = false;
		}
	}

	

	if (useTool)
	{
		
	}
	

	//if (colliding)
	//{
	//	m_player->setPosition(prevPos);
	//	printf("\nColliding");
	//	colliding = false;
	//}
	//else
	//{
	//	m_player->setPosition(m_playerPos);
	//}
	m_player->setPosition(m_playerPos);
}

void Player::Draw(sf::RenderWindow* window)
{
	window->draw(*m_player);
}


//==================== Crop Class =====================//
Crop::Crop(sf::Texture& texture, FoodType vegType, sf::Vector2f pos)
{
	objectType = CropObj;
	cropPicked = false;
	pickAlert = false;
	//veg = vegType;
	m_feastItem.foodTexture = new sf::Texture;
	m_foodItem = new sf::RectangleShape;

	switch (vegType)
	{
	case Pumpkin:
		m_feastItem.health = 20;
		m_feastItem.edibleType = vegType;
		m_feastItem.foodTexture = &texture;
		m_foodItem->setSize(sf::Vector2f(75, 75));
		m_foodItem->setOrigin(38, 38);
		m_foodItem->setTexture(m_feastItem.foodTexture);
		//m_foodItem->setFillColor(sf::Color::Yellow);
		m_foodItem->setPosition(pos);
		m_object = m_foodItem;
		break;
	case Corn:
		m_feastItem.health = 15;
		m_feastItem.edibleType = vegType;
		m_feastItem.foodTexture = &texture;
		m_foodItem->setSize(sf::Vector2f(35, 65));
		m_foodItem->setOrigin(17, 32);
		//m_foodItem->setTexture(m_foodTexture);
		m_foodItem->setFillColor(sf::Color::Yellow);
		m_foodItem->setPosition(pos);
		m_object = m_foodItem;
		break;
	case Yam:
		m_feastItem.health = 75;
		m_feastItem.edibleType = vegType;
		m_feastItem.foodTexture = &texture;
		m_foodItem->setSize(sf::Vector2f(45, 65));
		m_foodItem->setOrigin(22, 32);
		//m_foodItem->setTexture(m_foodTexture);
		m_foodItem->setFillColor(sf::Color::Magenta);
		m_foodItem->setPosition(pos);
		m_object = m_foodItem;
		break;
	}
}

void Crop::DestroyCrop()
{
	m_object = NULL;
	delete m_foodItem;
	delete m_feastItem.foodTexture;
}

void Crop::Update()
{
	if (pickAlert)
	{
		printf("\nCAN BE PICKED");
	}

	if (cropPicked)
	{
		DestroyCrop();
	}
}

void Crop::Draw(sf::RenderWindow* window)
{
	window->draw(*m_foodItem);
}


//================= Tool Class ======================//
Tool::Tool(ToolType thisTool)
{
	m_tool = new sf::RectangleShape;
	objectType = ToolObj;
	tool = thisTool;
	toolInInventory = tool;
	pickedUp = false;

	switch (tool)
	{
	case Sickle:
		m_tool->setSize(sf::Vector2f(5, 10));
		m_tool->setFillColor(sf::Color::White);
		m_tool->setPosition(sf::Vector2f(700, 125));
		m_object = m_tool;
		break;
	case Trowel:
		m_tool->setSize(sf::Vector2f(5, 10));
		m_tool->setFillColor(sf::Color::Blue);
		m_tool->setPosition(sf::Vector2f(700, 125));
		m_object = m_tool;
		break;
	case Hatchet:
		m_tool->setSize(sf::Vector2f(5, 10));
		m_tool->setFillColor(sf::Color::Green);
		m_tool->setPosition(sf::Vector2f(700, 125));
		m_object = m_tool;
		break;
	}
}

void Tool::DestroyTool()
{
	m_object = NULL;
	delete m_tool;
	delete m_toolTexture;
}

void Tool::Update(sf::RenderWindow* window, sf::Time dt)
{
	if (pickedUp)
	{
		DestroyTool();
	}
}

void Tool::Draw(sf::RenderWindow* window)
{
	window->draw(*m_tool);
}

//================== Animal Base Class ==================//
Animal::Animal()
{

}

void Animal::Update(sf::RenderWindow* window, sf::Time dt)
{

}

void Animal::Draw(sf::RenderWindow* window)
{

}

//===================== Turkey Class ======================//
TurkeyUnit::TurkeyUnit(sf::Vector2f pos)
{
	m_animal = new sf::RectangleShape;
	m_animal->setSize(sf::Vector2f(35, 35));
	m_animal->setFillColor(sf::Color::Cyan);
	m_animal->setPosition(pos);
	//m_animalObject = m_animal;
	m_object = m_animal;
}

void TurkeyUnit::Update(sf::RenderWindow* window, sf::Time dt)
{
	// TODO: Add wander/patrol behavior for turkeys.  Add attack behavior/ runaway behavior (without and with hatchet)
	
}

void TurkeyUnit::Draw(sf::RenderWindow* window)
{
	window->draw(*m_animal);
}
