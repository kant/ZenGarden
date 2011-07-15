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

#ifndef _DSP_MULTIPLY_H_
#define _DSP_MULTIPLY_H_

#include "DspObject.h"

enum DspMultiplyCodePath {
  DSP_MULTIPLY_DSP1_MESSAGE0, // zero or one dsp inputs on left inlet, no message inputs on right inlet
  DSP_MULTIPLY_DSPX_MESSAGE0, // many dsp inputs on left inlet, no message inputs on right inlet
  DSP_MULTIPLY_DSPX_MESSAGEX, // many dsp inputs on left inlets, many message inputs on right inlet
  DSP_MULTIPLY_DSP1_DSP1,     // zero or one dsp inputs on left inlet, one dsp input on the right inlet
  DSP_MULTIPLY_DSPX_DSP1,     // many dsp inputs on left inlets, one dsp input on right inlet
  DSP_MULTIPLY_DSPX_DSPX,     // many dsp inputs on left inlets, many dsp inputs on right inlet
  DSP_MULTIPLY_DEFAULT        // corner cases
};


class DspMultiply : public DspObject {
  
  public:
    DspMultiply(PdMessage *initMessage, PdGraph *graph);
    ~DspMultiply();
  
    const char *getObjectLabel();
  
    void addConnectionFromObjectToInlet(MessageObject *messageObject, int outletIndex, int inletIndex);
  
    void processDsp();
    
  private:
    void processMessage(int inletIndex, PdMessage *message);
    void processDspWithIndex(int fromIndex, int toIndex);
    
    DspMultiplyCodePath codePath;
    
    float inputConstant;
    float constant;
};

#endif // _DSP_MULTIPLY_H_
