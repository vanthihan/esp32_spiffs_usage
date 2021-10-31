ESP32 Flashing a Text file into SPIFFS partition
====================

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
