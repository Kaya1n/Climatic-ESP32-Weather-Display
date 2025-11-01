#include WiFi.h
#include HTTPClient.h
#include ArduinoJson.h
#include U8g2lib.h

 ----------------------------------------------------------------------
 🔧 User Configuration
 ----------------------------------------------------------------------
 Enter your Wi-Fi credentials and OpenWeatherMap API information here
const char WIFI_SSID = ENTER_WIFI_NAME;
const char WIFI_PASSWORD = ENTER_WIFI_PASSWORD;
const String API_KEY = ENTER_YOUR_API_KEY;
const String CITY_ID = 107304;     Example Riyadh city ID
const String UNITS = metric;       metric for °C, imperial for °F

 ----------------------------------------------------------------------
 🖥 OLED Display Initialization (I2C)
 ESP32 default I2C pins SCL = 22, SDA = 21
 ----------------------------------------------------------------------
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 22, 21);

 ----------------------------------------------------------------------
 🌤 Draw Weather Icon (small size for better layout)
 Icons from u8g2_font_open_iconic_weather_2x_t
 ----------------------------------------------------------------------
void drawWeatherIcon(String description) {
    u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);

    if (description.indexOf(clear) != -1) {
        u8g2.drawGlyph(105, 18, 0x20);  sunny
    } 
    else if (description.indexOf(rain) != -1  description.indexOf(drizzle) != -1) {
        u8g2.drawGlyph(105, 18, 0x21);  rainy
    }
    else if (description.indexOf(cloud) != -1  description.indexOf(mist) != -1  description.indexOf(overcast) != -1) {
        u8g2.drawGlyph(105, 18, 0x22);  cloudy
    }
    else {
        u8g2.drawGlyph(105, 18, 0x23);  unknown
    }

     return to normal text font
    u8g2.setFont(u8g2_font_7x13_tr);
}

 ----------------------------------------------------------------------
 📟 Render Weather Data on Screen
 Displays City, Status, Temperature, Humidity, Wind
 ----------------------------------------------------------------------
void renderWeatherDisplay(String cityName, String status, float temperature, float humidity, float windSpeed) {

    u8g2.clearBuffer(); 

     City name and weather status
    u8g2.setFont(u8g2_font_7x13_tr);
    u8g2.drawStr(0, 12, (City  + cityName).substring(0, 20).c_str());
    u8g2.drawStr(0, 25, (Status  + status).substring(0, 20).c_str());

     Temperature (large text)
    u8g2.setFont(u8g2_font_8x13_tr);
    String tempStr = String(temperature, 1) +  C;
    u8g2.drawStr(0, 60, tempStr.c_str());

     Humidity and wind (small text on right)
    u8g2.setFont(u8g2_font_6x12_tr);
    String humidityStr = H  + String(humidity, 0) + %;
    u8g2.drawStr(80, 45, humidityStr.c_str());

    String windStr = W  + String(windSpeed, 1) +  ms;
    u8g2.drawStr(80, 60, windStr.c_str());

     Weather icon
    drawWeatherIcon(status);

    u8g2.sendBuffer();
}

 ----------------------------------------------------------------------
 🔌 Prepare OLED and show boot message
 ----------------------------------------------------------------------
void initializeOLED() {
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x13_tr);
    u8g2.drawStr(0, 30, Connecting to Wi-Fi...);
    u8g2.sendBuffer();
}

 ----------------------------------------------------------------------
 📶 Connect ESP32 to Wi-Fi
 ----------------------------------------------------------------------
void connectToWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

 ----------------------------------------------------------------------
 🌐 Fetch Weather Data from OpenWeatherMap API
 ----------------------------------------------------------------------
void updateWeatherInformation() {
    if (WiFi.status() == WL_CONNECTED) {

        HTTPClient http;
        String url = httpapi.openweathermap.orgdata2.5weatherid= 
                     + CITY_ID + &units= + UNITS + &appid= + API_KEY;

        http.begin(url);

        if (http.GET() == HTTP_CODE_OK) {

            String payload = http.getString();
            DynamicJsonDocument doc(512);
            deserializeJson(doc, payload);

            String cityName = doc[name].asString();
            String description = doc[weather][0][description].asString();
            float temperature = doc[main][temp];
            float humidity = doc[main][humidity];
            float windSpeed = doc[wind][speed];

            renderWeatherDisplay(cityName, description, temperature, humidity, windSpeed);
        }

        http.end();
    }
}

 ----------------------------------------------------------------------
 ✅ Setup Run once
 ----------------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    initializeOLED();
    connectToWiFi();
}

 ----------------------------------------------------------------------
 🔁 Loop Refresh weather every 10 minutes
 ----------------------------------------------------------------------
void loop() {
    updateWeatherInformation();
    delay(600000);  600000 ms = 10 minutes
}