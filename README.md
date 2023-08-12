# RFID-Based Access Control using NodeMCU

This project implements an RFID-based access control system using NodeMCU (ESP8266) microcontroller, allowing you to control access to secure areas using RFID cards/tags.


## Table of Contents

1. [Overview](#overview)
2. [Prerequisites](#prerequisites)
3. [Hardware Setup](#hardware-setup)
4. [Web Interface](#web-interface)
5. [NodeMCU - RC522 Connection](#nodemcu-rc522-connection)
6. [Usage](#usage)
8. [License](#license)

## Overview

This project aims to provide a cost-effective and customizable solution for access control using RFID technology. It combines the power of NodeMCU with the RC522 RFID module to securely manage access to a designated area.

![RFID Module](https://github.com/VedKathe/rfid-based-access-control-using-nodemcu/assets/96811754/86bdb841-7cc0-4239-add3-8210dc794127)


## Prerequisites

- Basic knowledge of Arduino programming.
- Familiarity with NodeMCU and the Arduino IDE.
- RC522 RFID module.
- Access to the Arduino IDE and a compatible USB cable for NodeMCU.

## Hardware Setup

Follow these steps to set up the hardware:

1. **Connections**: Connect the components as shown in the following Fritzing diagram:

![Fritzing Diagram](https://github.com/VedKathe/rfid-based-access-control-using-nodemcu/assets/96811754/8118a3b1-d631-45c0-a736-7915ca989fa5)

2. **Assembly**: Assemble the components into the access control system, ensuring proper connections and power supply.

## Web Interface

The web interface allows you to configure authorized RFID cards/tags and view access logs. Here's a preview:

![Web Interface](https://github.com/VedKathe/rfid-based-access-control-using-nodemcu/assets/96811754/de5b9c32-ddc3-42e8-a334-6f7c97efd5ce)

## NodeMCU - RC522 Connection

The table below shows the pin connections between NodeMCU and the RC522 RFID module:

| NodeMCU  | RFID-RC522 |
| ------------- | ------------- |
| D4  | SDA  |
| D5  | SCK  |
| D7  | MOSI  |
| D6  | MISO  |
| NOT CONN  | IRQ  |
| GND  | GND  |
| D3  | RST  |
| 3V | 3.3V  |

## Node.js API Setup

1. Clone the repository to your local machine.
2. Navigate to the "API" folder.
3. Install the required npm packages by running `npm install` in the "API" folder.
4. Edit the "index.js" file to configure your MySQL connection.
5. Create a MySQL database and two tables using the following queries:

```sql
CREATE DATABASE mydatabase;

USE mydatabase;

CREATE TABLE rifds (
  id varchar(100)
);

CREATE TABLE usedata (
  id varchar(100),
  datetime varchar(100)
);
```
Start the Node.js API by running ```node index.js``` in the "API" folder.
## Usage

1. Upload the Arduino code to the NodeMCU using the Arduino IDE.
2. Configure the system using the web-based interface as explained above.
3. Connect the RFID reader and electronic lock/gate to the NodeMCU as outlined.
4. Start managing access to your secure area!

## License

This project is licensed under the [MIT License](LICENSE).
