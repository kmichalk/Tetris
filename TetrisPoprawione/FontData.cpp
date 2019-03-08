#include "FontData.h"

FontData FontData::instance;

FontData::FontData() {
	font.loadFromFile("res\\font.ttf");
}
