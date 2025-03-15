# **VNC Client for ESP32-S3 with Touch Display**

This project implements a **VNC (Virtual Network Computing) client** on an **ESP32-S3** device with a **touch screen display**. It allows you to **view and interact** with a remote computer's desktop through the **VNC protocol**.

---

## 📌 **Hardware Requirements**

- **ESP32-S3 Development Board** (e.g., ESP32-S3-DevKitC-1)
- **Compatible Touch Display**
- **USB Cable** for programming

---

## 🛠 **Software Requirements**

- [**PlatformIO**](https://platformio.org/) with **VS Code** extension
- **Arduino framework** for ESP32  

---

## 📁 **Project Structure**

```plaintext
VNC-Client-for-maTouch/
├── .gitignore  
├── platformio.ini        # PlatformIO configuration  
├── include/              # Header files  
│   ├── touch.h           # Touch handling header  
│   ├── VNC_GFX.h         # VNC graphics adapter header  
│   ├── config.h          # Configuration settings  
├── lib/                  # Project-specific libraries  
├── src/  
│   ├── main.cpp          # Main application code  
│   ├── touch.cpp         # Touch handling implementation  
│   ├── VNC_GFX.cpp       # VNC graphics adapter implementation  
├── ui/                   # UI library files  
│   ├── ui.h              # UI header file  
│   ├── ui.cpp            # UI implementation file  
├── library.properties  
└── README.md             # Project documentation  
```

---

## 🚀 **Setup Instructions**

### **1️⃣ Install PlatformIO**

Install the **PlatformIO** extension in **Visual Studio Code** if you haven't already.

```bash
code --install-extension platformio.platformio-ide
```

### **2️⃣ Clone the Repository**

```bash
git clone https://github.com/yourusername/VNC-Client-for-maTouch.git
cd VNC-Client-for-maTouch
```

### **3️⃣ Install the UI Library**

The UI library is included in the repository. After the first build, manually copy the UI library to the .pio directory:

```bash
# First, build the project once to create the directories
pio run

# Then copy the UI library
cp -r ui .pio/libdeps/esp32-s3-devkitc-1/
```

> **Note**: The .pio directory is usually not pushed to repositories, so the UI library is included separately.

### **4️⃣ Configure WiFi and VNC Settings**

Edit the main.cpp file to set your WiFi and VNC server details:

```cpp
// WiFi credentials
#define SSID_NAME "your_wifi_ssid"
#define SSID_PASSWORD "your_wifi_password"

// VNC server details
#define VNC_IP "192.168.1.100"
#define VNC_PORT 5900
#define VNC_PASSWORD "your_vnc_password"
```

### **5️⃣ Build and Upload**

Connect your ESP32-S3 board via USB and upload the firmware:

```bash
pio run -t upload
```

---

## 📟 **Usage**

After uploading the firmware, the ESP32-S3 will:
1. Connect to the configured WiFi network
2. Attempt to connect to the VNC server
3. Display the remote desktop on the screen
4. Allow interaction via the touch screen

The display will show connection status:
- "Init WiFi" → During WiFi connection
- Error messages → If any issues occur
- Remote desktop → Once connected

---

## ✨ **Touch Functionality**

The touch screen allows you to:

✔️ Move the cursor on the remote desktop  
✔️ Click by tapping the screen  
✔️ Perform other mouse operations based on touch gestures  

---

## 🔍 **Troubleshooting**

### WiFi Connection Issues

✔️ Ensure your WiFi credentials are correct  
✔️ Make sure your ESP32-S3 is within range of the network  

### VNC Connection Issues

✔️ Verify your VNC server is running  
✔️ Check the IP address and port number  
✔️ Ensure the VNC password is correct  
✔️ Make sure the VNC server allows new client connections  

### Display Issues

✔️ Check the physical connections between ESP32-S3 and display  
✔️ Verify that the correct display driver is being used in the code  

---

## 📦 **Dependencies**

This project uses the following libraries:
- Arduino GFX Library
- TFT_eSPI
- arduinoVNC
- WiFi library for ESP32
- Touch controller libraries (e.g., TAMC_GT911)

---

## 📜 **License**

[Your License Information]

---