//
//  MainComponent.cpp
//  MelodyStochaster
//
//  Created by Jack Atherton on 11/11/12.
//  Copyright (c) 2012. All rights reserved.
//

#include <iostream>
#include "MainComponent.h"


MainComponent::MainComponent () : playThread("Play Thread")
{
    // Initialization of all components, including buttons, sliders, and labels
    
    
    buttonWidth = 100;
    windowWidth = 400;
    windowLength = 490;
    goodSource = false;
    goodOutput = false;
    
    sourceLabel = new Label("Source","");
    addAndMakeVisible(sourceLabel);
    
    loadButton = new TextButton("Load File", "Load an existing midi file for melody stochastion");
    addAndMakeVisible(loadButton);
    loadButton->addListener(this);
    
    loadedLabel = new Label("Loaded Label", String::empty);
    addAndMakeVisible(loadedLabel);
    
    playButton = new TextButton("Play", "Play your own melody without recording (to stop playing, press the stop button on the midi controller)");
    addAndMakeVisible(playButton);
    playButton->addListener(this);
    
    recordButton = new TextButton("Record", "Record a new melody for stochastion using a midi controller");
    recordButton->setClickingTogglesState(true);
    addAndMakeVisible(recordButton);
    recordButton->addListener(this);
    
    saveRecordButton = new TextButton("Save Recording", "Save your recording to file for future use");
    addAndMakeVisible(saveRecordButton);
    saveRecordButton->addListener(this);
    
    playBackMode1Button = new ToggleButton("Use 1 timbre per note");
    addAndMakeVisible(playBackMode1Button);
    playBackMode1Button->addListener(this);
    playBackMode1Button->setRadioGroupId(1);
    playBackMode1Button->setToggleState(true, false);
    
    playBackMode2Button = new ToggleButton("Use 1 timbre at a time");
    addAndMakeVisible(playBackMode2Button);
    playBackMode2Button->addListener(this);
    playBackMode2Button->setRadioGroupId(1);
    
    playbackSettingsLabel = new Label("Playback Settings", "");
    addAndMakeVisible(playbackSettingsLabel);
    
    cutoffLabel = new Label("Base Cutoff", "Base Cutoff");
    addAndMakeVisible(cutoffLabel);
    
    cutoffSlider = new Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow);
    cutoffSlider->setRange(0.0, 1.0, 0.01);
    cutoffSlider->setValue(0.1);
    cutoffSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    addAndMakeVisible(cutoffSlider);
    cutoffSlider->addListener(this);
    
    cutoffSensitivityLabel = new Label("Cutoff Sensitivity", "Cutoff Sensitivity");
    addAndMakeVisible(cutoffSensitivityLabel);
    
    cutoffSensitivitySlider = new Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow);
    cutoffSensitivitySlider->setRange(0.0, 1.0, 0.01);
    cutoffSensitivitySlider->setValue(1.0);
    cutoffSensitivitySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    addAndMakeVisible(cutoffSensitivitySlider);
    cutoffSensitivitySlider->addListener(this);
    
    waveformLabel = new Label("Timbre Flux", "Timbre Flux");
    addAndMakeVisible(waveformLabel);
    
    waveformSpeedSlider = new Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow);
    waveformSpeedSlider->setRange(1, 100, 1);
    waveformSpeedSlider->setValue(20);
    waveformSpeedSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    addAndMakeVisible(waveformSpeedSlider);
    waveformSpeedSlider->addListener(this);
    
    octavesBelowLabel = new Label("Octaves Below/Above Source", "Octaves Below/Above Source");
    addAndMakeVisible(octavesBelowLabel);
    
    octavesBelowSlider = new Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow);
    octavesBelowSlider->setRange(-8, 8, 1);
    octavesBelowSlider->setValue(0);
    octavesBelowSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    addAndMakeVisible(octavesBelowSlider);
    octavesBelowSlider->addListener(this);
    
    octavesAboveSlider = new Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow);
    octavesAboveSlider->setRange(-8, 8, 1);
    octavesAboveSlider->setValue(0);
    octavesAboveSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    addAndMakeVisible(octavesAboveSlider);
    octavesAboveSlider->addListener(this);
    
    noteSpeedLabel = new Label("Speed Factor", "Speed Factor");
    addAndMakeVisible(noteSpeedLabel);
    
    noteSpeedSlider = new Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow);
    noteSpeedSlider->setRange(0.1, 10, 0.01);
    noteSpeedSlider->setSkewFactorFromMidPoint(1.0);
    noteSpeedSlider->setValue(1.0);
    noteSpeedSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    addAndMakeVisible(noteSpeedSlider);
    noteSpeedSlider->addListener(this); 
    
    playResultButton = new TextButton("Play Melody", "Play a stochastic melody based on the source");
    playResultButton->setClickingTogglesState(true);
    addAndMakeVisible(playResultButton);
    playResultButton->addListener(this);
    
    savePlayButton = new TextButton("Save Melody", "Save the stochastic melody currently playing");
    addAndMakeVisible(savePlayButton);
    savePlayButton->addListener(this);
    
    
    resized();
    
    secondsPerTick = 1.0/960.0;
    
    recordCallback = new MidiRecordInputCallback();
    device = MidiInput::openDevice(1, recordCallback);
    
}

MainComponent::~MainComponent() {
    deleteAllChildren();
    if (playThread.isThreadRunning()) {
        playThread.stopThread(10000);
    }
    delete device;
}


void MainComponent::resized() {
    sourceLabel->setBounds(20, 95, 200, 20);
    
    loadButton->setBounds(45, 123, buttonWidth, 20);
    loadedLabel->setBounds(45, 145, 300, 20);
    
    playButton->setBounds(windowWidth/2 - buttonWidth/2, 123, buttonWidth, 20);
    
    recordButton->setBounds(windowWidth - buttonWidth - 45, 123, buttonWidth, 20);
    saveRecordButton->setBounds(windowWidth - buttonWidth - 45, 145, buttonWidth, 20);
    
    playBackMode1Button->setBounds(45, 167, 150, 20);
    playBackMode2Button->setBounds(windowWidth - 150 - 45, 167, 150, 20);
    
    playbackSettingsLabel->setBounds(20, 230, 200, 20);
    
    cutoffLabel->setBounds(42, windowLength - 270, 80, 20);
    cutoffSlider->setBounds(65, windowLength - 242, 40, 60);
    cutoffSensitivityLabel->setBounds(125, windowLength - 270, 120, 20);
    cutoffSensitivitySlider->setBounds(160, windowLength - 242, 40, 60);
    waveformLabel->setBounds(275, windowLength - 270, 90, 20);
    waveformSpeedSlider->setBounds(300, windowLength - 242, 40, 60);
    
    octavesBelowLabel->setBounds(40, windowLength - 170, 200, 20);
    octavesBelowSlider->setBounds(65, windowLength - 142, 40, 60);
    octavesAboveSlider->setBounds(160, windowLength - 142, 40, 60);
    noteSpeedLabel->setBounds(275, windowLength - 170, 90, 20);
    noteSpeedSlider->setBounds(300, windowLength - 142, 40, 60);
    
    playResultButton->setBounds(80, windowLength - 50, buttonWidth, 20);
    savePlayButton->setBounds(windowWidth - buttonWidth - 80, windowLength - 50, buttonWidth, 20);
}

void MainComponent::paint (Graphics& g)
{
    g.setFont(38);
    g.drawFittedText("The Melody Stochaster", 20, 30, 360, 40, Justification::centred, 1);
    g.setColour (Colours::cadetblue.withAlpha (0.5f));
    // draw a rounded rectangle (2px thick, 20 'round')
    g.drawRoundedRectangle (35, 115, windowWidth - 70, 80, 10, 2);
    g.drawRoundedRectangle(35, 217, windowWidth - 70, 200, 10, 2);
}


//===================================================================================================
//
//Button Handling

void MainComponent::buttonClicked(Button* button) {
    if (button == playButton) {
        launchChuck();
    } else if (button == loadButton) {
        loadFile();
    } else if (button == recordButton) {
        if (recordButton->getToggleState()) {
            if (device == NULL) {
                recordButton->setToggleState(false, false);
                return;
            }
            recordButton->setButtonText("Recording");
            recordButton->setTooltip("Finish recording a new melody for stochastion");
            goodSource = false;
            //Logic for recording
            
            recordOutput.clear();
            recordCallback->initialise(&recordOutput);
            device->start();
            
            launchChuck();
            
        } else {
            recordButton->setButtonText("Record");
            recordButton->setTooltip("Record a new melody for stochastion using a midi controller");
            //Logic for finishing recording
            goodSource = true;
            loadedLabel->setText("unsaved recording loaded", false);
            
            device->stop();
            source = new MidiMessageSequence(recordOutput);
            goodSource = true;
            goodOutput = false;
            //make new generator
            generator = new MidiStochaster(*source);
            
        }
        
    } else if (button == saveRecordButton) {
        if (goodSource) {
            loadedLabel->setText(saveMidi(source) + " loaded", false);
        }
    } else if (button == playResultButton) {
        if (playResultButton->getToggleState()) {
            playResultButton->setButtonText("Playing");
            playResultButton->setTooltip("Stop the currently playing melody");
            if (goodSource) {
                makeMelody();
                //Logic for playing
                playThread.initialise(output, secondsPerTick, cutoffSlider->getValue(), cutoffSensitivitySlider->getValue(), noteSpeedSlider->getValue(), waveformSpeedSlider->getValue());
                playThread.startThread(10);                    
            }
        } else {
            playResultButton->setButtonText("Play Melody");
            playResultButton->setTooltip("Play a stochastic melody based on the source");
            if (goodSource) {
                //Logic for stopping
                playThread.stopThread(10000);
            }
        }
    } else if (button == savePlayButton) {
        if (goodOutput) {
            saveMidi(output);
        }
    }
}


void MainComponent::launchChuck() {
    String s = "chuck /Applications/MelodyStochaster.app/Contents/Resources/Play";
    if (playBackMode2Button->getToggleState()) {
        s+= "2";
    }
    s += ".ck:1:" + String(waveformSpeedSlider->getValue()/100.0) + ":" + String(cutoffSlider->getValue()) + ":" + String(cutoffSensitivitySlider->getValue()) + " &";
    std::system("pwd");
    std::system(s.getCharPointer());
}



void MainComponent::sliderValueChanged (Slider* s) {
    if (s == octavesAboveSlider) {
        if (s->getValue() < octavesBelowSlider->getValue()) {
            octavesBelowSlider->setValue(s->getValue());
        }
    } else if (s == octavesBelowSlider) {
        if (s->getValue() > octavesAboveSlider->getValue()) {
            octavesAboveSlider->setValue(s->getValue());
        }
    }
}

void MainComponent::sliderDragEnded(Slider* s) {
    if (playThread.isThreadRunning()) {
        if (s == octavesAboveSlider || s == octavesBelowSlider || s == noteSpeedSlider) {
            playThread.stopThread(10000);
            makeMelody();
            playThread.initialise(output, secondsPerTick, cutoffSlider->getValue(), cutoffSensitivitySlider->getValue(), noteSpeedSlider->getValue(), waveformSpeedSlider->getValue());
            playThread.startThread(10);
        } else if (s == cutoffSlider || s == cutoffSensitivitySlider || s == waveformSpeedSlider) {
            playThread.updateValues(cutoffSlider->getValue(), cutoffSensitivitySlider->getValue(), waveformSpeedSlider->getValue());
        }
    }
}


void MainComponent::loadFile() {
    FileChooser chooser ("Choose the midi file to open");
    if (chooser.browseForFileToOpen()) {
        File f = chooser.getResult();
        if (!f.existsAsFile()) {
            return;
        }
        FileInputStream fis(f);
        
        ScopedPointer<MidiFile> file (new MidiFile());
        bool b = file->readFrom(fis);
        if (!b) {
            return;
        }
        file->setTicksPerQuarterNote(480);
        MidiMessageSequence tempos;
        file->findAllTempoEvents(tempos);
        if (tempos.getNumEvents() > 0) {
            secondsPerTick = tempos.getEventPointer(0)->message.getTempoMetaEventTickLength(file->getTimeFormat());
        }
        ScopedPointer<const MidiMessageSequence> read (new MidiMessageSequence(*file->getTrack(0)));
        generator = new MidiStochaster(*read);
        source = new MidiMessageSequence(*read);
        goodSource = true;
        goodOutput = false;
        
        loadedLabel->setText(f.getFileName() + " loaded", false);
    }
}



String MainComponent::saveMidi(MidiMessageSequence const* sequence) {
    FileChooser chooser ("Choose the save location");
    if (chooser.browseForFileToSave(true)) {
        File f = chooser.getResult();
        if (f.existsAsFile()) {
            f.deleteFile();
        }
        f.create();
        ScopedPointer <FileOutputStream> fos (f.createOutputStream());
        ScopedPointer <MidiFile> outFile (new MidiFile());
        
        outFile->setTicksPerQuarterNote(480);
        outFile->addTrack(*sequence);
        outFile->writeTo(*fos);
        return f.getFileName();
    }
    return "";
}

void MainComponent::makeMelody() {
    if (!goodSource) {
        return;
    }
    output = generator->createMelody(octavesBelowSlider->getValue(), octavesAboveSlider->getValue(), noteSpeedSlider->getValue());
    goodOutput = true;
}

