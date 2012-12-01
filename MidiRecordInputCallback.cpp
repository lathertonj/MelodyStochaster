//
//  MidiRecordInputCallback.cpp
//  Mini MidiStochaster
//
//  Created by Jack Atherton on 11/10/12.
//  Copyright (c) 2012 N/A. All rights reserved.
//

#include <iostream>
#include "MidiRecordInputCallback.h"

void MidiRecordInputCallback::initialise(MidiMessageSequence* dest) {
    destination = dest;
    startTime = clock();
}

void MidiRecordInputCallback::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message) {
    double timeStamp =  ((double) (clock() - startTime))/((double)CLOCKS_PER_SEC) * 1000000.0 / 5.0;
    destination->addEvent(MidiMessage(message, timeStamp));
}