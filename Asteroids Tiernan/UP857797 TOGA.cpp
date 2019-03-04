

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// CT4TOGA OpenGL without using GLUT - uses excerpts from here:
// http://bobobobo.wordpress.com/2008/02/11/opengl-in-a-proper-windows-app-no-glut/
// Feel free to adapt this for what you need, but please leave these comments in.

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//Adapted from OpenGL introduction Lesson 



#pragma once
#include <math.h>
#include <windows.h>	// need this file if you want to create windows etc
#include <gl/gl.h>		// need this file to do graphics with opengl
#include <gl/glu.h>		// need this file to set up a perspective projection easily
#include <iostream> //Used to allow for string input
#include <stdio.h>
#include <time.h>  //Timer used as a seed for randomisation
#include <vector> //Used to hold all objects required for the game
using namespace std;
float framerate = 60; //Target frame rate for delta timing

void DrawCircle(float radius, float segments, float CircleX, float CircleY)  //Rewritten from http://slabode.exofire.net/circle_draw.shtml , changed slightly with variable names so that I could understand what it was doing. 
{  ////I struggled with drawing asteroids and they were not the focus of the project, and with few segments they look roughly like one, so this is used for those and the player's bullet.
	float theta = (2 * 3.1415926) / segments;
	float sine = sinf(theta); //Cos and Sine calculated for use
	float cos = cosf(theta);
	float t;
	float y = 0;
	float x = radius;

	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < segments; ii++)
	{
		glVertex2f(x + CircleX, y + CircleY);//Create a vertex piece for the current segment 

								   //create a new postion for the next segment in accordance to their rotation in accordance to the sine and cos calculations
		t = x;
		x = cos * x - sine * y;
		y = sine * t + cos * y;
	}
	glEnd();
}
float idealDelta (1000/framerate); //Target delta time
float scaleFactor = 0;
float boundaryX= 2.2f; //Used to store the deadzones of the screen.
float boundaryY= 1.24f; //Used to store the deadzones of the screen.
float stageoffset = -10.0f; //As the sin and cos calculations do not work around 0, the entire stage is offset by this to make the game work.
bool playgame = true;
// include the opengl and glu libraries
#pragma comment(lib, "opengl32.lib")	
#pragma comment(lib, "glu32.lib")




//Ship Array 
float myArray3[12] = {
	0.0f, 0.05f, 0.0f,
	0.1f, 0.0f, 0.0f,
	0.0f, -0.05f, 0.0f,
	0.02f, -0.0f, 0.0f
};





class object  //A class to be inherited from for all object. Anything that can move essentially.
{
public:
	float *x= new float;  //Floats for containing the X and Y position of the object
	float *y= new float;  
	float *rotation= new float; //A float containing the rotation of the object, if the object needs it that is.
	float *speed = new float; //The speed at which an object can move each frame.
	string *name= new string; //A name if the object requires it
	float scalefactorlocal; //A variable used to contain the delta time of the scene.
	float *directionx = new float; //Temporary use variables for storing the object's X & Y direction and then into a vector
	float *directiony = new float;
	float *width = new float;  //Used to create the bounding boxes
	float *height = new float; //Used to create the bounding boxes
	bool *killed = new bool;
	object()
	{
		*x = 0.0f;
		*y = 0.0f;
		*name = "default";
		*rotation = 0;
		//*speed = 0.015f;
		*speed = 0.02f;
		scalefactorlocal = 0.0f;
		*killed = false;
	}
	~object()
	{
		delete x;
		x = NULL;
		delete y;
		y = NULL;
		delete rotation;
		rotation = NULL;
		delete name;
		name = NULL;
		delete speed;
		speed = NULL;
		directionx = NULL;
		delete directiony;
		directiony = NULL;
		delete width;
		width = NULL;
		delete height;
		height = NULL;
		delete killed;
		killed = NULL;
		delete width;
		width = NULL;
	}
	float vectorAngle(float x1, float y1, float x2, float y2) //This function returns the angle of a vector direction, used to work out the new vector direction with new applied motion taken into account.
	{
		float angle = (float)atan2f(x1 - x2, y1 - y2);
		return angle;
	}
	//A function that moves the object to the correct side of the screen depending on which way the object flies through the boundaries, e.g left to right e.t.c
	void newposition(float *x, float *y) //This function is passed by reference so that by modifiying the values of the pointers
	{
		if (*x + -stageoffset>boundaryX)  //If player too far right, move them to the left side of the screen
		{
			*x = stageoffset - boundaryX;
		}
		else if (*x + -stageoffset<-boundaryX) //If player too far left, move them to the right side of the screen
		{
			*x = stageoffset + boundaryX;
		}
		else if (*y + -stageoffset>boundaryY) //If player too far down, move them to the top side of the screen
		{
			*y = stageoffset - boundaryY;
		}
		else if (*y + -stageoffset<-boundaryY) //If player too far up, move them to the bottom side of the screen
		{
			*y = stageoffset + boundaryY;
		}

	}
	float pythagDistance(float a, float b)
	{
		float result = sqrt((a*a +b*b));  //A squared times b squares, square routed to give the distance
		return result;
	}
	void Update() //Generic update function, overwritten by the player, but useful for asteroids and such that only need one direction.
	{
		
		*x += *speed*cosf(*rotation*0.01745)*scalefactorlocal;
		*y += *speed*sinf(*rotation*0.01745)*scalefactorlocal;
		newposition(x, y);
	}
	void DrawingCode()
	{
		glVertexPointer(3, GL_FLOAT, 0, myArray3);
		glLoadIdentity();
		glTranslatef(*x + 10, *y + 10, -3.0f);  //The representation of the player is offset from the player. As calculations do not work around 0 at with cos and sin, fixing my movement problem.
		glRotatef(*rotation, 0, 0, 1);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}

private:

};



class bullet : public object //Bullet that will spawn out of player
{
public:
	bool Active; //Controls whether the bullet is alive on the screen or not
	bullet()
	{
		*speed = 0.03f;
		Active = false;
		*width = 0.01f;
		*height = 0.01f;
	}
	void create(float *rotationtemp,float tempx,float tempy)
	{
		*rotation = *rotationtemp;
		*x = tempx;
		*y = tempy;
	}
	~bullet()
	{

	}
	void DrawingCode()  
	{	//Circle X offset, Circle Y offset, Radius, Number of segments
		glLoadIdentity();
		glTranslatef(*x + 10, *y + 10, -3.0f);
		DrawCircle(0.03f, 8,0.0f, 0.0f); //Bullet shape drawn as a small circle
	}
	void Update(float scalefactorlocaltemp) //Generic update function that moves the bullet
	{
		scalefactorlocal = scalefactorlocaltemp;
		*x += *speed*cosf(*rotation*0.01745)*scalefactorlocal;
		*y += *speed*sinf(*rotation*0.01745)*scalefactorlocal;
		newposition(x, y);
	}
	void newposition(float *x, float *y) //The bullet is turned off if it goes off screen
	{
		if (*x + -stageoffset>boundaryX)   //If out of bounds destory it
		{
			Active = false;
		}
		else if (*x + -stageoffset<-boundaryX) 
		{
			Active = false;
		}
		else if (*y + -stageoffset>boundaryY) 
		{
			Active = false;
		}
		else if (*y + -stageoffset<-boundaryY) 
		{
			Active = false;
		}

	}

};





class Player : public object //A Player class that inherits from the Object class.
{
public:

	char *dir = new char;   //A variable that simply manages which direction the player is rotating in  'L' for left, 'R' for right
	bool *forwards = new bool;  //True equals active thrusters false means no external forces on current vector
	float *velocityX = new float;  //Manages how fast the player is moving on the X axis
	float *velocityY = new float;  //Manages how fast the player is moving on the Y axis
	float *temp = new float;     //Temporary vector angle
	short *lives = new short;
	bool *invulnerable = new bool;
	Player()  //Standards for player creation
	{
		*dir = 'N';
		*forwards = false;
		*speed = 0.0003f;
		*rotation = 90;
		*temp = 0;
		*velocityX = 0;
		*velocityY = 0;
		*width = 0.01f;
		*height = 0.01f;
		*lives = 3;
		*invulnerable = true;
	}
	~Player() //Deleting all pointer data
	{
		delete dir;
		dir = NULL;
		delete velocityX;
		velocityX = NULL;
		delete velocityY;
		velocityY = NULL;
		delete forwards;
		forwards = NULL;
		delete temp;
		temp = NULL;
		delete lives;
		lives = NULL;
		delete invulnerable;
		invulnerable = NULL;
	}
	//Called once each frame, updating the player movement
	void Update()
	{
		if (*dir == 'L')
		{

			*rotation += 3.0f*scalefactorlocal;  //Rotates the player left in accordance to the delta timing
		}
		if (*dir == 'R')
		{

			*rotation += -3.0f*scalefactorlocal; //Rotates the player right in accordance to the delta timing
		}
		if (*forwards == true)  //If the player wishes to manipulate the ship, this occures.
		{
			//*0.01745 Radians conversion
			if (*velocityX < 0.2f)  //ensuring the player never moves too fast
			{
				*velocityX += *speed*cosf(*rotation*0.01745)*scalefactorlocal; //changing the velocity and therefore direction of the player's X direction in accordance to their rotation, their thrusting ability and to the delta time
			}
			if (*velocityY < 0.2f) //ensuring the player never moves too fast
			{
				*velocityY += *speed*sinf(*rotation*0.01745)*scalefactorlocal; //changing the velocity and therefore direction of the player's Y direction in accordance to their rotation, their thrusting ability and to the delta time
			}
			*directionx = *speed*cosf(*rotation*0.01745)*scalefactorlocal; //Creating these temporary variables for acknowledgement of the player's vector with the same calculation as above.
			*directiony = *speed*sinf(*rotation*0.01745)*scalefactorlocal;
		}

		*temp = vectorAngle(*x + *velocityX, *y + *velocityY, *directionx, *directiony); //Creating the angle of the vector, for use with moving the player.
		*x += (*velocityX*cosf(*temp)*scalefactorlocal)*-1; //Manipulating the X postion of the player in accordance to their newly designated vector.
		*y += (*velocityY*sinf(*temp)*scalefactorlocal)*-1;//Manipulating the Y postion of the player in accordance to their newly designated vector.
		newposition(x, y); //Passing by reference to check if the player is outside the boundaries and should spawn on the other side.
		if (*killed==true)
		{
			cout <<"You have " <<*lives<< " lives" << flush;
			*x = stageoffset;
			*velocityX = 0; //Reset the speed
			*velocityY = 0;
			*y = stageoffset; //set the player to the start
			*lives = *lives - 1;  //Take away a life
			*invulnerable = true;
			*killed = false;
		}
	}
};



class Asteroid : public object //Indivdual Asteroid object, many of these will be spawned, on screen at once, either from the game start, or from an already hit asteroid
{
public:
	float *radius = new float;
	bool *loop = new bool;
	short stage;
	Asteroid()
	{
		*loop = true;
		*radius = 0.13f;
		stage = 1;
		*speed = 0.015f;
	}


	void Spawn(float boundaryX, float boundaryY, float size, float playerX, float playerY, float radiii)
	{
		*radius = radiii;
		*width = *radius;  //Creating the bounding box for the asteroid through the width taken by the radius 
		*height = *radius;	//Creating the bounding box for the asteroid
		if (stage==2) //Depending on their stages the objects move faster or slower, at stage 1 they move at the default speed set above
		{
			*speed = 0.02f;
		}
		else if (stage == 3) 
		{
			*speed = 0.03f;
		}

		while (*loop == true) //Whilst the asteroid is npt far enough from the player it repeats
		{
			(float)*x = ((rand() % (int)(2 * (boundaryX * 10)) - (boundaryX * 10)) / 10) + stageoffset; //Here I create a randomised X position through the rand function. The area is dictated by the boundary of -2.1 to 2.1, however as rand does not support floats, I times the value by 10 and make it an int, dividing back into a float with the finished product
			(float)*y = ((rand() % (int)(2 * (boundaryY * 10)) - (boundaryY * 10)) / 10) + stageoffset;//Here I create a randomised Y position through the rand function. The area is dictated by the boundary of -1.2 to 1.2, however as rand does not support floats, I times the value by 10 and make it an int, dividing back into a float with the finished product
			playerX -= *x;  //Working out the length of A in accordance to pythagoras theorem
			playerY -= *y; //Working out the length of B in accordance to pythagoras theorem
			if (pythagDistance(playerX, playerY) >0.9f)	 //These values are sent with the correct offset value to determine the distance to C
			{
				*rotation = rand() % 360 + 1; //randomised so the asteroid can move on any 360 degree direction 
				*loop = false;  //It has been determined the asteroid is not too close to the player, and in the correct bounds. The spawning can end.
			}
			//Create truly random spawning here
		}
	}
	~Asteroid()
	{
		delete loop;
		loop = NULL;
		delete radius;
		radius = NULL;

	}
	void DrawingCode()  //As OpenGL comes into play here, the circle's X and Y are dependant on the current vertex object, there is no need to assign anything.
	{	//Circle X offset, Circle Y offset, Radius, Number of segments
		glLoadIdentity();
		glTranslatef(*x + 10, *y + 10, -3.0f);
		DrawCircle(*radius, 10 ,0.0f, 0.0f);
	}
	void Collisions(Player *playee,bullet *Bull)
	{ //So that the asteroid can manipulate the player the player is passed as a pointer
		//Collision code for all four sections of the circle which is the rough approximation of its shape.
		if (*playee->invulnerable == false) //If the player has just spawned they are invulnerable and cannot be hit
		{


			if (*x + *width > *playee->x && *x < *playee->x + *playee->width   //Creating the four trianges that collide with the player
				&& *y + *height > *playee->y && *y < *playee->y + *playee->height)
			{
				*playee->killed = true; //Killing the player on their own end
			}
			else if (*x - *width < *playee->x && *x > *playee->x - *playee->width
				&& *y - *height < *playee->y && *y > *playee->y - *playee->height)
			{
				*playee->killed = true;
			}
			else if (*x - *width < *playee->x && *x > *playee->x - *playee->width
				&& *y + *height > *playee->y && *y < *playee->y + *playee->height)
			{
				*playee->killed = true;
			}
			else if (*x + *width > *playee->x && *x < *playee->x + *playee->width
				&& *y - *height < *playee->y && *y > *playee->y - *playee->height)
			{
				*playee->killed = true;
			}
		}
		if (Bull->Active == true) // This same process occurs with the bullet but only when it is active
		{
			if (*x + *width > *Bull->x && *x < *Bull->x + *Bull->width
				&& *y + *height > *Bull->y && *y < *Bull->y + *Bull->height)
			{
				Bull->Active = false; //The bullet is disabled as it hit
				*killed = true; //This asteroid is killed and destroyed in the game controlling struct
			}
			else if (*x - *width < *Bull->x && *x > *Bull->x - *Bull->width
				&& *y - *height < *Bull->y && *y > *Bull->y - *Bull->height)
			{
				Bull->Active = false;
				*killed = true;
			}
			else if (*x - *width < *Bull->x && *x > *Bull->x - *Bull->width
				&& *y + *height > *Bull->y && *y < *Bull->y + *Bull->height)
			{
				Bull->Active = false;
				*killed = true;
			}
			else if (*x + *width > *Bull->x && *x < *Bull->x + *Bull->width
				&& *y - *height < *Bull->y && *y > *Bull->y - *Bull->height)
			{
				Bull->Active = false;
				*killed = true;
			}
		}
	
	}

private:


};

struct Game
{
public:
	// Defining the two main objects that are not repeated
	Player *p1 = new Player;  //These are created as pointers to save on memory
	bullet *Bullet = new bullet;  
	vector<Asteroid*> Asteroid_Vector; //A vector is made that holds pointers to the custom asteroid object. I used a vector instead of an array because asteroids split into many singular pieces, and I could save memory by adjusting the size as needed, whilst giving room for expansion if I were to need it
	unsigned short *count = new unsigned short; //This is used to check if any asteroids are actually alive
	Game() //Basic declaration
	{
		*p1->x = stageoffset;  // As all objects are well off the screen due to issues with calculations around the origin of 0,0. I moved all objects off the screen by this much. All calculations happen here, but the game is displayed with the offset added back on
		*p1->y = stageoffset;
		*Bullet->x = stageoffset; 
		*Bullet->y = stageoffset;
		Bullet->Active = false; //The bullet should not be displayed at the start, so it is turned off.
		Asteroid* asteroid = NULL; // An asteroid pointer is created as a basis for new generation
		for (int i = 0; i < 4; i++)  //The list is iterated through four times to create the asteroids
		{
			asteroid = new Asteroid; 
			*asteroid->x = stageoffset;  //Set the asteroid onto the right offset
			*asteroid->y = stageoffset;
			asteroid->Spawn(boundaryX, boundaryY, 1.0f, *p1->x, *p1->y, *asteroid->radius ); //Spawn the asteroid with a randomised postion and standard variables with its own custom function
			Asteroid_Vector.push_back(asteroid); //Add the asteroid to the vector list
		}
	}
	void deletevector() //Clear code that is called once the game is complete
	{
		for (unsigned int i = 0; i < Asteroid_Vector.size(); i++) //Ascertaining the size of the vector, I run through it to delete all asteroids to ensure memory clean up as deleting the vector by itself would not delete the old asteroids from memory. 
		{
			delete Asteroid_Vector[i];
			Asteroid_Vector[i] = NULL;
		}
	}
	void RunThroughAsteroidList()
	{
		for (unsigned int i = 0; i < Asteroid_Vector.size(); i++)
		{
			if (*Asteroid_Vector[i]->killed == false) //If the asteroid is not dead, draw it. 
				Asteroid_Vector[i]->DrawingCode();
		}

	}
	void checkAsteroidsForDead(float scalefactorlocaltemp) //Check to see if asteroids are well and truly dead.
	{
		for (unsigned int i = 0; i <Asteroid_Vector.size(); i++) //I then run through the length of the vector
		{
			if (Asteroid_Vector[i]==NULL) 
			{
				Asteroid_Vector.erase(Asteroid_Vector.begin() + i); //Removing the vector directory
			}
			if (*Asteroid_Vector[i]->killed == true) // Checking if all the asteroids are dead, if it is the stage is checked in the below function, with the correct action being taken
			{
					MakeAsteroids(Asteroid_Vector[i]->stage, *Asteroid_Vector[i]->radius, *Asteroid_Vector[i]->x, *Asteroid_Vector[i]->y, i); //I create two asteroids or kill this asteroid depending on its current state
					if (Asteroid_Vector.size()>i) //Ensuring that with the deletion of objects the selection is not out of range
					{
						if (Asteroid_Vector[i]->stage!=3)
						{
							delete Asteroid_Vector[i]; //Clearing the memory location for proper deletion
							Asteroid_Vector[i] = NULL;
							Asteroid_Vector.erase(Asteroid_Vector.begin() + i); //Removing the vector directory
						}
					}
					
			}
			else //If the object isn't dead it'll be drawn and updated with collisions		
			{
				*count = *count + 1;
				Asteroid_Vector[i]->scalefactorlocal = scalefactorlocaltemp; //Taking the scalefactor variable from the main loop to ensure all calculations are correct on all processor types
				Asteroid_Vector[i]->Update(); //As a long as the asteroid is alive they'll be updated
				Asteroid_Vector[i]->Collisions(p1,Bullet); //Checking if the asteroid has colided with anything
			}

		}
	}
	void MakeAsteroids(short stage, float radius, float X, float Y, int vectorpos) //This is called to make the new smaller asteroids once the parent asteroid has been killed 
	{
		Asteroid* asteroid = NULL;
		if (stage==1) //A new asteroid is made using the earlier function, but the size is halfed and the position set to the correct old position
		{
			asteroid = new Asteroid; // new asteroid for temporary holding
			asteroid->Spawn(boundaryX, boundaryY, 1.0f, *p1->x, *p1->y, radius *0.6); //The asteroid is spawned again and made 40% smaller, individual asteroids split off in random directions by default once used by this function, which makes sense for the context of the game
			*asteroid->x = X;
			*asteroid->y = Y;  //The asteroids have to be set to the postion of the old asteroids, and from their the random angles allow them to carry on
			asteroid->stage = 2; //Their stage controls how they move and their next shape in the next stage
			Asteroid_Vector.push_back(asteroid); //The asteroid is added to the asteroid vector list 
			asteroid = new Asteroid;
			asteroid->Spawn(boundaryX, boundaryY, 1.0f, *p1->x, *p1->y, radius *0.6);  //The same again
			*asteroid->x = X;
			*asteroid->y = Y;
			asteroid->stage = 2;
			Asteroid_Vector.push_back(asteroid);
			//Create 2
		}
		else if(stage==2) //All the same but they are set to stage 3, which kills them if they are set to that
		{
			asteroid = new Asteroid;
			asteroid->Spawn(boundaryX, boundaryY, 1.0f, *p1->x, *p1->y, radius *0.6);
			*asteroid->x = X;
			*asteroid->y = Y;
			asteroid->stage = 3;
			Asteroid_Vector.push_back(asteroid);
			asteroid = new Asteroid;
			asteroid->Spawn(boundaryX, boundaryY, 1.0f, *p1->x, *p1->y, radius *0.6);
			*asteroid->x = X;
			*asteroid->y = Y;
			asteroid->stage = 3;
			Asteroid_Vector.push_back(asteroid);
		}

	}
	void GameLogic(float scalefactorlocal) //This just checks for key conditions
	{
		if (*p1->lives == 0) // end the game for restart
		{
			playgame = false;
		}
		else //Run all conditions
		{
			if (Bullet->Active == true)
			{
				Bullet->Update(scalefactorlocal);
			}
			p1->Update(); //This controls all key mechanics of the player in its own script
			checkAsteroidsForDead(scalefactorlocal); //check if asteroids are dead or not
			if (*count==0)
			{
				playgame = false;
			}
			*count = 0;
		}
	}
	void DrawingCode() //Where all objects are drawn
	{
		p1->DrawingCode();
		if (Bullet->Active==true) //Only draw if active
		{
			Bullet->DrawingCode();
		} 
		RunThroughAsteroidList(); //All asteroids must be iterated through the list for this, the asteroids are only drawn if they are active
	}
	~Game() //Deletion of all pointers for end deletion
	{
		delete p1;
		p1 = NULL;
		delete Bullet;
		Bullet = NULL;
		delete count;
		count = NULL;

	}
};

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// function prototypes:
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// In a C++ Windows app, the starting point is WinMain() rather than _tmain() or main().
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	// some basic numbers to hold the position and size of the window
	int windowWidth = 1920;
	int windowHeight = 1080;
	int windowTopLeftX = 50;
	int windowTopLeftY = 50;
	srand(time(NULL)); //Randomising the seed with the timer
	// some other variables we need for our game...
	MSG msg;								// this will be used to store messages from the operating system
	bool keepPlaying = true;				// whether or not we want to keep playing
	DWORD frameStartTime;//Delta Dword variables
	DWORD frameEndTime;
	float currentDelta = 0.0f;
	float scaleFactor = 0.0f;
	Game *Master = new Game; 


	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// this section contains all the window initialisation code, 
	// and should probably be collapsed for the time being to avoid confusion	
#pragma region  <-- click the plus/minus sign to collapse/expand!

	// this bit creates a window class, basically a template for the window we will make later, so we can do more windows the same.
	WNDCLASS myWindowClass;
	myWindowClass.cbClsExtra = 0;											// no idea
	myWindowClass.cbWndExtra = 0;											// no idea
	myWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// background fill black
	myWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// arrow cursor       
	myWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// type of icon to use (default app icon)
	myWindowClass.hInstance = hInstance;									// window instance name (given by the OS when the window is created)   
	myWindowClass.lpfnWndProc = WndProc;									// window callback function - this is our function below that is called whenever something happens
	myWindowClass.lpszClassName = TEXT("Asteroids!");				// our new window class name
	myWindowClass.lpszMenuName = 0;											// window menu name (0 = default menu?) 
	myWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// redraw if the window is resized horizontally or vertically, allow different context for each window instance

	// Register that class with the Windows OS..
	RegisterClass(&myWindowClass);

	// create a rect structure to hold the dimensions of our window
	RECT rect;
	SetRect(&rect, windowTopLeftX,				// the top-left corner x-coordinate
		windowTopLeftY,				// the top-left corner y-coordinate
		windowTopLeftX + windowWidth,		// far right
		windowTopLeftY + windowHeight);	// far left

	// adjust the window, no idea why.
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// call CreateWindow to create the window
	HWND myWindow = CreateWindow(TEXT("Asteroids!"),		// window class to use - in this case the one we created a minute ago
		TEXT("UP857997 TOGA"),		// window title
		WS_OVERLAPPEDWINDOW,						// ??
		windowTopLeftX, windowTopLeftY,			// x, y
		windowWidth, windowHeight,				// width and height
		NULL, NULL,								// ??
		hInstance, NULL);							// ??


	// check to see that the window created okay
	if (myWindow == NULL)
	{
		FatalAppExit(NULL, TEXT("CreateWindow() failed!")); // ch16
	}

	// if so, show it
	ShowWindow(myWindow, iCmdShow);


	// get a device context from the window
	HDC myDeviceContext = GetDC(myWindow);


	// we create a pixel format descriptor, to describe our desired pixel format. 
	// we set all of the fields to 0 before we do anything else
	// this is because PIXELFORMATDESCRIPTOR has loads of fields that we won't use
	PIXELFORMATDESCRIPTOR myPfd = { 0 };


	// now set only the fields of the pfd we care about:
	myPfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);		// size of the pfd in memory
	myPfd.nVersion = 1;									// always 1

	myPfd.dwFlags = PFD_SUPPORT_OPENGL |				// OpenGL support - not DirectDraw
		PFD_DOUBLEBUFFER |				// double buffering support
		PFD_DRAW_TO_WINDOW;					// draw to the app window, not to a bitmap image

	myPfd.iPixelType = PFD_TYPE_RGBA;					// red, green, blue, alpha for each pixel
	myPfd.cColorBits = 24;								// 24 bit == 8 bits for red, 8 for green, 8 for blue.
	// This count of color bits EXCLUDES alpha.

	myPfd.cDepthBits = 32;								// 32 bits to measure pixel depth.


	// now we need to choose the closest pixel format to the one we wanted...	
	int chosenPixelFormat = ChoosePixelFormat(myDeviceContext, &myPfd);

	// if windows didnt have a suitable format, 0 would have been returned...
	if (chosenPixelFormat == 0)
	{
		FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));
	}

	// if we get this far it means we've got a valid pixel format
	// so now we need to set the device context up with that format...
	int result = SetPixelFormat(myDeviceContext, chosenPixelFormat, &myPfd);

	// if it failed...
	if (result == NULL)
	{
		FatalAppExit(NULL, TEXT("SetPixelFormat() failed!"));
	}

	// we now need to set up a render context (for opengl) that is compatible with the device context (from windows)...
	HGLRC myRenderContext = wglCreateContext(myDeviceContext);

	// then we connect the two together
	wglMakeCurrent(myDeviceContext, myRenderContext);



	// opengl display setup
	glMatrixMode(GL_PROJECTION);	// select the projection matrix, i.e. the one that controls the "camera"
	glLoadIdentity();				// reset it
	gluPerspective(45.0, (float)windowWidth / (float)windowHeight, 0.1, 1000);	// set up fov, and near / far clipping planes
	glViewport(0, 0, windowWidth, windowHeight);								// make the viewport cover the whole window
	glClearColor(0, 0, 0, 1.0);												// set the colour used for clearing the screen
	glEnableClientState(GL_VERTEX_ARRAY);										//	turn on the ability to do vertex arrays
	glMatrixMode(GL_MODELVIEW);													// select the modelview matrix, i.e. the one that controls how local space is mapped to world space
	GLuint triangleVBO;
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#pragma endregion



	// keep doing this as long as the player hasnt exited the app:
	AllocConsole();
	FILE* pCout; //for COUT to work
	FILE* pCin; //for CIN to work
	FILE* pCerr; //for CERR to work
	freopen_s(&pCin, "CONIN$", "r", stdin);
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	freopen_s(&pCerr, "CONOUT$", "w", stderr);
	while (keepPlaying == true)
	{
		if (playgame == true) //If the game in session is not finished keep going
		{
			frameStartTime = GetTickCount();
			// we need to listen out for OS messages.
			// if there is a windows message to process...
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// and if the message is a "quit" message...
				if (msg.message == WM_QUIT)
				{
					keepPlaying = false;	// we want to quit asap
				}
				else if (msg.message == WM_KEYDOWN)
				{
					*Master->p1->invulnerable = false; //If the player touches a button, they are made vulnerable
					//Designating the condtions for movement but not applying them here.
					if (msg.wParam == 'A')
					{
						*Master->p1->dir = 'L'; // Self explanatory turn left
					}
					else if (msg.wParam == 'D') //// Self explanatory turn right
					{
						*Master->p1->dir = 'R';
					}
					if (msg.wParam == 'W')
					{
						*Master->p1->forwards = TRUE; //Allow thrust

					}
					if (msg.wParam == 'F') //Fire the gun
					{
						Master->Bullet->create(Master->p1->rotation, *Master->p1->x, *Master->p1->y); //Create the bullet at this instance 
						Master->Bullet->Active = true; //Ensure the bullet is active

					}
				}

				else if (msg.message == WM_KEYUP) //On release of keyboard
				{
					if (msg.wParam == 'A')
					{
						*Master->p1->dir = 'N'; //No direciton is applied
					}
					else if (msg.wParam == 'D')   //No direciton is applied
					{
						*Master->p1->dir = 'N';
					}
					if (msg.wParam == 'W') //stop accelerating, all momentum is still retained
					{
						*Master->p1->forwards = FALSE;

					}

				}
				// or if it was any other type of message (i.e. one we don't care about), process it as normal...
				else 
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			Master->GameLogic(scaleFactor); //All Game logic happens in the struct above
			// draw code:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen
			Master->DrawingCode(); // All Drawing occurs in this struct, all objects have their own drawing code in their appropriate classes.
			SwapBuffers(myDeviceContext);							// update graphics
			frameEndTime = GetTickCount(); //Delta timing end
			currentDelta = (frameEndTime - frameStartTime);
			scaleFactor = (currentDelta / idealDelta);
			Master->p1->scalefactorlocal = scaleFactor; //give the player and all objects their scale factor due to scope issues.
			Master->p1->scalefactorlocal = scaleFactor;
		}
		else //If the game is over, reset everything and start a new, the player exits through the window itself.
		{
			Master->deletevector(); //Run through the asteroid vector list and delete all contents
			delete Master;
			Master = NULL;
			Master = new Game; //create the game struct object
			playgame = true; //Start a new 
		}
	}


	// the next bit will therefore happen when the player quits the app,
	// because they are trapped in the previous section as long as (keepPlaying == true).

	// UNmake our rendering context (make it 'uncurrent')
	wglMakeCurrent(NULL, NULL);

	// delete the rendering context, we no longer need it.
	wglDeleteContext(myRenderContext);

	// release our window's DC from the window
	ReleaseDC(myWindow, myDeviceContext);

	// end the program
	return msg.wParam;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// this part contains some code that should be collapsed for now too...
#pragma region keep_this_bit_collapsed_too!

// this function is called when any events happen to our window
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

	switch (message)
	{
		// if they exited the window...	
	case WM_DESTROY:
		// post a message "quit" message to the main windows loop
		PostQuitMessage(0);
		return 0;
		break;
	}

	// must do this as a default case (i.e. if no other event was handled)...
	return DefWindowProc(hwnd, message, wparam, lparam);

}

#pragma endregion
