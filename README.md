# Smart IoT Environment Monitor

A full-stack IoT project: a simulated ESP32 reads temperature, humidity, and light sensors,
streams the data over WiFi to a cloud backend, and a custom live dashboard displays it in
real time with interactive charts.

🔴 **Live demo:** https://karim44266.github.io/smart-iot-environment-monitor/
🔧 **Simulation:** [Run it live in Wokwi](https://wokwi.com/projects/470117791300470785)

## Architecture
[ESP32 + DHT22 + LDR] --WiFi/HTTP--> [Adafruit IO cloud] <--fetch-- [Live web dashboard]
(Wokwi simulation) (data storage) (HTML/JS/Chart.js)

## What it does

- Firmware (C++) reads temperature, humidity, and light level every 15 seconds
- Sends each reading to Adafruit IO via HTTP POST, authenticated with an API key
- A custom dashboard polls the cloud every 10 seconds and renders live gauges + a
  real-time multi-line chart
- Credentials are entered client-side at runtime — never hardcoded, safe to host publicly

## Tech stack

`C++` `ESP32` `WiFi/HTTP` `Adafruit IO` `JavaScript` `Chart.js` `HTML/CSS`

## Try it yourself

1. Open the [live dashboard](https://karim44266.github.io/smart-iot-environment-monitor/)
2. Create a free [Adafruit IO](https://io.adafruit.com) account and three feeds:
   `temperature`, `humidity`, `light`
3. Run the simulation in Wokwi with your own credentials in `firmware.ino`
4. Enter your Adafruit IO username + key in the dashboard and watch it go live

## Screenshot

<img width="1917" height="867" alt="image" src="https://github.com/user-attachments/assets/eece9c0c-218e-4260-b99d-a863887b393b" />
