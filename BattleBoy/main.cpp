#include "Globals.h"

int main(int argc, char* argv[])
{
	// initialize the platform:
	Boy::Environment::instance()->init(
		BattleBoy::instance(),
		1024, // width
		768, // height
		false, // fullscreen
		"demo1", // window title
		"demo1.dat", // persistence layer file name
		NULL); // no encryption

	// start the main loop
	Boy::Environment::instance()->startMainLoop();

	// destroy the game:
	BattleBoy::destroy();

	// destroy the environment:
	Boy::Environment::instance()->destroy();

	return 0;
}

