
#include "Globals.h"

Map::Map()
{

}

void Map::draw(Boy::Graphics *g)
{	
	g->pushTransform();
		g->translate(512.0f, 384.0f);
		g->drawImage(Boy::Environment::getImage("IMAGE_MAP"));
	g->popTransform();
}
