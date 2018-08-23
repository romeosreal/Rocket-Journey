#pragma once
#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Funcs.h"

using namespace std;
using namespace sf;

class AsteroidCloud {
public:

	/*
	Конструктор облака астероидов.
	Принимает параметры:
	Путь до картинки
	Предельная скорость вращения
	Количество астероидов в длину
	Размер астероида
	Количество астероидов в высоту
	*/

	AsteroidCloud(String way, double speed = 10, uint16_t l = 2,
		double size = 0.15, uint16_t ySize = 6) {
		l *= ySize;
		asteroidImg.loadFromFile(way);
		asteroidImg.setSmooth(true);
		asteroids = new Sprite[l];
		this->len = l;
		this->spd = speed;
		this->size = size;
		this->speed = new double[l];
		this->ySize = ySize;
	}

	/*
	Установка количества астероидов в одном столбце.
	Принимает параметры:
	Количество
	*/

	void setYLen(uint16_t ySize) {
		this->ySize = ySize;
	}

	/*
	Установка количества астероидов.
	Принимает параметры:
	Количество
	*/

	void setLength(uint16_t l) {
		this->len = l;
	}

	/*
	Установка размера.
	Принимает параметры:
	Размер
	*/

	void setSize(double size) {
		this->size = size;
	}

	/*
	Установка скорости.
	Принимает параметры:
	Скорость
	*/

	void setSpeed(double speed) {
		this->spd = speed;
		this->speed = new double[len];
	}

	/*
	Получение массива астероидов.
	*/

	Sprite* get() {
		return asteroids;
	}

	/*
	Запись размеров окна.
	Принимает параметры:
	Размеры окна
	*/

	void updateWindow(Vector2u winSize) {
		this->winSize = winSize;
	}

	/*
	Инициализация астероидов.
	Принимает параметры:
	Размеры окна
	*/

	void init(Vector2u winSize) {
		updateWindow(winSize);
		for (int i = 0; i < len; i++) {
			this->speed[i] = (random() * 2 - 1) * this->spd;
			asteroids[i].setOrigin(Vector2f(asteroids[i].getGlobalBounds().width / 2,
				asteroids[i].getGlobalBounds().height / 2));
			double scaleK = (size * this->winSize.y / asteroidImg.getSize().y) * (1 - random() / 2);
			asteroids[i].setScale(scaleK, scaleK);
			asteroids[i].setPosition(winSize.x + (i / ySize + random()) * winSize.x / ySize,
				((i % ySize) - 2 + random()) * winSize.y / ySize * 3);
			asteroids[i].setTexture(asteroidImg);
		}
	}

	/*
	Обработка астероидов.
	Принимает параметры:
	Время итерации
	Смещение
	*/

	void update(double t, Vector2f move) {
		for (uint16_t i = 0; i < len; i++) {
			asteroids[i].rotate(t * this->speed[i]);
			Vector2f pos = asteroids[i].getPosition();
			asteroids[i].setPosition(pos.x + move.x * t, pos.y + move.y * t);
		}
	}

	void reinit() {
		asteroids = new Sprite[len];
		this->size = size;
		this->speed = new double[len];
		this->ySize = ySize;
	}

	uint16_t len;
private:
	Texture asteroidImg;
	Sprite* asteroids;
	Vector2u winSize;
	double* speed;
	double spd;
	double size;
	double width;
	uint16_t ySize;
};