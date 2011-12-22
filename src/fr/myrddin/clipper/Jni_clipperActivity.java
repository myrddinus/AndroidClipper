package fr.myrddin.clipper;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class Jni_clipperActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        Clipper clipper = new Clipper();
        
        double[] sp = {325,272,118,272,118,98,325,98};
        double[] cp = {154,146,274,146,274,231};
        
        boolean s = clipper.addSubject(sp);
        boolean c = clipper.addClip(cp);
        
        double[][] polygons = clipper.execute(Clipper.DIFFERENCE);
        
        Log.e("","" + polygons.length + " " + c + " " + s);
        
        for (int i = 0; i < polygons.length; i++) {
        	double[] polygon = polygons[i];
        	for (int j = 0; j < polygon.length; j++) {
        		Log.e("", "" + polygon[j] );
            }
        	Log.e("", "----" );
        }

    }
}