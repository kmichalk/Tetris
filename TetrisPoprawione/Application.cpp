#include "Application.h"
#include <thread>
#include "FontData.h"


Application::Application(std::string const & name):
	_window(sf::VideoMode(400, 800), name),
	_board(&_window, 10, 18, 40),
	_playButton({150, 400}, {100, 50}, "PLAY")
{
	_playButton.setParentApp(this);
	_headerText.setPosition(110,200);
	_headerText.setFont(FontData::instance.font);
	_headerText.setCharacterSize(60);
	_headerText.setFillColor(sf::Color::White);
	_scoreInfoText.setPosition(100,300);
	_scoreInfoText.setFont(FontData::instance.font);
	_scoreInfoText.setCharacterSize(30);
	_scoreInfoText.setFillColor(sf::Color::White);
	_scoreInfoText.setString("");
	_headerText.setString("TETRIS");
	_state = Init;
	_music.openFromFile("res\\music.wav");
	_music.setLoop(true);
}

Application::~Application() {
}


void Application::run() {
	while (_window.isOpen()) {
		process();
		redraw();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

sf::RenderWindow & Application::window() {
	return _window;
}

void Application::process() {
	processWindowEvents();
	if (_state == Play) {
		if (!_board.process()) {
			_state = GameOver;
			_headerText.setPosition(75, 200);
			_headerText.setString("GAME OVER");
			_scoreInfoText.setString("YOUR SCORE: " + std::to_string(_board.score()));
			_board.clear();
			_music.stop();
		}		
	}
	else {
		if (_playButton.clicked()) {
			//_music.play();
			_state = Play;
		}
	}
}


void Application::processWindowEvents() {
	sf::Event event;
	while (_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			_window.close();
		else if (event.type == sf::Event::KeyPressed) {
			_board.onKeyEvent(event.key);
		}
	}
}

void Application::redraw() {
	_window.clear();
	if (_state == Play)
		_board.draw(_window);
	else {
		_playButton.draw(_window);
		_window.draw(_headerText);
		_window.draw(_scoreInfoText);
	}
	_window.display();
}
