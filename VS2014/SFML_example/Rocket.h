#pragma once
#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Funcs.h"

using namespace std;
using namespace sf;

class Rocket {
public:

	/*
	Конструктор ракеты.
	Параметры:
	Путь до ракеты
	Позиция по x
	Позиция по y
	*/

	Rocket(String way, String way2, double x = 0, double y = 0) {
		currentAngle = 90;
		rocketImg.loadFromFile(way);
		rocketImg.setSmooth(true);
		rocket.setTexture(rocketImg);
		rocket.setOrigin(Vector2f(rocket.getGlobalBounds().width / 2,
			rocket.getGlobalBounds().height / 2));
		rocket.setPosition(x, y);
		bumImg.loadFromFile(way2);
		bumImg.setSmooth(true);
		bumIndex = 0;
		bumT = 0;
	}

	/*
	Взрыв ракеты.
	Принимает параметры:
	Время итерации
	*/

	int bum(double t) {
		bumT += t;
		if (bumT > 0.05) {
			int posx = 64 * (bumIndex % 4),
				posy = 64 * int(bumIndex / 4);
			rocket.setTexture(bumImg);
			rocket.setTextureRect(IntRect(Vector2i(posx, posy), Vector2i(64, 64)));
			rocket.setOrigin(rocket.getLocalBounds().width / 2, 
				rocket.getLocalBounds().height / 2);
			rocket.setScale(1, 1);
			bumIndex++;
			bumT = 0;
		}
		return bumIndex;
	}

	/*
	Масштабирование размера ракеты под экран.
	Принимает параметры:
	Ширина ракеты в процентах от экрана
	*/

	void scale(double width = 0.1) {
		double scaleK = (width * winSize.y / rocketImg.getSize().y);
		rocket.setScale(scaleK, scaleK);
	}

	/*
	Получение спрайта ракеты.
	*/

	Sprite get() {
		return rocket;
	}

	/*
	Установка угла под которым ракета будет повернута.
	Принимает параметры:
	Угол
	*/

	void setRotate(double angle) {
		if (rocket.getGlobalBounds().contains(Vector2f(mouse))) {
			return;
		}
		if (rocket.getPosition().x > mouse.x) currentAngle = 0.05 *
			(atan2(mouse.y - rocket.getPosition().y,
				40) / PI * 180 + 90) + currentAngle * 0.95;
		else currentAngle = 0.05 * angle + currentAngle * 0.95;
		rocket.setRotation(currentAngle);
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
	Запись позиции мыши.
	Принимает параметры:
	Позиция мыши
	*/

	void updateMouse(Vector2i mouse) {
		this->mouse = mouse;
	}

	/*
	Вычисления угла на мышь.
	*/

	double angleToMouse() {
		return atan2(mouse.y - rocket.getPosition().y,
			mouse.x - rocket.getPosition().x) / PI * 180 + 90;
	}

	/*
	Запись скорости ракеты.
	Принимает параметры:
	Скорость
	*/

	void updateSpeed(double speed) {
		this->speed = speed;
	}

	/*
	Перемещение ракеты.
	Принимает параметры:
	Время итерации
	*/

	void fly(double t) {
		rocket.move(Vector2f(0, -t * cos(rocket.getRotation() / 180 * PI) * winSize.y * speed / 80 + gravity.y));
	}

	/*
	Установка гравитации.
	Принимает параметры:
	Сила гравитации
	*/

	void setGravity(Vector2f gravity) {
		this->gravity = gravity;
	}

	/*
	Сброс параметров ракеты.
	*/

	void reinit() {
		rocket.setPosition(rocket.getPosition().x, winSize.y / 2);
		currentAngle = 90;
		gravity = Vector2f(0, 0);
		bumIndex = 0;
		rocket.setTexture(rocketImg);
		rocket.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(110, 177)));
	}

private:
	Texture rocketImg;
	Texture bumImg;
	Sprite rocket;
	Vector2u winSize;
	Vector2i mouse;
	Vector2f gravity;
	double currentAngle;
	double speed;
	int bumIndex;
	double bumT;
};