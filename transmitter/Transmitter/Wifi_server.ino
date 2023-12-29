void handleRoot() {
  String slongitude;
  String slatitude;
  slongitude=String(longitude,6);
  slatitude=String(latitude,6);
  
  server.send(200, "text/html",String("<a href=")+String('"')+String("https://www.google.com/maps/search/?api=1&query=")+slatitude+String(',')+slongitude+String('"')+String('>')+String("Map")+String("</a>"));
}
