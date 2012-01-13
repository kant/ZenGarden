/*
 *  Copyright 2009,2010,2011 Reality Jockey, Ltd.
 *                 info@rjdj.me
 *                 http://rjdj.me/
 *
 *  This file is part of ZenGarden.
 *
 *  ZenGarden is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ZenGarden is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with ZenGarden.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ArrayArithmetic.h"
#include "DspSubtract.h"
#include "PdGraph.h"

MessageObject *DspSubtract::newObject(PdMessage *initMessage, PdGraph *graph) {
  return new DspSubtract(initMessage, graph);
}

DspSubtract::DspSubtract(PdMessage *initMessage, PdGraph *graph) : DspObject(2, 2, 0, 1, graph) {
  constant = initMessage->isFloat(0) ? initMessage->getFloat(0) : 0.0f;
  codePath = DSP_SUBTRACT_DSP_MESSAGE;
}

DspSubtract::~DspSubtract() {
  // nothing to do
}

const char *DspSubtract::getObjectLabel() {
  return "-~";
}

string DspSubtract::toString() {
  const char *fmt = (constant == 0.0f) ? "%s" : "%s %g";
  char str[snprintf(NULL, 0, fmt, getObjectLabel(), constant)+1];
  snprintf(str, sizeof(str), fmt, getObjectLabel(), constant);
  return string(str);
}

void DspSubtract::onInletConnectionUpdate(unsigned int inletIndex) {
  codePath = (incomingDspConnections[0].size() > 0 && incomingDspConnections[1].size() > 0)
      ? DSP_SUBTRACT_DSP_DSP : DSP_SUBTRACT_DSP_MESSAGE;
}

void DspSubtract::processDsp() {
  switch (codePath) {
    case DSP_SUBTRACT_DSP_DSP: {
      ArrayArithmetic::subtract(dspBufferAtInlet[0], dspBufferAtInlet[1], dspBufferAtOutlet0, 0, blockSizeInt);
      break;
    }
    default: DspObject::processDsp(); break;
  }
}

void DspSubtract::processMessage(int inletIndex, PdMessage *message) {
  if (inletIndex == 1) {
    if (message->isFloat(0)) {
      constant = message->getFloat(0);
    }
  }
}

void DspSubtract::processDspWithIndex(int fromIndex, int toIndex) {
  ArrayArithmetic::subtract(dspBufferAtInlet[0], constant, dspBufferAtOutlet0, fromIndex, toIndex);
}
