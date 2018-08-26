package com.danim.iotalexa.Helpers;

public class Utils
{
    /**
     * Metodo que codifica una URL.
     * @param url: URL a codificar.
     * @return URL codificada.
     */
    public static String fixUrl(final String url)
    {
        return url.replaceAll(" ", "%20");
    }
}
