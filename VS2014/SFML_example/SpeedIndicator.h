#pragma once
#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Funcs.h"

using namespace std;
using namespace sf;

class SpeedIndicator {
public:

	/*
	Конструктор индикатора скорости.
	Принимает параметры:
	Отступ слева
	Отступ сверху
	Ширина
	Высота
	*/

	SpeedIndicator(double left = 0.05, double top = 0.95, double width = 0.05, double height = 0.015) {
		this->left = left;
		this->top = top;
		this->blockWidth = width;
		this->blockHeight = height;
		windowSize = Vector2u(1400, 700);
		calculateSize();
	}

	/*
	Установка размеров окна.
	*/

	void updateWindow(Vector2u window) {
		windowSize = window;
	}

	/*
	Расчет размера.
	*/

	void calculateSize() {
		pLeft = left * windowSize.y;
		pTop = top * windowSize.y;
		pBlockWidth = blockWidth * windowSize.y;
		pBlockHeight = blockHeight * windowSize.y;
		for (int i = 0; i < 3; i++) {
			indic[i].setSize(Vector2f(pBlockWidth, pBlockHeight));
			indic[i].setPosition(pLeft, pTop - (pBlockHeight + 6) * i);
			indic[i].setFillColor(Color::Transparent);
			indic[i].setOutlineThickness(2);
			indic[i].setOutlineColor(Color::Color(80, 80, 80));
		}
	}

	/*
	Получение массива элементов.
	*/

	RectangleShape* get() {
		return indic;
	}

	/*
	Отображение скорости.
	*/

	void updateSpeed(int index) {
		for (int i = 0; i < index; i++) indic[i].setFillColor(Color::Green);
		for (int i = index; i < 3; i++) indic[i].setFillColor(Color::Transparent);
	}

private:
	double left, pLeft;
	double top, pTop;
	double blockWidth, pBlockWidth;
	double blockHeight, pBlockHeight;
	Vector2u windowSize;
	RectangleShape indic[3];
};