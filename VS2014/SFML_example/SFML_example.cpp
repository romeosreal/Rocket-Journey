// SFML_example.cpp: определяет точку входа для консольного приложения.
//

// Библиотеки
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
	// Инициализация настроек окна
	ContextSettings sets;
	sets.antialiasingLevel = 2;

	// Инициализация окна
	RenderWindow win(VideoMode(1400, 700), "Hello SFML", 
		Style::Fullscreen, sets);

	// Разрешение вертикальной синхронизации
	win.setVerticalSyncEnabled(true);

	// Установка сида рандомных чисел
	srand(time(NULL));

	// Инициализация ракеты
	Rocket rocket("rocket.png", "bum.png", win.getSize().x / 10, win.getSize().y / 2);

	// Инициализация начальной скорости
	double speed = 50;

	// Инициализация счетчика времени
	Clock timer;

	// Инициализация кнопки старта
	RectangleShape startButton;
	startButton.setFillColor(Color::Color(100, 100, 5));
	startButton.setOutlineColor(Color::Color(255, 255, 5));
	startButton.setOutlineThickness(5);
	startButton.setSize(Vector2f(win.getSize().x / 3, win.getSize().y / 4));
	startButton.setOrigin(Vector2f(startButton.getGlobalBounds().width / 2,
		startButton.getGlobalBounds().height / 2));
	startButton.setPosition(Vector2f(win.getSize().x / 2, win.getSize().y / 2));

	// Загрузка шрифта
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

	// Инициализация текста количества очков в игре
	Text score;
	score.setFont(font);
	score.setString("");
	score.setCharacterSize(win.getSize().x / 20);
	score.setFillColor(Color::Color(220, 250, 5));
	score.setOrigin(Vector2f(score.getGlobalBounds().width / 2, 0));
	score.setPosition(Vector2f(win.getSize().x / 2, 0));
	score.setStyle(Text::Bold);

	// Инициализация текста рекорда в игре
	Text hScore;
	hScore.setFont(font);
	hScore.setString("High score:\t0");
	hScore.setCharacterSize(win.getSize().x / 30);
	hScore.setFillColor(Color::Color(220, 250, 5));
	hScore.setPosition(Vector2f(0, 0));
	hScore.setStyle(Text::Bold);

	// Инициализация текста количества очков на экране проигрыша
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

	// Инициализация текста рекорда на экрнае проигрыша
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

	// Инициализация кнопки рестарта
	RectangleShape restartButton;
	restartButton.setFillColor(Color::Color(100, 100, 5));
	restartButton.setOutlineColor(Color::Color(255, 255, 5));
	restartButton.setOutlineThickness(5);
	restartButton.setSize(Vector2f(win.getSize().x / 3, win.getSize().y / 4));
	restartButton.setOrigin(Vector2f(restartButton.getGlobalBounds().width / 2,
		restartButton.getGlobalBounds().height / 2));
	restartButton.setPosition(Vector2f(win.getSize().x / 2, win.getSize().y / 3 * 2));

	// Инициализация текста на кнопке рестарта
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

	// Позиция мыши
	Vector2i mouse;

	// Позиция ракеты и скорость ее движени
	Vector2f position = Vector2f(0, 0), move = Vector2f(0, 0);

	// Индикатор скорости
	SpeedIndicator indicator;

	// Задний фон
	Background background(300);
	
	// Облако астероидов
	AsteroidCloud asteroidCloud("asteroid.png", 80, 15);

	// Гравитационное облако
	GravityCloud gravityCloud("star.png", 5);

	// Переменные для подсчета времени итерации
	double _timer = timer.getElapsedTime().asSeconds(), t, now;

	// Всякие флаги
	bool speedy = false, slowly = false, lastEnd = true,
		inGame = false, click = false, start = true, lose = false,
		loseFlag = false;

	Music music;
	music.openFromFile("strange.wav");
	music.play();

	// Создание фона-заглушки
	RectangleShape tranBackground;
	tranBackground.setSize(Vector2f(win.getSize()));
	tranBackground.setPosition(Vector2f(0, 0));
	tranBackground.setFillColor(Color::Color(190, 180, 60, 80));

	// Наилучший результат и количество очков
	uint32_t highScore = 0, scoreValue = 0;

	// Проблемы и индекс взрыва
	int problems = 0, bumIndex = 0;

	// Установка размеров окна в ракету
	rocket.updateWindow(win.getSize());

	// Масштабирование ракеты
	rocket.scale();

	// Установка размера окна в индикатор
	indicator.updateWindow(win.getSize());

	// Расчет размера индикатора скорости
	indicator.calculateSize();

	// Инициализация заднего фона
	background.init(win.getSize());

	// Инициализация гравитационного облака
	gravityCloud.init(win.getSize());

	// Инициализация облака астероидов
	asteroidCloud.init(win.getSize());
	
	// Основной цикл - пока окно открыто
	while (win.isOpen()) {

		// Считаем время итерации
		now = timer.getElapsedTime().asSeconds();
		t = now - _timer;
		_timer = now;

		// Обрабатываем события
		Event event;
		while (win.pollEvent(event)) {

			// Проверяем окно на закрытие
			if (event.type == Event::Closed) win.close();

			// Проверяем нажатые клавиши
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) win.close();
				if (event.key.code == Keyboard::W) speedy = true;
				if (event.key.code == Keyboard::S) slowly = true;
			}

			// Проверяем отжатые клавиши
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Escape) win.close();
				if (event.key.code == Keyboard::W) speedy = false;
				if (event.key.code == Keyboard::S) slowly = false;
			}
			if (event.type == Event::MouseMoved) mouse = Mouse::getPosition(win);
			if (event.type == Event::MouseButtonPressed) click = true;
			if (event.type == Event::MouseButtonReleased) click = false;
		}

		// Если в игре и прошло больше секунды с запуска игры
		if (inGame && timer.getElapsedTime().asSeconds() > 1) {
			
			// Обрабатываем разгон/замедление
			if (speedy) speed += t * (79 - speed);
			if (slowly) speed -= t * speed;

			// Обрабатываем индикатор скорости
			indicator.updateSpeed(int(speed / 20));

			//Устанавливаем гравитацию
			rocket.setGravity(Vector2f(0,0));

			// Устанавливаем мышь
			rocket.updateMouse(mouse);
			double angleToMouse = rocket.angleToMouse();

			// Поворачиваем ракету
			rocket.setRotate(angleToMouse);

			// Устанавливаем скорость ракеты
			rocket.updateSpeed(speed);

			// Двигаем ракету
			rocket.fly(t);

			// Рассчитываем скорости
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
			// Сдвигаем звезды в 2 раза медленее реальной скорости
			background.update(t, Vector2f(move.x / 2, move.y / 2));
			ConvexShape* stars = background.get();
			for (int i = 0; i < background.len; i++) win.draw(stars[i]);

			// Сдвигаем позицию ракеты
			position.x -= move.x;
			position.y += move.y;

			// Отрисовываем ракету
			win.draw(rocket.get());

			// Отрисовываем индикатор скорости
			RectangleShape* indic = indicator.get();
			for (int i = 0; i < 3; i++) win.draw(indic[i]);
			
			if (lastEnd == true) {
				// Если проблем нет - то создаем ее
				problems = round(random() - (problems - 0.5) * 0.5);
				if (problems == GRAVITYS) gravityCloud.init(win.getSize());
				if (problems == ASTEROIDS) asteroidCloud.init(win.getSize());
				// Теперь проблема есть
				lastEnd = false;
			}

			if (problems == GRAVITYS) {
				// Если гравитончики
				lastEnd = true;
				// Сдвигаем гравитончики
				gravityCloud.update(t, Vector2f(move.x, move.y));
				// Получаем гравитончики
				Sprite* gravitys = gravityCloud.get();
				for (int i = 0; i < gravityCloud.len; i++) {
					// Проверяем гравитончики на врезание
					if (gravityCloud.contains(i, rocket.get().getPosition())) {
						inGame = false;
						lose = true;
					}
					// Если улетели далеко - то отключаем
					if (gravitys[i].getPosition().x > win.getSize().x * -1.0) lastEnd = false;
					// Отрисовываем гравитончик
					win.draw(gravitys[i]);
				}
			}
			if (problems == ASTEROIDS) {
				// Если астероиды
				lastEnd = true;
				// То мы их сдвигаем
				asteroidCloud.update(t, Vector2f(move.x, move.y));
				// Получаем
				Sprite* asteroids = asteroidCloud.get();
				// Отрисовываем
				for (int i = 0; i < asteroidCloud.len; i++) {
					// Проверяем ракету на врезание в астероид
					if (asteroids[i].getGlobalBounds().contains(rocket.get().getPosition())) {
						inGame = false;
						lose = true;
					}
					// Если все астероиды улетели за экран - то удаляем их
					if (asteroids[i].getPosition().x > win.getSize().x * -0.2) lastEnd = false;
					// Отрисовываем астероиды
					win.draw(asteroids[i]);
				}
			}
			// Рассчитываем очки
			scoreValue = uint32_t(position.x / win.getSize().x);
			ostringstream ss;
			ss << scoreValue;
			score.setString(ss.str());
			// Сдвигаем центр текста - для ровной центровки
			score.setOrigin(Vector2f(score.getGlobalBounds().width / 2, 0));
			// Устанавливаем позицию для очков - по центру экрана сверху
			score.setPosition(Vector2f(win.getSize().x / 2, 0));
			// Отрисовываем результат
			win.draw(score);
			// Если есть рекорд - то рисуем его
			if (highScore != 0) win.draw(hScore);
			win.display();
		}
		else {
			if (start) {
				if (startButton.getGlobalBounds().contains(Vector2f(mouse))) {
					startText.setFillColor(Color::White);
					// Если мышь наведена на кнопку - то текст белый
					if (click) {
						// Если нажата кнопка - Запуск игры
						inGame = true;
						start = false;
					}
				}
				else {
					// Если мышь не наведена на кнопку - то текст красный
					startText.setFillColor(Color::Color(80, 0, 0));
				}
				// Сдвигаем задний фон
				move = Vector2f(-t * sin(rocket.get().getRotation() / 180 * PI)
					* win.getSize().y * speed,
					t * cos(rocket.get().getRotation() / 180 * PI) * win.getSize().y * speed);
				background.update(t, Vector2f(move.x / 2, move.y / 2));
				ConvexShape* stars = background.get();
				win.clear();
				// Отрисовываем звезды
				for (int i = 0; i < background.len; i++) win.draw(stars[i]);
				// Устанавливаем мышь
				rocket.updateMouse(mouse);
				// Устанавливаем угол вращения
				rocket.setRotate(rocket.angleToMouse());
				// Устанавливаем скорость
				rocket.updateSpeed(80);
				// Смещение ракеты
				rocket.fly(t);
				// Отрисовка ракеты
				win.draw(rocket.get());
				// Отрисовка кнопки старта
				win.draw(startButton);
				win.draw(startText);
				win.display();
			}
			else if (lose) {
				if (loseFlag) {
					// Сброс всего
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
						// Если мышь наведена на кнопку - то текст белый
						restartText.setFillColor(Color::White);
						if (click) {
							loseFlag = true;
							// Если клик по кнопке - сброс характеристик и запуск
						}
					}
					else {
						// Если мышь не наведена на кнопку - то текст красный
						restartText.setFillColor(Color::Color(80, 0, 0));
					}
					/*
					Отрисовка взрыва ракеты
					*/
					if (bumIndex < 15) {
						bumIndex = rocket.bum(t);
					}
					// Подсчет очков
					scoreValue = uint32_t(position.x / win.getSize().x);
					// Подсчет рекорда
					if (scoreValue > highScore) highScore = scoreValue;
					win.clear();
					// Отрисовка звезд
					ConvexShape* stars = background.get();
					for (int i = 0; i < background.len; i++) win.draw(stars[i]);
					// Если гравитончки - то рисуем гравитончики
					if (problems == GRAVITYS) {
						Sprite* gravitys = gravityCloud.get();
						for (int i = 0; i < gravityCloud.len; i++) win.draw(gravitys[i]);
					} 
					// Если астероиды - то рисуем астероиды
					if (problems == ASTEROIDS) {
						Sprite* asteroids = asteroidCloud.get();
						for (int i = 0; i < asteroidCloud.len; i++) win.draw(asteroids[i]);
					}
					// Отрисовываем ракету
					win.draw(rocket.get());
					// Отрисовываем заглушку фон
					win.draw(tranBackground);
					// Выводим набранные очки
					ostringstream ss;
					ss << scoreValue;
					resultScore.setString(ss.str());
					resultScore.setOrigin(Vector2f(resultScore.getGlobalBounds().width / 2,
						resultScore.getGlobalBounds().height / 2));
					resultScore.setPosition(win.getSize().x / 2,
						win.getSize().y / 4);
					// Выводим максимальный результат
					ostringstream ss2;
					ss2 << "High score: " << highScore << endl;
					resultHighScore.setString(ss2.str());
					resultHighScore.setOrigin(Vector2f(resultHighScore.getGlobalBounds().width / 2,
						resultHighScore.getGlobalBounds().height / 2));
					resultHighScore.setPosition(win.getSize().x / 2,
						win.getSize().y / 4 + win.getSize().x / 10);
					win.draw(resultScore);
					win.draw(resultHighScore);
					// Отрисовка кнопки и текста на ней
					win.draw(restartButton);
					win.draw(restartText);
					win.display();
				}
			}
		}
	}
	return 0;
}