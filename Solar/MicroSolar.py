# This script will provide a rough estimate for a solar panel and battery for a microcontroller

# References 
    # Andreas Spiess
    # 142 Solar Power for the ESP8266, Arduino, etc.
    # https://www.youtube.com/watch?v=WdP4nVQX-j0&t=3s
    
    # NREL Photovoltatic Calculator
    # https://pvwatts.nrel.gov/index.php

hours = 0
watts = 0

def yearlyHours():
    """This function calculates the number of hours in a year and updates a global variable"""
    #print("There are 365 days in a year")
    #print("There are 24 hours in a day")
    #print("There are 8760 hours in a year")
    yH =+ 365*24
    print("There are ", yH, " hours in a year\n")

    # this updates a global variable and is bad form ... todo redo script using object oriented programming
    global hours
    #print(hours)  
    hours = yH + hours
    #print(hours)
    
def microPower():    
#def microPower(microCurrent, microVoltage):
    """This function calculates the number of kilowatt hours per year required by a typical microcontroller""" 
    print("Current is defined as the flow of electrons and uses the units of Amps per unit of time")
    print("There are 1000 milliamps in a Amp")
    #print("A typical IoT microcontroller requires 150 milliamps per hour or 1,314 Amps per year\n")    
    print("A typical IoT microcontroller requires 150 milliamps per hour or", hours*150/1000, "Amps per year\n")
    
    print("Voltage is defined as the electrical potential difference between two points and uses the units of Volts")
    print("Water pressure is similar to voltage")
    print("A typical IoT microcontroller requires 3.3 volts\n")
    
    print("Power, or the rate of energy transfer, uses the units of Watts per unit of time in electrical systems")
    print("Watts can be calculated by multiplying Volts by Amps")
    #print("A typical IoT microcontroller requires 0.5 Watts per hour or 4.3 kWh per year\n")    
    print("A typical IoT microcontroller requires 0.5 Watts per hour or", hours*0.5/1000, "kWh per year\n")

    microCurrent = int(input("How many milliamps does your IoT microcontroller need? "))
    microVoltage = float(input("How many Volts does your IoT microcontroller need? "))    
    #microWatts = (150/1000)*3.3
    microWatts = (microCurrent/1000)*microVoltage
    print("Your IoT microcontroller likely needs ", microWatts, "Watts per hour or ", hours*microWatts/1000, "kWh per year\n")
    
def solarPanel():
    """This function estimates the size of solar panel needed to support a microcontroller"""
    print("Anchorage Alaska receives approximately 0.7 kWh of sunlight per square meter per day in January")
    print("Anchorage Alaska receives approximately 5.06 kWh of sunlight per square meter per day in June")
    
def main():    
    yearlyHours()
    microPower()
    solarPanel()
    
    #print("Hours in a earth year", hours)

if __name__ == '__main__':
    main()
