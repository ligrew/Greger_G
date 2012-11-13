package com.ligrew.trojkat;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.SystemClock;
import android.util.Log;

public class Render implements GLSurfaceView.Renderer {

    private static final String TAG = "Render";
    private Trojkat trojkat;
    private Trojkat trojkat2;

    private final float[] wynikM = new float[16];
    private final float[] projekcjaM = new float[16];
    private final float[] widokM = new float[16];
    private final float[] rotacjaM = new float[16];

    public volatile float kat;	// Declare as volatile because we are updating it from another thread
    public volatile float katN;
    public volatile float x;
    public volatile float y;
    public volatile float z = 4;
    public volatile boolean nacisk;

    public void onSurfaceCreated(GL10 unused, EGLConfig config)
    {
        GLES20.glEnable(GLES20.GL_DEPTH_TEST);
        GLES20.glClearColor(0.097f, 0.184f, 0.199f, 1.0f);	// kolor t³a

        trojkat = new Trojkat(  -0.5f, -0.25f, 0.0f,
    							 0.5f, -0.25f, 0.0f,
    							 0.0f, 0.559016994f, 0.0f,
           	 					 0.184f, 0.598f, 0.754f, 1.0f );

        trojkat2 = new Trojkat(  -0.5f, -0.25f, 0.0f,
    							 0.5f, -0.25f, 0.0f,
    							 0.0f, 0.25f, 1.0f,
           	 					 0.684f, 0.598f, 0.554f, 1.0f );
    }

    public void onDrawFrame(GL10 unused)
    {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);		// ustaw kolor t³a

        Matrix.setLookAtM(widokM, 0, 0, 0, -1f,		//kierunek
        							 0, 0, 5.0f, 		//pozycja
        							 0f, 1.0f, 0.0f);	//gora

        if (z < 1)
        	z = 1;
        else if (z > 10)
        	z=10;
        
        Matrix.translateM(widokM, 0, 0, 0, z);
        
         //Create a rotation for the triangle
        
        Matrix.rotateM(widokM, 0, x, 0, 1, 0);
        Matrix.rotateM(widokM, 0, y, 1, 0, 0);
        //long time = SystemClock.uptimeMillis() % 4000L;
        //kat = 0.090f * ((int) time);
        
        Matrix.setRotateM(rotacjaM, 0, kat, 0.0f, 0.0f, 1.0f);

        Matrix.multiplyMM(wynikM, 0, projekcjaM, 0, widokM, 0);		// obliczanie transformacji projekcji i widoku
        // Combine the rotation matrix with the projection and camera view
        Matrix.multiplyMM(wynikM, 0, rotacjaM, 0, wynikM, 0);

        // Draw triangle
        trojkat.draw(wynikM);
        trojkat2.draw(wynikM);
    }

    public void onSurfaceChanged(GL10 unused, int width, int height)
    {
        GLES20.glViewport(0, 0, width, height);

        float ratio = (float) width / height;
        //Matrix.perspectiveM(projekcjaM, 0, 60, ratio, 2, 100);
        Matrix.frustumM(projekcjaM, 0, -ratio, ratio, -1f, 1f, 1f, 100f);		//0, left, right, bottom, top, near, far
    }

    public static int loadShader(int type, String shaderCode)
    {
        // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
        // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
        int shader = GLES20.glCreateShader(type);

        // add the source code to the shader and compile it
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }

    /**
     * Utility method for debugging OpenGL calls. Provide the name of the call
     * just after making it:
     *
     * <pre>
     * mColorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
     * MyGLRenderer.checkGlError("glGetUniformLocation");</pre>
     *
     * If the operation is not successful, the check throws an error.
     *
     * @param glOperation - Name of the OpenGL call to check.
     */
    public static void checkGlError(String glOperation)
    {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e(TAG, glOperation + ": glError " + error);
            throw new RuntimeException(glOperation + ": glError " + error);
        }
    }
}

