// Our first OpenGL program that will just draw a triangle on the screen.
#include <Windows.h>
#include <GLTools.h>
#include <GL/glew.h> // OpenGL toolkit


#define FREEGLUT_STATIC
#include <GL/glut.h>            // Windows FreeGlut equivalent

#include <GLFrame.h>
#include <GLFrustum.h>
#include <StopWatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

#define _USE_MATH_DEFINES
#include <cmath> 


GLuint MVPMatrixLocation;

CStopWatch timer;
GLFrame cameraFrame;

//ustawienie kamery
M3DMatrix44f mCamera;
GLFrustum viewFrustum;

void SetUpFrame(GLFrame &frame,const M3DVector3f origin, const M3DVector3f forward, const M3DVector3f up)
{
	frame.SetOrigin(origin);
	frame.SetForwardVector(forward);
	M3DVector3f side,oUp;
	m3dCrossProduct3(side,forward,up);
	m3dCrossProduct3(oUp,side,forward);
	frame.SetUpVector(oUp);
	frame.Normalize();
}

void LookAt(GLFrame &frame, const M3DVector3f eye, const M3DVector3f at, const M3DVector3f up)
{
    M3DVector3f forward;
    m3dSubtractVectors3(forward, at, eye);
    SetUpFrame(frame, eye, forward, up);
}

float kz = 5;

void KeyboardKeys(unsigned char key, int xmouse, int ymouse)//unsigned char key, int xmouse, int ymouse)
{
	//printf("Nacisnieto: %c\n", key);
	switch(key)
	{
        case 'q':
			exit(0);
        case 'w':			//obróć do góry
             kz++; break;
        case 's':			//obróć w dół
             kz--; break;
        default: 
			break; 
	}
}

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.

void ChangeSize(int w, int h)
{
	float fov = 90.0f;
    glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(fov,(float)(w/h),1.0f,50.0f);	//fov, ratio, blisko, daleko
}

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.

GLuint shader;


void SetupRC()
{
    glClearColor(0.097f, 0.184f, 0.199f, 1.0f);// szaro/niebieskie tło assassin
	
    glEnable(GL_DEPTH_TEST);

    shader = gltLoadShaderPairWithAttributes("shader.vp", "shader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_COLOR, "vColor"); /**szader wierzcho�k�w spass_thru_shader.vp i szader fragment�w **/
    fprintf(stdout, "GLT_ATTRIBUTE_VERTEX : %d\nGLT_ATTRIBUTE_COLOR : %d \n", GLT_ATTRIBUTE_VERTEX, GLT_ATTRIBUTE_COLOR);
	

	MVPMatrixLocation = glGetUniformLocation(shader, "MVPMatrix");
	if(MVPMatrixLocation==-1)
		fprintf(stderr,"Nie mozna znalezc macierzy MVPMatrix\n");
}



void rysujOstroslup()
{
	glBegin(GL_TRIANGLES);
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.5, 0.0, 0.0);//przod
		
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0, 0.5, 0.0);//tyl
		
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0, 0.0, 0.5);//prawa
		
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0, 0.5, 0.5);//lewa
		
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.5, 0.5, 0.0);//dno prawe
		
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.5, 0.5, 5.0);//dno lewe
		
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
}

void rysujSiatke()
{
    glBegin(GL_LINES);

    glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0f, 0.0f, 0.0f);

    for(int i=-10;i<11;i++)
        for(int j=-10; j<11;j++)
		{
            glVertex3f(i, j, 0.0f);
            glVertex3f(-i, j, 0.0f);
            glVertex3f(i, -j, 0.0f);
            glVertex3f(i, j, 0.0f);
        }
		
    glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.7f, 0.7f, 0.7f);

    for(float i=-10;i<10.25;i+=0.25)
        for(float j=-10; j<10.25;j+=0.25)
			if (fmod(i,1) != 0 && fmod(j,1) != 0)
			{
				glVertex3f(i, j, 0.0f);
				glVertex3f(-i, j, 0.0f);
				glVertex3f(i, -j, 0.0f);
				glVertex3f(i, j, 0.0f);
			}
    glEnd();
}


///////////////////////////////////////////////////////////////////////////////
// Called to draw scene

	GLMatrixStack matrixStack;
	
	GLMatrixStack modelView;
	GLMatrixStack projection;
	GLGeometryTransform geometryPipeline;

void RenderScene(void) {
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glFrontFace(GL_CW);
    glUseProgram(shader);

	
	float angle=timer.GetElapsedSeconds()*M_PI/5;

	M3DVector3f at={1,0,0};
	M3DVector3f up={0,0,1};
	M3DVector3f eye;
	eye[0]=6.0f*cos(angle);
	eye[1]=6.0f*sin(angle);
	eye[2]=kz; 

	LookAt(cameraFrame,eye,at,up);
	

	geometryPipeline.SetMatrixStacks(modelView,projection);

	projection.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelView.PushMatrix();

	cameraFrame.GetCameraMatrix(mCamera);
	modelView.LoadMatrix(mCamera);
	modelView.PushMatrix();

	glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());

	rysujSiatke();
	
	rysujOstroslup();

	for (int i=-9; i<10; i+=3)
		for (int j=-9; j< 10; j+=3)
			if ((i < -2 || i > 2) && (j < -2 || j > 2))
			{
				modelView.PopMatrix();
				modelView.PushMatrix();

				modelView.Translate(i,j,0.0f);
				glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
				rysujOstroslup();
			}

	modelView.PopMatrix();
	modelView.PushMatrix();
	modelView.Translate(1,0.0,2.0);
	modelView.Rotate(180.0,180.0,0,1);
	glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());

	
	rysujOstroslup();
	modelView.PopMatrix();
	modelView.PopMatrix();


    // Perform the buffer swap to display back buffer
	
    glutSwapBuffers();
	glutPostRedisplay();
	
}

void initRendering()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Ostroslupy i siatka");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
	glutKeyboardFunc(KeyboardKeys);
	//glutSpecialFunc(specialKeys);
	
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs

int main(int argc, char* argv[]) {
  
    glutInit(&argc, argv);
	
	initRendering();
	
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    SetupRC();

    glutMainLoop();
    return 0;
}