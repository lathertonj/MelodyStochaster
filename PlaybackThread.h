//
//  PlaybackThread.h
//  MelodyStochaster
//
//  Created by Jack Atherton on 10/22/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#ifndef Mini_MidiStochaster_PlaybackThread_h
#define Mini_MidiStochaster_PlaybackThread_h

#include "juce_Thread.h"
#include "juce_MidiMessageSequence.h"
#include "juce_MidiKeyboardState.h"
#include <stdlib.h>

class PlaybackThread : public Thread {
public:
    PlaybackThread(const String &threadName) : Thread(threadName) {
    }
    void initialise(MidiMessageSequence const* sequenceToPlay, double secondsPerTick, double baseCutoffNum, double cutoffSensitivityNum, double timbreFluxNum, double speed) {
        seqIndex = 0;
        globalGain = 0.2;
        timbreFlux = timbreFluxNum;
        baseCutoff = baseCutoffNum;
        cutoffSensitivity = cutoffSensitivityNum;
        release = 50.0 / speed;
        oGain[0] = 0.33333;
        oGain[1] = 0.33333;
        oGain[2] = 1 - oGain[0] - oGain[1];
        msPerTick = secondsPerTick * 1000.0;
        seq = new MidiMessageSequence(*sequenceToPlay);
        seq->updateMatchedPairs();
        while (seq->getEventPointer(seqIndex) != NULL && !seq->getEventPointer(seqIndex)->message.isNoteOn()) {
            seqIndex++;
        }
    }
    void run() {
        if (seqIndex >= seq->getNumEvents()) {
            return;
        }
        while (true) {
            if (threadShouldExit()) {
                return;
            }
            playFrom(seqIndex);
        }
        
    }
    
    void playFrom(int index) {
        MidiMessage* currentMessage = &(seq->getEventPointer(index)->message);
        int lastEventTime = currentMessage->getTimeStamp();
        int thisEventTime;
        
        std::cout << seq->getNumEvents() << std::endl;
        
        while(index < seq->getNumEvents()) {
            if (threadShouldExit()) {
                return;
            }
            
            currentMessage = &(seq->getEventPointer(index)->message);
            thisEventTime = currentMessage->getTimeStamp();
            
            
            if (currentMessage->isNoteOn()) {
                adjustTimbre();
                
                MidiMessage* noteOff = &(seq->getEventPointer(index)->noteOffObject->message);
                double length = noteOff->getTimeStamp() - thisEventTime;
                double velGain = globalGain * currentMessage->getVelocity() / 127.0;
                double cutoff = MidiMessage::getMidiNoteInHertz((baseCutoff + (1.0 - baseCutoff) * cutoffSensitivity * ((double)currentMessage->getVelocity()) / 127.0) * 145.0);
                String s = "chuck /Applications/MelodyStochaster.app/Contents/Resources/PlayNote.ck:" + String(MidiMessage::getMidiNoteInHertz(currentMessage->getNoteNumber()+12)) + ":" + String(length) + ":" + String(velGain*oGain[0]) + ":" + String(velGain*oGain[1]) + ":" + String(velGain*oGain[2]) + ":" + String(release) + ":" + String(cutoff) + " &";
                std::system(s.getCharPointer());
            }
            wait(std::max(0.0, msPerTick * (thisEventTime - lastEventTime)));
            
            lastEventTime = thisEventTime;
            index++;
        }
    }
    
    void updateValues(double base, double sensitivity, double waveformSpeed) {
        baseCutoff = base;
        cutoffSensitivity = sensitivity;
        timbreFlux = waveformSpeed;
    }
    
private:
    int seqIndex;
    double msPerTick, globalGain, timbreFlux, baseCutoff, cutoffSensitivity, release;
    double oGain[3];
    ScopedPointer<MidiMessageSequence>seq;
    
    void adjustTimbre() {
        double randDouble = randomDouble(0, timbreFlux);
        int randIndex = rand() % 3;
        switch (randIndex) {
            case 2:
                subtract(2, 0, 1, randDouble);
                break;
            case 1:
                subtract(1, 0, 2, randDouble);
                break;
            case 0:
                subtract(0, 1, 2, randDouble);
                break;
        }
        
    }
    
    void subtract (int index1, int index2, int index3, double amount) {
		double diff = std::min(1 - oGain[index1], amount); 
		oGain[index1] += diff;
		oGain[index2] -= diff/2;
		oGain[index3] -= diff/2;
		//Invariant: oGain[2] and oGain[3] can't both be < 0.
		//oGain[1] is at most 1, and oGain[1] + oGain[2] + oGain[3] is at most 1.
		//If oGain[2] == oGain[3], they can only be reduced by exactly their own amount
		//Otherwise, one will be higher and will be positive.
		if (oGain[index2] < 0) {
			oGain[index3] += oGain[index2];
			oGain[index2] = 0;
		} else if (oGain[index3] < 0) {
			oGain[index2] += oGain[index3];
			oGain[index3] = 0;
		}
	}
    
    double randomDouble (double min, double max) {
        double zeroOne = (double) rand() / RAND_MAX;
        return min + zeroOne * (max - min);
    }
};

#endif
