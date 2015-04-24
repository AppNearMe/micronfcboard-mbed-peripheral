/**
 * \file micronfcboard.h
 * \copyright Copyright (c) AppNearMe Ltd 2015
 * \author Donatien Garnier
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

  bool type2();

  bool p2p();

  bool polling();

  bool ndefReadable();

  bool ndefWriteable();

  bool ndefPresent();

  bool ndefBusy();

  bool ndefSuccess();

  void startPolling();

  void stopPolling();

  void ndefRead();

  void ndefWrite();

  bool readNdefUri(char* uri, size_t maxUriLength);

  bool readNdefText(char* text, size_t maxTextLength);

  void writeNdefUri(char* uri);

  void writeNdefText(char* text);

protected:
  Transport _transport;

private:
  uint32_t _status;
};

#endif /* SRC_MICRONFCBOARD_H_ */
