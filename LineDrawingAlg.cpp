#include <GLut/glut.h>
#include <OpenGL/gl.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int lineNum; // define the number of lines entered by user
int dashLength; // define the dash length entered by user
clock_t start, finish; // define the start and finish timer
double duration = 0; // define the calculation of the algorithm
int flag; // define a flag to determine which algorithm to choose

void init (void)
{
  glClearColor (0.0, 0.0, 0.0, 0.0); // set display-window color to black
	glMatrixMode (GL_PROJECTION); // set projection parameters
	gluOrtho2D (0.0, 600.0, 0.0, 600.0); // set the x-coordinate from 0 to 600 and y-coordinate from 0 to 600
}

void setPixel (GLint x, GLint y)
{
	glColor3f (1.0, 1.0, 1.0); // activate the pixel by setting the point color to white
	glBegin (GL_POINTS);
		glVertex2i (x, y);	// set the point
	glEnd ();
	glFlush (); // process all openGL routines as quickly as possible
}

void offPixel (GLint x, GLint y)
{
	glColor3f (0.0, 0.0, 0.0); // deactivate the pixel by setting the point color to black
	glBegin (GL_POINTS);
		glVertex2i (x, y);	// set the point
	glEnd ();
	glFlush (); // process all openGL routines as quickly as possible
}

// truncate the number
inline int truncate (const float a)
{
	return int (a);
}


// basic line drawing algorithm
void basicAlg (int dashLength, int x0, int y0, int x1, int y1)
{
	int dx, dy, x, y, xInc;
	float yInc, m;

	dx = abs (x1 - x0);
	dy = abs (y1 - y0);	

	if (dashLength == 0) { // draw the solid line
		if (dx >= dy) { // slope |m| <= 1;
			if (x0 <= x1) { // set the left point to the starting point
				x = x0;
				y = y0;
			} else { // exchange the right point to the starting point and left point to the end point
				x = x1;
				y = y1;
				x1 = x0;
				y1 = y0;
			}
			m = (float) dy / (float) dx; // calculate the slope
			for (int i = 0; i <= (dx - 1); i++)
			{
				xInc = x + i;
				if (y > y1)
					yInc = -m * i + y;
				else
					yInc = m * i + y;
				setPixel (xInc, truncate (yInc)); // activate the pixel 
			}
		}
		else { // slope |m| > 1;
			if (y0 <= y1) { // set the left point to the starting point
				x = x0;
				y = y0;
			} else { // exchange the right point to the starting point and left point to the end point
				x = x1;
				y = y1;
				x1 = x0;
				y1 = y0;
			}
			m = (float) dx / (float) dy; // calculate the slope
			for (int i = 0; i <= (dy - 1); i++)
			{
				yInc = y + i;
				if (x > x1)
					xInc = - m * i + x; 
				else
					xInc = m * i + x;
				setPixel (truncate (xInc), yInc); // activate the pixel 
			}
		}
	} 
	else { // draw the dashed line
		int dashed = 0;
		int dot = 0;
		if (dx >= dy) { // slope |m| <= 1;
			if (x0 <= x1) { // set the left point to the starting point
				x = x0;
				y = y0;
			} else { // exchange the right point to the starting point and left point to the end point
				x = x1;
				y = y1;
				x1 = x0;
				y1 = y0;
			}
			m = (float) dy / (float) dx; // calculate the slope
			for (int i = 0; i <= (dx - 1); i++)
			{
				xInc = x + i; 
				if (y > y1)
					yInc = -m * i + y;
				else
					yInc = m * i + y;
				if (dashed) {  // if the dashed flag is on, deactivate the number of pixels determined by the dash length
					offPixel (xInc, truncate (yInc)); // deactivate the pixel 
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 0;
					}
				} 
				else { // if the dashed flag is off, activate the number of pixels determined by the dash length
					setPixel (xInc, truncate (yInc)); // activate the pixel
					dot++;
					// if dot == dashLength, activated pixels are done, turn on the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 1;
					}
				}
			}
		}
		else { // slope |m| > 1;
			if (y0 <= y1) { // set the left point to the starting point
				x = x0;
				y = y0;
			} else { // exchange the right point to the starting point and left point to the end point
				x = x1;
				y = y1;
				x1 = x0;
				y1 = y0;
			}
			m = (float) dx / (float) dy; // calculate the slope
			for (int i = 0; i <= (dy - 1); i++)
			{
				yInc = y + i;
				if (x > x1)
					xInc = - m * i + x;
				else
					xInc = m * i + x;
				setPixel (truncate (xInc), yInc); // activate the pixel 

				if (dashed) {  // if the dashed flag is on, deactivate the number of pixels determined by the dash length
					offPixel (truncate (xInc), yInc); // activate the pixel 
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 0;
					}
				} 
				else { // if the dashed flag is off, activate the number of pixels determined by the dash length
					setPixel (truncate (xInc), yInc); // activate the pixel 
					dot++;
					// if dot == dashLength, activated pixels are done, turn on the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 1;
					}
				}
			}
		}
	}
}


// bresenham line drawing algorithm
void bresenhamAlg (int dashLength, int x0, int y0, int x1, int y1)
{
	int dx = abs (x1 - x0);
	int dy = abs (y1 - y0);
	int x, y;
	
	// draw the solid line
	if (dashLength == 0) {
		if (dx >= dy){ //slope |m| <= 1.0;
			int e = (dy << 1) - dx;
			int inc1 = dy << 1;
			int inc2 = (dy - dx) << 1;	
			if (x0 < x1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				x1 = x0;
				y1 = y0;
			}
			setPixel (x, y); // activate the starting point
			while (x < x1) {
				if (e < 0)
					e += inc1;
				else {
					if (y < y1) {
						y++;
						e += inc2;
					} else {
						y--;
						e += inc2;
					}
				}
				x++;
				setPixel (x, y);
			}
		} 
		else { //slope |m| > 1.0;
			int e = (dx << 1) - dy;
			int inc1 = dx << 1;
			int inc2 = (dx - dy) << 1;
			if (y0 < y1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				y1 = y0;
				x1 = x0;
			}
			setPixel (x, y); // activate the starting point
			while (y < y1) {
				if (e < 0)
					e += inc1;
				else {
					if (x > x1){
						x--;
						e += inc2;
					} else {
						x++;
						e += inc2;
					}
				}
				y++;
				setPixel (x, y);
			}
		}
	}
	// draw the dashed line
	else { 
		int dashed = 0; // define a dashed flag to determined if set or turn off the pixel
		int dot = 0; // used to count the activated pixels;

		if (dx >= dy){ //slope |m| <= 1.0;
			int e = (dy << 1) - dx;
			int inc1 = dy << 1;
			int inc2 = (dy - dx) << 1;
			// set the left point to the starting point
			if (x0 < x1) {
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				x1 = x0;
				y1 = y0;
			}
			setPixel (x, y); // activate the starting point
			dot++;
			while (x < x1) {
				if (e < 0)
					e += inc1;
				else {
					if (y < y1) {
						y++;
						e += inc2;
					} else {
						y--;
						e += inc2;
					}
				}
				x++;
				// if the dashed flag is on, deactivate the number of pixels determined by the dash length
				if (dashed) { 
					offPixel (x, y);
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 0;
					}
				} else {
					setPixel (x, y);
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 1;
					}
				}		
			}
		} else { // slope |m| > 1.0
			int e = (dx << 1) - dy;
			int inc1 = dx << 1;
			int inc2 = (dx - dy) << 1;

			if (y0 < y1) { // set the left point to the starting point
				x = x0;
				y = y0;
			}
			else { // exchange the right point to the starting point and left point to the end point
				x = x1;			
				y = y1;
				y1 = y0;
				x1 = x0;
			}
			
			setPixel (x, y); // activate the starting point
			dot++;
			while (y < y1) {
				if (e < 0)
					e += inc1;
				else {
					if (x > x1){
						x--;
						e += inc2;
					} else {
						x++;
						e += inc2;
					}
				}
				y++;
				if (dashed) { // if the dashed flag is on, deactivate the number of pixels determined by the dash length
					offPixel (x, y);
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 0;
					}
				} else {
					setPixel (x, y);
					dot++;
					// if dot == dashLength, deactivated pixels are done, turn off the dashed flag
					if (dot%dashLength == 0) 
					{
						dashed = 1;
					}
				}				
			} 
		}
	}
}

void drawLines (void)
{
	int x0, y0, x1, y1;
	for (int i = 0; i < lineNum; i++)
	{
		// x and y coordinates determined by a random number generator
		x0 = 1 + rand() % 600;
		y0 = 1 + rand() % 600;
		x1 = 1 + rand() % 600;
		y1 = 1 + rand() % 600;
		// print out the x and y coordinates of each lines
		cout << "Line " << i + 1 << ": " << "(x0, y0) = (" << x0 << ", "<< y0 << "); " << "(x1, y1) = (" << x1 << ", "<< y1 << "); " << endl;
		if (flag){
			//start = clock(); // start the clock
			bresenhamAlg (dashLength, x0, y0, x1, y1); // call the bresenham line drawing algorithm
			//finish = clock(); // finish timing
			//duration += (double)(finish - start) / CLOCKS_PER_SEC; // calculate the time of the basic algorithm
		}else {
			//start = clock(); // start the clock
			basicAlg (dashLength, x0, y0, x1, y1); // call the basic line drawing algorithm
			//finish = clock(); // finish timing
			//duration += (double)(finish - start) / CLOCKS_PER_SEC; // calculate the time of the basic algorithm
		}
	}
	//cout << "The time for drawing " << lineNum << " line(s): " << duration << " seconds"<< endl; // print a message showing the time of the basic line drawing algorithm.
}

// draw a line in a specific x and y coordinates and fixed dash length
void drawSingleLine (void)
{
	if (flag){
		//start = clock(); // start the clock
		bresenhamAlg (dashLength, 50, 50, 200, 200); // call the bresenham line drawing algorithm
		//finish = clock(); // finish timing
		//duration += (double)(finish - start) / CLOCKS_PER_SEC; // calculate the time of the basic algorithm
		//cout << "You are calling the Bresenham Algorithm function." << endl;
	}else {
		//start = clock(); // start the clock
		basicAlg (dashLength, 50, 50, 200, 200); // call the basic line drawing algorithm
		//finish = clock(); // finish timing
		//duration += (double)(finish - start) / CLOCKS_PER_SEC; // calculate the time of the basic algorithm
		//cout << "You are calling the Basic Algorithm function." << endl;
	}
	cout << "The time for drawing " << lineNum << " line(s): " << duration << " seconds"<< endl; // print a message showing the time of the basic line drawing algorithm.
}

int main (int argc, char** argv)
{
	cout << "Enter the number of lines: "; // N is provide by user
	cin >> lineNum;
	cout << "Enter the dash length: "; // dash length is provede by user
	cin >> dashLength;
	cout << "Choose your algorithm: (0 for Basic, 1 for Bresenham): ";
	cin >> flag;
	
	glutInit (&argc, argv); // initialize GLUT
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowPosition (0, 0); // set top-left display-window position
	glutInitWindowSize (600, 600); // set display-window sidth and height
	glutCreateWindow ("Screen"); // create display window

	init (); // execute initialization procedure
	//glutDisplayFunc (drawLines); // send graphics to display window
	glutDisplayFunc (drawSingleLine); // send graphics to display window
	glutMainLoop (); // display everything and wait

	return 0;
}
