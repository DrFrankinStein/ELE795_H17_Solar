setlocal
cd /d %~dp0
mklink SPIDualLS7366R.cpp ..\..\Source\SPIDualLS7366R.cpp
mklink SPIDualLS7366R.hpp ..\..\Source\SPIDualLS7366R.hpp
mklink ArduinoSwitch.cpp ..\..\Source\ArduinoSwitch.cpp 
mklink ArduinoSwitch.hpp ..\..\Source\ArduinoSwitch.hpp 
mklink ADCLightCaptor.cpp ..\..\Source\ADCLightCaptor.cpp 
mklink ADCLightCaptor.hpp ..\..\Source\ADCLightCaptor.hpp
endlocal
pause