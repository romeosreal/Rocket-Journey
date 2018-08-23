#pragma once
#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Funcs.h"

using namespace std;
using namespace sf;

class Background {
public:

	/*
	Конструктор фона.
	Принимает параметры:
	Количество звезд на небе
	Предельный размер звезды
	*/

	Background(const uint16_t l = 100, double size = 15) {
		this->size = size;
		this->len = l;
		obj = new ConvexShape[l];
	}

	/*
	Инициализация фона.
	Принимает параметры:
	Размер окна
	*/

	void init(Vector2u winSize) {
		this->winSize = winSize;
		for (int i = 0; i < 100; i++) {
			reinit(i);
			obj[i].setPosition(Vector2f(winSize.x * random(),
				winSize.y * (random() - 0.5) * 3));
			obj[i].setOrigin(Vector2f(obj[i].getGlobalBounds().width / 2,
				obj[i].getGlobalBounds().height / 2));
		}
	}

	/*
	Обработка фона.
	Принимает параметры:
	Время итерации
	Смещение
	*/

	void update(double t, Vector2f speed) {
		for (int i = 0; i < 100; i++) {
			Vector2f pos = obj[i].getPosition();
			pos.x += speed.x * t;
			pos.y += speed.y * t;
			if (pos.x < 0) {
				pos.x = winSize.x * (1 + random());
				pos.y = winSize.y * (random() - 0.5) * 3;
				reinit(i);
			}
			obj[i].setPosition(Vector2f(pos.x, pos.y));
		}
	}

	/*
	Получение массива звезд.
	*/

	ConvexShape* get() {
		return obj;
	}

	uint16_t len;
private:

	/*
	Сброс звезды по индексу.
	Принимает параметры:
	Индекс
	*/

	void reinit(uint16_t index) {
		obj[index].setFillColor(Color::Color(255 - random() * 100,
			255 - random() * 100,
			random() * 100, 255 - random() * 50));
		int Num = int(random() * 8) * 2 + 10;
		obj[index].setPointCount(Num);
		double sz = size * random() * random() + 5;
		for (int j = 0; j < Num; j++) {
			double angle = PI * 2 / Num * j;
			obj[index].setPoint(j, Vector2f(cos(angle) * (sz - sz * 0.5 * (j % 2)),
				sin(angle) * (sz - sz * 0.5 * (j % 2))));
		}
	}

	ConvexShape* obj;
	Vector2u winSize;
	double size;
};