/*
 * vfs_handler.h
 *
 *  Created on: Oct 19, 2021
 *      Author: kai
 */

/**************Command In Use**************************
* flashing file into esp32's spiffs partition
//put file need to gen into
~/samba/workSpace/spiffs_test/data

//Genarate
cd /home/kai/esp/mkspiffs_tool/mkspiffs
./mkspiffs -c ~/samba/workSpace/spiffs_test/data/ -b 4096 -p 256 -s 0x100000 ~/samba/workSpace/spiffs_test/data/mobile_gestures.bin

//Flash
cd /home/kai/esp/esp-idf/components/esptool_py/esptool/
python esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 write_flash -z 0x210000 ~/samba/workSpace/spiffs_test/data/mobile_gestures.bin

**************End Of Command In Use**************************/

#ifndef MAIN_COMPONENT_MLC_HANDLER_SPIFFS_HANDLER_H_
#define MAIN_COMPONENT_MLC_HANDLER_SPIFFS_HANDLER_H_

#include "spiffs_config.h"

void spiffs_handler_register();
void spiffs_handler_read();
void spiffs_handler_deregister();


#endif /* MAIN_COMPONENT_MLC_HANDLER_SPIFFS_HANDLER_H_ */
