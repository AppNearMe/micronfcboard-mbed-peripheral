/*
MicroNFCBoard mbed API

Copyright (c) 2014-2015 AppNearMe Ltd

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 */

#ifndef SRC_MICRONFCBOARD_H_
#define SRC_MICRONFCBOARD_H_

#include "mbed.h"
#include <cstddef>

#include "transport.h"

class MicroNFCBoard
{
public:
  MicroNFCBoard(PinName mosi, PinName miso, PinName sck, PinName cs, PinName irq);

  void init();

  void setLeds(bool led1, bool led2);

  void updateStatus();

  bool connected();

  bool type2Tag();

  bool type4Emulator();

  bool p2p();

  bool polling();

  bool ndefReadable();

  bool ndefWriteable();

  bool ndefPresent();

  bool ndefBusy();

  bool ndefSuccess();

  void startPolling(bool readerWriter, bool emulator, bool p2p);

  void stopPolling();

  void ndefRead();

  void ndefWrite();

  bool readNdefUri(char* uri, size_t maxUriLength);

  bool readNdefText(char* text, size_t maxTextLength);

  void writeNdefUri(const char* uri);

  void writeNdefText(const char* lang, const char* text);

protected:
  Transport _transport;

private:
  uint32_t _status;
};

#endif /* SRC_MICRONFCBOARD_H_ */
