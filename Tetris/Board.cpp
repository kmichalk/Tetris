#include "Board.h"
#include "Random.h"
#include "FontData.h"
#include <iostream>



void Board::init(size_t w, size_t h) {
	_rows = h;
	_cols = w;
	_data = 
		std::vector<std::vector<sf::RectangleShape*>>(
			w, 
			std::vector<sf::RectangleShape*>(
				h, 
				nullptr
			)
		);
}

void Board::moveDown(float time) {
	for (auto& fb : _falling.blocks)
		fb.block->move(_fallSpeed*time);
}

void Board::fall(float time) {
	moveDown(time);
	auto winSize = _window->getSize();
	for (auto& fb : _falling.blocks) {		
		sf::RectangleShape* back = lastInColumn(fb.column);
		/*sf::RectangleShape* back = nullptr;
		auto& col = _data[fb.column];
		for (int i = col.size() - 1; i >= 0; --i) {
			if (col[i] != nullptr) {
				back = col[i];
				break;
			}
		}*/
		auto pos = fb.block->getPosition();
		if (back == nullptr) {
			if (pos.y >= _blockSize.y*(_rows-1)) {
				onFallen();
				break;
			}
		} else {
			auto backPos = back->getPosition();
			if (pos.y >= (backPos.y - _blockSize.y)) {
				onFallen();
				break;
			}
		}
	}
}


void Board::checkRows() {
	bool removed;
	int nrows = 0;
	do {
		removed = false;
		for (size_t row = 0; row < _rows; ++row) {
			if (rowFull(row)) {
				removeRow(row);
				++_score;
				removed = true;
				++nrows;
			}
		}
	} while (removed);
	if (nrows > 0) {
		while (nrows > 1) {
			--nrows;
			_score += nrows;
			//updateFallSpeed();
		}
		_scoreText.setString(std::to_string(_score));
	}
}

bool Board::rowFull(size_t row) {
	for (auto& col : _data) {
		if (row >= col.size())
			return false;
		else if (col[row] == nullptr)
			return false;
	}
	return true;
}

void Board::removeRow(size_t row) {
	for (auto& col : _data) {
		//if (row < col.size()) {
			delete col[row];
			for (size_t i = row + 1; i < col.size(); ++i) {
				sf::RectangleShape* b = col[i];
				if (b != nullptr)
					b->move({0, _blockSize.y});				
				col[i - 1] = b;
			}
			col.back() = nullptr;
		//}
	}
}

void Board::processGenerate() {
	_fallSpeed = _defaultFallSpeed;
	_falling.blocks.clear();
	nextBlockType();
	//auto color = sf::Color(0, 162, 232);//_colors.get();
	//auto color = sf::Color(207, 255, 92);
	auto color = sf::Color(60, 60, 60);
	switch (_blockType) {
	case 0: 
		return createBlock_O(color);
	case 1:
		return createBlock_I(color);
	case 2:
		return createBlock_T(color);
	case 3:
		return createBlock_L1(color);
	case 4:
		return createBlock_S1(color);
	case 5:
		return createBlock_L2(color);
	case 6:
		return createBlock_S2(color);
	default:
		break;
	}
	randomRotateFalling();
}

void Board::createBlock_O(sf::Color const& color) {
	int col = Random::nextInt(_cols - 1);
	_falling.minCol = col;
	_falling.maxCol = col + 1;
	createBlock(col, 2, color);
	createBlock(col+1, 2, color);
	createBlock(col, 1, color);
	createBlock(col+1, 1, color);
}

void Board::createBlock_S1(sf::Color const& color) {
	int col = Random::nextInt(_cols - 1);
	_falling.minCol = col;
	_falling.maxCol = col + 1;
	createBlock(col + 1, 3, color);
	createBlock(col, 2, color);
	createBlock(col + 1, 2, color);
	createBlock(col, 1, color);
}

void Board::createBlock_S2(sf::Color const& color) {
	int col = Random::nextInt(_cols - 1);
	_falling.minCol = col;
	_falling.maxCol = col + 1;
	createBlock(col, 3, color);
	createBlock(col, 2, color);
	createBlock(col + 1, 2, color);
	createBlock(col + 1, 1, color);
}

void Board::createBlock_L1(sf::Color const& color) {
	int col = Random::nextInt(_cols - 1);
	_falling.minCol = col;
	_falling.maxCol = col + 1;
	createBlock(col + 1, 3, color);
	createBlock(col + 1, 2, color);
	createBlock(col, 1, color);
	createBlock(col + 1, 1, color);
}

void Board::createBlock_L2(sf::Color const& color) {
	int col = Random::nextInt(_cols - 1);
	_falling.minCol = col;
	_falling.maxCol = col + 1;
	createBlock(col, 3, color);
	createBlock(col, 2, color);
	createBlock(col, 1, color);
	createBlock(col + 1, 1, color);
}

void Board::createBlock_I(sf::Color const& color) {
	int col = Random::nextInt(_cols);
	_falling.minCol = col;
	_falling.maxCol = col;
	createBlock(col, 4, color);
	createBlock(col, 3, color);
	createBlock(col, 2, color);
	createBlock(col, 1, color);
}

void Board::createBlock_T(sf::Color const & color) {
	int col = Random::nextInt(_cols - 2);
	_falling.minCol = col;
	_falling.maxCol = col + 2;
	createBlock(col, 2, color);
	createBlock(col+1, 2, color);
	createBlock(col+2, 2, color);
	createBlock(col+1, 1, color);
}

void Board::rotateFallingRelativeTo(size_t idx) {
	FallingBlocks temp = _falling;
	for (auto& fb : temp.blocks)
		fb.block = new sf::RectangleShape(*fb.block);

	sf::Vector2f relpos = temp.blocks[1].block->getPosition();
	for (auto& fb : temp.blocks)
		rotateBlock(fb, relpos);
	updateFallingMinMaxColumns(temp);
	moveFallingFromWall(temp);
	for (auto& fb : temp.blocks) {
		int row = rowByPosition(fb.block->getPosition());
		if (row < _rows && _data[fb.column][row] != nullptr) {
			for (auto& fbb : temp.blocks)
				delete fbb.block;
			return;
		}
	}
	for (auto& fb : _falling.blocks)
		delete fb.block;
	_falling = temp;
	/*sf::Vector2f relpos = _falling.blocks[1].block->getPosition();
	for (auto& fb : _falling.blocks)
		if (!canRotateBlock(fb, relpos))
			return;
	for (auto& fb : _falling.blocks)
		rotateBlock(fb, relpos);
	updateFallingMinMaxColumns();
	moveFallingFromWall();*/
}

void Board::createBlock(int col, int topdist, sf::Color color) {
	sf::RectangleShape* b = new sf::RectangleShape({_blockSize.x - 6, _blockSize.y - 6});
	b->setFillColor(color);
	//b->setOutlineColor(sf::Color(60,60,60));
	b->setOutlineColor(sf::Color(0, 162, 232));
	//b->setOutlineColor(sf::Color(207, 255, 92));
	//b->setOutlineColor(sf::Color(255, 71, 133));
	//b->setOutlineColor(sf::Color(255,186,21));
	b->setOutlineThickness(3);
	b->setOrigin(-3, -3);
	b->setPosition(col*_blockSize.x, -topdist * _blockSize.y);
	_falling.blocks.push_back({b, col});
}

sf::RectangleShape * Board::lastInColumn(size_t col) {
	auto& column = _data[col];
	for (int i = column.size() - 1; i >= 0; --i) {
		if (column[i] != nullptr) {
			if (allFallingOver(column[i], col))
				return column[i];
		}
	}
	return nullptr;
}

void Board::moveFallingHorizontally(FallingBlocks& falling, int columnOffset) {
	if ((int)falling.minCol + columnOffset < 0)
		return;
	if ((int)falling.maxCol + columnOffset > _data.size() - 1)
		return;
	for (auto& fb : falling.blocks) {
		int row = _rows - 1 - int(fb.block->getPosition().y / _blockSize.y + 0.1);
		if (row >= 0 & row < _rows) {
			if (_data[fb.column + columnOffset][row] != nullptr)
				return;
		}
	}
	falling.minCol += columnOffset;
	falling.maxCol += columnOffset;
	for (auto& fb : falling.blocks) {
		fb.column += columnOffset;
		fb.block->move(columnOffset*_blockSize.x, 0);
	}
}

void Board::rotateFalling() {
	if (_blockType != 0)
		rotateFallingRelativeTo(1);
}

void Board::speedUp() {
	if (_fallSpeed != _defaultFallSpeed * 10.0f)
		_fallSpeed.y *= 10.0f;
}

void Board::randomRotateFalling() {
	int rotations = Random::nextInt(4);
	for (int i = 0; i < rotations; ++i)
		rotateFalling();
}

void Board::putFallingToGrid() {	
	for (auto& fb : _falling.blocks) {
		auto pos = fb.block->getPosition();
		int row = rowByPosition(pos);
		if (row >= _rows) {
			_gameOver = true;
			return;
		}
		_data[fb.column][row] = fb.block;
	}
}

void Board::onFallen() {
	snapFallingPositions();
	putFallingToGrid();
	_fallSpeed = _defaultFallSpeed;
	checkRows();
	processGenerate();
}

void Board::snapFallingPositions() {
	for (auto& fb : _falling.blocks)
		fb.block->setPosition(roundTo(fb.block->getPosition(), _blockSize.y));
}

void Board::rotateBlock(FallingBlock & b, sf::Vector2f const & origin) {
	sf::Vector2f pos = b.block->getPosition();
	sf::Vector2f dir = pos - origin;
	sf::Vector2f newdir = {-dir.y, dir.x};
	pos = pos - dir + newdir;
	b.block->setPosition(pos);
	b.column = columnByPosition(pos);
}

int Board::columnByPosition(sf::Vector2f const & pos) {
	if (pos.x < 0)
		return int(pos.x / _blockSize.x - 0.1);
	else
		return int(pos.x / _blockSize.x + 0.1);
}

int Board::rowByPosition(sf::Vector2f const & pos) {
	return _rows - 1 - int(pos.y / _blockSize.y + 0.1);
}

void Board::updateFallingMinMaxColumns(FallingBlocks& falling) {
	int min = _data.size();
	int max = 0;
	for (auto& fb : falling.blocks) {
		if (fb.column < min)
			min = fb.column;
		if (fb.column > max)
			max = fb.column;
	}
	falling.minCol = min;
	falling.maxCol = max;
}

void Board::moveFallingFromWall(FallingBlocks& falling) {
	if (falling.minCol < 0)
		moveFallingHorizontally(falling, -falling.minCol);
	if (falling.maxCol >= _data.size())
		moveFallingHorizontally(falling, -(falling.maxCol - (int)_data.size() + 1));
}

int Board::randomBlockType() {
	int type = Random::nextInt(5);
	if (type == 3) {
		if (Random::nextBool())
			return 3;
		else
			return 5;
	}
	else if (type == 4){
		if (Random::nextBool())
			return 4;
		else
			return 6;
	}
	else
		return type;
}

void Board::nextBlockType() {
	_blockType = _nextBlockType;
	_nextBlockType = randomBlockType();
	_nextShape.setTexture(&_images[_nextBlockType]);
}

bool Board::allFallingOver(sf::RectangleShape * r, int col) {
	float y = r->getPosition().y;
	for (auto& fb : _falling.blocks) {
		if (fb.column == col && fb.block->getPosition().y >= y)
			return false;
	}
	return true;
}

float Board::roundTo(float value, float precision) {
	return floor(value/precision) * precision;
}

sf::Vector2f Board::roundTo(sf::Vector2f v, float precision) {
	return {roundTo(v.x, precision), roundTo(v.y, precision)};
}


Board::Board(sf::RenderWindow* window, size_t w, size_t h, unsigned blockSize):
	_window(window),
	_blockSize(float(blockSize), float(blockSize)),
	_colors({
		sf::Color(115,235,115), 
		sf::Color(125,154,255), 
		sf::Color(115,240,210), 
		sf::Color(200,105,240), 
		sf::Color(250,110,110), 
		sf::Color(250,205,75)})
{
	init(w, h);
	init(w, h);
	_gameOver = false;
	_bottomBar.setSize({_blockSize.x*_cols, 80});
	_bottomBar.setPosition(0, _blockSize.y*_rows);
	_bottomBar.setFillColor(sf::Color(60,60,60));
	_scoreText.setPosition(20, _blockSize.y*_rows + 20);
	_scoreText.setFont(FontData::instance.font);
	_scoreText.setCharacterSize(30);
	_scoreText.setFillColor(sf::Color(0,162,232));
	_scoreText.setString("0");
	_nextShape.setPosition(280, _blockSize.y*_rows + 15);
	_nextShape.setSize({104, 52});
	for (int i = 0; i < 7; ++i) {
		_images[i].loadFromFile("res\\" + std::to_string(i) + ".bmp");
	}
	nextBlockType();
	_blockType = _nextBlockType;
	_score = 0;
	_defaultFallSpeed = {0, 100};
	_fallSpeed = _defaultFallSpeed;
}

Board::~Board() {
	clear();
}

void Board::onKeyEvent(sf::Event::KeyEvent event) {
	switch (event.code) {
	case sf::Keyboard::Left:
		return moveFallingHorizontally(_falling, -1);
	case sf::Keyboard::Right:
		return moveFallingHorizontally(_falling, 1);
	case sf::Keyboard::Down:
		return speedUp();
	case sf::Keyboard::Up:
		return rotateFalling();
	default:
		break;
	}
}

void Board::draw(sf::RenderWindow & window) {
	for (auto& v : _data) {
		for (auto& b : v)
			if (b != nullptr)
				window.draw(*b);
	}
	for (auto& fb : _falling.blocks)
		window.draw(*fb.block);
	window.draw(_bottomBar);
	window.draw(_scoreText);
	window.draw(_nextShape);
}

void Board::clear() {
	for (auto& col : _data) {
		for (auto& b : col) {
			delete b;
			b = nullptr;
		}
	}
	_falling.blocks.clear();
	_data = std::vector<std::vector<sf::RectangleShape*>>(_cols, std::vector<sf::RectangleShape*>(_rows, nullptr));
	_gameOver = false;
	_score = 0;
	//updateFallSpeed();
	nextBlockType();
	_blockType = _nextBlockType;
	_scoreText.setString("0");
}


void Board::setFallSpeed(float value) {
	_fallSpeed = {0, value};
	_defaultFallSpeed = _fallSpeed;
}

unsigned Board::score() {
	return _score;
}

bool Board::process() {
	if (_falling.blocks.empty())
		processGenerate();
	/*float time = _frameTimer.measure();
	_frameTimer.reset();*/
	/*if (_createTimer.measure() >= _createPeriod) {
		_createTimer.reset();
		processGenerate();
	}*/
	fall(0.01);
	return !_gameOver;
}
