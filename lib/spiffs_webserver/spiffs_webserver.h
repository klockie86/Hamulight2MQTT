// ***************************************************************************
// SPIFFS Webserver
// Source: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/examples/FSBrowser
// modified by Jeroen Klock 16-10-2019
// ***************************************************************************
#ifndef SPIFFS_WEBSERVER_H
#define SPIFFS_WEBSERVER_H
// webserver
#define WEBSERVER_PORT 80

#include <Arduino.h>
#include <global.h>

#include <ESP8266WebServer.h>
// library for SPIFFS
#include <FS.h>


////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

class Webserver: public ESP8266WebServer{
private:
  String formatBytes(size_t);     // calculate filesize in byte
  String getContentType(String);  // return content type from file
  bool handleFileRead(String);    // read file for webserver
  File uploadfile;
public:
  Webserver(int port):ESP8266WebServer(port){};
  // predefined functions
  void begin(void);
  void handleFileUpload(void);
  void handleFileDelete(void);
  void handleFileCreate(void);
  void handleFileList(void);
  void handleNotFound(void);
};
////////////////////////////////////////////////////////////////////////////////
#endif
