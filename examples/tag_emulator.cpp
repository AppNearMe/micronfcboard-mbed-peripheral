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
        if( !nfc.connected() )
        {
            printf("Writing message\r\n");
            nfc.writeNdefText("en", "AppNearMe");
        }

        printf("Poll\r\n");
        nfc.startPolling(false, true, false);

        while( nfc.polling() );

        if( nfc.type4Emulator() )
        {
            printf("Emulating type 4 tag\r\n");
        }
        else
        {
          continue;
        }

        bool ndefRead = false;
        while( nfc.connected() || nfc.polling() )
        {
            if( !ndefRead && nfc.ndefPresent() )
            {
                printf("Got message\r\n");
                char buf[512];
                if( nfc.readNdefUri(buf, sizeof(buf)) )
                {
                  printf("Got URI: %s\r\n", buf);
                }
                if( nfc.readNdefText(buf, sizeof(buf)) )
                {
                  printf("Got Text: %s\r\n", buf);
                }
                ndefRead = true;
            }
        }

        printf("Disconnected\r\n");
    }
}
