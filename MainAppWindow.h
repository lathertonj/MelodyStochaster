//
//  MainAppWindow.h
//  MelodyStochaster
//
//  Created by Jack Atherton on 10/18/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#ifndef Mini_MidiStochaster_MainAppWindow_h
#define Mini_MidiStochaster_MainAppWindow_h

#include "JuceHeader.h" //

#define VALIDCHARS "ABCDEFGHIJKLMNOPQRTSUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789"
#define PROJECT_NAME_IDENTIFIER (String(PROJECT_NAME).replaceCharacter (' ', '_')		\
.replaceCharacter ('.', '_')		\
.retainCharacters (T(VALIDCHARS)))

//==============================================================================
class MainAppWindow  : public DocumentWindow
{
public:
    //==============================================================================
    MainAppWindow();
    ~MainAppWindow();
    
    //==============================================================================
    // called when the close button is pressed or esc is pushed
    void closeButtonPressed();
};


#endif
