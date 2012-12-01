//
//  MainComponent.h
//  Mini MidiStochaster
//
//  Created by Jack Atherton on 10/18/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#ifndef Mini_MidiStochaster_MainComponent_h
#define Mini_MidiStochaster_MainComponent_h

#include "JuceHeader.h"
#include "MidiStochaster.h"
#include "PlaybackThread.h"
#include "MidiRecordInputCallback.h"

class MainComponent  : public Component, public ButtonListener, public Slider::Listener {
private:
    TooltipWindow tooltipWindow;
    
    Label* sourceLabel;
    
    TextButton* playButton;
    
    TextButton* loadButton;
    Label* loadedLabel;
    
    TextButton* recordButton;
    TextButton* saveRecordButton;
    
    ToggleButton* playBackMode1Button;
    ToggleButton* playBackMode2Button;
    
    Label* playbackSettingsLabel;
    
    Label* cutoffLabel;
    Slider* cutoffSlider;
    Label* cutoffSensitivityLabel;
    Slider* cutoffSensitivitySlider;
    Label* waveformLabel;
    Slider* waveformSpeedSlider;
    
    Label* octavesBelowLabel;
    Slider* octavesBelowSlider;
    Slider* octavesAboveSlider;
    Label* noteSpeedLabel;
    Slider* noteSpeedSlider;
    
    TextButton* playResultButton;
    TextButton* savePlayButton;
    
    int buttonWidth, windowWidth, windowLength;
    
    ScopedPointer<MidiMessageSequence const> source;
    ScopedPointer<MidiMessageSequence const> output;
    ScopedPointer<MidiStochaster> generator;
    
    bool goodSource;
    bool goodOutput;
    
    PlaybackThread playThread;
    
    double secondsPerTick;
    
    MidiInput* device;
    ScopedPointer<MidiRecordInputCallback> recordCallback;
    MidiMessageSequence recordOutput;
    
    void launchChuck();
    void loadFile();
    String saveMidi(MidiMessageSequence const* sequence);
    void makeMelody();
    
    
public:
    //==============================================================================
    
    
    MainComponent();
    
    ~MainComponent ();
    
    //==============================================================================
    void resized ();
    void paint (Graphics& g);
    void buttonClicked(Button* button);
    void sliderValueChanged (Slider* s);
    void sliderDragEnded(Slider* s);
    
    
    //==============================================================================
};


#endif
