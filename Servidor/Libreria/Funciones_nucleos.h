void camara(void * parametres){
     String respuesta;
     Serial.print("Corriendo Hilo");
     Serial.println(xPortGetCoreID());

     WiFi cliente = *((WiFi*)parametres);

     respuesta = cliente.readStringUntil('\r');

     Serial.println(respuesta);

}