// ***************************************************************************
// SPIFFS Webserver
// Source: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/examples/FSBrowser
// modified by Jeroen Klock 16-10-2019
// ***************************************************************************

#include <spiffs_webserver.h>

void Webserver::begin(void){
  DBG_OUTPUT_PORT.println("Setting up spiffs");
  if(!SPIFFS.begin())
  {
      DBG_OUTPUT_PORT.println("ERROR - Setting up spiffs failed");
  }
  else
  {
    Dir dir = SPIFFS.openDir("");

    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    DBG_OUTPUT_PORT.printf("FS Usage: %d/%d bytes\r\n", fs_info.usedBytes, fs_info.totalBytes);
  }
  ESP8266WebServer::begin();
};

// return filesize in correct format from byte
String Webserver::formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

// return content of file
String Webserver::getContentType(String filename) {
  DBG_OUTPUT_PORT.println("Getting content: " + filename);
  if (hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool Webserver::handleFileRead(String path) {
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    sendHeader("Access-Control-Allow-Origin", "*");
    size_t sent = streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void Webserver::handleFileUpload(void) {
  if (uri() != "/edit") return;
  HTTPUpload& upload = ESP8266WebServer::upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    DBG_OUTPUT_PORT.print("handleFileUpload Name: ");
    DBG_OUTPUT_PORT.println(filename);
    uploadfile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
  if (uploadfile)
    uploadfile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadfile)
      uploadfile.close();
    DBG_OUTPUT_PORT.print("handleFileUpload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}

void Webserver::handleFileDelete(void) {
  if (args() == 0) return send(500, "text/plain", "BAD ARGS");
  String path = arg(0);
  DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
  if (path == "/")
    return send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  send(200, "text/plain", "");
  path = String();
}

void Webserver::handleFileCreate(void) {
  if (args() == 0)
    return send(500, "text/plain", "BAD ARGS");
  String path = arg(0);
  DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
  if (path == "/")
    return send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return send(500, "text/plain", "CREATE FAILED");
  send(200, "text/plain", "");
  path = String();
}

void Webserver::handleFileList(void) {
  if (!hasArg("dir")) {
    send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = arg("dir");
  DBG_OUTPUT_PORT.println("handleFileList: " + path);
  Dir dir = SPIFFS.openDir(path);
  path = String();

  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }

  output += "]";
  sendHeader("Access-Control-Allow-Origin", "*");
  send(200, "text/json", output);
}

void Webserver::handleNotFound(void){
  if (!handleFileRead(uri())) {
    send(404, "text/plain", "FileNotFound");
  }
};
