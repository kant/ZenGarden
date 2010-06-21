/*
 *  Copyright 2009,2010 Reality Jockey, Ltd.
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
#include "DspDelayWrite.h"
#include "DspVariableDelay.h"
#include "PdGraph.h"

DspVariableDelay::DspVariableDelay(PdMessage *initMessage, PdGraph *graph) : DelayReceiver(0, 1, 0, 1, graph) {
  if (initMessage->isSymbol(0)) {
    name = StaticUtils::copyString(initMessage->getSymbol(0));
    sampleRate = graph->getSampleRate();
    xArray = (float *) malloc(blockSizeInt * sizeof(float));
    targetIndexBaseArray = (float *) malloc(blockSizeInt * sizeof(float));
  } else {
    graph->printErr("vd~ requires the name of a delayline. None given.");
    name = NULL;
    xArray = NULL;
    targetIndexBaseArray = NULL;
  }
}

DspVariableDelay::~DspVariableDelay() {
  free(xArray);
  free(targetIndexBaseArray);
}

const char *DspVariableDelay::getObjectLabel() {
  return "vd~";
}

void DspVariableDelay::processDspToIndex(float newBlockIndex) {
  int headIndex;
  int bufferLength;
  float *buffer = delayline->getBuffer(&headIndex, &bufferLength);
  float bufferLengthFloat = (float) bufferLength;
  float *inputBuffer = localDspBufferAtInlet[0];
  float *outputBuffer = localDspBufferAtOutlet[0];
  
  float targetIndexBase = (float) (headIndex - blockSizeInt);
  if (ArrayArithmetic::hasAccelerate) {
    #if __APPLE__
    // calculate delay in samples (vector version of StaticUtils::millisecondsToSamples)
    float samplesPerMillisecond = sampleRate / 1000.0f;
    vDSP_vsmul(inputBuffer, 1, &samplesPerMillisecond, xArray, 1, blockSizeInt);
    
    float zero = 0.0f;
    float one = 1.0f;
    // TODO(mhroth): because vDSP_vtabi already does clipping to the buffer bounds, can the following
    // steps be rearranged or optimised in order to provide the minimum necessary information to the
    // tablel lookup. Let it do as much work as possible.
    vDSP_vclip(xArray, 1, &zero, &bufferLengthFloat, xArray, 1, blockSizeInt); // clip the delay between 0 and the buffer length
    vDSP_vramp(&targetIndexBase, &one, targetIndexBaseArray, 1, blockSizeInt);  // create targetIndexBaseArray
    vDSP_vsub(xArray, 1, targetIndexBaseArray, 1, xArray, 1, blockSizeInt); // targetIndexBaseArray - xArray (== targetSampleIndex)
    
    // ensure that targetSampleIndex is positive
    // TODO(mhroth): vectorise this!
    for (int i = 0; i < blockSizeInt; i++) {
      if (xArray[i] < 0.0f) {
        xArray[i] += bufferLengthFloat;
      }
    }
    
    // do table lookup (in buffer) using xArray as indicies, with linear interpolation 
    vDSP_vtabi(xArray, 1, &one, &zero, buffer, bufferLength+1, outputBuffer, 1, blockSizeInt);
    #endif
  } else {
    for (int i = 0; i < blockSizeInt; i++, targetIndexBase+=1.0f) {
      float delayInSamples = StaticUtils::millisecondsToSamples(inputBuffer[i], sampleRate);
      if (delayInSamples < 0.0f) {
        delayInSamples = 0.0f;
      } else if (delayInSamples > bufferLengthFloat) {
        delayInSamples = bufferLengthFloat;
      }
      
      float targetSampleIndex = targetIndexBase - delayInSamples;
      if (targetSampleIndex < 0.0f) {
        targetSampleIndex += bufferLengthFloat;
      }
      
      // 2-point linear interpolation (basic and fast)
      int x0 = (int) targetSampleIndex;
      float dx = targetSampleIndex - ((float) x0);
      float y0 = buffer[x0];
      float y1 = buffer[x0+1];
      float slope = (y1 - y0); // /(x1 - x0) == 1.0f!
      outputBuffer[i] = (slope * dx) + y0;
    }
  }
}
