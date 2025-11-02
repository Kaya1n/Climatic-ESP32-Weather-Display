#include WiFi.h
#include HTTPClient.h
#include ArduinoJson.h
#include U8g2lib.h

// ----------------------------------------------------------------------
// 🔧 User Configuration
// ----------------------------------------------------------------------
 Enter your Wi-Fi credentials and OpenWeatherMap API information here
const char WIFI_SSID = ENTER_WIFI_NAME;
const char WIFI_PASSWORD = ENTER_WIFI_PASSWORD;
const String API_KEY = ENTER_YOUR_API_KEY;
const String CITY_ID = 107304; // Example Riyadh city ID
const String UNITS = metric;   // metric for °C, imperial for °F

// ----------------------------------------------------------------------
// 💻 OLED Display (128x64) using I2C on ESP32
// SCL = GPIO 22, SDA = GPIO 21
// ----------------------------------------------------------------------
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 22, 21);

// ----------------------------------------------------------------------
// ☁ Draw a weather icon that fits the display
// ----------------------------------------------------------------------
void drawWeatherIcon(const String& status) {
  u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);

  uint16_t icon = 0x40; // Cloud as default 

  if (status.indexOf("clear") >= 0)       icon = 0x45;  // Sun ✅
  else if (status.indexOf("cloud") >= 0)  icon = 0x40;  // Cloud ✅
  else if (status.indexOf("rain") >= 0)   icon = 0x43;  // Rain ✅
  else if (status.indexOf("snow") >= 0)   icon = 0x44;  // Snow ✅
  else if (status.indexOf("storm") >= 0)  icon = 0x43;  // Thunder ✅
  else if (status.indexOf("wind") >= 0)   icon = 0x41;  // Wind ✅

  u8g2.drawGlyph(105, 32, icon);
}

// ----------------------------------------------------------------------
// 🖥 Render weather data on OLED with clean layout
// ----------------------------------------------------------------------
void renderWeatherDisplay(const String& cityName, const String& status,
                          float temperature, float humidity, float windSpeed) {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_7x13_tr);
  u8g2.drawStr(0, 12, ("City: " + cityName).substring(0, 18).c_str());

  String shortStatus = status;
  if (shortStatus.indexOf("clear") >= 0) shortStatus = "Clear";
  else if (shortStatus.indexOf("cloud") >= 0) shortStatus = "Cloudy";
  else if (shortStatus.indexOf("rain") >= 0) shortStatus = "Rain";
  else if (shortStatus.indexOf("snow") >= 0) shortStatus = "Snow";
  else if (shortStatus.indexOf("storm") >= 0) shortStatus = "Thunders";
  else if (shortStatus.indexOf("wind") >= 0) shortStatus = "Windy";
  u8g2.drawStr(0, 26, ("Status: " + shortStatus).c_str());

  u8g2.setFont(u8g2_font_8x13_tr);
  u8g2.drawStr(0, 45, (String(temperature, 1) + " C").c_str());

  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(0, 60, ("H: " + String((int)humidity) + "%").c_str());
  u8g2.drawStr(60, 60, ("W: " + String(windSpeed, 1) + " m/s").c_str());


  drawWeatherIcon(status);

  u8g2.sendBuffer();
}

// ----------------------------------------------------------------------
// 🔌 Display startup message
// ----------------------------------------------------------------------
void initializeOLED() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x13_tr);
  u8g2.drawStr(0, 30, "Connecting...");
  u8g2.sendBuffer();
}

// ----------------------------------------------------------------------
// 🌐 Connect to WiFi
// ----------------------------------------------------------------------
void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
}

// ----------------------------------------------------------------------
// 🌦 Fetch and parse JSON weather data
// ----------------------------------------------------------------------
void updateWeatherInformation() {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?id=" + CITY_ID +
                 "&units=" + UNITS + "&appid=" + API_KEY;

    http.begin(url);

    if (http.GET() == HTTP_CODE_OK) {
      String payload = http.getString();

      DynamicJsonDocument doc(512);
      deserializeJson(doc, payload);

      String cityName = doc["name"].as<String>();
      String description = doc["weather"][0]["description"].as<String>();
      float temperature = doc["main"]["temp"];
      float humidity = doc["main"]["humidity"];
      float windSpeed = doc["wind"]["speed"];

      renderWeatherDisplay(cityName, description, temperature, humidity, windSpeed);
    }

    http.end();
  }
}

// ----------------------------------------------------------------------
// ⚙ Setup & Loop
// ----------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  initializeOLED();
  connectToWiFi();
}

void loop() {
  updateWeatherInformation();
  delay(600000); // every 10 minutes
}
