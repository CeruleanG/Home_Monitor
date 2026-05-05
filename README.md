# Home Monitor

## Motivation
Waking up with dry throat and discovering mold in your bathroom, is probably what most of us would describe as a "bad day". Recently I had one of those days and something has to be done. Besides ways to balance indoor condition, I wish to have a reliable way to monitor the air I am breathing in my apartment. Sadly, a lot off-the-shelf air sensors are either too expensive or don't allow data export. This is why I wish to build a custom IoT stack to have granular control over the telemetry and gain better insights over my living space for both my own health and its preventive maintenance.

## Project Overview
This project implements a Internet-Of-Things (IoT) stack to monitor environemental conditions such as tempereture, humidity and air pressure across multiple rooms within a house. This system would be degsigned for its data soverenty(local server hosting), scalability(MQTT protocol with scalable structure) and engergy efficiency analysis(live power consumption monitoring).

### Current Status

Hardware prototyping and Infrastructure setup.

## Project Structure
### Tech Stack
| Layers | Technologies |
| --- | ----------- |
| Sensors | BME280(Temperature/Humidity/Pressure), INA219(Current/Voltage) |
| Edge Nodes | ESP32 |
| Communication | MQTT, I2C |
| Processing Unit | Raspberry Pi 4, InfluxDB |
| Application | Grafana |

### System Architecture
The system follows a classic Publisher-Subscriber model:

The ESP32 microcontroller reads data, collected by sensors, via I2C protocol and publishes JSON payloads to specific MQTT topic over Wi-Fi. A Raspberry Pi 4, acts as the hub, runs containerized services via Docker in order to store and process data. The Hub would also host a web application for user interfacing.