#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define sqrt2 (sqrt(2))
#define M_PI 3.14159265358979323846

typedef struct point
{
	double x,y,z;
}Point;
class BoxColors
{
public:
    float SideColors[3][3];
    ///0 Y axis, 1 X axis, and 2 Z axis
    BoxColors(float param[3][3])
    {
        for(int i = 0; i<3; i++)
        {
            for(int j = 0; j<3; j++)
            {
                SideColors[i][j] = param[i][j];
            }
        }
    }
};


bool drawAxes = true;
float tyreAngle = 0;

float quadAngle = 0;
float boxAngle = 0;
float boxScaleFactor = 1;
float tyreScaleFactor = 1;
float tinyBoxAngle = 0;
float background = 0;
float windowAngle = 0;
float doorOpened = 0;
float houseScale = 1;
float houseAngle = 0;
int FanRotate = 0; ///0 stop 1 clockwise 2 anti
int DoorRotate = 0; /// 0 -> freeze 1-> Open 2-> close
float WindowOpenFactor = 0;
int WindowOpen = 0; /// 0 -> freeze 1->Slide up 2-> slide down
Point pos, u, r, l;
using namespace std;
void windowAngleChanger(int angle)
{
    cout<<windowAngle<<endl;
    windowAngle += angle;
    if(windowAngle >= 0)
    {
        windowAngle = 0;
    }
    else if(windowAngle <= -180)
    {
        windowAngle = -180;
    }

}

void displayAxes()
{
	if(drawAxes)
	{
		glBegin(GL_LINES);
		{
		    glColor3f(1.0, 0, 0);

			glVertex3f( 500,0,0); // a point on pos x axis
			glVertex3f(-500,0,0); // a point on neg x axis

			glColor3f(0, 1.0, 0);

			glVertex3f(0,-500,0); // a point on pos y axis
			glVertex3f(0, 500,0); // a point on neg y axis

			glColor3f(0, 0, 1.0);

			glVertex3f(0,0, 500);  // a point on pos z axis
			glVertex3f(0,0,-500);   // a point on neg z axis
		}
		glEnd();
	}
}

Point func(Point vect, Point perp, int dir)
{
    double c = cos(pi/180);
    double s = dir * sin(pi/180);
    Point point;
    point.x = c * vect.x + s * perp.x;
    point.y = c * vect.y + s * perp.y;
    point.z = c * vect.z + s * perp.z;
    c = sqrt(point.x*point.x + point.y*point.y + point.z*point.z);
    point.x /= c;
    point.y /= c;
    point.z /= c;
    return point;
}

void doorOpenFunction(int val)
{
    if(doorOpened + val >= 70)
    {
        doorOpened = 70;
    }
    else if(doorOpened + val <= 0)
    {
        doorOpened = 0;
    }
    else
    {
        doorOpened += val;
    }
}

void windowSlideFunction(int val)
{
    if(WindowOpenFactor + val >= 76)
    {
        WindowOpenFactor = 76;
    }
    else if(WindowOpenFactor + val <= 0)
    {
        WindowOpenFactor = 0;
    }
    else
    {
        WindowOpenFactor += val;
    }

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1': {
            Point l1 = func(l, r, -1);
            r = func(r, l, 1);
            l = l1;
            break;
		}
		case '2': {
            Point l1 = func(l, r, 1);
            r = func(r, l, -1);
            l = l1;
            break;
		}

		case '3': {
            Point u1 = func(u, l, -1);
            l = func(l, u, 1);
            u = u1;
            break;
        }
        case '4': {
            Point u1 = func(u, l, 1);
            l = func(l, u, -1);
            u = u1;
            break;
        }
        case '5': {
            Point r1 = func(r, u, -1);
            u = func(u, r, 1);
            r = r1;
            break;
        }
        case '6':{
            Point r1 = func(r, u, 1);
            u = func(u, r, -1);
            r = r1;
            break;
        }
        case '7':{
            tyreScaleFactor += 0.2;
            break;
        }
        case '8':{
            tyreScaleFactor -= 0.2;
            break;
        }
        case '9':{
            //std::cout<<windowAngle<<std::endl;
            windowAngleChanger(5);
            break;
        }
        case '0':{

            windowAngleChanger(-5);
            //std::cout<<windowAngle<<std::endl;
            break;
        }
        case 'l':{
            houseAngle += 5;
            break;
        }
        case 'r':{
            houseAngle -= 5;
            break;
        }
        case 'u':{
            houseScale += 0.1;
            break;
        }
        case 'd':{
            houseScale -= 0.1;
            break;
        }
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN: // down arrow
		    {
		        pos.x = pos.x - l.x*2;
		        pos.y = pos.y - l.y*2;
		        pos.z = pos.z - l.z*2;

		        printf("Quad Angle %f\n", quadAngle);
		        break;
		    }

		case GLUT_KEY_UP:   // up arrow
		    {
		        pos.x = pos.x + l.x*2;
		        pos.y = pos.y + l.y*2;
		        pos.z = pos.z + l.z*2;
		        //quadAngle -= 5;
		        printf("Quad Angle %f\n", quadAngle);
		        break;
		    }

		case GLUT_KEY_RIGHT:    // right arrow
		    {
		        pos.x = pos.x + r.x*2;
		        pos.y = pos.y + r.y*2;
		        pos.z = pos.z + r.z*2;
		        //boxAngle += 5;
		        printf("Box Angle %f\n", boxAngle);
		        break;
		    }

		case GLUT_KEY_LEFT:     // left arrow
		    {
                pos.x = pos.x - r.x*2;
		        pos.y = pos.y - r.y*2;
		        pos.z = pos.z - r.z*2;
		        //boxAngle -= 5;
		        printf("Box Angle %f\n", boxAngle);
		        break;
		    }

		case GLUT_KEY_PAGE_UP:
		    {
		        pos.x = pos.x + u.x*2;
		        pos.y = pos.y + u.y*2;
		        pos.z = pos.z + u.z*2;
                //boxScaleFactor += 0.2;
                break;
		    }

		case GLUT_KEY_PAGE_DOWN:
		    {
		        pos.x = pos.x - u.x*2;
		        pos.y = pos.y - u.y*2;
		        pos.z = pos.z - u.z*2;
		        //boxScaleFactor -= 0.2;
                break;
		    }

		case GLUT_KEY_HOME:
		    {
		        doorOpenFunction(5);
		        //doorOpened += 5;
		        break;
		    }

        case GLUT_KEY_END:
            {
                doorOpenFunction(-5);
                break;
            }

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN) // 2 times?? in ONE click? -- solution is checking DOWN or UP
            {
                printf("Mouse Left Button Clicked\n");
			}
			else if(state == GLUT_UP)
            {
                printf("Mouse Left Button Released\n");
            }
            break;
		default:
			break;
	}
}

void displayTyre(float tyreRadius, float tyreWidth)
{
    int tyreSlices = 10;
    float sliceWidth = (2 * M_PI * tyreRadius) / tyreSlices;
    float sliceAngle = 360 / tyreSlices;

    float halfTyreWidth = tyreWidth / 2, quartTyreWidth = halfTyreWidth / 2;
    float halfSliceWidth = sliceWidth / 2;

    /*glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    {
        glVertex3f(halfTyreWidth, tyreRadius, 0);
        glVertex3f(-halfTyreWidth, tyreRadius, 0);
        glVertex3f(-halfTyreWidth, -tyreRadius, 0);
        glVertex3f(halfTyreWidth, -tyreRadius, 0);
    }
    glEnd();

    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);
    {
        glVertex3f(halfTyreWidth, 0, tyreRadius);
        glVertex3f(halfTyreWidth, 0, -tyreRadius);
        glVertex3f(-halfTyreWidth, 0, -tyreRadius);
        glVertex3f(-halfTyreWidth, 0, tyreRadius);
    }
    glEnd();*/



    ///tyreSlices
    for(int i=0; i<tyreSlices; i++)
    {
        glPushMatrix();
        if(i%2 == 1)
        {
            glColor3f(1, 1, 1);
        }
        else
        {
            glColor3f(0, 0, 0);
        }
        glRotatef(sliceAngle*i, 1, 0, 0);
        glTranslatef(0, tyreRadius, 0);
        glBegin(GL_QUADS);
        {
            glVertex3f(halfTyreWidth, 0, halfSliceWidth);
            glVertex3f(halfTyreWidth, 0, -halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0, -halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0, halfSliceWidth);
        }
        glEnd();
        glPopMatrix();
    }
}

void displayQuad(float quadLength, float quadWidth, bool gradient) // width along x axis, length along z axis
{
    float halfQuadWidth = quadWidth / 2, halfQuadLength = quadLength / 2;

    glBegin(GL_QUADS);
    {
        /** Solid Fill Quad **/

        //glColor3f(1, 1, 0);

        //glVertex3f(halfQuadWidth, 0, halfQuadLength);
        //glVertex3f(halfQuadWidth, 0, -halfQuadLength);
        //glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
        //glVertex3f(-halfQuadWidth, 0, halfQuadLength);

        /** Gradient Fill Quad **/
        (gradient) ? glColor3f(0, 0, 1) : glColor3f(1, 1, 1);


        glVertex3f(halfQuadWidth, 0, halfQuadLength);
        glVertex3f(halfQuadWidth, 0, -halfQuadLength);

        (!gradient) ? glColor3f(0, 0, 1) : glColor3f(1, 1, 1);

        glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
        glVertex3f(-halfQuadWidth, 0, halfQuadLength);

    }
    glEnd();
}

void displayBox(float boxLength, float boxWidth, float boxHeight, BoxColors SideColors) // length -> x axis, width -> yaxis, height -> z axis
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;
    glBegin(GL_QUADS);
    {
        glColor3f(SideColors.SideColors[0][0], SideColors.SideColors[0][1], SideColors.SideColors[0][2]);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);


        glColor3f(SideColors.SideColors[1][0], SideColors.SideColors[1][1], SideColors.SideColors[1][2]);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glColor3f(SideColors.SideColors[2][0], SideColors.SideColors[2][1], SideColors.SideColors[2][2]);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();
}

void displayScene()
{
    //Primitive Object dimensions

    float quadLength = 180, quadWidth = 80;
    float largeBoxLength = 300, largeBoxWidth = 200, largeBoxHeight = 30;
    float smallBoxLength = 150, smallBoxWidth = 70, smallBoxHeight = 30;
    float tinyBoxLength = 10, tinyBoxWidth = 10, tinyBoxHeight = 10;
    float tyreRadius = 50, tyreWidth = 20;
    float smallerTyreRadius = 20;


    /** Principal Axes **/

    displayAxes();

    /**/

    /** Quad **/

    //glRotatef(quadAngle, 1, 0, 0); // rotates with respect to x axis
    //glRotatef(quadAngle, 0, 1, 0); // rotates with respect to y axis
    //glRotatef(quadAngle, 0, 0, 1); // rotates with respect to z axis//
    //displayQuad(quadLength, quadWidth);

    /**/

    //glScalef(1, 1, houseScale);
    glRotatef(houseAngle, 0, 1, 0);
    glScalef(houseScale, houseScale, houseScale);
    glPushMatrix();
    //glRotatef();
    glTranslatef(0, 0, largeBoxHeight/2);
    float param_box1[3][3] = {{0,0,1}, {0,0,0.8}, {0,0,0.7}};
    BoxColors Floor(param_box1);
    displayBox(largeBoxLength, largeBoxWidth, largeBoxHeight, Floor);
    glPopMatrix();


    int house[3] = {largeBoxLength-20, largeBoxWidth-20, 250};
    glPushMatrix();
    //glRotatef();
    glTranslatef(0,0, largeBoxHeight + house[2]/2);
    float house_param[3][3] = {{0.7529,0,0}, {0.5,0,0}, {1,0,0}};
    BoxColors houseColor(house_param);
    displayBox(house[0], house[1], house[2], houseColor);
    glPopMatrix();

    int window[3] = {1, 76, 76};
    glPushMatrix();
    glTranslated(0, 0, WindowOpenFactor);
    glTranslatef(house[0]/2 + 1,window[1]/2, largeBoxHeight + house[2]/2 + window[2]/2);

    if(WindowOpen == 0)
    {
        glRotatef(windowAngle, 0,1,0);
    }
    glTranslatef(0, -window[1]/2, -window[2]/2);
    float window_param[3][3] = {{0.9,0.9,0}, {0.7,0.7,0}, {0.5,0.5,0}};
    BoxColors windowColor(window_param);
    displayBox(window[0], window[1], window[2], windowColor);
    glPopMatrix();

    int rim[3] = {6, 76, 10};
    glPushMatrix();
    //glRotatef();
    glTranslatef(house[0]/2 + 1,0, largeBoxHeight + house[2]/2 + window[2]/2 + rim[2]/2);
    float rim_param[3][3] = {{0,0.75,0.75}, {0,0.5,0.5}, {0,0.25,0.25}};
    BoxColors rimColor(rim_param);
    displayBox(rim[0], rim[1], rim[2], rimColor);
    glPopMatrix();
    ///Window
    glPushMatrix();
    glTranslatef(-house[0]/2 - 1,window[1]/2, largeBoxHeight + house[2]/2 + window[2]/2);
    glRotatef(-windowAngle, 0,1,0);
    glTranslatef(0, -window[1]/2, -window[2]/2);
    //float window_param[3][3] = {{0.9,0.9,0}, {0.7,0.7,0}, {0.5,0.5,0}};
    //BoxColors windowColor(window_param);
    displayBox(window[0], window[1], window[2], windowColor);
    glPopMatrix();


    glPushMatrix();
    glTranslated(0, 0, WindowOpenFactor);
    glTranslatef(house[0]/2 + 1,window[1]/2, largeBoxHeight + house[2]/2 + window[2]/2);
    if(WindowOpen == 0)
    {
        glRotatef(-windowAngle, 0,1,0);
    }
    glTranslatef(0, -window[1]/2, -window[2]/2);
    //float window_param[3][3] = {{0.9,0.9,0}, {0.7,0.7,0}, {0.5,0.5,0}};
    //BoxColors windowColor(window_param);
    displayBox(window[0], window[1], window[2], windowColor);
    glPopMatrix();

    //int rim[3] = {1, 76, 10};
    glPushMatrix();
    //glRotatef();
    glTranslatef(-house[0]/2 - 1,0, largeBoxHeight + house[2]/2 + window[2]/2 + rim[2]/2);
    //float rim_param[3][3] = {{1,1,0}, {0.5,0.5,0}, {0.5,0.7,0}};
    //BoxColors rimColor(rim_param);
    displayBox(rim[0], rim[1], rim[2], rimColor);
    glPopMatrix();

    int ceiling[3] = {largeBoxLength, largeBoxWidth, 10};
    glPushMatrix();
    //glRotatef();
    glTranslatef(0,0, largeBoxHeight + house[2] + ceiling[2]/2);
    float ceiling_param[3][3] = {{0,1,0}, {0,0.5,0}, {0,0.25,0}};
    BoxColors ceilingColor(ceiling_param);
    displayBox(ceiling[0], ceiling[1], ceiling[2], ceilingColor);
    glPopMatrix();

    int fanHolder[3] = {120, 20, 120};
    glPushMatrix();
    //glRotatef();
    glTranslatef(0,0, largeBoxHeight + house[2] + ceiling[2] + fanHolder[2]/2);
    float fanHolder_param[3][3] = {{0.9,0.9,0}, {0.7,0.7,0}, {0.5,0.5,0}};
    BoxColors fanHolderColor(fanHolder_param);
    displayBox(fanHolder[0], fanHolder[1], fanHolder[2], fanHolderColor);
    glPopMatrix();

    int lowerStairs[3] = {150, 300, 10};
    glPushMatrix();
    //glRotatef();
    glTranslatef(0,0,lowerStairs[2]/2);
    float lowerStairs_param[3][3] = {{0.66,0.66,0.66}, {0.5,0.5,0.5}, {0.3,0.3,0.3}};
    BoxColors lowerStairsColor(lowerStairs_param);
    displayBox(lowerStairs[0], lowerStairs[1], lowerStairs[2], lowerStairsColor);
    glPopMatrix();


    int middleStairs[3] = {130, 275, 10};
    glPushMatrix();
    //glRotatef();
    glTranslatef(0,0, lowerStairs[2] + middleStairs[2]/2);
    float middleStairs_param[3][3] = {{0,0.752,0.737}, {0,0.5,0.5}, {0,0.25,0.25}};
    BoxColors middleStairsColor(middleStairs_param);
    displayBox(middleStairs[0], middleStairs[1], middleStairs[2], middleStairsColor);
    glPopMatrix();

    int upperStairs[3] = {110, 250, 9};
    glPushMatrix();
    //glRotatef();
    glTranslatef(0,0, lowerStairs[2] + middleStairs[2] + upperStairs[2]/2);
    float upperStairs_param[3][3] = {{0.77,0.77,0}, {0.5,0.5,0}, {0.25,0.25,0}};
    BoxColors upperStairsColor(upperStairs_param);
    displayBox(upperStairs[0], upperStairs[1], upperStairs[2], upperStairsColor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,fanHolder[1]/2 + tyreWidth/2,largeBoxHeight + house[2] + ceiling[2] + fanHolder[2]/2);
    glRotatef(-tyreAngle, 0, 1, 0);
    //glTranslatef(0,-(fanHolder[1]/2 + tyreWidth/2), -(largeBoxHeight + house[2] + ceiling[2] + fanHolder[2]/2));

    glPushMatrix();
    //glTranslatef(0,fanHolder[1]/2 + tyreWidth/2,largeBoxHeight + house[2] + ceiling[2] + fanHolder[2]/2);
    glRotatef(90, 0,0,1);
    displayTyre(tyreRadius, tyreWidth);
    glPopMatrix();

    /*int window[3] = {1, 76, 76};
    glPushMatrix();
    glTranslatef(house[0]/2 + 1,window[1]/2, largeBoxHeight + house[2]/2 + window[2]/2);
    glRotatef(windowAngle, 0,1,0);
    glTranslatef(0, -window[1]/2, -window[2]/2);
    float window_param[3][3] = {{0.9,0.9,0}, {0.7,0.7,0}, {0.5,0.5,0}};
    BoxColors windowColor(window_param);
    displayBox(window[0], window[1], window[2], windowColor);
    glPopMatrix();*/

    glPushMatrix();
    //glTranslatef(0,fanHolder[1]/2 + tyreWidth/2,largeBoxHeight + house[2] + ceiling[2] + fanHolder[2]/2);
    glRotatef(90, 0,0,1);
    displayTyre(smallerTyreRadius, tyreWidth);
    glPopMatrix();

    glPushMatrix();
    //glTranslatef(0,fanHolder[1]/2 + tyreWidth/2, largeBoxHeight + house[2] + ceiling[2] + fanHolder[2]/2);
    glRotatef(90, 0, 0, 1);
    int tyreSlices = 10;
    float sliceAngle = 360/tyreSlices;

    for(int i =0; i<tyreSlices; i++)
    {
        glPushMatrix();
        glRotatef(sliceAngle*i, 1, 0, 0);
        //glScalef(1, 0.75, 1);
        float temp = smallerTyreRadius;
        glTranslatef( 0,tyreRadius - smallerTyreRadius +5 , 0);
        glRotatef(90, 0, 0, 1);
        glBegin(GL_QUADS);
        {
            if(i%2 == 1)
            {
                glColor3f(0, 0, 1);
            }
            else
            {
                glColor3f(1, 0, 0);
            }
            glVertex3f(tyreWidth-5, temp/2, 0);
            glVertex3f(tyreWidth-5,-temp/2, 0);
            glVertex3f(-tyreWidth+5,-temp/2, 0);
            glVertex3f(-tyreWidth+5,temp/2, 0);
        }
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
    glPopMatrix();

    int door_rim[3] = {quadWidth*2, 10, 10};
    glPushMatrix();
    //glRotatef();
    glTranslatef(0, house[1]/2 + 1, largeBoxHeight + quadLength);
    float door_rim_param[3][3] = {{0,0.75,0.75}, {0,0.5,0.5}, {0,0.25,0.25}};
    BoxColors DoorrimColor(door_rim_param);
    displayBox(door_rim[0], door_rim[1], door_rim[2], DoorrimColor);
    glPopMatrix();

    ///Front Door
    glPushMatrix();
    glTranslatef((-quadWidth-doorOpened)/2, house[1]/2 + 1, largeBoxHeight + quadLength/2);
    //glRotatef(90, 0, 0, 1);
    displayQuad(quadLength, quadWidth, false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((quadWidth+doorOpened)/2, house[1]/2 + 1, largeBoxHeight + quadLength/2);
    //glRotatef(90, 0, 0, 1);
    displayQuad(quadLength, quadWidth, true);
    glPopMatrix();
    ///Back Door
    glPushMatrix();
    glTranslatef((-quadWidth-doorOpened)/2, -house[1]/2 - 1, largeBoxHeight + quadLength/2);
    //glRotatef(90, 0, 0, 1);
    displayQuad(quadLength, quadWidth, false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((quadWidth+doorOpened)/2, -house[1]/2 - 1, largeBoxHeight + quadLength/2);
    //glRotatef(90, 0, 0, 1);
    displayQuad(quadLength, quadWidth, true);
    glPopMatrix();

    //lPushMatrix();
    //glRotatef(tinyBoxAngle, 0, 0, 1); // angle is updated in animate function
    //glTranslatef(250, 40, 50);

    //displayBox(tinyBoxLength, tinyBoxWidth, tinyBoxHeight, Floor);
    //glPopMatrix();
}

void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(background, background, background,0);	//sets background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // sets the current matrix to identity
    gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z); // sets camera position and direction

    displayScene();

	glutSwapBuffers();
}



void animate(){
    if(FanRotate == 2)
    {
        tyreAngle += 0.2;
    }
    else if(FanRotate == 3)
    {
        tyreAngle -= 0.2;
    }
    if(DoorRotate == 1)
    {
        doorOpenFunction(1);
        if(doorOpened == 70)
        {
            DoorRotate = 0;
        }
    }
    else if(DoorRotate == 2)
    {
        doorOpenFunction(-1);
        if(doorOpened == 0)
        {
            DoorRotate = 0;
        }
    }
    if(WindowOpen == 1)
    {
        windowSlideFunction(2);
        windowAngleChanger(-5);
        if(WindowOpenFactor >= 76)
        {
            WindowOpen = 3;
            //windowAngle = 0;
        }
    }
    else if(WindowOpen == 2)
    {
        windowSlideFunction(-2);
        windowAngleChanger(5);
        if(WindowOpenFactor <= 0)
        {
            WindowOpen = 3;
        }
    }
    tinyBoxAngle += 0.05;
	glutPostRedisplay();
}

void init(){
    glClearColor(background, background, background,0); 	//sets background color

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity(); // sets the current matrix to identity
    gluPerspective(80,	1,	1,	1000.0);

	// to understand the following values, read the specification pdf
    double d = 1 / sqrt2;
    pos = {300, 300, 150};
    u = {0, 0, 1};
    r = {-d, d, 0};
    l = {-d, -d, 0};
}

void subMenu(int id)
{
    if(id == 3)
    {
        background = 1;
    }
    else if(id == 4)
    {
        background = 0;
    }
    else if(id == 1)
    {
        background= 0.5;
    }
    else if(id == 2)
    {
        background = 0.3;
    }
}

void mainMenu(int id)
{
    if(id == 8)
    {
        houseScale = 1;
    }
}

void SlidingWindow(int id)
{
    if(id == 1)
    {

        WindowOpen = 1;
        //windowAngle = 180;
    }
    else if(id == 2)
    {
        WindowOpen = 2;
        //windowAngle = 0;
    }
}
void RotatingDoor(int id)
{
    if(id == 3)
    {
       DoorRotate = 1;
    }
    else if(id == 4)
    {
        DoorRotate = 2;
    }
}
void ExhaustFan(int id)
{
    if(id == 5)
    {
        FanRotate = 2;
    }
    else if(id == 6)
    {
        FanRotate = 3;
    }
    else if(id == 7)
    {
        FanRotate = 1;
    }
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	int subMenuNo = glutCreateMenu(SlidingWindow);
    glutAddMenuEntry("Slide Up", 1);
	glutAddMenuEntry("Slide Down", 2);

    int subMenu2 = glutCreateMenu(RotatingDoor);
	glutAddMenuEntry("Open", 3);
	glutAddMenuEntry("Close", 4);
	//glutAddSubMenu("Background", subMenuNo);
    int subMenu3 = glutCreateMenu(ExhaustFan);
	glutAddMenuEntry("Clockwise", 5);
	glutAddMenuEntry("Anti-Clockwise", 6);
	glutAddMenuEntry("Stop", 7);

    glutCreateMenu(mainMenu);
	glutAddSubMenu("Sliding Window", subMenuNo);
	glutAddSubMenu("Rotating Door", subMenu2);
	glutAddSubMenu("Exhaust Fan", subMenu3);
    glutAddMenuEntry("Original Size", 8);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();		//The main loop of OpenGL, this function never returns

	return 0;
}
