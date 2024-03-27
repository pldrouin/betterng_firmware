This is an open source firmware for the BigNG dual-mode (voltage + PWM) fan controller.
It allows full control of the 4 fan channels using digital, analog and software temperature readings.
It supports the calibration of the fans for both the voltage and PWM control modes and also to seamlessly switch
control mode as a function of the desired fan output. Calibration of the digital and analog temperature proves is
also supported. The firmware uses an advanced algorithm to reconstruct fan speed in PWM mode. Alarms can be defined
for overtemperature and fan stalling conditions. The firmware can be configured using the https://github.com/pldrouin/betterng_controller
host software.
