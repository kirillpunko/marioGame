#include <SFML/Graphics.hpp>

using namespace sf;

sf::View view;

View getPlayerViewCoordinates(float x, float y) {
	float tempX = x;
	float tempY = y;

	if (x < 640) tempX = 640;
	if (x > 9440) tempX = 9440;
	view.setCenter(tempX, 290);
	return view;
}

View changeView(){

	if (Keyboard::isKeyPressed(Keyboard::U)) {
		view.zoom(1.1f);
	}
	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.zoom(0.9f);
	}
	return view;
}
