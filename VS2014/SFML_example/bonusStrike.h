#pragma once
#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Funcs.h"

using namespace std;
using namespace sf;

class BonusStrike {
public:
	BonusStrike(String way, double radius = 0.1, double mvX = 0.05, double smallSize = 0.05) {
		bonusImg.loadFromFile(way);
		bonusImg.setSmooth(true);
		bonus.setTexture(bonusImg);
		bonus.setOrigin(Vector2f(bonus.getLocalBounds().width / 2,
			bonus.getLocalBounds().height / 2));
	}

	void scale(double width = 0.05) {
		double scaleK = (width * winSize.x / bonusImg.getSize().x);
		bonus.setScale(scaleK, scaleK);
	}

	Sprite get() {
		return bonus;
	}

	void add() {
		bonus.setPosition(winSize.x * (random() + 1), winSize.y * (random()));
	}

	void updateWindow(Vector2u winSize) {
		this->winSize = winSize;
	}

	void update(double t, Vector2f move) {
		Vector2f pos = bonus.getPosition();
		bonus.setPosition(pos.x + move.x * t, pos.y + move.y * t);
	}
private:
	Texture bonusImg;
	Sprite bonus;
	Vector2u winSize;
};