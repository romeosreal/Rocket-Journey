// SFML_example.cpp: ���������� ����� ����� ��� ����������� ����������.
//

// ����������
#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "Funcs.h"
#include "Rocket.h"
#include "SpeedIndicator.h"
#include "Background.h"
#include "Asteroid.h"
#include "GravityCenter.h"

int main()
{
	// ������������� �������� ����
	ContextSettings sets;
	sets.antialiasingLevel = 2;

	// ������������� ����
	RenderWindow win(VideoMode(1400, 700), "Hello SFML", 
		Style::Fullscreen, sets);

	// ���������� ������������ �������������
	win.setVerticalSyncEnabled(true);

	// ��������� ���� ��������� �����
	srand(time(NULL));

	// ������������� ������
	Rocket rocket("rocket.png", "bum.png", win.getSize().x / 10, win.getSize().y / 2);

	// ������������� ��������� ��������
	double speed = 50;

	// ������������� �������� �������
	Clock timer;

	// ������������� ������ ������
	RectangleShape startButton;
	startButton.setFillColor(Color::Color(100, 100, 5));
	startButton.setOutlineColor(Color::Color(255, 255, 5));
	startButton.setOutlineThickness(5);
	startButton.setSize(Vector2f(win.getSize().x / 3, win.getSize().y / 4));
	startButton.setOrigin(Vector2f(startButton.getGlobalBounds().width / 2,
		startButton.getGlobalBounds().height / 2));
	startButton.setPosition(Vector2f(win.getSize().x / 2, win.getSize().y / 2));

	// �������� ������
	Font font;
	font.loadFromFile("arial.ttf");

	Text startText;
	startText.setFont(font);
	startText.setString("Start");
	startText.setStyle(Text::Bold);
	startText.setCharacterSize(win.getSize().x / 7);
	startText.setFillColor(Color::Color(80, 0, 0));
	startText.setOrigin(Vector2f(startText.getGlobalBounds().width / 2,
		startText.getGlobalBounds().height / 2));
	startText.setPosition(win.getSize().x / 2 - win.getSize().x / 75, 
		win.getSize().y / 2 - win.getSize().y / 15);

	// ������������� ������ ���������� ����� � ����
	Text score;
	score.setFont(font);
	score.setString("");
	score.setCharacterSize(win.getSize().x / 20);
	score.setFillColor(Color::Color(220, 250, 5));
	score.setOrigin(Vector2f(score.getGlobalBounds().width / 2, 0));
	score.setPosition(Vector2f(win.getSize().x / 2, 0));
	score.setStyle(Text::Bold);

	// ������������� ������ ������� � ����
	Text hScore;
	hScore.setFont(font);
	hScore.setString("High score:\t0");
	hScore.setCharacterSize(win.getSize().x / 30);
	hScore.setFillColor(Color::Color(220, 250, 5));
	hScore.setPosition(Vector2f(0, 0));
	hScore.setStyle(Text::Bold);

	// ������������� ������ ���������� ����� �� ������ ���������
	Text resultScore;
	resultScore.setFont(font);
	resultScore.setString("");
	resultScore.setCharacterSize(win.getSize().x / 12);
	resultScore.setOrigin(Vector2f(resultScore.getGlobalBounds().width / 2,
		resultScore.getGlobalBounds().height / 2));
	resultScore.setPosition(win.getSize().x / 2 - win.getSize().x / 12, 
		win.getSize().y / 4);
	resultScore.setFillColor(Color::Green);
	resultScore.setStyle(Text::Bold);

	// ������������� ������ ������� �� ������ ���������
	Text resultHighScore;
	resultHighScore.setFont(font);
	resultHighScore.setString("");
	resultHighScore.setCharacterSize(win.getSize().x / 48);
	resultHighScore.setOrigin(Vector2f(resultHighScore.getGlobalBounds().width / 2,
		resultHighScore.getGlobalBounds().height / 2));
	resultHighScore.setPosition(win.getSize().x / 2 - win.getSize().x / 8,
		win.getSize().y / 4 + win.getSize().x / 12);
	resultHighScore.setFillColor(Color::Green);
	resultHighScore.setStyle(Text::Bold);

	// ������������� ������ ��������
	RectangleShape restartButton;
	restartButton.setFillColor(Color::Color(100, 100, 5));
	restartButton.setOutlineColor(Color::Color(255, 255, 5));
	restartButton.setOutlineThickness(5);
	restartButton.setSize(Vector2f(win.getSize().x / 3, win.getSize().y / 4));
	restartButton.setOrigin(Vector2f(restartButton.getGlobalBounds().width / 2,
		restartButton.getGlobalBounds().height / 2));
	restartButton.setPosition(Vector2f(win.getSize().x / 2, win.getSize().y / 3 * 2));

	// ������������� ������ �� ������ ��������
	Text restartText;
	restartText.setFont(font);
	restartText.setString("Restart");
	restartText.setStyle(Text::Bold);
	restartText.setCharacterSize(win.getSize().x / 10);
	restartText.setFillColor(Color::Color(80, 0, 0));
	restartText.setOrigin(Vector2f(startText.getGlobalBounds().width / 2,
		restartText.getGlobalBounds().height / 2));
	restartText.setPosition(win.getSize().x / 2 - win.getSize().x / 50,
		win.getSize().y / 3 * 2 - win.getSize().y / 15);

	// ������� ����
	Vector2i mouse;

	// ������� ������ � �������� �� �������
	Vector2f position = Vector2f(0, 0), move = Vector2f(0, 0);

	// ��������� ��������
	SpeedIndicator indicator;

	// ������ ���
	Background background(300);
	
	// ������ ����������
	AsteroidCloud asteroidCloud("asteroid.png", 80, 15);

	// �������������� ������
	GravityCloud gravityCloud("star.png", 5);

	// ���������� ��� �������� ������� ��������
	double _timer = timer.getElapsedTime().asSeconds(), t, now;

	// ������ �����
	bool speedy = false, slowly = false, lastEnd = true,
		inGame = false, click = false, start = true, lose = false,
		loseFlag = false;

	Music music;
	music.openFromFile("strange.wav");
	music.play();

	// �������� ����-��������
	RectangleShape tranBackground;
	tranBackground.setSize(Vector2f(win.getSize()));
	tranBackground.setPosition(Vector2f(0, 0));
	tranBackground.setFillColor(Color::Color(190, 180, 60, 80));

	// ��������� ��������� � ���������� �����
	uint32_t highScore = 0, scoreValue = 0;

	// �������� � ������ ������
	int problems = 0, bumIndex = 0;

	// ��������� �������� ���� � ������
	rocket.updateWindow(win.getSize());

	// ��������������� ������
	rocket.scale();

	// ��������� ������� ���� � ���������
	indicator.updateWindow(win.getSize());

	// ������ ������� ���������� ��������
	indicator.calculateSize();

	// ������������� ������� ����
	background.init(win.getSize());

	// ������������� ��������������� ������
	gravityCloud.init(win.getSize());

	// ������������� ������ ����������
	asteroidCloud.init(win.getSize());
	
	// �������� ���� - ���� ���� �������
	while (win.isOpen()) {

		// ������� ����� ��������
		now = timer.getElapsedTime().asSeconds();
		t = now - _timer;
		_timer = now;

		// ������������ �������
		Event event;
		while (win.pollEvent(event)) {

			// ��������� ���� �� ��������
			if (event.type == Event::Closed) win.close();

			// ��������� ������� �������
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) win.close();
				if (event.key.code == Keyboard::W) speedy = true;
				if (event.key.code == Keyboard::S) slowly = true;
			}

			// ��������� ������� �������
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Escape) win.close();
				if (event.key.code == Keyboard::W) speedy = false;
				if (event.key.code == Keyboard::S) slowly = false;
			}
			if (event.type == Event::MouseMoved) mouse = Mouse::getPosition(win);
			if (event.type == Event::MouseButtonPressed) click = true;
			if (event.type == Event::MouseButtonReleased) click = false;
		}

		// ���� � ���� � ������ ������ ������� � ������� ����
		if (inGame && timer.getElapsedTime().asSeconds() > 1) {
			
			// ������������ ������/����������
			if (speedy) speed += t * (79 - speed);
			if (slowly) speed -= t * speed;

			// ������������ ��������� ��������
			indicator.updateSpeed(int(speed / 20));

			//������������� ����������
			rocket.setGravity(Vector2f(0,0));

			// ������������� ����
			rocket.updateMouse(mouse);
			double angleToMouse = rocket.angleToMouse();

			// ������������ ������
			rocket.setRotate(angleToMouse);

			// ������������� �������� ������
			rocket.updateSpeed(speed);

			// ������� ������
			rocket.fly(t);

			// ������������ ��������
			if (problems == GRAVITYS) {
				Vector2f gravity = gravityCloud.getGravity(rocket.get().getPosition());
				move = Vector2f(-t * sin(rocket.get().getRotation() / 180 * PI)
					* win.getSize().y * speed + gravity.x,
					t * cos(rocket.get().getRotation() / 180 * PI) * 
					win.getSize().y * speed - gravity.y);
			}
			else move = Vector2f(-t * sin(rocket.get().getRotation() / 180 * PI)
				* win.getSize().y * speed,
				t * cos(rocket.get().getRotation() / 180 * PI) * win.getSize().y * speed);

			win.clear();
			// �������� ������ � 2 ���� �������� �������� ��������
			background.update(t, Vector2f(move.x / 2, move.y / 2));
			ConvexShape* stars = background.get();
			for (int i = 0; i < background.len; i++) win.draw(stars[i]);

			// �������� ������� ������
			position.x -= move.x;
			position.y += move.y;

			// ������������ ������
			win.draw(rocket.get());

			// ������������ ��������� ��������
			RectangleShape* indic = indicator.get();
			for (int i = 0; i < 3; i++) win.draw(indic[i]);
			
			if (lastEnd == true) {
				// ���� ������� ��� - �� ������� ��
				problems = round(random() - (problems - 0.5) * 0.5);
				if (problems == GRAVITYS) gravityCloud.init(win.getSize());
				if (problems == ASTEROIDS) asteroidCloud.init(win.getSize());
				// ������ �������� ����
				lastEnd = false;
			}

			if (problems == GRAVITYS) {
				// ���� ������������
				lastEnd = true;
				// �������� ������������
				gravityCloud.update(t, Vector2f(move.x, move.y));
				// �������� ������������
				Sprite* gravitys = gravityCloud.get();
				for (int i = 0; i < gravityCloud.len; i++) {
					// ��������� ������������ �� ��������
					if (gravityCloud.contains(i, rocket.get().getPosition())) {
						inGame = false;
						lose = true;
					}
					// ���� ������� ������ - �� ���������
					if (gravitys[i].getPosition().x > win.getSize().x * -1.0) lastEnd = false;
					// ������������ �����������
					win.draw(gravitys[i]);
				}
			}
			if (problems == ASTEROIDS) {
				// ���� ���������
				lastEnd = true;
				// �� �� �� ��������
				asteroidCloud.update(t, Vector2f(move.x, move.y));
				// ��������
				Sprite* asteroids = asteroidCloud.get();
				// ������������
				for (int i = 0; i < asteroidCloud.len; i++) {
					// ��������� ������ �� �������� � ��������
					if (asteroids[i].getGlobalBounds().contains(rocket.get().getPosition())) {
						inGame = false;
						lose = true;
					}
					// ���� ��� ��������� ������� �� ����� - �� ������� ��
					if (asteroids[i].getPosition().x > win.getSize().x * -0.2) lastEnd = false;
					// ������������ ���������
					win.draw(asteroids[i]);
				}
			}
			// ������������ ����
			scoreValue = uint32_t(position.x / win.getSize().x);
			ostringstream ss;
			ss << scoreValue;
			score.setString(ss.str());
			// �������� ����� ������ - ��� ������ ���������
			score.setOrigin(Vector2f(score.getGlobalBounds().width / 2, 0));
			// ������������� ������� ��� ����� - �� ������ ������ ������
			score.setPosition(Vector2f(win.getSize().x / 2, 0));
			// ������������ ���������
			win.draw(score);
			// ���� ���� ������ - �� ������ ���
			if (highScore != 0) win.draw(hScore);
			win.display();
		}
		else {
			if (start) {
				if (startButton.getGlobalBounds().contains(Vector2f(mouse))) {
					startText.setFillColor(Color::White);
					// ���� ���� �������� �� ������ - �� ����� �����
					if (click) {
						// ���� ������ ������ - ������ ����
						inGame = true;
						start = false;
					}
				}
				else {
					// ���� ���� �� �������� �� ������ - �� ����� �������
					startText.setFillColor(Color::Color(80, 0, 0));
				}
				// �������� ������ ���
				move = Vector2f(-t * sin(rocket.get().getRotation() / 180 * PI)
					* win.getSize().y * speed,
					t * cos(rocket.get().getRotation() / 180 * PI) * win.getSize().y * speed);
				background.update(t, Vector2f(move.x / 2, move.y / 2));
				ConvexShape* stars = background.get();
				win.clear();
				// ������������ ������
				for (int i = 0; i < background.len; i++) win.draw(stars[i]);
				// ������������� ����
				rocket.updateMouse(mouse);
				// ������������� ���� ��������
				rocket.setRotate(rocket.angleToMouse());
				// ������������� ��������
				rocket.updateSpeed(80);
				// �������� ������
				rocket.fly(t);
				// ��������� ������
				win.draw(rocket.get());
				// ��������� ������ ������
				win.draw(startButton);
				win.draw(startText);
				win.display();
			}
			else if (lose) {
				if (loseFlag) {
					// ����� �����
					rocket.reinit();
					asteroidCloud.reinit();
					gravityCloud.reinit();
					rocket.updateWindow(win.getSize());
					rocket.scale();
					indicator.updateWindow(win.getSize());
					indicator.calculateSize();
					background.init(win.getSize());
					gravityCloud.init(win.getSize());
					asteroidCloud.init(win.getSize());
					ostringstream ss;
					ss << "High score\t" << highScore;
					hScore.setString(ss.str());
					position.x = 0;
					position.y = 0;
					lose = false;
					loseFlag = false;
					inGame = true;
					problems = round(random() - (problems - 0.5) * 0.5);
					bumIndex = 0;
				}
				else {
					if (restartButton.getGlobalBounds().contains(Vector2f(mouse))) {
						// ���� ���� �������� �� ������ - �� ����� �����
						restartText.setFillColor(Color::White);
						if (click) {
							loseFlag = true;
							// ���� ���� �� ������ - ����� ������������� � ������
						}
					}
					else {
						// ���� ���� �� �������� �� ������ - �� ����� �������
						restartText.setFillColor(Color::Color(80, 0, 0));
					}
					/*
					��������� ������ ������
					*/
					if (bumIndex < 15) {
						bumIndex = rocket.bum(t);
					}
					// ������� �����
					scoreValue = uint32_t(position.x / win.getSize().x);
					// ������� �������
					if (scoreValue > highScore) highScore = scoreValue;
					win.clear();
					// ��������� �����
					ConvexShape* stars = background.get();
					for (int i = 0; i < background.len; i++) win.draw(stars[i]);
					// ���� ����������� - �� ������ ������������
					if (problems == GRAVITYS) {
						Sprite* gravitys = gravityCloud.get();
						for (int i = 0; i < gravityCloud.len; i++) win.draw(gravitys[i]);
					} 
					// ���� ��������� - �� ������ ���������
					if (problems == ASTEROIDS) {
						Sprite* asteroids = asteroidCloud.get();
						for (int i = 0; i < asteroidCloud.len; i++) win.draw(asteroids[i]);
					}
					// ������������ ������
					win.draw(rocket.get());
					// ������������ �������� ���
					win.draw(tranBackground);
					// ������� ��������� ����
					ostringstream ss;
					ss << scoreValue;
					resultScore.setString(ss.str());
					resultScore.setOrigin(Vector2f(resultScore.getGlobalBounds().width / 2,
						resultScore.getGlobalBounds().height / 2));
					resultScore.setPosition(win.getSize().x / 2,
						win.getSize().y / 4);
					// ������� ������������ ���������
					ostringstream ss2;
					ss2 << "High score: " << highScore << endl;
					resultHighScore.setString(ss2.str());
					resultHighScore.setOrigin(Vector2f(resultHighScore.getGlobalBounds().width / 2,
						resultHighScore.getGlobalBounds().height / 2));
					resultHighScore.setPosition(win.getSize().x / 2,
						win.getSize().y / 4 + win.getSize().x / 10);
					win.draw(resultScore);
					win.draw(resultHighScore);
					// ��������� ������ � ������ �� ���
					win.draw(restartButton);
					win.draw(restartText);
					win.display();
				}
			}
		}
	}
	return 0;
}