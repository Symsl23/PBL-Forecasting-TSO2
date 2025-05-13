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

### 1. **Linear Regression**

#### 📌 Overview
Linear Regression is a simple method used to predict future values based on an underlying linear trend over time.

#### 📐 Formula
\[
y = mx + b
\]

- `m`: Slope (rate of change)
- `b`: Intercept
- `x`: Time index
- `y`: Forecasted value

#### 🧠 Why Selected
- **Trend detection**: Captures linear increase or decrease in sensor data over time.
- **Lightweight**: Easy to implement and fast to compute.
- **Best for**: Steady environments where values increase or decrease predictably.

#### 📉 Limitations
- Cannot model seasonality or repeating patterns.
- Assumes data follows a straight-line trend, which may not always hold.

---

### 2. **Holt-Winters Exponential Smoothing**

#### 📌 Overview
Holt-Winters (double exponential smoothing) accounts for both **level** and **trend** in the data, offering better adaptability for non-static data.

#### 🧮 Key Parameters
- `alpha`: Level smoothing factor
- `beta`: Trend smoothing factor

#### 🧠 Why Selected
- **Handles trend and fluctuation**: Smoother and more accurate for gradually changing sensor environments.
- **Flexible**: Adjusts dynamically with the incoming data.
- **Best for**: Periodic or gradually changing environments (e.g., temperature variation over a day).

#### 📉 Limitations
- Requires parameter tuning (`alpha`, `beta`)
- Does not account for complex seasonality (e.g., weekly or monthly cycles unless extended)

---

## 📊 Comparison Summary

| Feature                  | Linear Regression      | Holt-Winters              |
|--------------------------|------------------------|---------------------------|
| Trend detection          | ✅ Yes                 | ✅ Yes                    |
| Seasonality handling     | ❌ No                  | ⚠️ Partial (basic trend)  |
| Complexity               | ✅ Low                 | ⚠️ Medium                 |
| Apps Script compatibility| ✅ Easy to implement   | ✅ Easy to implement      |
| Accuracy                 | ⚠️ Moderate            | ✅ Higher for trends      |

---

## 🛠 Implementation in Google Apps Script

Both algorithms were implemented in `generateForecasts()`:

- `calculateLinearForecast()` – uses least squares method
- `holtWintersForecast()` – uses smoothing formulas with parameters

Forecast results are stored in a `Forecasts` sheet with:
- Point forecast
- Upper/Lower bounds (+/- 10% for uncertainty range)

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

