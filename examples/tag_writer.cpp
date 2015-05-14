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

#include "mbed.h"
#include "micronfcboard.h"

MicroNFCBoard nfc(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, D9);

int main() {
    nfc.init();

    //Start polling
    while(true)
    {
        printf("Poll\r\n");
        nfc.startPolling(true, false, false);

        while( nfc.polling() );

        if( nfc.type2Tag() )
        {
            printf("Connected to type 2 tag\r\n");
        }
        else
        {
          continue;
        }

        bool ndefMessageWritten = false;
        bool ndefWritingStarted = false;
        while( nfc.connected() )
        {
            if( !ndefWritingStarted && nfc.ndefWriteable() )
            {
                printf("Writing tag\r\n");
                nfc.writeNdefUri("http://www.micronfcboard.com");
                nfc.ndefWrite();
                ndefWritingStarted = true;
            }
            if( ndefWritingStarted && !ndefMessageWritten && !nfc.ndefBusy() )
            {
              ndefMessageWritten = true;
              if( nfc.ndefSuccess() )
              {
                printf("Tag write successful\r\n");
              }
              else
              {
                printf("Tag write failed\r\n");
              }
            }
        }

        printf("Disconnected\r\n");
    }
}
