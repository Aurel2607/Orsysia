//Legends of Meruvia - C++ / SFML 2.5.1
//Copyright / Droits d'auteur : Aurel

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include <iostream>

int main()
{

	// setup window
	sf::Vector2i screenDimensions(800, 600);
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y),
		"Test Animated Sprite");
	window.setFramerateLimit(60);

	const int playerWidth = 48;
	const int playerHeight = 48;

	bool isEyesOpened = true;

	// load texture (spritesheet)
	sf::Texture texture;
	if (!texture.loadFromFile("player.png"))
	{
		std::cout << "Failed to load player spritesheet!" << std::endl;
		return 1;
	}

	 // set up the animations for all four directions (set spritesheet and push frames)
	CAnimation walkingAnimationDownEyesClose;
	walkingAnimationDownEyesClose.setSpriteSheet(texture);
	walkingAnimationDownEyesClose.addFrame(sf::IntRect(playerWidth,         0, playerWidth, playerHeight));
	walkingAnimationDownEyesClose.addFrame(sf::IntRect((2* playerWidth),    0, playerWidth, playerHeight));
	walkingAnimationDownEyesClose.addFrame(sf::IntRect(playerWidth,         0, playerWidth, playerHeight));
	walkingAnimationDownEyesClose.addFrame(sf::IntRect(0,                   0, playerWidth, playerHeight));

	CAnimation walkingAnimationDownEyesOpen;
	walkingAnimationDownEyesOpen.setSpriteSheet(texture);
	walkingAnimationDownEyesOpen.addFrame(sf::IntRect(playerWidth,      (4 * playerHeight), playerWidth, playerHeight));
	walkingAnimationDownEyesOpen.addFrame(sf::IntRect((2* playerWidth), (4 * playerHeight), playerWidth, playerHeight));
	walkingAnimationDownEyesOpen.addFrame(sf::IntRect(playerWidth,      (4 * playerHeight), playerWidth, playerHeight));
	walkingAnimationDownEyesOpen.addFrame(sf::IntRect(0,                (4 * playerHeight), playerWidth, playerHeight));

	CAnimation walkingAnimationLeftEyesClose;
	walkingAnimationLeftEyesClose.setSpriteSheet(texture);
	walkingAnimationLeftEyesClose.addFrame(sf::IntRect(playerWidth,         playerHeight, playerWidth, playerHeight));
	walkingAnimationLeftEyesClose.addFrame(sf::IntRect((2 * playerWidth),   playerHeight, playerWidth, playerHeight));
	walkingAnimationLeftEyesClose.addFrame(sf::IntRect(playerWidth,         playerHeight, playerWidth, playerHeight));
	walkingAnimationLeftEyesClose.addFrame(sf::IntRect(0,                   playerHeight, playerWidth, playerHeight));

	CAnimation walkingAnimationLeftEyesOpen;
	walkingAnimationLeftEyesOpen.setSpriteSheet(texture);
	walkingAnimationLeftEyesOpen.addFrame(sf::IntRect(playerWidth,          (5 * playerHeight), playerWidth, playerHeight));
	walkingAnimationLeftEyesOpen.addFrame(sf::IntRect((2 * playerWidth),    (5 * playerHeight), playerWidth, playerHeight));
	walkingAnimationLeftEyesOpen.addFrame(sf::IntRect(playerWidth,          (5 * playerHeight), playerWidth, playerHeight));
	walkingAnimationLeftEyesOpen.addFrame(sf::IntRect(0,                    (5 * playerHeight), playerWidth, playerHeight));

	CAnimation walkingAnimationRightEyesClose;
	walkingAnimationRightEyesClose.setSpriteSheet(texture);
	walkingAnimationRightEyesClose.addFrame(sf::IntRect(playerWidth,        (2 * playerHeight), playerWidth, playerHeight));
	walkingAnimationRightEyesClose.addFrame(sf::IntRect((2 * playerWidth),  (2 * playerHeight), playerWidth, playerHeight));
	walkingAnimationRightEyesClose.addFrame(sf::IntRect(playerWidth,        (2 * playerHeight), playerWidth, playerHeight));
	walkingAnimationRightEyesClose.addFrame(sf::IntRect(0,                  (2 * playerHeight), playerWidth, playerHeight));

	CAnimation walkingAnimationRightEyesOpen;
	walkingAnimationRightEyesOpen.setSpriteSheet(texture);
	walkingAnimationRightEyesOpen.addFrame(sf::IntRect(playerWidth,         (6 * playerHeight), playerWidth, playerHeight));
	walkingAnimationRightEyesOpen.addFrame(sf::IntRect((2 * playerWidth),   (6 * playerHeight), playerWidth, playerHeight));
	walkingAnimationRightEyesOpen.addFrame(sf::IntRect(playerWidth,         (6 * playerHeight), playerWidth, playerHeight));
	walkingAnimationRightEyesOpen.addFrame(sf::IntRect(0,                   (6 * playerHeight), playerWidth, playerHeight));

	CAnimation walkingAnimationUpEyesClose;
	walkingAnimationUpEyesClose.setSpriteSheet(texture);
	walkingAnimationUpEyesClose.addFrame(sf::IntRect(playerWidth,           (3 * playerHeight), playerWidth, playerHeight));
	walkingAnimationUpEyesClose.addFrame(sf::IntRect((2 * playerWidth),     (3 * playerHeight), playerWidth, playerHeight));
	walkingAnimationUpEyesClose.addFrame(sf::IntRect(playerWidth,           (3 * playerHeight), playerWidth, playerHeight));
	walkingAnimationUpEyesClose.addFrame(sf::IntRect(0,                     (3 * playerHeight), playerWidth, playerHeight));

	CAnimation walkingAnimationUpEyesOpen;
	walkingAnimationUpEyesOpen.setSpriteSheet(texture);
	walkingAnimationUpEyesOpen.addFrame(sf::IntRect(playerWidth,            (7 * playerHeight), playerWidth, playerHeight));
	walkingAnimationUpEyesOpen.addFrame(sf::IntRect((2 * playerWidth),      (7 * playerHeight), playerWidth, playerHeight));
	walkingAnimationUpEyesOpen.addFrame(sf::IntRect(playerWidth,            (7 * playerHeight), playerWidth, playerHeight));
	walkingAnimationUpEyesOpen.addFrame(sf::IntRect(0,                      (7 * playerHeight), playerWidth, playerHeight));

	CAnimation* currentAnimation = &walkingAnimationDownEyesOpen;

	// set up AnimatedSprite
	CAnimatedSprite animatedSprite(sf::seconds(0.2), true, false);
	animatedSprite.setPosition(sf::Vector2f(screenDimensions / 2));

	sf::Clock frameClock;

	float speed = 80.f;
	bool noKeyWasPressed = true;
	bool oldStatePressed = false;

	while (window.isOpen())	{
		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();

            // Gestion de l'appuie touche O (bascule keyPressed/keyReleased)
            if ((event.type == sf::Event::KeyPressed) &&
                (event.key.code == sf::Keyboard::O) &&
                (oldStatePressed == false))
            {
                isEyesOpened = !isEyesOpened;
                noKeyWasPressed = false;

                oldStatePressed = true;

				// Switch Eyes Close / Eyes Open - Down Animation
				if(currentAnimation == &walkingAnimationDownEyesClose) {
					currentAnimation = &walkingAnimationDownEyesOpen;
				}else if(currentAnimation == &walkingAnimationDownEyesOpen) {
					currentAnimation = &walkingAnimationDownEyesClose;
				}

				// Switch Eyes Close / Eyes Open - Up Animation
				if(currentAnimation == &walkingAnimationUpEyesClose) {
					currentAnimation = &walkingAnimationUpEyesOpen;
				}else if(currentAnimation == &walkingAnimationUpEyesOpen) {
					currentAnimation = &walkingAnimationUpEyesClose;
				}

				// Switch Eyes Close / Eyes Open - Left Animation
				if(currentAnimation == &walkingAnimationLeftEyesClose) {
					currentAnimation = &walkingAnimationLeftEyesOpen;
				}else if(currentAnimation == &walkingAnimationLeftEyesOpen) {
					currentAnimation = &walkingAnimationLeftEyesClose;
				}

				// Switch Eyes Close / Eyes Open - Right Animation
				if(currentAnimation == &walkingAnimationRightEyesClose) {
					currentAnimation = &walkingAnimationRightEyesOpen;
				}else if(currentAnimation == &walkingAnimationRightEyesOpen) {
					currentAnimation = &walkingAnimationRightEyesClose;
				}
            }

            // Gestion de l'appuie touche O (bascule keyPressed/keyReleased)
            if ((event.type == sf::Event::KeyReleased) &&
                (event.key.code == sf::Keyboard::O) &&
                (oldStatePressed == true)) {
                oldStatePressed = false;
            }
		}

		sf::Time frameTime = frameClock.restart();


		sf::Vector2f movement(0.f, 0.f);
		// if a key was pressed set the correct animation and move correctly
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			if(isEyesOpened == true)
				currentAnimation = &walkingAnimationUpEyesOpen;
			else
				currentAnimation = &walkingAnimationUpEyesClose;
			movement.y -= speed;
			noKeyWasPressed = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{
			if (isEyesOpened == true)
				currentAnimation = &walkingAnimationDownEyesOpen;
			else
				currentAnimation = &walkingAnimationDownEyesClose;
			movement.y += speed;
			noKeyWasPressed = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{
			if (isEyesOpened == true)
				currentAnimation = &walkingAnimationLeftEyesOpen;
			else
				currentAnimation = &walkingAnimationLeftEyesClose;
			movement.x -= speed;
			noKeyWasPressed = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			if (isEyesOpened == true)
				currentAnimation = &walkingAnimationRightEyesOpen;
			else
				currentAnimation = &walkingAnimationRightEyesClose;
			movement.x += speed;
			noKeyWasPressed = false;
		}
		animatedSprite.play(*currentAnimation);
		animatedSprite.move(movement * frameTime.asSeconds());

		// if no key was pressed stop the animation
		if (noKeyWasPressed) {
			animatedSprite.stop();
		}
		noKeyWasPressed = true;

		// update AnimatedSprite
		animatedSprite.update(frameTime);

		// draw
		window.clear();
		window.draw(animatedSprite);
		window.display();
	}

	return 0;
}






