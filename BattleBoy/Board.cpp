
#include "Globals.h"

Board::Board()
{

}

void Board::draw(Boy::Graphics *g)
{
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();

	g->pushTransform();

	g->drawLine(w*0.4, 0, w*0.4, h*0.4);
	g->drawLine(w*0.6, 0, w*0.6, h*0.4);

	g->drawLine(w*0.6, h*0.4, w, h*0.4);
	g->drawLine(w*0.6, h*0.6, w, h*0.6);

	g->drawLine(w*0.6, h*0.6, w*0.6, h);
	g->drawLine(w*0.4, h*0.6, w*0.4, h);

	g->drawLine(0, h*0.6, w*0.4, h*0.6);
	g->drawLine(0, h*0.4, w*0.4, h*0.4);

	g->popTransform();
}
