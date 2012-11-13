package ligrew.P1;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.SystemClock;
import android.util.Log;

public class Render implements GLSurfaceView.Renderer
{
    private static final String TAG = "Render";
    private Trojkat [] tabTrojkat = new Trojkat [6];

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

        //ty³
        tabTrojkat[0] = new Trojkat(-1.0f, -1.0f, 0.0f,
									 1.0f, -1.0f, 0.0f,
									 0.0f, 0.0f, 2.0f,
           	 					 	 0.5f,  0.5f,  0.5f, 1.0f );
        //dno prawe
        tabTrojkat[1] = new Trojkat( 1.0f, 1.0f, 0.0f,
									 1.0f, -1.0f, 0.0f,
									-1.0f, 1.0f, 0.0f,
           	 					 	 0.0f, 0.5f, 0.5f, 1.0f );
        //dno lewe
        tabTrojkat[2] = new Trojkat(-1.0f, 1.0f, 0.0f,
									-1.0f, -1.0f, 0.0f,
									 1.0f, -1.0f, 0.0f,
           	 					 	 0.5f, 0.5f, 0.0f, 1.0f );
        //prawa
        tabTrojkat[3] = new Trojkat( 1.0f, -1.0f, 0.0f,
									 1.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 2.0f,
           	 					 	 0.5f, 0.0f, 0.0f, 1.0f );
        //lewa
        tabTrojkat[4] = new Trojkat(-1.0f, 1.0f, 0.0f,
									-1.0f, -1.0f, 0.0f,
									 0.0f, 0.0f, 2.0f,
           	 					 	 0.0f, 0.0f, 0.5f, 1.0f );
        //przód
        tabTrojkat[5] = new Trojkat( 1.0f, 1.0f, 0.0f,
									-1.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 2.0f,
           	 					 	 0.0f, 0.5f, 0.0f, 1.0f );
    }

    public void onDrawFrame(GL10 unused)
    {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);		// ustaw kolor t³a

        Matrix.setLookAtM(widokM, 0, 0, 0, 1,		//kierunek
        							 0, 0, 5.0f, 		//pozycja
        							 0f, 1.0f, 0.0f);	//gora

        Matrix.multiplyMM(wynikM, 0, projekcjaM, 0, widokM, 0);		// obliczanie transformacji projekcji i widoku
        
        if (z < 1)
        	z = 1;
        else if (z > 10)
        	z=10;
        
        Matrix.translateM(wynikM, 0, 0, 0, z);
        
         //Create a rotation for the triangle
        
        Matrix.rotateM(wynikM, 0, x, 0, 1, 0);
        Matrix.rotateM(wynikM, 0, y, 1, 0, 0);
        long time = SystemClock.uptimeMillis() % 4000L;
        kat = 0.090f * ((int) time);
        
        Matrix.setRotateM(rotacjaM, 0, kat, 0.0f, 0.0f, 1.0f);

        // Combine the rotation matrix with the projection and camera view
        Matrix.multiplyMM(wynikM, 0, rotacjaM, 0, wynikM, 0);

        // Draw triangle
        for (int i=0;i<6;i++)
        	tabTrojkat[i].draw(wynikM);
    }

	public static void perspektywa(float[] mM, int mOffset, float f_fov, float f_aspect, float f_near, float f_far)
	{
		float f_w, f_h;

		f_h = (float)(Math.tan(f_fov * Math.PI / 180 * .5f)) * f_near;
		f_w = f_h * f_aspect;
		// calc half width of frustum in x-y plane at z = f_near

		Matrix.frustumM(mM, mOffset, -f_w, f_w, -f_h, f_h, f_near, f_far);
		// set frustum
	}
	
    public void onSurfaceChanged(GL10 unused, int width, int height)
    {
        GLES20.glViewport(0, 0, width, height);

        float ratio = (float) width / height;
        //perspektywa(projekcjaM, 0, 90, ratio, 1f, 1000.0f);
        //Matrix.perspectiveM(projekcjaM, 0, 90, ratio, 1, 100);
        Matrix.frustumM(projekcjaM, 0, -ratio, ratio, -1f, 1f, 1f, 10f);		//0, left, right, bottom, top, near, far
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

