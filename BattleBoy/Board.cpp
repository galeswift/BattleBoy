
#include "Globals.h"

Board::Board()
{

}

void Board::draw(Boy::Graphics *g)
{
	int w = Boy::Environment::screenWidth();
	int h = Boy::Environment::screenHeight();

	g->pushTransform();

	g->drawLine(int(w*0.4),	0,			int(w*0.4), int(h*0.4));
	g->drawLine(int(w*0.6),	0,			int(w*0.6), int(h*0.4));

	g->drawLine(int(w*0.6),	int(h*0.4), w,			int(h*0.4));
	g->drawLine(int(w*0.6),	int(h*0.6), w,			int(h*0.6));

	g->drawLine(int(w*0.6),	int(h*0.6), int(w*0.6),	h);
	g->drawLine(int(w*0.4),	int(h*0.6), int(w*0.4), h);

	g->drawLine(0,			int(h*0.6), int(w*0.4),	int(h*0.6));
	g->drawLine(0,			int(h*0.4), int(w*0.4),	int(h*0.4));

	g->popTransform();
}
