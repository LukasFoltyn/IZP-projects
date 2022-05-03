		//****************************************//
		// IZP - druhy projekt - iteracni vypocty //
		//		Lukas Foltyn		  //	
		//		24.11.2019		  //
		//****************************************//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EQUATION_CONSTANT 1
#define SPLIT_INTERVAL 2
#define SOURCE_VOLTAGE 1
#define RESISTANCE 2
#define EPS 3
#define NEEDED_ARG 4
#define SATUR_CURRENT 1e-12
#define THERMAL_VOLTAGE 25.8563e-3
#define INTERVAL_BEGGINING 0

/* USED FUNCTION */

//returns the diode voltage
double diode(double U0, double r ,double eps);
//returns the function value
double get_fun_val(double x_value, double u0, double r);
//returns the absolute value on number
double absolute_value(double to_positive);
//returns the diode current
double shockley_equation(double diode_voltage);

int main(int argc, char *argv[])
{
	double diode_current;
	double diode_voltage;
	double eps, r, u0;
	char *ptrvolt, *ptrres, *ptreps;

	//checking for right amount of arguments
	if(argc != NEEDED_ARG)
	{	
		fprintf(stderr, "Invlaid input\n");
		return 1;
	}
	else
	{	
		//converting argument line strings into floating point numbers
		u0 = strtod(argv[SOURCE_VOLTAGE], &ptrvolt);
		r = strtod(argv[RESISTANCE], &ptrres);
		eps = strtod(argv[EPS], &ptreps);
		//checking if the strings were convertable and positive
		if(*ptrvolt != 0 || *ptrres != 0 || *ptreps != 0 || u0 < 0 || r < 0 || eps < 0)
		{
			fprintf(stderr, "Invalid input\n");
			return 1;
		
		}
	}	
	//passing arguments to diode function for calculating the voltage	
	diode_voltage = diode(u0, r, eps);
	
	//passing diode voltage fot calculating the current
	diode_current = shockley_equation(diode_voltage);
		
	//result of calculation
	printf("Up=%g V\nIp=%g A\n", diode_voltage, diode_current);

	return 0;
}

double diode(double u0, double r,double eps)
{
	double half_split = 0;

	double left_value = INTERVAL_BEGGINING;
	double right_value = u0;
	
	double old_half_split;

	do
	{	
		old_half_split = half_split; 
		
		
		//splitting the interval into two intervals of the same size
		half_split =(left_value+right_value)/SPLIT_INTERVAL;
	
		//choosing the interval, which contains the value of diode voltage
		if(get_fun_val(half_split, u0, r) * get_fun_val(left_value, u0, r) < 0)
			
			right_value = half_split;
		else
			left_value = half_split;
		
			
	}while(absolute_value(right_value-left_value) > eps && old_half_split != half_split); //looping until the value is close to the tolerance or the value of middle does not change
	
	return half_split;

}
double shockley_equation(double diode_voltage)
{
	double diode_current;
	//equation for current in diode circuit
	diode_current = SATUR_CURRENT*(exp(diode_voltage/THERMAL_VOLTAGE)-EQUATION_CONSTANT);
	return diode_current;

}	
double get_fun_val(double x_value, double u0, double r)
{
	
	double function_value;
	//Shockley equation build in to the first Kirchhoff's law for current in circuit
	function_value = SATUR_CURRENT*(exp(x_value/THERMAL_VOLTAGE)-EQUATION_CONSTANT)-(u0-x_value)/r;
	return function_value;

}
double absolute_value(double to_positive)
{
	if(to_positive < 0)
		return -to_positive;
	else
		return to_positive;
}
