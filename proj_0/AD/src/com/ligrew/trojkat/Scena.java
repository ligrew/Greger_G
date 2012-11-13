package com.ligrew.trojkat;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.FloatMath;
import android.view.MotionEvent;

public class Scena extends Activity
{
    private GLSurfaceView widok;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        widok = new Powierzchnia(this);		// stwórz now¹ instancjê GLSurfaceView

        //widok.setEGLContextClientVersion(2);		// Za³ó¿ kontekst OpenGL ES 2.0

        //widok.setRenderer(new Render());		// ustaw stworzony render jako render rysowania
        setContentView(widok);		// i ustaw go jako widok aplikacji
    }

    protected void onPause()
    {
        super.onPause();
        widok.onPause();
    }

    protected void onResume()
    {
        super.onResume();
        widok.onResume();
    }
}

class Powierzchnia extends GLSurfaceView
{
    private final Render render;

    public Powierzchnia(Context context)
    {
        super(context);

        setEGLContextClientVersion(2);		// Za³ó¿ kontekst OpenGL ES 2.0

        render = new Render();
        setRenderer(render);		// ustaw stworzony render jako render rysowania

        //setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);		// renderuj widok tylko wtedy, kiedy istnieje szansa narysowania danych rysowania
    }

    
    private final float TOUCH_SCALE_FACTOR = 0.5f;//90.0f / 320;
    private float poprzedniX;
    private float poprzedniY;
    private float poprzedniZ;
    private boolean nacisk = true;


	public boolean onTouchEvent(MotionEvent e)
    {
        float wskaznik = e.getPointerCount();

        switch (e.getAction())
        {
	    	case MotionEvent.ACTION_UP:
	    		nacisk = true;
	    		break;
            case MotionEvent.ACTION_MOVE:

            	if (wskaznik == 1)
            	{
                    float x = e.getX();
                    float y = e.getY();

                    if (nacisk)
                    {
                    	poprzedniY = y;
                    	poprzedniX = x;
                    	nacisk = false;
                    }
                    
	                float dx = x - poprzedniX;
	                float dy = y - poprzedniY;
	                // reverse direction of rotation above the mid-line
	                if (y > getHeight() / 2)
	                  dx = dx * -1 ;
	
	                // reverse direction of rotation to left of the mid-line
	                if (x < getWidth() / 2)
		                  dy = dy * -1 ;
	
	                render.x += dx * TOUCH_SCALE_FACTOR;
	                render.y += dy * TOUCH_SCALE_FACTOR;

	                poprzedniX = x;
	                poprzedniY = y;
	                //render.katN += (dx + dy) * TOUCH_SCALE_FACTOR;  // = 180.0f / 320
            	}
            	else if (wskaznik == 2)
            	{
            		
            		
                    float x = e.getX(0) - e.getX(1);
                    float y = e.getY(0) - e.getY(1);
                    float z = FloatMath.sqrt(x * x + y * y);
                    
                    if (nacisk)
                    {
                    	poprzedniZ = z;
                    	nacisk = false;
                    }
                    
                    float dz = z - poprzedniZ;
                    
                    render.z -= dz * 0.02f;
                    
                    poprzedniZ = z;
            	}
            	
                requestRender();
        }

        return true;
    }
}
