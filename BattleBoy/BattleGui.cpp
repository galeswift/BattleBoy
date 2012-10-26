#include "Globals.h"

using namespace CEGUI ;


// Change according to your installation path:
const std::string & CEGUIInstallBasePath =
"../libs/CEGUI/CEGUI-0.7.7" ;

/*
const std::string & CEGUIInstallBasePath = "/usr" ;
*/

uint boyToCEScan( Boy::Keyboard::Key inKey )
{
	uint result = -1;
	switch( inKey )
	{
	case Boy::Keyboard::KEY_BACKSPACE:
		result = 0x0E;
		break;
	case Boy::Keyboard::KEY_DELETE:
		result = 0xD3;
		break;
	case Boy::Keyboard::KEY_TAB:
		result = 0x0F;
		break;
	}

	return result;
}

void BattleGui::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{	
	CEGUI::System::getSingleton().injectKeyUp( boyToCEScan(key) ) ;
}

void BattleGui::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
	CEGUI::System::getSingleton().injectKeyDown( boyToCEScan(key) ) ;
	if( (key ^ -1) == 0 )
	{
		CEGUI::System::getSingleton().injectChar( unicode ) ;
	}
}

void BattleGui::mouseMove(Boy::Mouse *mouse)
{	
	CEGUI::System::getSingleton().injectMousePosition(
				static_cast<float>( mouse->getPosition().x ), static_cast<float>( mouse->getPosition().y ) ) ;
}

void BattleGui::mouseButtonDown(Boy::Mouse *mouse, Boy::Mouse::Button button, int clickCount)
{ 
	switch ( button )
	{

	case Boy::Mouse::BUTTON_LEFT:
		CEGUI::System::getSingleton().injectMouseButtonDown( CEGUI::LeftButton ) ;
		break ;

	case Boy::Mouse::BUTTON_MIDDLE:
		CEGUI::System::getSingleton().injectMouseButtonDown( CEGUI::MiddleButton ) ;
		break ;

	case Boy::Mouse::BUTTON_RIGHT:
		CEGUI::System::getSingleton().injectMouseButtonDown( CEGUI::RightButton) ;
		break ;

	default:
		debug_con << "handle_mouse_down ignored '" << static_cast<int>( button )
			<< "'" << "" ;
		break ;

	}
}

void BattleGui::mouseButtonUp(Boy::Mouse *mouse, Boy::Mouse::Button button)
{
	switch ( button )
	{

	case Boy::Mouse::BUTTON_LEFT:
		CEGUI::System::getSingleton().injectMouseButtonUp( CEGUI::LeftButton ) ;
		break ;

	case Boy::Mouse::BUTTON_MIDDLE:
		CEGUI::System::getSingleton().injectMouseButtonUp( CEGUI::MiddleButton ) ;
		break ;

	case Boy::Mouse::BUTTON_RIGHT:
		CEGUI::System::getSingleton().injectMouseButtonUp( CEGUI::RightButton ) ;
		break ;

	default:
		debug_con << "handle_mouse_up ignored '" << static_cast<int>( button )
			<< "'" << "" ;
		break ;

	}
}

void BattleGui::mouseWheel(Boy::Mouse *mouse, int wheelDelta)
{ 
	CEGUI::System::getSingleton().injectMouseWheelChange( (float)wheelDelta ) ;
}

void BattleGui::mouseEnter(Boy::Mouse *mouse)
{
}

void BattleGui::mouseLeave(Boy::Mouse *mouse)
{
}

void BattleGui::update( float dt )
{

	// Inject the time that passed since the last call:
	CEGUI::System::getSingleton().injectTimePulse( static_cast<float>(
		dt ) ) ; 
}

void set_CEGUI_paths()
{

	// Initialises the required directories for the DefaultResourceProvider:
	CEGUI::ResourceProvider *resProvider = CEGUI::System::getSingleton().getResourceProvider();
	CEGUI::DefaultResourceProvider* defaultResProvider = static_cast<CEGUI::DefaultResourceProvider*>( resProvider ) ;

	const std::string CEGUIInstallSharePath = CEGUIInstallBasePath + "/datafiles/" ;

	// For each resource type, sets a corresponding resource group directory:

	debug_con << "Using scheme directory '" << CEGUIInstallSharePath + "schemes/"
		<< "'" << "" ;

	defaultResProvider->setResourceGroupDirectory( "schemes", 
		std::string(CEGUIInstallSharePath + "schemes/").c_str() ) ;

	defaultResProvider->setResourceGroupDirectory( "imagesets",
		std::string(CEGUIInstallSharePath + "imagesets/").c_str() ) ;

	defaultResProvider->setResourceGroupDirectory( "fonts",
		std::string(CEGUIInstallSharePath + "fonts/").c_str() ) ;

	defaultResProvider->setResourceGroupDirectory( "layouts",
		std::string(CEGUIInstallSharePath + "layouts/").c_str() ) ;

	defaultResProvider->setResourceGroupDirectory( "looknfeels",
		std::string(CEGUIInstallSharePath + "looknfeel/").c_str() ) ;

	defaultResProvider->setResourceGroupDirectory( "lua_scripts",
		std::string(CEGUIInstallSharePath + "lua_scripts/").c_str() ) ;

	defaultResProvider->setResourceGroupDirectory( "schemas",
		std::string(CEGUIInstallSharePath + "xml_schemas/").c_str() ) ;

	defaultResProvider->setResourceGroupDirectory( "animations",
		std::string(CEGUIInstallSharePath + "animations/").c_str() ) ;

	// Sets the default resource groups to be used:
	CEGUI::Imageset::setDefaultResourceGroup( "imagesets" ) ;
	CEGUI::Font::setDefaultResourceGroup( "fonts" ) ;
	CEGUI::Scheme::setDefaultResourceGroup( "schemes" ) ;
	CEGUI::WidgetLookManager::setDefaultResourceGroup( "looknfeels" ) ;
	CEGUI::WindowManager::setDefaultResourceGroup( "layouts" ) ;
	CEGUI::ScriptModule::setDefaultResourceGroup( "lua_scripts" ) ;
	CEGUI::AnimationManager::setDefaultResourceGroup( "animations" ) ;

	// Set-up default group for validation schemas:
	CEGUI::XMLParser * parser = CEGUI::System::getSingleton().getXMLParser() ;
	if ( parser->isPropertyPresent( "SchemaDefaultResourceGroup" ) )
		parser->setProperty( "SchemaDefaultResourceGroup", "schemas" ) ;

}


WindowManager & init_CEGUI( )
{

	debug_con << " - initializing CEGUI" << "" ;

	set_CEGUI_paths() ;
	SchemeManager::getSingleton().create( "TaharezLook.scheme" ) ;

	System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" ) ;

	FontManager::getSingleton().create( "DejaVuSans-10.font" ) ;

	return WindowManager::getSingleton() ;

}


Window & create_background_window( WindowManager & winManager )
{

	/*
	* Here we will use a StaticImage as the root, then we can use it to place a
	* background image:
	*
	*/
	Window & backgroundWin = * winManager.createWindow( "TaharezLook/StaticImage",
		"MyBackgroundWin" ) ;

	// Fine-tunes this background window:

	// Spread over the full CEGUI window:
	backgroundWin.setPosition( UVector2( cegui_reldim(0), cegui_reldim(0) ) ) ;
	backgroundWin.setSize( UVector2( cegui_reldim(1), cegui_reldim(1) ) ) ;

	// Disables the frame and standard background:
	backgroundWin.setProperty( "FrameEnabled", "false" ) ;
	backgroundWin.setProperty( "BackgroundEnabled", "false" ) ;

	// Finally sets the previously loaded background image:
	backgroundWin.setProperty( "Image", "set:BackgroundImage image:full_image" ) ;

	// Installs this as the root GUI sheet:
	System::getSingleton().setGUISheet( &backgroundWin ) ;

	return backgroundWin ;

}


MultiColumnList & create_multi_column_list( WindowManager & winManager,
										   Window & backgroundWin )
{

	// Creates a multi-column list:
	MultiColumnList & mcl = * static_cast<MultiColumnList*>(
		winManager.createWindow( "TaharezLook/MultiColumnList", "MyMainList" ) ) ;

	backgroundWin.addChildWindow( &mcl ) ;

	mcl.setPosition( UVector2( cegui_reldim(0.01f), cegui_reldim(0.1f) ) ) ;
	mcl.setSize( UVector2( cegui_reldim(0.6f), cegui_reldim(0.4f) ) ) ;

	// We want three columns here:
	mcl.addColumn( "First column",  /* id */ 1, /* width */ cegui_reldim(0.5f) ) ;
	mcl.addColumn( "Second column", /* id */ 2, /* width */ cegui_reldim(0.3f) ) ;
	mcl.addColumn( "Third column",  /* id */ 3, /* width */ cegui_reldim(0.2f) ) ;

	// Adds two entries to that list:

	ListboxTextItem & firstRowItem1 = * new ListboxTextItem( "First row 1" ) ;
	ListboxTextItem & firstRowItem2 = * new ListboxTextItem( "First row 2" ) ;
	ListboxTextItem & firstRowItem3 = * new ListboxTextItem( "First row 3" ) ;

	/*
	* Sets the selection brush to use for this item (blue background when
	* selected); the second row will not react when selected.
	*/
	firstRowItem1.setSelectionBrushImage(
		&ImagesetManager::getSingleton().get("TaharezLook").getImage(
		"MultiListSelectionBrush" ) ) ;

	CEGUI::uint firstRow = mcl.addRow( &firstRowItem1, /* column id */ 1 ) ;
	mcl.setItem( &firstRowItem2,  /* column id */ 2, firstRow ) ;
	mcl.setItem( &firstRowItem3,  /* column id */ 3, firstRow ) ;

	ListboxTextItem & secondRowItem1 = * new ListboxTextItem( "Second row 1" ) ;
	ListboxTextItem & secondRowItem2 = * new ListboxTextItem( "Second row 2" ) ;
	ListboxTextItem & secondRowItem3 = * new ListboxTextItem( "Second row 3" ) ;

	CEGUI::uint secondRow = mcl.addRow( &secondRowItem1, /* column id */ 1 ) ;
	mcl.setItem( &secondRowItem2,  /* column id */ 2, secondRow ) ;
	mcl.setItem( &secondRowItem3,  /* column id */ 3, secondRow ) ;

	return mcl;
}



void create_basic_widgets_frame( WindowManager & winManager,
								Window & backgroundWin )
{

	FrameWindow & frameWin = * static_cast<FrameWindow*>( winManager.createWindow(
		"TaharezLook/FrameWindow", "MyBasicFrame") ) ;

	backgroundWin.addChildWindow( &frameWin ) ;

	frameWin.setPosition( UVector2( cegui_reldim(0.13f), cegui_reldim(0.03f) ) ) ;
	frameWin.setMaxSize( UVector2( cegui_reldim(1.0f), cegui_reldim(1.0f) ) ) ;
	frameWin.setSize( UVector2( cegui_reldim(0.44f), cegui_reldim( 0.94f) ) ) ;
	frameWin.setText( "A Frame" ) ;

	Listbox & listBox = * static_cast<Listbox *>( winManager.createWindow(
		"TaharezLook/Listbox", "MyListBox" ) ) ;

	frameWin.addChildWindow( &listBox ) ;
	listBox.setPosition( UVector2( cegui_reldim(0.13f), cegui_reldim(0.03f) ) ) ;
	listBox.setSize( UVector2( cegui_reldim(0.44f), cegui_reldim( 0.14f) ) ) ;

	ListboxTextItem & firstItem = * new ListboxTextItem( "First list element",
		0 ) ;
	listBox.addItem( &firstItem ) ;

	ListboxTextItem & secondItem = * new ListboxTextItem( "Second list element",
		0 ) ;
	listBox.addItem( &secondItem ) ;

	ListboxTextItem & thirdItem = * new ListboxTextItem( "Third list element",
		0 ) ;
	listBox.addItem( &thirdItem ) ;


	Window & firstLabel = * winManager.createWindow( "TaharezLook/StaticText",
		"MyFirstLabel" ) ;

	frameWin.addChildWindow( &firstLabel ) ;

	firstLabel.setProperty( "FrameEnabled", "false" ) ;
	firstLabel.setProperty( "BackgroundEnabled", "false" ) ;

	firstLabel.setPosition( UVector2( cegui_reldim(0.02f),
		cegui_reldim(0.2f) ) ) ;

	firstLabel.setSize( UVector2( cegui_reldim(0.4f), cegui_reldim(0.12f) ) ) ;
	firstLabel.setText( "A (static) label" ) ;

	PushButton & firstButton = * static_cast<PushButton*>(
		winManager.createWindow( "TaharezLook/Button", "MyFirstButton" ) ) ;

	frameWin.addChildWindow( &firstButton ) ;

	firstButton.setPosition( UVector2( cegui_reldim(0.20f),
		cegui_reldim(0.32f) ) ) ;

	firstButton.setSize( UVector2( cegui_reldim(0.3f), cegui_reldim(0.05f) ) ) ;
	firstButton.setText( "First button" ) ;

	Combobox & combo = * static_cast<Combobox*>( winManager.createWindow(
		"TaharezLook/Combobox", "MyComboBox" ) ) ;

	frameWin.addChildWindow( &combo ) ;
	combo.setPosition( UVector2( cegui_reldim(0.04f), cegui_reldim(0.6f) ) ) ;
	combo.setSize( UVector2( cegui_reldim(0.66f), cegui_reldim(0.33f) ) ) ;

	const CEGUI::Image & selectionImage = ImagesetManager::getSingleton().get(
		"TaharezLook").getImage( "MultiListSelectionBrush" ) ;

	ListboxTextItem & firstChoice = * new ListboxTextItem( "First Choice", 0 ) ;
	firstChoice.setSelectionBrushImage( &selectionImage ) ;

	combo.addItem( &firstChoice ) ;

	ListboxTextItem & secondChoice = * new ListboxTextItem( "Second Choice", 0 ) ;
	secondChoice.setSelectionBrushImage( &selectionImage ) ;

	combo.addItem( &secondChoice ) ;

	combo.setReadOnly( true ) ;

	Editbox & editBox = * static_cast<Editbox*>( winManager.createWindow(
		"TaharezLook/Editbox", "MyEditBox" ) ) ;

	frameWin.addChildWindow( &editBox ) ;

	editBox.setPosition( UVector2( cegui_reldim(0.2f), cegui_reldim(0.5f) ) ) ;
	editBox.setSize( UVector2( cegui_reldim(0.5f), cegui_reldim(0.05f) ) ) ;
	editBox.setValidationString( "\\d*" ) ;
	editBox.setText( "A single line editbox" ) ;

	GroupBox & radioBox = * static_cast<GroupBox*>(
		winManager.createWindow( "TaharezLook/GroupBox", "MyRadioBox" ) ) ;

	radioBox.setPosition( UVector2( cegui_reldim(0.2f), cegui_reldim(0.7f) ) ) ;
	radioBox.setSize( UVector2( cegui_reldim(0.7f), cegui_reldim(0.25f) ) ) ;
	radioBox.setText( "A group box" ) ;
	frameWin.addChildWindow( &radioBox ) ;

	RadioButton & firstRadio = * static_cast<RadioButton*>(
		winManager.createWindow( "TaharezLook/RadioButton", "MyFirstRadio" ) ) ;

	firstRadio.setSize( UVector2( cegui_reldim(0.6f), cegui_reldim(0.2f) ) ) ;
	firstRadio.setPosition( UVector2( cegui_reldim(0.2f), cegui_reldim(0.1f) ) ) ;
	firstRadio.setText( "First radio button" ) ;
	firstRadio.setGroupID( 0 ) ;

	radioBox.addChildWindow( &firstRadio ) ;

	RadioButton & secondRadio = * static_cast<RadioButton*>(
		winManager.createWindow( "TaharezLook/RadioButton", "MySecondRadio" ) ) ;

	secondRadio.setSize( UVector2( cegui_reldim(0.6f), cegui_reldim(0.2f) ) ) ;
	secondRadio.setPosition( UVector2( cegui_reldim(0.2f),
		cegui_reldim(0.75f) ) ) ;
	secondRadio.setText( "Second radio button" ) ;
	secondRadio.setGroupID( 0 ) ;

	radioBox.addChildWindow( &secondRadio ) ;

}


void create_advanced_widgets_frame(  WindowManager & winManager,
								   Window & backgroundWin )
{

	FrameWindow & frameWin = * static_cast<FrameWindow*>( winManager.createWindow(
		"TaharezLook/FrameWindow", "MyAdvancedFrame") ) ;

	backgroundWin.addChildWindow( &frameWin ) ;

	frameWin.setPosition( UVector2( cegui_reldim(0.33f), cegui_reldim(0.23f) ) ) ;
	frameWin.setMaxSize( UVector2( cegui_reldim(1.0f), cegui_reldim(1.0f) ) ) ;
	frameWin.setSize( UVector2( cegui_reldim(0.44f), cegui_reldim( 0.94f) ) ) ;
	frameWin.setText( "Another Frame" ) ;
	frameWin.setTooltipText( "This is an example tooltip" ) ;

	ProgressBar & progressBar = * static_cast<ProgressBar*>(
		winManager.createWindow( "TaharezLook/ProgressBar", "MyProgressBar" ) ) ;

	frameWin.addChildWindow( &progressBar ) ;

	progressBar.setPosition( UVector2( cegui_reldim(0.1f),
		cegui_reldim(0.05f) ) ) ;

	progressBar.setSize( UVector2( cegui_reldim(0.5f), cegui_reldim(0.04f) ) ) ;
	progressBar.setProgress( 0.8f ) ;


	// AlternateProgressBar & alternateProgressBar =
	// 	* static_cast<AlternateProgressBar*>(
	// 	  winManager.createWindow( "TaharezLook/AlternateProgressBar",
	// 		"MyAlternateprogressbar" ) ) ;

	// frameWin.addChildWindow( &alternateProgressBar ) ;

	// alternateProgressBar.setPosition( UVector2( cegui_reldim(0.1f),
	// 	  cegui_reldim(0.05f) ) ) ;

	// alternateProgressBar.setSize( UVector2( cegui_reldim(0.5f),
	// 	  cegui_reldim(0.24f) ) ) ;
	// alternateProgressBar.setProgress( 0.8f ) ;


	MultiLineEditbox & multiLineEditbox = * static_cast<MultiLineEditbox*>(
		winManager.createWindow( "TaharezLook/MultiLineEditbox", "MyMultiEditbox" )
		) ;
	frameWin.addChildWindow( &multiLineEditbox ) ;

	multiLineEditbox.setText( "This is a multi-line edit-box." ) ;

	multiLineEditbox.setPosition( UVector2( cegui_reldim(0.1f),
		cegui_reldim(0.1f) ) ) ;

	multiLineEditbox.setSize( UVector2( cegui_reldim(0.5f),
		cegui_reldim(0.1f) ) ) ;

	CEGUI::System::getSingleton().setDefaultTooltip( (CEGUI::utf8*)
		"TaharezLook/Tooltip" ) ;

	Checkbox & checkBox = * static_cast<Checkbox*>(
		winManager.createWindow( "TaharezLook/Checkbox", "MyCheckbox" ) ) ;

	frameWin.addChildWindow( &checkBox ) ;

	checkBox.setPosition( UVector2( cegui_reldim(0.1f), cegui_reldim(0.2f) ) ) ;
	checkBox.setSize( UVector2(cegui_reldim(0.4f), cegui_reldim(0.1f) ) ) ;

	checkBox.setText( "I am a checkbox" ) ;

	Slider & slider = * static_cast<Slider*>( winManager.createWindow(
		"TaharezLook/Slider", "MySlider" ) ) ;

	frameWin.addChildWindow( &slider ) ;

	slider.setPosition( UVector2( cegui_reldim(0.1f), cegui_reldim(0.30f) ) ) ;
	slider.setSize( UVector2( cegui_reldim(0.05f), cegui_reldim(0.4f) ) ) ;
	slider.setMaxValue( 1.0f ) ;
	slider.setClickStep( 0.1f ) ;
	slider.setCurrentValue( 0.5f ) ;

	Spinner & spinner= * static_cast<Spinner*>( winManager.createWindow(
		"TaharezLook/Spinner", "MySpinner" ) ) ;

	spinner.setPosition( UVector2( cegui_reldim(0.5f), cegui_reldim(0.30f) ) ) ;
	spinner.setSize( UVector2( cegui_reldim(0.4f), cegui_reldim(0.05f) ) ) ;
	spinner.setStepSize( 0.5f ) ;
	spinner.setMinimumValue( 2.0f ) ;
	spinner.setMaximumValue( 5.0f ) ;
	spinner.setCurrentValue( 0.5f ) ;

	frameWin.addChildWindow( &spinner ) ;

	UDim bar_bottom( 0, /* d_font->getLineSpacing(2) */ 25 ) ;

	Window & menuBar = * winManager.createWindow( "TaharezLook/Menubar" ) ;
	menuBar.setArea( UDim(0,0), UDim(0,0), UDim(1,0), bar_bottom ) ;
	menuBar.setAlwaysOnTop( true ) ;
	frameWin.addChildWindow( &menuBar ) ;

	Window & firstMenuItem = * winManager.createWindow( "TaharezLook/MenuItem" ) ;
	firstMenuItem.setText( "First Menu" ) ;
	menuBar.addChildWindow( &firstMenuItem ) ;

	Window & firstPopUp = * winManager.createWindow( "TaharezLook/PopupMenu" ) ;
	firstMenuItem.addChildWindow( &firstPopUp ) ;
	firstMenuItem.setText( "A menu entry" ) ;

	Window & firstEntryOfFirstPopUp = * winManager.createWindow(
		"TaharezLook/MenuItem" ) ;
	firstEntryOfFirstPopUp.setText( "First entry" ) ;
	firstPopUp.addChildWindow( &firstEntryOfFirstPopUp ) ;


	Window & secondMenuItem = * winManager.createWindow( "TaharezLook/MenuItem" ) ;
	secondMenuItem.setText( "Second Menu" ) ;
	menuBar.addChildWindow( &secondMenuItem ) ;

	Window & secondPopUp = * winManager.createWindow( "TaharezLook/PopupMenu" ) ;
	secondMenuItem.addChildWindow( &secondPopUp ) ;
	secondMenuItem.setText( "Another menu entry" ) ;

	Window & secondEntryOfSecondPopUp = * winManager.createWindow(
		"TaharezLook/MenuItem" ) ;
	secondEntryOfSecondPopUp.setText( "Second entry" ) ;
	secondPopUp.addChildWindow( &secondEntryOfSecondPopUp ) ;

	// VerticalScrollbar & vertScrollbar = *
	// 	static_cast<VerticalScrollbar*>(
	// 	  winManager.createWindow( "TaharezLook/LargeVerticalScrollbar",
	// 		"MyVertScroolbar" ) ) ;

	// vertScrollbar.setPosition( UVector2( cegui_reldim(0.8f),
	// 	  cegui_reldim(0.30f) ) ) ;

	// vertScrollbar.setSize( UVector2( cegui_reldim(0.4f),
	//   cegui_reldim(0.05f) ) ) ;

	// frameWin.addChildWindow( &vertScrollbar ) ;

}


void create_gui( WindowManager & winManager )
{

	debug_con << " - creating the GUI" << "" ;


	/*
	* Loads an image to use as a background:
	* (will be registered as: "set:BackgroundImage image:full_image")
	*
	*/
	ImagesetManager::getSingleton().createFromImageFile( "BackgroundImage",
		"GPN-2000-001437.tga" ) ;

	Window & backgroundWin = create_background_window( winManager ) ;

	create_multi_column_list( winManager, backgroundWin ) ;

	create_basic_widgets_frame( winManager, backgroundWin ) ;

	create_advanced_widgets_frame( winManager, backgroundWin ) ;

}

bool BattleGui::handleMineButtonClicked(const CEGUI::EventArgs& event)
{
	mOwner->spawnUnit(ESpawnType_ROCK, 0);
	return true;
}
void BattleGui::init()
{
	WindowManager & winManager = init_CEGUI( ) ;
	//create_gui( winManager ) ;

	DefaultWindow & rootWin = * static_cast<DefaultWindow*>(
		winManager.createWindow( "DefaultWindow", "Root" ) ) ;

	System::getSingleton().setGUISheet( &rootWin ) ;

	FrameWindow & myWin = * static_cast<FrameWindow*>( winManager.createWindow(
		"TaharezLook/FrameWindow", "Demo Window" ) ) ;

	rootWin.addChildWindow( &myWin ) ;

	myWin.setPosition( UVector2( cegui_reldim(0.25f), cegui_reldim(0.25f) ) ) ;
	myWin.setSize( UVector2( cegui_reldim(0.5f), cegui_reldim(0.5f) ) ) ;

	myWin.setMaxSize( UVector2( cegui_reldim(1.0f), cegui_reldim(1.0f) ) ) ;
	myWin.setMinSize( UVector2( cegui_reldim(0.1f), cegui_reldim(0.1f) ) ) ;

	myWin.setText( "Battle Boy Control Panel" ) ;

    PushButton* spawnRockButton = (PushButton*)winManager.createWindow("TaharezLook/Button");
	spawnRockButton->setArea(URect(UDim(0, 0), UDim(0,0), UDim(0.2f, 0), UDim(0.2f,0)));
	spawnRockButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&BattleGui::handleMineButtonClicked, this));
     //gJumpBtnWindow->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OurPlayer::Jump,this));
	//spawnRockButton->subscribeEvent(Window::EventMouseButtonDown, Event::Subscriber(&BattleGui::handleMineButtonClicked, this));
	myWin.addChildWindow(spawnRockButton);

	//spawnRockButton.setEnabled(true);

	 // create a container for each row
       /* Window* row = winMgr.createWindow("DefaultWindow");
        row->setArea(URect(UDim(0,0), UDim(d_inc * i, 0),
            UDim(1,0), UDim(d_inc * (i + 1), 0)));
        grid->addChildWindow(row);*/
        //for(size_t j = 0 ; j < MinesweeperSize ; ++j)
        //{
            // Initialize buttons coordinate
            //d_buttonsMapping[i][j].d_col = j;
            //d_buttonsMapping[i][j].d_row = i;
            //d_buttons[i][j] = (PushButton*)winMgr.createWindow("Vanilla/Button");
            //row->addChildWindow(d_buttons[i][j]);
           // d_buttons[i][j]->setArea(URect(UDim(d_inc * j, 0), UDim(0,0), UDim(d_inc * (j + 1), 0), UDim(1,0)));
           // d_buttons[i][j]->setEnabled(false);
            // Associate user data
            //d_buttons[i][j]->setUserData(&(d_buttonsMapping[i][j]));
            //d_buttons[i][j]->setID(0);
            // Connect event handlers
           // d_buttons[i][j]->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&MinesweeperSample::handleMineButtonClicked, this));
            //d_buttons[i][j]->subscribeEvent(Window::EventMouseButtonDown, Event::Subscriber(&MinesweeperSample::handleMineButtonDown, this));
        //}
	/*void CreateJumpButton()
	{
	  gJumpBtnWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button","JumpPushButton");  // Create Window
	  gJumpBtnWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75,0),CEGUI::UDim(0.50,0)));
	  gJumpBtnWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0,50),CEGUI::UDim(0,50)));
	  gJumpBtnWindow->setText("Jump!");
	  CEGUI::System::getSingleton().getGUISheet()->addChildWindow(gJumpBtnWindow);  
	}*/
}