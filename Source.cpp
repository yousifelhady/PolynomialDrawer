#include "ccc_win.h"
#include <string> 
#include <cmath>
using namespace std;

void instructions(); //used to display instructions before drawing
void Lattice(int a, int b); //draws the lines of lattice where a and b are upper and lower limits
void Axes(int a, int b); //draws the axes where a and b are upper and lower limits
void Power_Parsing(string s, int *k); // parses string 's' into an array of integers 'k'
void Coeff_Parsing(string s, double *k, int &n); //parses string 's' into and array of double 'k' and n is the number of terms
int ccc_win_main()
{
START:
	int n; int n2; //counts the number of terms entered by the user
	int q = 0; // to check whether to draw the second function or not, initially =zero so it isn't drawn
	instructions();
	string input_raw = cwin.get_string("Enter Function");
	string input2_raw = ""; //initially empty when message2 is printed before entering second function
	string msg = "Zoom in : 1st quad, Another function : 2nd , Zoom out : 3rd , Reset: 4th";
	string message = "The function is " + input_raw + "\n" + msg;
	string input = input_raw.substr(2, input_raw.length() - 2);  // erases the 'y=' from the beginning of the string 
	double co[10]; int po[10]; //the arrays holding the values of coefficients and powers to be used later on in drawing
	double co2[10]; int po2[10]; // the arrays holding the values of the coefficients and powers of the 2nd function

	Coeff_Parsing(input, co, n);
	Power_Parsing(input, po);

	double xP, yP; //coordinates of the point clicked by the user
	int upperlimit = 10, lowerlimit = -10; //limits of coordinates, changed later
	do {
		cwin.clear(); // to clear the instructions
		cwin.coord(lowerlimit, upperlimit, upperlimit, lowerlimit); //changing coordinates before drawing 
		Lattice(lowerlimit, upperlimit);
		Axes(lowerlimit, upperlimit);

		double x = 0, y = 0;
		double y2 = 0;
		for (x = lowerlimit; x<upperlimit; x = x + 0.00005) // drawing the functions
		{
			y = 0, y2 = 0; //must be initialized by zero at the beginning of each loop
			for (int u = 0; u<n; u++)
			{
				y = y + ((co[u]) * (pow(x, po[u])));//drawing 1st function
			}
			//cwin << Point(x,y); // incase we want the lines to be thicker
			cwin << Line(Point(x - 0.01, y - 0.01), Point(x + 0.01, y + 0.01));
			for (int i = 0; i<10; i++); //for delay

			if (q == 1)
			{
				for (int v = 0; v<n2; v++)
				{
					y2 = y2 + ((co2[v]) * (pow(x, po2[v]))); //drawing 2nd function
				}
				//cwin << Point(x,y2);  // incase we want the lines to be thicker
				cwin << Line(Point(x - 0.01, y2 - 0.01), Point(x + 0.01, y2 + 0.01));
				for (int i2 = 0; i2<10; i2++); //for delay
			}
		}
		Point P;
		if (q == 0) P = cwin.get_mouse(message); //message shown before entering 2nd function
		if (q == 1) // message shown after entering 2nd function
		{
			string message2 = "the functions are " + input_raw + " and " + input2_raw + "\n" + msg;
			P = cwin.get_mouse(message2);
		}
		xP = P.get_x();
		yP = P.get_y();
		if ((yP>0) && (xP>0))// in case of zoom in
		{
			if (lowerlimit != -2 || upperlimit != 2)
			{
				lowerlimit += 2, upperlimit -= 2;
			}
			else; // to stop zooming after 4th time
		}
		if ((yP<0) && (xP<0)) //in case of zoom out
			lowerlimit -= 2, upperlimit += 2;
		cwin.clear();
		if ((xP>0) && (yP<0)) // resetting the program
		{
			cwin.clear();
			cwin.coord(-10, 10, 10, -10);
			goto START;
		}
		if ((xP<0) && (yP>0)) // getting the second string from user and parsing it
		{
			q = 1; // so that the function is drawn in the next loop
			input2_raw = cwin.get_string("enter the second function");
			string input2 = input2_raw.substr(2, input2_raw.length() - 2);
			Coeff_Parsing(input2, co2, n2);
			Power_Parsing(input2, po2);
		}
	} while (1);

	return 0;
}

void instructions()
{
	cwin << Message(Point(-8, 4), "*Instructions*");
	cwin << Message(Point(-8, 3), "-The input must be in the form as 'y=2(x^3)+5(x^2) + ..etc'");
	cwin << Message(Point(-8, 2), "-Click at the 1st quadrant to zoom in, in the 3rd to zoom out");
	cwin << Message(Point(-8, 1), "-Click at the 2nd quadrant to draw another function ");
	cwin << Message(Point(-8, 0), "-Click at the 4th quadrant to reset the program");
	cwin << Message(Point(-8, -1), "-You are allowed to zoom in only 4 times ");

}
void Lattice(int a, int b)
{
	for (int k = a; k<(b); k++) // for vertical lines of lattice
	{
		cwin << Line(Point(k, a), Point(k, b));
		cwin << Message(Point(k, 0), k);
	}
	for (int l = a; l<(b); l++) // for horizontal lines of lattice
	{
		cwin << Line(Point(a, l), Point(b, l));
		cwin << Message(Point(0, l), l);
	}
}
void Axes(int a, int b)
{
	for (double i = -0.02; i<0.02; i = i + 0.01)
	{
		cwin << Line(Point(a, i), Point(b, i));
		cwin << Line(Point(i, a), Point(i, b));
	}
}
void Power_Parsing(string s, int *k)
{
	string temp[10];
	for (int t = 0; t < 10; t++)
	{

		string::size_type pos1 = s.find('^');
		string::size_type pos2 = s.find(')');
		if (pos1 == string::npos || pos2 == string::npos) break;
		temp[t] = s.substr(pos1 + 1, pos2 - pos1 - 1);
		s = s.substr(pos2 + 1, s.length() - pos2 - 1);
		k[t] = stoi(temp[t], NULL);

	}

}
void Coeff_Parsing(string s, double *k, int &n)
{
	string temp[10];
	for (int r = 0; r < 10; r++)
	{
		if (r == 0)
		{
			string::size_type pos3 = s.find('(');
			temp[0] = s.substr(0, pos3);
			s = s.substr(pos3 + 1, s.length() - pos3);

			k[0] = stod(temp[0], NULL);
		}
		else
		{
			string::size_type pos1 = s.find(')');
			string::size_type pos2 = s.find('(');
			if (pos1 == string::npos || pos2 == string::npos)
			{
				n = r;
				break;
			}
			temp[r] = s.substr(pos1 + 1, pos2 - pos1 - 1);
			s = s.substr(pos2 + 1, s.length() - pos2 - 1);
			k[r] = stod(temp[r], NULL);
		}
	}
}
