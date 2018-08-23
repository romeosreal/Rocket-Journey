#pragma once
#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Funcs.h"

using namespace std;
using namespace sf;

class GravityCloud {
public:

	/*
	Конструктор облака гравитончиков.
	Принимает параметры:
	Путь до изображения
	Количество гравитончиков в длину
	Размер гравитончика
	Коеффицент притяжения
	Количество гравитончиков в высоту
	*/

	GravityCloud(String way, uint16_t l = 5,
		double size = 0.15, double strong = 15, uint16_t ySize = 5) {
		l *= ySize;
		gravityImg.loadFromFile(way);
		gravityImg.setSmooth(true);
		gravitys = new Sprite[l];
		mvY = new double[l];
		mvX = new double[l];
		this->len = l;
		this->size = size;
		this->ySize = ySize;
		this->strong = sq(strong);
		for (int i = 0; i < len; i++) {
			mvY[i] = 0;
			mvX[i] = 0;
			gravitys[i].setOrigin(Vector2f(gravitys[i].getGlobalBounds().width / 2,
				gravitys[i].getGlobalBounds().height / 2));
		}
	}

	/*
	Установка количества гравитончиков в высоту.
	Принимает параметры:
	Количество гравитончиков в высоту
	*/

	void setYLen(uint16_t ySize) {
		this->ySize = ySize;
	}

	/*
	Установка количества гравитончиков.
	Принимает параметры:
	Количество гравитончиков
	*/

	void setLength(uint16_t l) {
		this->len = l * this->ySize;
	}

	/*
	Установка размера гравитончика.
	Принимает параметры:
	Размер гравитончика
	*/

	void setSize(double size) {
		this->size = size;
	}

	/*
	Получение массива гравитончиков.
	*/

	Sprite* get() {
		return gravitys;
	}

	/*
	Получение силы гравитации.
	Принимает параметры:
	Позиция ракеты
	*/

	Vector2f getGravity(Vector2f pos1) {
		Vector2f gravity;
		for (int i = 0; i < len; i++) {
			Vector2f pos2 = gravitys[i].getPosition();
			double k = gravitys[i].getScale().y;
			gravity.x += -(1 / (sq((pos2.x - pos1.x) / winSize.x) + 1)) * strong * sgn(pos2.x - pos1.x) * k;
			gravity.y += 1 / (sq((pos2.y - pos1.y) / winSize.y) + 1) * strong * sgn(pos2.y - pos1.y) * k;
		}
		return gravity;
	}

	/*
	Установка размера окна.
	Принимает параметры:
	Размеры окна
	*/

	void updateWindow(Vector2u winSize) {
		this->winSize = winSize;
	}

	/*
	Инициализация гравитончиков.
	Принимает параметры:
	Размеры окна
	*/

	void init(Vector2u winSize) {
		updateWindow(winSize);
		for (int i = 0; i < len; i++) {
			gravitys[i].setTexture(gravityImg);
			gravitys[i].setOrigin(Vector2f(gravitys[i].getLocalBounds().width / 2,
				gravitys[i].getLocalBounds().height / 2));
			gravitys[i].setPosition(winSize.x + (i / ySize + random()) * winSize.x / ySize,
				((i % ySize) - 2 + random()) * winSize.y / ySize * 2);
			double scaleK = (size * this->winSize.y / gravityImg.getSize().y) * (1 - random() / 2);
			gravitys[i].setScale(scaleK, scaleK);
		}
	}

	/*
	Проверка на наличие точки внутри заданного гравитончика.
	Принимает параметры:
	Индекс гравитончика
	Позиция точки
	*/

	bool contains(uint16_t index, Vector2f pos) {
		Sprite obj = gravitys[index];
		double radius = obj.getScale().y * gravityImg.getSize().y / 2;
		if(sqrt(sq(pos.x - obj.getPosition().x) + 
			sq(pos.y - obj.getPosition().y)) < radius * 1.1) {
			return true;
		}
		return false;
	}

	/*
	Обработка гравитончиков.
	Принимает параметры:
	Время итерации
	Смещение
	*/

	void update(double t, Vector2f move) {
		for (uint16_t i = 0; i < len; i++) {
			Vector2f pos = gravitys[i].getPosition();
			mvY[i] += sq(random() - 0.5) * sgn(random() - 0.5);
			mvX[i] += sq(random() - 0.5) * sgn(random() - 0.5);
			gravitys[i].setPosition(pos.x + move.x * t + mvX[i], pos.y + move.y * t + mvY[i]);
		}
	}

	/*
	Сброс облака гравитончиков.
	*/

	void reinit() {
		gravitys = new Sprite[len];
		this->size = size;
		this->mvX = new double[len];
		this->mvY = new double[len];
		this->ySize = ySize;
		for (int i = 0; i < len; i++) {
			mvY[i] = 0;
			mvX[i] = 0;
			gravitys[i].setOrigin(Vector2f(gravitys[i].getGlobalBounds().width / 2,
				gravitys[i].getGlobalBounds().height / 2));
		}
	}

	uint16_t len;
private:
	Texture gravityImg;
	Sprite* gravitys;
	Vector2u winSize;
	double size;
	double strong;
	double* mvY;
	double* mvX;
	uint16_t ySize;
};