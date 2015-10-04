# Flash firmware to device
#
# Dependencies: esptool.py

./flash-mode.py
esptool.py --port /dev/ttyUSB0 write_flash 0x40000 firmware/eagle.irom0text.bin 0x00000 firmware/eagle.flash.bin
./normal-mode.py
