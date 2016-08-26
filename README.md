# humidity_sensing_setup
Humidity Sensing Setup For Graphene Studies. LabView and Arduino IDE codes with PCB files of electronics.

The files in this repository are for graphene atmosphere interaction project that held in Nanoscale Surface Science Laboratory 
in Istanbul Technical University, Istanbul. The project is suported by Tübitak 1003 project with the grant number of 114F036.

The files consist three parts; Arduino IDE codes, LabView codes and PCB files for electronics. The purposes of these files will
be listed further.

In order to observe effects of the atmospheric events on graphene, we created an isolated chamber with variance of humidity, 
pressure and temperature levels. In order to measure these quantities a SHT75 humidity temperature sensor, a BME280 pressure 
sensor and MAX31855 thermocouple sensor used with an arduino. The data comes from these sensors are read from serial port with 
the code written in Arduino IDE with corresponding libraries of the sensors. In order to prevet the wire chaos, a shield for
Arduino designed. A LabView code reads the serial port and plots the real tie graph of the temperature data that comes from the
thermocouple, humidity, pressure and atmospheric temperature. Also the resistivity of graphene sample is measured with a Keithley
2400 SourceMeter and this sourcemeter is controlled with the LabView code and readouts are plotted with LabView. 


For further information you can contact with me;
haltugyildirim_at_protonmail.com
