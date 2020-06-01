// HelloSFML.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side{ LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	//Create video object
	VideoMode vm(1920, 1080);

	//Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	//Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	//Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	//Create a Sprite
	Sprite spriteBackground;

	//Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	//Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	//Make tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//Make bee sprite
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	//Is bee currently moving?
	bool beeActive = false;
	//How fast can be fly?
	float beeSpeed = 0.0f;

	//Make 3 clouds
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	//Are clouds currently on screem?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	//Cloud speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//Variable to control time 
	Clock clock;

	//Time Bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


	//Game pause
	bool paused = true;

	//Score and Message
	int score = 0;
	Text messageText;
	Text scoreText; 

	//Font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//Set the font to text objects
	messageText.setFont(font);
	scoreText.setFont(font);

	//Assign actual message to text objects
	messageText.setString("Press Enter to start");
	scoreText.setString("Score = 0");

	//Change size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Set color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Positioning the texts
	scoreText.setPosition(20, 20);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);	
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	//6 Branches sprites
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	//Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//Player side in start
	side playerSide = side::LEFT;

	//Gravestone 
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//Axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line the axe up with tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Flying Log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	//Log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//Control input 
	bool acceptInput = false;

	//Sounds
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer outBuffer;
	outBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(outBuffer);
	

	while (window.isOpen()) {	
			
		/* Handle the players input */

		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::KeyReleased && !paused) {
				acceptInput = true;

				//hide axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//Start/Restart the game
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;	

			score = 0;
			timeRemaining = 6;

			//Make all branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			//Hide RIP
			spriteRIP.setPosition(675, 2000);

			//Move the player into position 
			spritePlayer.setPosition(580, 720);
			acceptInput = true;

		}

		//Take input
		if (acceptInput) {
			//Handling right key press
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				//Make the player right
				playerSide = side::RIGHT;
				score++;

				//Add time remaining
				timeRemaining += (2 / score) + 0.15;

				//set axe
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				//set player
				spritePlayer.setPosition(1200, 720);

				//update branches
				updateBranches(score);

				//set flying log to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}
			//Handling left key press
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				//Make the player left
				playerSide = side::LEFT;
				score++;

				//Add time remaining
				timeRemaining += (2 / score) + 0.15;

				//set axe
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				//set player
				spritePlayer.setPosition(580, 720);

				//update branches
				updateBranches(score);

				//set flying log to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}
		}

		/*********** Update the scene ***************/
		if (!paused) {
			//Measure time
			Time dt = clock.restart();

			//Update time remaining 
			timeRemaining -= dt.asSeconds();
			//Resize the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond*timeRemaining, timeBarHeight));

			//Time Run Out
			if (timeRemaining <= 0.0f) {
				//pause the game
				paused = true;	
				//Change message
				messageText.setString("Out of Time!");

				//reposition message for new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();

			}

			//Setup the bee
			if (beeActive == false) {

				//New Bee speed
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				//New bee height
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;

			}
			else {
				//Move the existing bee
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				//If bee reaches left end
				if (spriteBee.getPosition().x < -100) {
					// Delete this one, and set up a new in the right in next loop
					beeActive = false;
				}

			}

			//Setup cloud1
			if (cloud1Active == false) {

				//speed
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				//height
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else {
				//Move the cloud
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);

				//If cloud reaches right end
				if (spriteCloud1.getPosition().x > 1920) {
					//Deactivate this cloud
					cloud1Active = false;
				}
			}

			//Setup cloud2
			if (cloud2Active == false) {

				//speed
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				//height
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else {
				//Move the cloud
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

				//If cloud reaches right end
				if (spriteCloud2.getPosition().x > 1920) {
					//Deactivate this cloud
					cloud2Active = false;
				}
			}

			//Setup cloud3
			if (cloud3Active == false) {

				//speed
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				//height
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else {
				//Move the cloud
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

				//If cloud reaches right end
				if (spriteCloud3.getPosition().x > 1920) {
					//Deactivate this cloud
					cloud3Active = false;
				}
			}

			//Update Score
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			//Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {

					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {

					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else {
					//Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			//Handle flying log
			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				
				//Check if log has reached right/left edge
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			//Squished code
			if (playerSide == branchPositions[5]) {
				paused = true;
				acceptInput = false;

				//rip
				spriteRIP.setPosition(525, 760);
				//Hide player
				spritePlayer.setPosition(2000, 660);

				//Message
				messageText.setString("SQUISHED!");
				//Put the message in center
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}

		} //paused condition ends


		/* Draw the scene */
		//Clear everything from the last frame
		window.clear();

		//Draw the game scene here
		window.draw(spriteBackground);
		
		//Draw clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		//Draw 6 branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		//Draw tree
		window.draw(spriteTree);

		//Draw the Player
		window.draw(spritePlayer);

		//Draw the axe
		window.draw(spriteAxe);

		//Draw the log
		window.draw(spriteLog);

		//Draw the RIP
		window.draw(spriteRIP);

		//Draw bee
		window.draw(spriteBee);

		//Draw score
		window.draw(scoreText);

		//Draw timebar
		window.draw(timeBar);

		if (paused) {

			//Draw message
			window.draw(messageText);
		}

		//Show everything we just drew
		window.display();

	}

    return 0;
}

void updateBranches(int seed) {
	
	//Shift all branches down 
	for (int i = NUM_BRANCHES - 1; i > 0; i--) {
		branchPositions[i] = branchPositions[i - 1];
	}

	//New branch at zeroth position (Top of tree)
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
