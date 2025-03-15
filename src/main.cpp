#include <Arduino_GFX_Library.h>
#include <ui.h>
#include <WiFi.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(RTL8722DM)
#include <WiFi.h>
#endif

#include "VNC_GFX.h"
#include <VNC.h>

// Add color definitions
#define RGB565_BLACK 0x0000
#define RGB565_RED   0xF800
#define RGB565_GREEN 0x07E0
#define RGB565_BLUE  0x001F
#define RGB565_WHITE 0xFFFF


const char *SSID_NAME = "Sharad-4G";
const char *SSID_PASSWORD = "VFGoudar";
const char *VNC_IP = "192.168.29.82";
const uint16_t VNC_PORT = 5905;
const char *VNC_PASSWORD = "123456789";

#define TFT_BL 10

#define SCREEN_HD
#define SCREEN_W 1024
#define SCREEN_H 600

#include "touch.h"

/*Don't forget to set Sketchbook location in File/Preferencesto the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/
static const uint16_t screenWidth = 1024;
static const uint16_t screenHeight = 600;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL with actual backlight pin

#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);

Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus,
    SCREEN_W /* width */, 1 /* hsync_polarity */, 40 /* hsync_front_porch */, 48 /* hsync_pulse_width */, 128 /* hsync_back_porch */,
    SCREEN_H /* height */, 1 /* vsync_polarity */, 13 /* vsync_front_porch */, 3 /* vsync_pulse_width */, 45 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */, true /* auto_flush */);

#endif /* !defined(DISPLAY_DEV_KIT) */


 VNC_GFX *vnc_gfx = new VNC_GFX(gfx);
arduinoVNC vnc = arduinoVNC(vnc_gfx);


void TFTnoWifi(void)
{
  gfx->fillScreen(RGB565_BLACK);
  gfx->setCursor(0, ((gfx->height() / 2) - (5 * 8)));
  gfx->setTextColor(RGB565_RED);
  gfx->setTextSize(5);
  gfx->println("NO WIFI!");
  gfx->setTextSize(2);
  gfx->println();
}

void TFTnoVNC(void)
{
  gfx->fillScreen(RGB565_BLACK);
  gfx->setCursor(0, ((gfx->height() / 2) - (4 * 8)));
  gfx->setTextColor(RGB565_GREEN);
  gfx->setTextSize(4);
  gfx->println("connect VNC");
  gfx->setTextSize(2);
  gfx->println();
  gfx->print(VNC_IP);
  gfx->print(":");
  gfx->println(VNC_PORT);
}


void handle_touch()
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      // Send press event when finger first touches screen
      vnc.mouseEvent(touch_last_x, touch_last_y, 0b001);
    }
    else if (touch_released())
    {
      // Send release event when finger is lifted
      vnc.mouseEvent(touch_last_x, touch_last_y, 0b000);
    }
  }
}


void setup(void)
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX VNC example");

  // Init keyboard device

  Serial.println("Init display");
  // Simply initialize without checking return
  gfx->begin();
  gfx->fillScreen(RGB565_BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  // Init touch device with only the required parameters
  touch_init(gfx->width(), gfx->height());

  TFTnoWifi();

  Serial.println("Init WiFi");
  gfx->println("Init WiFi");
#if defined(ESP32)
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
#elif defined(ESP8266)
  // disable sleep mode for better data rate
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
#elif defined(RTL8722DM)
  WiFi.begin((char *)SSID_NAME, (char *)SSID_PASSWORD);
#endif
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    gfx->print(".");
  }
  Serial.println(" CONNECTED");
  gfx->println(" CONNECTED");
  Serial.println("IP address: ");
  gfx->println("IP address: ");
  Serial.println(WiFi.localIP());
  gfx->println(WiFi.localIP());
  TFTnoVNC();

  Serial.println(F("[SETUP] VNC..."));

#ifdef SEPARATE_DRAW_TASK
  draw_task_setup();
#endif

  vnc.begin(VNC_IP, VNC_PORT);
  vnc.setPassword(VNC_PASSWORD); // optional
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    vnc.reconnect();
    TFTnoWifi();
    delay(100);
  }
  else
  {
    if (vnc.connected())
    {
      handle_touch();
    }
    vnc.loop();
    if (!vnc.connected())
    {
      TFTnoVNC();
      // some delay to not flood the server
      delay(5000);
    }
  }
}
