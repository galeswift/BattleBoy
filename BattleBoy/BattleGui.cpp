#include "Globals.h"

void BattleGui::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{	
}

void BattleGui::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
}

void BattleGui::mouseMove(Boy::Mouse *mouse)
{	
}

void BattleGui::mouseButtonDown(Boy::Mouse *mouse, Boy::Mouse::Button button, int clickCount)
{ 
}

void BattleGui::mouseButtonUp(Boy::Mouse *mouse, Boy::Mouse::Button button)
{
	switch ( button )
	{

	case Boy::Mouse::BUTTON_LEFT:
		break ;

	case Boy::Mouse::BUTTON_MIDDLE:
		break ;

	case Boy::Mouse::BUTTON_RIGHT:
		break ;

	default:
		debug_con << "handle_mouse_up ignored '" << static_cast<int>( button )
			<< "'" << "" ;
		break ;

	}
}

void BattleGui::mouseWheel(Boy::Mouse *mouse, int wheelDelta)
{ 
}

void BattleGui::mouseEnter(Boy::Mouse *mouse)
{
}

void BattleGui::mouseLeave(Boy::Mouse *mouse)
{
}

void BattleGui::update( float dt )
{
}


void BattleGui::init()
{
}