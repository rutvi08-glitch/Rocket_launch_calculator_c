#include<stdio.h>
#include<math.h>
#include<time.h>

#define PI 3.14159265359
#define L0_earth 100.4646
#define rate_earth 0.985647
#define payload_fraction 0.015
//                            mercury    venus      mars
const double period_days[] = {87.97,    224.70,    686.98};
const double L0_planet[] =   {252.2503, 181.9791, -4.5534};
const double rate_planet[] = {4.092334, 1.602131,  0.524038};

typedef struct 
{
    double lat; //north is to be considered positive
    double lon; //east is to be considered positive
    double alt; //height in metres
    
}Location; 

double Radians(double deg)
{
    return (deg * PI) / 180.0;
}

double gravity(const Location *p)
{
    double phi = Radians(p->lat);
    double s1 = sin(phi);
    double s2 = sin(2.0 * phi);

    double g = 9.780327 * (1.0 + 0.0053024 * s1 *s1 - 0.0000058 * s2 *s2);

    return g - 3.086e-6 * p->alt;
}

double travel_time(int *planet)
{
    const double Earth_sun_distance = 149597870700.0;
    double target_sun_distance;
    int target_planet;
    double a, T, t;
    
    planet_selection:
    printf("Target planet[mercury = 1, venus = 2, mars = 3]: ");
    if(scanf("%d", &target_planet) != 1){
        printf("Invalid Input\n");
        while (getchar() !='\n');
        goto planet_selection;
    }
    else if(target_planet < 1 || target_planet >3){
        printf("Invalid input\n");
        while(getchar() != '\n');
        goto planet_selection;
    }
    else if(target_planet == 1){
        target_sun_distance = 5.8e10;
    }
    else if(target_planet == 2){
        target_sun_distance = 1.082e11;
    }
    else if(target_planet == 3){
        target_sun_distance = 2.279e11;
    }

    a = (Earth_sun_distance + target_sun_distance) /2;
    const double gp_sun = 1.327e20; //gravitaional parameter
    T = 2*PI * (sqrt((a*a*a) / gp_sun));
    t = T/2;
    *planet = target_planet;
    return t;
}
double required_angle(double t_days, double T_target)
{
    double theta;
    double required;
    theta = (t_days * 360)/ T_target;
    required = 180 - theta;

    if(required < 0)
    {
        required = required + 360;
    }

    return required;
}
double wrap360(double angle){
    angle = fmod(angle, 360.0);
    if (angle < 0) 
        angle+= 360.0;
    
    return angle;
}
double max_payload_thrust(double thrust, double g, double rocketmass)
{
    double max_total_mass = thrust / (g * 1.2);
    double max_payload_1 = max_total_mass - rocketmass;
    double max_payload_2 = payload_fraction * rocketmass / (1 - payload_fraction); //limit based on typical payload of real rockets
    double max_payload = (max_payload_1 < max_payload_2) ? max_payload_1 : max_payload_2;

    return max_payload;
}

int main(void){

    printf("==============================================\n");
    printf("          Rocket Launch Command               \n");
    printf("==============================================\n");
    printf("                     ^                        \n");
    printf("                   /   \\                     \n");
    printf("                  /_____\\                    \n");
    printf("                  |  R  |                     \n");
    printf("                  |  O  |                     \n");
    printf("                  |  C  |                     \n");
    printf("                  |  K  |                     \n");
    printf("                  |  E  |                     \n");
    printf("                 /|  T  |\\                   \n");
    printf("               / /|_____|\\ \\                \n");
    printf("               \\/   \\/    \\/               \n");
    printf("               ||   ||    ||                  \n");

    Location here;
    double rocketmass, payloadmass, totalmass;
    double g, thrust, spinboost;
    double weight, days;
    double TWR ; //Thrust to weight ratio must be > 1 to lift off
    int planet;
    int day, month, year;
    double d;
    double required, earth, target, gap, rel, wait;
    time_t launch, arrival;
    char buf[32];
    double max_payload;

    printf("\n\nLatitude (-90 to 90 in degrees north): ");
    if (scanf("%lf", &here.lat) != 1 || here.lat < -90 || here.lat > 90){
        printf("\nInvalid latitude.");
        return 1;
    }
    printf("Longitude (-180 to 180 in degrees east): ");
    if (scanf("%lf", &here.lon) !=1 || here.lon < -180 || here.lon > 180 ){
        printf("\nInvalid Longitude.");
        return 1;
    }
    printf("Altitude above sea level (m): ");
    if (scanf("%lf", &here.alt) != 1){
        printf("\nInvalid altitude.");
        return 1;
    }
    printf("Rocket mass(kg): ");
    if(scanf("%lf", &rocketmass) != 1){
        printf("\nInvalid Input");
        return 1;
    }
    printf("Payload mass (kg): ");
    if(scanf("%lf",&payloadmass) !=1){
        printf("\nInvalid input");
        return 1;
    }
    printf("Today's date(dd/mm/yyyy):");
    if(scanf("%d/%d/%d",&day, &month, &year) != 3){
        printf("Invalid date");
        return 1;
    }
    else if(day<1 || month<1 || year<2000){
        printf("Invalid date");
        return 1;
    }
    else if(day>31 || month> 12){
        printf("Invalid date");
        return 1;
    }


    struct tm date = {0};
    date.tm_mday = day;
    date.tm_mon = month - 1 ;
    date.tm_year = year - 1900;
    date.tm_hour = 12;

    time_t when = mktime(&date);
    if(when == -1 || date.tm_mday != day){
        printf("Invalid date");
        return 1;
    }
    

    totalmass = rocketmass + payloadmass;
    g         = gravity(&here);
    weight    = totalmass * g;  // F = m x g
    spinboost = 465.1 * cos(Radians(here.lat));
    printf("Thrust(N): ");
    if(scanf("%lf",&thrust) != 1){
        return 1;
    };
    TWR = thrust/weight;
    max_payload = max_payload_thrust(thrust, g, rocketmass);

    days = travel_time(&planet) / 86400.0;
    d = (when - 946728000) / 86400.0;

    required = required_angle(days, period_days[planet - 1]);
    earth = wrap360(L0_earth + rate_earth *d);
    target = wrap360(L0_planet[planet - 1] + rate_planet[planet - 1] * d);
    gap = wrap360(target - earth);
    rel = rate_planet[planet - 1] - rate_earth;
    wait = (required - gap) / rel;
    while (wait<0)
        wait += 360.0 / fabs(rel);

    launch = when + (time_t)(wait *86400);
    arrival = launch + (time_t)(days * 86400);



    printf("\n\tX---RESULTS FOR YOUR LOCATION---x\t\n");
    printf("Local gravity                    : %.5f m/s^2\n", g);
    printf("Weight of full rocket            : %.0f N or %.0f kN\n", weight, weight/1000);
    printf("Thrust needed to just take off   : %.0f N \n", weight);
    printf("Earth's spin gives you           : %.0f m/s eastward for free\n", spinboost);
    printf("Thrust to weight Ratio           : %f \n", TWR);
    printf("Travel time to reach the planet  : %.1f days\n",days);
    printf("Days to wait for launch          : %.0f days\n", wait);
    strftime(buf, sizeof buf, "%d %b %Y", localtime(&launch));
    printf("Best launch date                 : %s\n", buf);
    strftime(buf, sizeof buf, "%d %b %Y", localtime(&arrival)); 
    printf("Arrival date                     : %s\n", buf);
    printf("Maximum Payload for this rocket  : %.0fkg\n", max_payload);
    

    if(TWR > 1)
        printf("\n\n\t----Thrust to weight ratio > 1: GO----\n");
    
    else
        printf("\t-----NO GO-----\t\n");

    if(payloadmass <= max_payload)
        printf("\n\t----Payload mass within limit  : GO\n");
    else if (max_payload <= 0)
    printf("\t----Thrust cannot lift the rocket itself: NO GO----\n");
    else
    printf("\t----Payload exceeds limit by %.0f kg: NO GO----\n", payloadmass - max_payload);


    return 0;
}