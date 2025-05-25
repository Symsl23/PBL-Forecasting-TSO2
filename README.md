# 📡PBL-Forecasting-TSO2

# 📈 Forecasting Algorithm Selection – IoT Sensor Data Project

This document explains the rationale behind selecting **Linear Regression** and **Holt-Winters** as the forecasting algorithms for our IoT Sensor Data Forecasting System, which uses an ESP32 microcontroller and Google Sheets + Apps Script for processing and visualization.

---

## 🔍 Project Context

The system collects time-series data from a DHT11 sensor (temperature and humidity) and aims to forecast future values to support early detection, pattern monitoring, and decision-making. The goal is to choose forecasting methods that:
- Handle real-time sensor data effectively
- Can be implemented in **Google Apps Script**
- Provide both **trend** and **seasonal pattern** insights

---

## ✅ Selected Forecasting Algorithms

📌 Overview
This project demonstrates the use of Simple Moving Average (SMA) to smooth out temperature and humidity data from a DHT11 sensor. The DHT11 is a basic, low-cost sensor used to measure temperature and humidity, but its readings may fluctuate due to minor environmental changes or sensor noise. SMA helps to filter out these short-term fluctuations and display a more stable reading.

---

🧮 Formula
The Simple Moving Average (SMA) of a data series is calculated by taking the arithmetic mean of the latest n data points.

𝑆
𝑀
𝐴
=
𝑋
1
+
𝑋
2
+
.
.
.
+
𝑋
𝑛
𝑛
SMA= 
n
X 
1
​
 +X 
2
​
 +...+X 
n
​
 
​
 
Where:

𝑋
1
,
𝑋
2
,
.
.
.
,
𝑋
𝑛
X 
1
​
 ,X 
2
​
 ,...,X 
n
​
  are the most recent sensor values

𝑛
n is the number of periods (e.g., last 5 readings)

---

❓ Why SMA is Selected
Simplicity: Easy to implement using arrays and basic math.

Effectiveness: Reduces short-term fluctuation (noise) in DHT11 readings.

Low Resource Use: Ideal for use in microcontrollers and cloud scripts like Google Apps Script.

---

👍 Benefits
Noise Reduction: Smoothens erratic sensor readings.

Real-Time Filtering: Provides more consistent data without delay.

Easy to Implement: Requires minimal code and computational power.

---

⚠️ Limitations
Lag: Introduces a slight delay in reflecting recent changes.

Fixed Window Size: A static window may not be ideal for all scenarios.

Not Ideal for Sudden Changes: SMA may under-represent rapid spikes or drops in temperature/humidity.

---

☁️ Google Apps Script Implementation
Google Apps Script is used to log DHT11 sensor data (from ESP32 or similar) into Google Sheets and apply SMA for smoothing.

function calculateSMA(values, period) {
  if (values.length < period) return null;
  
  let sum = 0;
  for (let i = values.length - period; i < values.length; i++) {
    sum += values[i];
  }
  return sum / period;
}

function logSensorData(temp, hum) {
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("SensorData");
  sheet.appendRow([new Date(), temp, hum]);

  const data = sheet.getRange("B2:B").getValues().flat().filter(Number);
  const sma = calculateSMA(data, 5); // Using last 5 readings

  if (sma !== null) {
    const row = sheet.getLastRow();
    sheet.getRange(row, 4).setValue(sma); // Column D for SMA
  }
}



---

## 📦 Future Plans
To improve forecasting:
- Add error metrics (MAE, RMSE, MAPE) to ensuring forecasting are reliable and justifiable
- Introduce ARIMA or another advanced algorithm for better accuracy and adaptibility
- Enable anomaly detection using forecast deviation to trigger alert if exceeds a threshold

---

## 📁 Related Files
- `main/Google App Script.txt` – App script implementation to generate data on Google Sheet
- `main/Latest_Data_Logger.ino` – Source time-series DHT11 data on Arduino IDE
- https://lookerstudio.google.com/reporting/539c465f-5813-4fad-b333-68e2e341b934 – Visualization of forecast data on Looker Studio

