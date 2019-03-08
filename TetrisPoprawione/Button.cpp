#include "Button.h"
#include"Application.h"
#include "FontData.h"

Button::Button(sf::Vector2f pos, sf::Vector2f size, std::string const & text) {
	setSize(size);
	setPosition(pos);
	_pressed = false;
	_text.setPosition(pos + sf::Vector2f(20, 10));
	_text.setFont(FontData::instance.font);
	_text.setCharacterSize(30);
	_text.setFillColor(sf::Color::Black);
	_text.setString(text);
	setFillColor(sf::Color(150, 150, 150));
	setOutlineColor(sf::Color(100, 100, 100));
	setOutlineThickness(4);
}


bool Button::clicked() {
	bool p = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (_pressed) {
		_pressed = p;
		return false;
	} else {
		if (p) {
			sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_app->window());
			if (this->getGlobalBounds().contains(mousePos)) {
				_pressed = true;
				return true;
			} else
				return false;
		}		
	}
	return false;
}

void Button::draw(sf::RenderTarget & target) const {
	target.draw(*this);
	target.draw(_text);
}

void Button::setParentApp(Application * app) {
	_app = app;
}

