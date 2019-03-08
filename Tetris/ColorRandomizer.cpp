#include "ColorRandomizer.h"
#include "Random.h"


ColorRandomizer::ColorRandomizer(std::vector<sf::Color> const & colors):
	_colors(colors),
	_lastIdx(-1),
	_prelastIdx(-1)
{
}

sf::Color ColorRandomizer::get() {
	int idx = Random::nextInt(_colors.size());
	while (idx == _lastIdx || idx == _prelastIdx)
		idx = Random::nextInt(_colors.size());
	_prelastIdx = _lastIdx;
	_lastIdx = idx;
	return _colors[idx];
}
