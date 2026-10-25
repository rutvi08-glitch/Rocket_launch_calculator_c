# Rocket Launch Command

A prototype C program that calculates local gravity, thrust-to-weight ratio, payload limit, minimum thrust required, and interplanetary launch window for a rocket.

Enter a location, rocket details, and a target planet, and the program tells you whether the rocket can lift off, how much payload it can carry, and when to launch to reach Mercury, Venus, or Mars.

This is a first-year C programming project. It uses simplified models, so treat the results as estimates.

## Features

- Local gravity from latitude and altitude
- Weight of the full rocket and the thrust needed just to lift off
- Thrust-to-weight ratio with a GO / NO GO result
- Maximum payload the rocket can carry, with a GO / NO GO result
- Free eastward speed gained from Earth's rotation
- Travel time to Mercury, Venus, or Mars
- Best launch date and arrival date

## Inputs

| Input | Unit / range |
|---|---|
| Latitude | -90 to 90 degrees (north positive) |
| Longitude | -180 to 180 degrees (east positive) |
| Altitude above sea level | metres |
| Rocket mass | kg |
| Payload mass | kg |
| Today's date | dd/mm/yyyy |
| Thrust | newtons |
| Target planet | 1 = Mercury, 2 = Venus, 3 = Mars |

## How it works

- **Gravity:** uses a latitude-based gravity formula, with a correction that reduces gravity as altitude increases.
- **Weight and thrust-to-weight ratio:** weight = total mass x gravity. The rocket can lift off only if thrust / weight is greater than 1.
- **Earth's spin:** speed at the equator is about 465.1 m/s, scaled by cos(latitude).
- **Maximum payload:** the smaller of two limits: what the thrust can lift (with a 1.2 safety margin) and a typical real-rocket payload fraction of 1.5% of rocket mass.
- **Travel time:** a Hohmann transfer orbit, taking half of the transfer orbit period.
- **Launch window:** finds the date when the angle between Earth and the target planet is right for the transfer, using simplified planet positions with circular orbits.


## Example run

==============================================
          Rocket Launch Command               
==============================================
                     ^                        
                   /   \                     
                  /_____\                    
                  |  R  |                     
                  |  O  |                     
                  |  C  |                     
                  |  K  |                     
                  |  E  |                     
                 /|  T  |\                   
               / /|_____|\ \                
               \/   \/    \/               
               ||   ||    ||                  


Latitude (-90 to 90 in degrees north): 3.5
Longitude (-180 to 180 in degrees east): 42.7
Altitude above sea level (m): 500
Rocket mass(kg): 5600000
Payload mass (kg): 72600
Today's date(dd/mm/yyyy):25/10/2026
Thrust(N): 67000000
Target planet[mercury = 1, venus = 2, mars = 3]: 3

        X---RESULTS FOR YOUR LOCATION---x
Local gravity                    : 9.77898 m/s^2
Weight of full rocket            : 55472222 N or 55472 kN
Thrust needed to just take off   : 55472222 N 
Earth's spin gives you           : 464 m/s eastward for free
Thrust to weight Ratio           : 1.207812 
Travel time to reach the planet  : 258.8 days
Days to wait for launch          : 21 days
Best launch date                 : 15 Nov 2026
Arrival date                     : 01 Aug 2027
Maximum Payload for this rocket  : 85279kg


        ----Thrust to weight ratio > 1: GO----

        ----Payload mass within limit  : GO



## Limitations

- Longitude is asked for but not used in any calculation yet.
- Planet positions and orbits are simplified (circular orbits, fixed rates), so dates are approximate.
- Only Mercury, Venus, and Mars are supported.
- Does not simulate the ascent, atmospheric drag, or range safety.

## Future work

- Use longitude (for example, to find the nearest real launch site)
- Add an ocean or downrange safety check
- Add more planets and more accurate orbits

## License

(MIT)# Rocket_launch_calculator_c
A prototype C program that calculates local gravity, thrust-to-weight ratio, payload limit, minimum thrust required, and interplanetary launch window for a rocket.
