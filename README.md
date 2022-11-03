# ESP-Riders_Mecanum

https://youtu.be/-3dxDYtFfpw

ESP-Riders Mecanum uses an ESP32 Dev kit module to control a small robot's direction and speed with two L9110S H-bridges to control 4 motors.

The code uses PWM and channels.

The code will set up a wifi access point on the car which will allow a user to log in and access 192.168.4.1 which is the robot's control html page.

Esp32 must be set up in preferences in Arduino IDE. https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

Parts required:
ESP32 devkit or similar.
Two L9110S H-bridge.
Power and Electric motors of your choice.
A robot frame and 4 mecanum motors properly spaced and with the tires in the proper orientation.  X on top.

Tested on Android Phone.

Check the code for pin to motor assignments.

I powered mine with a Meccanoid battery connected to both the Vin and Ground of the Esp32 Dev kit 1 and to the L9110S H-Bridges through separate 
switches/connectors.  

Power up is a two step process which involves first connecting the power to the ESP32 and ensuring a wifi link to your phone by loading the webpage,
then second, connecting the same battery to the H-Bridges with a separate switch/connection.
