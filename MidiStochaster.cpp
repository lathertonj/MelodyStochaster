//
//  MidiStochaster.cpp
//
//  Created by Jack Atherton on 10/13/12.
//  Copyright (c) 2012 Jack Atherton. All rights reserved.
//

#include "MidiStochaster.h"
#include <cstdlib>

double randomDouble (double min, double max);
int randomInt(int min, int max);

MidiStochaster::MidiStochaster() {
    MidiMessageSequence track;
    init(track);
}

MidiStochaster::MidiStochaster(MidiMessageSequence const& track) {
    init(track);
}

void MidiStochaster::init(MidiMessageSequence const& originalTrack) {
    srand(time(0));
    ScopedPointer<MidiMessageSequence> track (new MidiMessageSequence(originalTrack));
    track->updateMatchedPairs();
    
    int numEvents = track->getNumEvents();
    if (numEvents == 0) {
        //No Events handling
        //...
        return;
    }
    
    int numNotes = 0;
    for (int i = 0; i < numEvents; i++) {
        if (track->getEventPointer(i)->message.isNoteOn()) {
            numNotes++;
        }
    }
    
    if (numNotes == 0) {
        //No Notes handling
        //...
        return;
    }
    
    pitches = new std::vector<int>(0);
        
    MidiMessageSequence::MidiEventHolder* eventHolder = track->getEventPointer(0);
    int i = 0;
    while (!eventHolder->message.isNoteOn()) {
        starter.addEvent(eventHolder->message);
        i++;
        eventHolder = track->getEventPointer(i);
    }

    pitches->push_back(eventHolder->message.getNoteNumber());
    lowVelocity = (int) eventHolder->message.getVelocity();
    highVelocity = lowVelocity;
    lowNoteLength = eventHolder->noteOffObject->message.getTimeStamp() - eventHolder->message.getTimeStamp();
    highNoteLength = lowNoteLength;
    
    if (numNotes == 1) {
        //One Note handling
        //...
        return;
    }
    lowRestLength = -1.0;
    highRestLength = lowRestLength;
    lowNegRestLength = 1.0;
    highNegRestLength = lowNegRestLength;
    numPosRest = 0;
    numNegRest = 0;
    
    double lastNoteEndPoint;
    
    bool brokeInner = false;
    for (i = i+1; i < numEvents; i++) {
        lastNoteEndPoint = eventHolder->noteOffObject->message.getTimeStamp();
        eventHolder = track->getEventPointer(i);
        while(!eventHolder->message.isNoteOn())  {
            i++;
            if (i >= numEvents) {
                brokeInner = true;
                break;
            }
            eventHolder = track->getEventPointer(i);
        }
        if(brokeInner) {
            break;
        }
        
        double restLength = eventHolder->message.getTimeStamp() - lastNoteEndPoint;
        addRest(restLength);
        
        addNote(eventHolder, i, *track);
                
        
    }
    negRestProbability = ((double) numNegRest) / ((double) (numNegRest + numPosRest));
    if (negRestProbability > .9) {
        if (negRestProbability == 1.0) {
            lowRestLength = 1;
            highRestLength = 1;
        }
        negRestProbability = .9;
    }
}

void MidiStochaster::addNote(MidiMessageSequence::MidiEventHolder* note, int i, MidiMessageSequence const& track) {
    pitches->push_back(note->message.getNoteNumber());
    
    int noteVelocity = (int) note->message.getVelocity();
    if (noteVelocity > highVelocity) {
        highVelocity = noteVelocity;
    } else if (noteVelocity < lowVelocity) {
        lowVelocity = noteVelocity;
    }
    
    double noteLength = note->noteOffObject->message.getTimeStamp() - note->message.getTimeStamp();
    if (noteLength > highNoteLength) {
        highNoteLength = noteLength;
    } else if (noteLength < lowNoteLength) {
        lowNoteLength = noteLength;
    }
}

void MidiStochaster::addRest(double restLength) {
    if (restLength < 0.0) {
        //Is a negative distance / overlapping note
        numNegRest++;
        if (lowNegRestLength < 0.0) {
            if (restLength > highNegRestLength) {
                //Mark it's the highest neg we've seen yet
                highNegRestLength = restLength;
            } else if (restLength < lowNegRestLength) {
                //Mark it's the lowest neg we've seen yet
                lowNegRestLength = restLength;
            }
            //Else do nothing
        } else {
            //We haven't seen any negs yet, this is the first
            lowNegRestLength = restLength;
            highNegRestLength = lowNegRestLength;
        }
    } else {
        //Is a regular rest
        numPosRest++;
        if (lowRestLength > 0.0) {
            if (restLength > highRestLength) {
                //Mark it's the longest rest we've seen yet
                highRestLength = restLength;
            } else if (restLength < lowRestLength) {
                //Mark it's the shortest rest we've seen yet
                lowRestLength = restLength;
            }
            //Else do nothing
        } else {
            //We haven't seen any rests yet, this is the first
            lowRestLength = restLength;
            highRestLength = lowRestLength;
        }
    }
}

MidiMessageSequence* MidiStochaster::createMelody() {
    return createMelody(0, 0, 1);
}

MidiMessageSequence* MidiStochaster::createMelody(int minOctaveOffset, int maxOctaveOffset, double speedFactor) {
    
    int currentOctaveOffset;
    if (minOctaveOffset <= 0) {
        currentOctaveOffset = 0;
    } else { 
        currentOctaveOffset = minOctaveOffset;
    }
    
    int randInt1 = randomInt(100, 200);
    double stopProbability = (double) randInt1 / (randInt1 + 1);
    int randInt2 = randomInt(4, 10);
    double octaveSwitchProbability = (double) randInt2 / (randInt2 + 1);
    
    
    double currentTime = 0.0;
    MidiMessageSequence* melody = new MidiMessageSequence(starter);
    
    while (randomDouble(0,1) < stopProbability) {
        int notePitch = pitches->at(randomInt(0, pitches->size() - 1)) + 12 * currentOctaveOffset;
        while (notePitch < 0) {
            notePitch+=12;
            currentOctaveOffset++;
            minOctaveOffset++;
        }
        while (notePitch > 127) {
            notePitch-=12;
            currentOctaveOffset--;
            maxOctaveOffset--;
        }
        int noteVelocity = randomInt(lowVelocity, highVelocity);
        int noteLength = randomDouble(lowNoteLength, highNoteLength) * (1.0 / speedFactor);
        int nextRestLength;
        if (randomDouble(0,1) < negRestProbability) {
            nextRestLength = randomDouble(lowNegRestLength, highNegRestLength) * 1.0 / speedFactor;
        } else {
            nextRestLength = randomDouble(lowRestLength, highRestLength) * 1.0 / speedFactor;
        }
        
        MidiMessage noteDown = MidiMessage::noteOn(1, notePitch, (uint8_t) noteVelocity);
        noteDown.setTimeStamp(currentTime);
        MidiMessage noteUp = MidiMessage::noteOff(1, notePitch);
        noteUp.setTimeStamp(currentTime + noteLength);

        melody->addEvent(noteDown);
        melody->addEvent(noteUp);
        melody->updateMatchedPairs();
        
        currentTime += noteLength + nextRestLength;
        
        if (randomDouble(0,1) > octaveSwitchProbability) {
            currentOctaveOffset = randomInt(minOctaveOffset, maxOctaveOffset);
        }
    }
        
    return melody;
    
    
}

double randomDouble (double min, double max) {
    double zeroOne = (double) rand() / RAND_MAX;
    return min + zeroOne * (max - min);
}

int randomInt(int min, int max) {
    return min + rand() % (max + 1 - min);
}