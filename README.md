Initial PlatformIO project for TTGO-T-Watch-2020 based of SimpleWatch example

A BLE UART device is advertised to emulate a bangle.js/Espruino to receive [json messages](https://www.espruino.com/Gadgetbridge) from [Gadgetbridge](https://gadgetbridge.org/). This allows receiving all notifications from an Android phone.

Time updates from Gadgetbridge are parsed and applied to the RTC and system time.

Gadgetbridge `notify` messages are shown on screen in a popup message.

`gui.cpp` was refactored slightly to separate GUI header and class implementation. Class definitions are now in `gui.h` so that other files may reference the GUI classes.

Thanks @bburky for starting this up!

I am completely new to LVGL so this will all take a bit...

Status:
- Added notifications for Music Player
- Added notifications for Calls
- Added notifications for Weather (using Weather Notification app from Fdroid)
- Added queue screen with Icon.
- Added weather screen with Icon.
- Added music screen with Icon.
- Limit mssga body display in popup to 165 (more or less??) char.

To Do:
- Create message queue screen showing last X messages
- Create weather screen with forecast
- Create music player screen with pause/play buttons
- Add small game

