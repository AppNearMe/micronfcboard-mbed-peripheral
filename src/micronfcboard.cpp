/**
 * \file micronfcboard.cpp
 * \copyright Copyright (c) AppNearMe Ltd 2015
 * \author Donatien Garnier
 */

#include "micronfcboard.h"

#define STATUS_POLLING          (1 << 0)
#define STATUS_CONNECTED        (1 << 1)
#define STATUS_NDEF_PRESENT     (1 << 2)
#define STATUS_NDEF_READABLE    (1 << 3)
#define STATUS_NDEF_WRITEABLE   (1 << 4)
#define STATUS_NDEF_BUSY        (1 << 5)
#define STATUS_NDEF_SUCCESS     (1 << 6)

#define STATUS_TYPE_MASK        (0xFF << 8)
#define STATUS_TYPE2            (2 << 8)
#define STATUS_P2P              (8 << 8)

#define RECORD_URI  1
#define RECORD_TEXT 2
#define RECORD_SP   3

MicroNFCBoard::MicroNFCBoard(PinName mosi, PinName miso, PinName sck, PinName cs, PinName irq) : _transport(mosi, miso, sck, cs, irq)
{

}

void MicroNFCBoard::init()
{
  _transport.init();
  _status = _transport.status();
}

void MicroNFCBoard::setLeds(bool led1, bool led2)
{
  _transport.leds(led1, led2);
}

void MicroNFCBoard::updateStatus()
{
  if(_transport.statusChanged())
  {
    _status = _transport.status();
  }
}

bool MicroNFCBoard::connected()
{
  updateStatus();
  return _status & STATUS_CONNECTED;
}

bool MicroNFCBoard::type2()
{
  updateStatus();
  return (_status & STATUS_TYPE_MASK) == STATUS_TYPE2;
}

bool MicroNFCBoard::p2p()
{
  updateStatus();
  return (_status & STATUS_TYPE_MASK) == STATUS_P2P;
}

bool MicroNFCBoard::polling()
{
  updateStatus();
  return _status & STATUS_POLLING;
}

bool MicroNFCBoard::ndefReadable()
{
  updateStatus();
  return _status & STATUS_NDEF_READABLE;
}

bool MicroNFCBoard::ndefWriteable()
{
  updateStatus();
  return _status & STATUS_NDEF_WRITEABLE;
}

bool MicroNFCBoard::ndefPresent()
{
  updateStatus();
  return _status & STATUS_NDEF_PRESENT;
}

bool MicroNFCBoard::ndefBusy()
{
  updateStatus();
  return _status & STATUS_NDEF_BUSY;
}

bool MicroNFCBoard::ndefSuccess()
{
  updateStatus();
  return _status & STATUS_NDEF_SUCCESS;
}

void MicroNFCBoard::startPolling()
{
  _transport.nfcPoll(true);
}

void MicroNFCBoard::stopPolling()
{
  _transport.nfcPoll(false);
}

void MicroNFCBoard::ndefRead()
{
  _transport.nfcOperation(true, false);
}

void MicroNFCBoard::ndefWrite()
{
  _transport.nfcOperation(false, true);
}

bool MicroNFCBoard::readNdefUri(char* uri, size_t maxUriLength)
{
  if(!ndefPresent())
  {
    return false;
  }

  size_t recordCount = 0;
  _transport.nfcGetMessageInfo(&recordCount);


  size_t recordNumber = 0;
  uint16_t info[4];
  uint16_t type;
  size_t infoCount = 4;

  for(recordNumber = 0; recordNumber < recordCount; recordNumber++)
  {
    _transport.nfcGetRecordInfo(recordNumber, &type, info, infoCount);
    if(type == RECORD_URI)
    {
      break;
    }
    if(type == RECORD_SP)
    {
      recordCount += info[1];
    }
  }
  if(recordNumber == recordCount)
  {
    return false;
  }

  //Recover prefix
  size_t length = maxUriLength - 1;
  _transport.nfcDecodePrefix(info[0], uri, &length);

  maxUriLength -= length;
  uri += length;

  if(maxUriLength <= 1)
  {
    return false;
  }

  length = info[1];
  if(length > maxUriLength - 1)
  {
    return false;
  }

  size_t off = 0;
  while(length > 0)
  {
    size_t cpyLength = length;
    if(cpyLength > 32)
    {
      cpyLength = 32;
    }
    _transport.nfcGetRecordData(recordNumber, 0, off, (uint8_t*)uri, cpyLength);
    length -= cpyLength;
    off += cpyLength;
    uri += cpyLength;
  }

  uri[0] = '\0';

  return true;
}

bool MicroNFCBoard::readNdefText(char* text, size_t maxTextLength)
{
  if(!ndefPresent())
  {
    return false;
  }

  size_t recordCount = 0;
  _transport.nfcGetMessageInfo(&recordCount);


  size_t recordNumber = 0;
  uint16_t info[4];
  uint16_t type;
  size_t infoCount = 4;

  for(recordNumber = 0; recordNumber < recordCount; recordNumber++)
  {
    _transport.nfcGetRecordInfo(recordNumber, &type, info, infoCount);
    if(type == RECORD_TEXT)
    {
      break;
    }
    if(type == RECORD_SP)
    {
      recordCount += info[1];
    }
  }
  if(recordNumber == recordCount)
  {
    return false;
  }

  size_t length = info[2];
  if(length > maxTextLength - 1)
  {
    return false;
  }

  size_t off = 0;
  while(length > 0)
  {
    size_t cpyLength = length;
    if(cpyLength > 32)
    {
      cpyLength = 32;
    }
    _transport.nfcGetRecordData(recordNumber, 1, off, (uint8_t*)text, cpyLength);
    length -= cpyLength;
    off += cpyLength;
    text += cpyLength;
  }

  text[0] = '\0';

  return true;
}



