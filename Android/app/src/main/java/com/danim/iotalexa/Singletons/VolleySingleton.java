package com.danim.iotalexa.Singletons;

import android.content.Context;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;
import com.danim.iotalexa.Constants.Constants;

/**
 * Class to create one Volley instance.
 * Created by Daniel Mancebo Aldea on 26/08/2018.
 */

public class VolleySingleton
{
    private static VolleySingleton mInstance;
    private static Context mContext;
    private RequestQueue mRequestQueue;

    private VolleySingleton(final Context context)
    {
        mContext = context;

        mRequestQueue = getRequestQueue();
    }

    /**
     * Metodo que devuelve una instancia usando un Singleton.
     * @param context: contexto.
     * @return instancia de Volley.
     */
    public static synchronized VolleySingleton getInstance(Context context)
    {
        if (mInstance == null)
        {
            mInstance = new VolleySingleton(context);
        }

        return mInstance;
    }

    /**
     * Metodo que devuelve la lista de peticiones.
     * @return lista de peticiones.
     */
    public RequestQueue getRequestQueue()
    {
        if (mRequestQueue == null)
        {
            // getApplicationContext() is key, it keeps you from leaking the
            // Activity or BroadcastReceiver if someone passes one in.
            mRequestQueue = Volley.newRequestQueue(mContext.getApplicationContext());
        }

        return mRequestQueue;
    }

    /**
     * Metodo que añade una peticion a la lista de peticiones.
     * @param req: peticion a enviar.
     * @param <T>: tipo de la peticion.
     */
    public <T> void addToRequestQueue(Request<T> req)
    {
        req.setRetryPolicy(new DefaultRetryPolicy(
                Constants.REQUEST_TIMEOUT,
                0,
                0));

        getRequestQueue().add(req);
    }
}
