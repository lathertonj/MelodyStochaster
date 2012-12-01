//
//  MainAppWindow.cpp
//  Mini MidiStochaster
//
//  Created by Jack Atherton on 10/18/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#include <iostream>
#include "MainAppWindow.h"
#include "MainComponent.h"

//==============================================================================
MainAppWindow::MainAppWindow()
:   
// Initialise the base 'DocumentWindow'...
DocumentWindow (
				"The Melody Stochaster",        // Set the text to use for the title
				Colours::azure,					// Set the colour of the window
				DocumentWindow::allButtons,		// Set which buttons are displayed
				true							// This window should be added to the desktop
				)
{
    setResizable (false, false);
	
	setTitleBarHeight (22);
	
	// create the main component, which is described in MainComponent.h
    MainComponent* contentComponent = new MainComponent ();
	
    // This sets the main content component for the window to be whatever MainComponent
    // is. The nature of DocumentWindow means that the contentComponent will fill the main
	// area of the window, and will be deleted automatically when the window is deleted.
    setContentOwned (contentComponent, false);
}

MainAppWindow::~MainAppWindow()
{
    // The content component will get deleted by the destructor in the DialogWindow
	// base class.  Nothing to do here.
}

void MainAppWindow::closeButtonPressed()
{
    // Call the JUCEApplication::systemRequestedQuit() method to quit.
    JUCEApplication::getInstance()->systemRequestedQuit();
} 
