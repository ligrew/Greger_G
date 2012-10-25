package ligrew.P1;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

public class Scena extends Activity
{
    private GLSurfaceView widok;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        widok = new Powierzchnia(this);		// stwórz now¹ instancjê GLSurfaceView
        setContentView(widok);		// i ustaw go jako widok aplikacji
    }

    protected void onPause()
    {
        super.onPause();
        // The following call pauses the rendering thread.
        // If your OpenGL application is memory intensive,
        // you should consider de-allocating objects that
        // consume significant memory here.
        widok.onPause();
    }

    protected void onResume()
    {
        super.onResume();
        // The following call resumes a paused rendering thread.
        // If you de-allocated graphic objects for onPause()
        // this is a good place to re-allocate them.
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

        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);		// renderuj widok tylko wtedy, kiedy istnieje szansa narysowania danych rysowania
    }

    
    private final float TOUCH_SCALE_FACTOR = 180.0f / 320;
    private float poprzedniX;
    private float poprzedniY;

    public boolean onTouchEvent(MotionEvent e)
    {
        // MotionEvent reports input details from the touch screen
        // and other input controls. In this case, you are only
        // interested in events where the touch position changed.

        float x = e.getX();
        float y = e.getY();

        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:

                float dx = x - poprzedniX;
                float dy = y - poprzedniY;

                // reverse direction of rotation above the mid-line
                if (y > getHeight() / 2) {
                  dx = dx * -1 ;
                }

                // reverse direction of rotation to left of the mid-line
                if (x < getWidth() / 2) {
                  dy = dy * -1 ;
                }

                render.kat += (dx + dy) * TOUCH_SCALE_FACTOR;  // = 180.0f / 320
                requestRender();
        }

        poprzedniX = x;
        poprzedniY = y;
        return true;
    }
}
