setlocal
cd /d %~dp0
mklink ArduinoSwitch.cpp ..\..\Source\ArduinoSwitch.cpp
mklink ArduinoSwitch.hpp ..\..\Source\ArduinoSwitch.hpp
endlocal
pause