#include <Adafruit_NeoPixel.h>
#include <LittleFS.h>
#include <SD.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "LGFX_Display.hpp" // Display settings are here
#include "secret.h"         // Contains ssid and password

LGFX display;

// Wi-Fi credentials
const char *ssid = SSID;
const char *password = PASSWORD;

// Create an AsyncWebServer on port 80
AsyncWebServer server(80);
volatile bool doDraw = false;

// LED on ESP32-S3-DevKitC-1-N8R8
#define LED_PIN 38   // GPIO38 for RGB DIN
#define NUM_PIXELS 1 // Only one RGB LED onboard
Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Frame settings
struct
{
  uint8_t rotation = 0;
  bool highQuality = true;
  bool clearOut = true;
  String filename = "/test.jpg";
} settings;

void setup()
{
  display.init();
  display.setRotation(0);

  // Verbose output to display itself
  display.fillScreen(TFT_WHITE);
  display.setCursor(200, 200);
  display.setFont(&fonts::Font4);
  display.setTextSize(2);

  if (!LittleFS.begin())
  {
    display.println("LittleFS mount failed!");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
  }
  // display.printf("Connected to WiFi %s\n", WiFi.localIP().toString().c_str());

  server.on(
      "/draw", HTTP_GET, [](AsyncWebServerRequest *request)
      {
              doDraw = true;
              request->send(200, "text/plain", "Drawing scheduled."); });

  server.on(
      "/upload", HTTP_POST,
      // onRequest: called after all parts are received
      [](AsyncWebServerRequest *req)
      {
        // Parse JSON part
        if (auto *p = req->getParam("config", true))
        {
          JsonDocument doc;
          auto err = deserializeJson(doc, p->value());
          if (!err)
          {
            settings.clearOut = doc["clearout"] | settings.clearOut;
            settings.rotation = doc["rotation"] | settings.rotation;
            settings.highQuality = doc["quality"] | settings.highQuality;
          }
        }
        req->send(200, "application/json", "{\"status\":\"ok\"}");
      },
      // onUpload: called once per chunk
      [](AsyncWebServerRequest *req,
         String filename,
         size_t index,
         uint8_t *data,
         size_t len,
         bool final)
      {
        static File f;
        if (index == 0)
        {
          // first chunk: open file for writing (overwrite)
          f = LittleFS.open(settings.filename, "w");
        }
        if (f)
        {
          pixels.setPixelColor(0, pixels.Color(255, 0, 0));
          pixels.show();

          f.write(data, len);
          if (final)
          {
            f.close();
            doDraw = true; // Schedule a draw after upload
          }
        }
      });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.begin();

  display.printf("Server started at http://%s\n", WiFi.localIP().toString().c_str());
}

void draw()
{
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();

  display.setRotation(settings.rotation);
  display.setEpdMode(settings.highQuality ? epd_quality : epd_fast);

  display.setCursor(0, 0);
  display.startWrite();
  display.drawJpgFile(LittleFS, "/white.jpg", 0, 0);
  display.drawJpgFile(LittleFS, settings.filename, 0, 0);
  display.display();
  display.endWrite();

  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

void loop(void)
{
  if (doDraw)
  {
    doDraw = false;
    draw();
  }
}
