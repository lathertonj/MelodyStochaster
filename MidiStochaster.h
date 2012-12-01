//
//  MidiStochaster.h
//  PROJ NAME
//
//  Created by Jack Atherton on 10/13/12.
//  Copyright (c) 2012 Jack Atherton. All rights reserved.
//

#ifndef HiWorld_MidiStochaster_h
#define HiWorld_MidiStochaster_h

#include "../../JuceLibraryCode/JuceHeader.h"
#include <vector>


/**
 A generator for stochastic melodies based on the input.
 */


class MidiStochaster
{
public:
    //==============================================
    /** Creates a default midi stochaster object. */
    MidiStochaster();
    
    /** Creates a midi stochaster object. */
    MidiStochaster (MidiMessageSequence const& track);
    
    
    
    //==============================================
    /** Creates a melody with default parameters. */
    MidiMessageSequence* createMelody();
    
    /** Creates a melody. */
    MidiMessageSequence* createMelody(int minOctaveOffset, int maxOctaveOffset, double speedFactor);
    
private:
    std::vector<int>* pitches;
    int lowVelocity;
    int highVelocity;
    double lowNoteLength;
    double highNoteLength;
    double lowRestLength;
    double highRestLength;
    double lowNegRestLength;
    double highNegRestLength;
    int numPosRest;
    int numNegRest;
    double negRestProbability;
    MidiMessageSequence starter;
    
    void init(MidiMessageSequence const& track);
    void addRest(double restLength);
    void addNote(MidiMessageSequence::MidiEventHolder* note, int i, MidiMessageSequence const& track);
};



#endif
