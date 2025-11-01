# 🌦 Climatic — ESP32 Real-Time Weather Display

Climatic is a compact IoT weather station built using *ESP32* and an *OLED 128x64 I2C screen*.  
It connects to Wi-Fi, requests weather data from *OpenWeatherMap*, reads the JSON response, and displays:

- Temperature 🌡  
- Humidity 💧  
- Wind Speed 🌬  
- Weather Icon ☁ / 🌧 / ☀  

---

## ✅ Hardware Used
| Component | Function |
|----------|----------|
| *ESP32 Dev Board* | Wi-Fi + main microcontroller |
| *OLED SSD1306 128x64 (I2C)* | Displays live weather data |
| *Jumper wires* | Simple wiring |

---

## 🔌 Wiring
| OLED Pin | ESP32 Pin |
|----------|-----------|
| VCC | 3.3V |
| GND | GND |
| SCL | GPIO 22 |
| SDA | GPIO 21 |

---

## 🌐 How Networking Works
- ESP32 connects to your Wi-Fi network
- Sends an HTTP request to OpenWeatherMap API
- API returns JSON data (temp, humidity, wind, weather description)
- The ESP32 decodes JSON and prints it on the OLED screen
✅ No server needed — direct device → internet → screen

---

## ▶ Setup Steps
1. Insert your Wi-Fi name/password at the top of the code  
2. Add your OpenWeatherMap API key  
3. Upload to ESP32  
4. The screen updates every 10 minutes

