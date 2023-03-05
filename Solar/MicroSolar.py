# This script will provide a calculation that helps to size a solar panel and battery for a microcontroller

# References 
    # Andreas Spiess
    # 142 Solar Power for the ESP8266, Arduino, etc.
    # https://www.youtube.com/watch?v=WdP4nVQX-j0&t=3s
    
    # NREL Photovoltatic Calculator
    # https://pvwatts.nrel.gov/index.php
    
    # Alaska dot org Daylight Hours Calculator
    # https://www.alaska.org/weather/daylight-hours/anchorage/december
    
    # NWS Weather Data for Anchorage Alaska
    # https://w1.weather.gov/data/obhistory/PANC.html
    
# Made with Mu v.1.0.3
    # February 2023
    # https://codewith.mu

hours = 0
watts = 0

def hoursCalc():
    """This function calculates the number of earth year hours and updates a global variable"""
    #print("There are 365 days in a year")
    #print("There are 24 hours in a day")
    #print("There are 8760 hours in a year")
    yH = 365*24
    #print("There are ", yH, " hours in a earth year\n")

    # this updates a global variable and is bad form ... to do: redo script using object oriented programming
    global hours
    #print(hours)  
    hours = yH + hours
    #print(hours)
    
def microPower():    
#def microPower(mcuCurrent, mcuVoltage):
    """This function calculates the number of watts required by a typical microcontroller""" 
    print("Current is defined as the flow of electrons and uses the units of Amps (columbs per second)")
    print("There are 1000 milliamps in a Amp")
    print("A typical IoT microcontroller requires 150 milliamps\n")
    
    print("Voltage is defined as the electrical potential difference between two points and uses the units of Volts")
    print("Water pressure is similar to voltage")
    print("A typical IoT microcontroller requires 3.3 volts\n")
    
    print("Power, or the rate of energy transfer, uses the units of Watts (J/s) in electrical systems")
    print("Watts can be calculated by multiplying Volts by Amps")
    print("A typical IoT microcontroller requires 0.5 watts\n")    

    mcuCurrent = float(input("How many milliamps does your IoT microcontroller need? "))
    mcuVoltage = float(input("How many Volts does your IoT microcontroller need? "))    
    mcuWatts = round((mcuCurrent/1000)*mcuVoltage,2)
    print("Your IoT microcontroller likely needs ", round(mcuWatts,2), "watts\n")
    
    # this updates a global variable and is bad form ... to do: redo script using object oriented programming    
    global watts 
    #print(watts)
    watts  = mcuWatts + watts
    #print(watts)
    
def solarPanel():
    """This function helps to estimate the size of solar panel needed to support a microcontroller"""
    print("A typical solar panel and microcontroller system is likely to be able to capture and use 7% of available sunlight\n")
    
    print("Anchorage Alaska receives approximately 0.7 kWh of sunlight per square meter and 6.5 hours of sunlight per day in January")
    janHours = 6.5
    janWattsSqMeter = 0.7*1000*6.5
    #print(watts)
    print("Your microcontroller will need about ", round(watts*janHours,1), "watts and the sun will provide about ", round(janWattsSqMeter,1), "watts/m^2 during a typical January day")
    sysEfficiency = 0.07
    xAreaWinter = (watts*janHours)*(1/(janWattsSqMeter*sysEfficiency))
    #print(watts*janHours == xAreaWinter*janWattsSqMeter*sysEfficiency)
    print("Your microcontroller will likely need a panel that is ", round(xAreaWinter,4), "sqaure meters in size")
    print("This equates to ", round(xAreaWinter*10000,4), "square centimeters or", round(xAreaWinter*10000*(1/6.452),2), "square inches in size in January\n")
    WinterTestPanelHeight = 3.5 # inches
    WinterTestPanelWidth = 4.5 # inches
    print("The January 2023 indoor test panel, for a 3.3V 150 milliAmp MCU, was about", WinterTestPanelHeight*WinterTestPanelWidth, "square inches in size")
    print("https://www.adafruit.com/product/3809\n")
    
    print("Anchorage Alaska receives approximately 5.06 kWh of sunlight per square meter and 19.2 hours of sunlight per day in June")
    juneHours = 19.2
    juneWattsSqMeter = 5.06*1000*19.2
    #print(watts)
    print("Your microcontroller will need about ", round(watts*juneHours,1), "watts and the sun will provide about", round(juneWattsSqMeter,1), "watts/m^2 during a typical June day")
    sysEfficiency = 0.10
    xAreaSummer = (watts*juneHours)*(1/(juneWattsSqMeter*sysEfficiency))
    #print(watts*juneHours == xAreaSummer*juneWattsSqMeter*sysEfficiency)
    print("Your microcontroller will likely need a panel that is ", round(xAreaSummer,4), "sqaure meters in size")
    print("This equates to ", round(xAreaSummer*10000,4), "square centimeters or", round(xAreaSummer*10000*(1/6.452),2), "square inches in size in June\n")
    SummerTestPanelDiameter = 2 # inches
    SummerTestPanelQuantity = 2
    print("The June 2022 indoor test panel, for a 3.3V 150 milliAmp MCU, was about", round(SummerTestPanelQuantity*3.14*(SummerTestPanelDiameter*SummerTestPanelDiameter)*0.25,1), "square inches in size")
    print("https://www.adafruit.com/product/700\n")    
    
def battery():
    """This function helps to estimate the size of a battery needed to support a microcontroller"""
    print("Anchorage Alaska will likely reach -15F during a typical January")

def main():
    """This function calls the various script functions"""
    hoursCalc()
    microPower()
    solarPanel()
    battery()
    
    #print("Hours in a earth year", hours)

if __name__ == '__main__':
    main()
