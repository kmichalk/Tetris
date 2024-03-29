#include "Random.h"


Random::RandomImpl Random::impl = {};


Random::RandomImpl::RandomImpl():
	rd(),
	distf(0.0f, 1.0f),
	engine(rd())
{
}

int Random::nextInt() {
	return impl.engine();
}

int Random::nextInt(int bound) {
	return impl.engine() % bound;
}

int Random::nextInt(int min, int max) {
	return (impl.engine() % (max - min + 1)) + min;
}

bool Random::nextBool() {
	return bool(nextInt(2));
}
