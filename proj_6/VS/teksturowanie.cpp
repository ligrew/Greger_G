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
#include <vector>


#define _USE_MATH_DEFINES
#include <math.h>

GLuint shader;
GLuint MVMatrixLocation;
GLuint MVPMatrixLocation;
GLuint normalMatrixLocation;
GLuint ambientLightLocation;
GLuint shaderPositionLocation;
GLuint shaderColorLocation;
GLuint shaderAngleLocation;
GLuint shaderAttenuation0Location;
GLuint shaderAttenuation1Location;
GLuint shaderAttenuation2Location;
GLuint shaderAmbientColorLocation;
GLuint shaderDiffuseColorLocation;
GLuint shaderSpecularColorLocation;
GLuint shaderSpecularExponentLocation;


CStopWatch timer;
GLFrame cameraFrame;

//ustawienie kamery
M3DMatrix44f mCamera;
GLFrustum viewFrustum;

GLuint textureID[3];
GLbyte *pBits=NULL;

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
float il = 3;		//odleglosc szescianow

void KeyboardKeys(unsigned char key, int xmouse, int ymouse)//unsigned char key, int xmouse, int ymouse)
{
	//printf("Nacisnieto: %c\n", key);
	switch(key)
	{
        case 'q':
			exit(0);
        case 'w':			//obróæ do góry
             kz++; break;
        case 's':			//obróæ w dó³
             kz--; break;
        case '1':			//odleglosc szescianow = 1.2
             il=1.2f; break;
        case '2':			//odleglosc szescianow = 2
             il=2; break;
        case '3':			//odleglosc szescianow = 3
             il=3; break;
        case '4':			//odleglosc szescianow = 4
             il=4; break;
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
	viewFrustum.SetPerspective(fov,(float)(w/h),1.0f,2000.0f);	//fov, ratio, blisko, daleko
}


//----------------------------------------------------
void drawQuad()
{
	glVertexAttrib3f(GLT_ATTRIBUTE_NORMAL, 0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
    glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 0);
	glVertex3f(0.f, 0.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 0);
	glVertex3f(1.f, 0.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 1);
	glVertex3f(1.f, 1.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 1);
	glVertex3f(0.f, 1.f, 2.f);
	
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 1);
	glVertex3f(0.f, 0.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 1);
	glVertex3f(1.f, 0.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 0);
	glVertex3f(1.f, 0.f, 1.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 0);
	glVertex3f(0.f, 0.f, 1.f);
	
    glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 1);
	glVertex3f(1.f, 0.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 1);
	glVertex3f(1.f, 1.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 0);
	glVertex3f(1.f, 1.f, 1.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 0);
	glVertex3f(1.f, 0.f, 1.f);

    glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 1);
	glVertex3f(1.f, 1.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 1);
	glVertex3f(0.f, 1.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 0);
	glVertex3f(0.f, 1.f, 1.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 0);
	glVertex3f(1.f, 1.f, 1.f);
	
    glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 1);
	glVertex3f(0.f, 1.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 1);
	glVertex3f(0.f, 0.f, 2.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 0);
	glVertex3f(0.f, 0.f, 1.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 0);
	glVertex3f(0.f, 1.f, 1.f);

    glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 0);
	glVertex3f(0.f, 0.f, 1.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 0);
	glVertex3f(1.f, 0.f, 1.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 1, 1);
	glVertex3f(1.f, 1.f, 1.f);
	glVertexAttrib2f(GLT_ATTRIBUTE_TEXTURE0, 0, 1);
	glVertex3f(0.f, 1.f, 1.f);
	
	glEnd();
}

//----------------------------------------------------
void drawGrid() {
	glBegin(GL_LINES);
	for(float i=-10.0; i<=10.0; i++)
	{
		glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 1.0f, 1.0f, 1.0f);
		glVertex3f(-10.0f, i, 0.0f);
		glVertex3f( 10.0f, i, 0.0f);
		
		glVertex3f(i, -10.0f, 0.0f);
		glVertex3f(i,  10.0f, 0.0f);
	}
    glEnd();
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.

void SetupRC()
{
    glClearColor(0.097f, 0.184f, 0.199f, 1.0f);// szaro/niebieskie t³o assassin

	glEnable(GL_DEPTH_TEST);

	shader = gltLoadShaderPairWithAttributes("shader.vp", "shader.fp", 3, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "texCoord0"); // proj4
    fprintf(stdout, "GLT_ATTRIBUTE_VERTEX : %d\nGLT_ATTRIBUTE_COLOR : %d \n", GLT_ATTRIBUTE_VERTEX, GLT_ATTRIBUTE_COLOR);

	MVPMatrixLocation = glGetUniformLocation(shader, "MVPMatrix");
	if(MVPMatrixLocation == -1)
		fprintf(stderr,"uniform MVPMatrix could not be found\n");

	MVPMatrixLocation = glGetUniformLocation(shader,"MVPMatrix");
	normalMatrixLocation = glGetUniformLocation(shader,"normalMatrix");
	MVMatrixLocation = glGetUniformLocation(shader,"MVMatrix");

	shaderPositionLocation		= glGetUniformLocation(shader, "light1.position");
	shaderColorLocation			= glGetUniformLocation(shader, "light1.color");
	shaderAngleLocation			= glGetUniformLocation(shader, "light1.angle");
	shaderAttenuation0Location	= glGetUniformLocation(shader, "light1.attenuation0");
	shaderAttenuation1Location	= glGetUniformLocation(shader, "light1.attenuation1");
	shaderAttenuation2Location	= glGetUniformLocation(shader, "light1.attenuation2");
	
	shaderAmbientColorLocation	= glGetUniformLocation(shader, "material.ambientColor");
	shaderDiffuseColorLocation	= glGetUniformLocation(shader, "material.diffuseColor");
	shaderSpecularColorLocation	= glGetUniformLocation(shader, "material.specularColor");
	shaderSpecularExponentLocation	= glGetUniformLocation(shader, "material.specularExponent");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(3, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	pBits=gltReadTGABits("ezio.tga", &nWidth, &nHeight, &nComponents, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
	glUseProgram(shader);
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

	//swiatlo
	M3DVector3f ambientLight = {1.0f, 1.0f, 1.0f};
	M3DVector3f position = {2.0f, 2.0f, 3.0f};
	M3DVector3f color = {1.0f, 1.0f, 1.0f};
	float l_angle = 90.0f;
	float attenuation0 = 0.5f;
	float attenuation1 = 0.05f;
	float attenuation2 = 0.05f;
	M3DVector3f ambientColor = {0.1f, 0.1, 0.1};
	M3DVector3f diffuseColor = {0.184f, 0.598f, 0.75f};
	M3DVector3f specularColor = {1.0f, 1.0f, 1.0f};
	float specularExponent = 5;
	

	geometryPipeline.SetMatrixStacks(modelView,projection);

	projection.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelView.PushMatrix();

	cameraFrame.GetCameraMatrix(mCamera);
	modelView.LoadMatrix(mCamera);
	modelView.PushMatrix();

	
	glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(MVMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(normalMatrixLocation,1,GL_FALSE,geometryPipeline.GetNormalMatrix());
	
	glUniform3fv(shaderPositionLocation, 1, position);
	glUniform3fv(shaderColorLocation, 1, color);
	glUniform1f(shaderAngleLocation, angle);
	glUniform1f(shaderAttenuation0Location, attenuation0);
	glUniform1f(shaderAttenuation1Location, attenuation1);
	glUniform1f(shaderAttenuation2Location, attenuation2);
	
	glUniform3fv(shaderAmbientColorLocation, 1, ambientColor);
	glUniform3fv(shaderDiffuseColorLocation, 1, diffuseColor);
	glUniform3fv(shaderSpecularColorLocation, 1, specularColor);
	glUniform1f(shaderSpecularExponentLocation, specularExponent);

	glPolygonOffset(1.0f, 1.0f);
	drawGrid(); 
	glEnable(GL_POLYGON_OFFSET_FILL);
	drawGrid(); 
	glDisable(GL_POLYGON_OFFSET_FILL);

		
	glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(MVMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(normalMatrixLocation,1,GL_FALSE,geometryPipeline.GetNormalMatrix());
	
	glUniform3fv(shaderPositionLocation, 1, position);
	glUniform3fv(shaderColorLocation, 1, color);
	glUniform1f(shaderAngleLocation, angle);
	glUniform1f(shaderAttenuation0Location, attenuation0);
	glUniform1f(shaderAttenuation1Location, attenuation1);
	glUniform1f(shaderAttenuation2Location, attenuation2);
	
	glUniform3fv(shaderAmbientColorLocation, 1, ambientColor);
	glUniform3fv(shaderDiffuseColorLocation, 1, diffuseColor);
	glUniform3fv(shaderSpecularColorLocation, 1, specularColor);
	glUniform1f(shaderSpecularExponentLocation, specularExponent);
	

	for (float i=-6;i<7;i+=il)
		for (float j=-6;j<7;j+=il)
			for (float k=0;k<6;k+=il)
			{
				modelView.PopMatrix();
				modelView.PushMatrix();

				modelView.Translate(i,j,k);
	
				glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewProjectionMatrix());
				glUniformMatrix4fv(MVMatrixLocation,1,GL_FALSE,geometryPipeline.GetModelViewMatrix());
				glUniformMatrix3fv(normalMatrixLocation,1,GL_FALSE,geometryPipeline.GetNormalMatrix());
	
				glUniform3fv(shaderPositionLocation, 1, position);
				glUniform3fv(shaderColorLocation, 1, color);
				glUniform1f(shaderAngleLocation, angle);
				glUniform1f(shaderAttenuation0Location, attenuation0);
				glUniform1f(shaderAttenuation1Location, attenuation1);
				glUniform1f(shaderAttenuation2Location, attenuation2);
	
				glUniform3fv(shaderAmbientColorLocation, 1, ambientColor);
				glUniform3fv(shaderDiffuseColorLocation, 1, diffuseColor);
				glUniform3fv(shaderSpecularColorLocation, 1, specularColor);
				glUniform1f(shaderSpecularExponentLocation, specularExponent);

				drawQuad();
			}
			
	modelView.PopMatrix();
	modelView.PopMatrix();

    glutSwapBuffers();
	glutPostRedisplay();
	
}

void initRendering()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Teksturowanie");

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