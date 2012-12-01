//
//  MidiRecordInputCallback.h
//  Mini MidiStochaster
//
//  Created by Jack Atherton on 11/10/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#ifndef Mini_MidiStochaster_MidiRecordInputCallback_h
#define Mini_MidiStochaster_MidiRecordInputCallback_h

//#include "juce_MidiInput.h"
//#include "juce_MidiMessageSequence.h"
#include "JuceHeader.h"
#include <time.h>


class MidiRecordInputCallback : public MidiInputCallback
{
public:
    void initialise(MidiMessageSequence* dest);
    void handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message);
    
private:
    MidiMessageSequence* destination;
    clock_t startTime;
};

#endif
