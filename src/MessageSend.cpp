/*
 *  Copyright 2009 Reality Jockey, Ltd.
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

#include "MessageSend.h"
#include "PdGraph.h"

MessageSend::MessageSend(PdMessage *initMessage, PdGraph *graph) : MessageObject(1, 0, graph) {
  if (initMessage->getNumElements() > 0 && initMessage->getElement(0)->getType() == SYMBOL) {
    name = StaticUtils::copyString(initMessage->getElement(0)->getSymbol());
  } else {
    name = NULL;
  }
}

MessageSend::~MessageSend() {
  free(name);
}

const char *MessageSend::getObjectLabel() {
  return "send";
}

ObjectType MessageSend::getObjectType() {
  return MESSAGE_SEND;
}

void MessageSend::processMessage(int inletIndex, PdMessage *message) {
  graph->dispatchMessageToNamedReceivers(name, message);
}
