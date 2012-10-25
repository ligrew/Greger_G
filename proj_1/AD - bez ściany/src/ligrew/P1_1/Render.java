package ligrew.P1_1;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

public class Render implements GLSurfaceView.Renderer {

    private static final String TAG = "Render";
    private Trojkat [] tabTrojkat = new Trojkat [6];

    private final float[] mMVPMatrix = new float[16];
    private final float[] mProjMatrix = new float[16];
    private final float[] mVMatrix = new float[16];
    private final float[] mRotationMatrix = new float[16];

    public volatile float kat;	// Declare as volatile because we are updating it from another thread

    public void onSurfaceCreated(GL10 unused, EGLConfig config)
    {
        GLES20.glClearColor(0.097f, 0.184f, 0.199f, 1.0f);	// kolor t³a

        //ty³
        tabTrojkat[0] = new Trojkat(
									 
									 0.0f,  0.75f, 0.5f,
									 0.5f,  0.0f,  0.0f,
										-0.25f, 0.0f,  0.0f,
           	 					 	 0.5f,  0.5f,  0.5f, 0.5f );
        //dno prawe
        tabTrojkat[1] = new Trojkat( 0.5f,   0.0f, 0.0f,
        							-0.25f,  0.0f, 0.0f,
        							 0.25f, -0.5f, 1.0f,
           	 					 	 0.0f, 0.5f, 0.5f, 1.0f );
        //dno lewe
        tabTrojkat[2] = new Trojkat(-0.25f, 0.0f, 0.0f,
        							 0.25f, -0.5f, 1.0f,
        							-0.5f, -0.5f, 1.0f,
           	 					 	 0.5f, 0.5f, 0.0f, 1.0f );
        //prawa
        tabTrojkat[3] = new Trojkat( 0.5f, 0.0f, 0.0f,
        							 0.25f, -0.5f, 1.0f,
        							 0.0f, 0.75f, 0.5f,
           	 					 	 0.5f, 0.0f, 0.0f, 1.0f );
        //lewa
        tabTrojkat[4] = new Trojkat(-0.25f, 0.0f, 0.0f,
        							-0.5f, -0.5f, 1.0f,
        							 0.0f, 0.75f, 0.5f,
           	 					 	 0.0f, 0.0f, 0.5f, 1.0f );
        //przód
        tabTrojkat[5] = new Trojkat(
        							-0.5f, -0.5f, 1.0f,
				 					 0.0f, 0.75f, 0.5f,
				 					 0.25f, -0.5f, 1.0f,
           	 					 	 0.0f, 0.5f, 0.0f, 1.0f );
    }

    public void onDrawFrame(GL10 unused)
    {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);		// ustaw kolor t³a

        Matrix.setLookAtM(mVMatrix, 0, 0, 0, 4, 0f, 0f, 0f, 0f, 1.0f, 0.0f);	// ustaw pozycjê kamery (View matrix)

        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mVMatrix, 0);		// obliczanie transformacji projekcji i widoku

        // Create a rotation for the triangle
//        long time = SystemClock.uptimeMillis() % 4000L;
//        float angle = 0.090f * ((int) time);
        Matrix.setRotateM(mRotationMatrix, 0, kat/2, 0, 0, 1.0f);

        // Combine the rotation matrix with the projection and camera view
        Matrix.multiplyMM(mMVPMatrix, 0, mRotationMatrix, 0, mMVPMatrix, 0);

        // Draw triangle
        for (int i =0; i < 5; i++)
        	tabTrojkat[i].draw(mMVPMatrix);
    }

    public void onSurfaceChanged(GL10 unused, int width, int height)
    {
        // Adjust the viewport based on geometry changes,
        // such as screen rotation
        GLES20.glViewport(0, 0, width, height);

        float ratio = (float) width / height;

        // this projection matrix is applied to object coordinates
        // in the onDrawFrame() method
        Matrix.frustumM(mProjMatrix, 0, -ratio, ratio, -1, 1, 3, 7);
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

