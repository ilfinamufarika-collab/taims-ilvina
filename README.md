# T-AIMS IoT Heartbeat - Ilvina

Mekanisme heartbeat ESP32 dan Raspberry Pi untuk T-AIMS.

## Struktur Folder

- `esp32/` - Firmware ESP32 menggunakan PlatformIO.
- `simulator/` - Simulator ESP32 menggunakan Python.
- `raspi/` - Agent heartbeat Raspberry Pi menggunakan Python.
- `docs/` - Dokumentasi dan hasil pengujian.

## Cara Menjalankan Simulator

1. Install Mosquitto sebagai broker MQTT lokal.
2. Install library Python:

   ```bash
   pip install paho-mqtt
   ```

3. Jalankan simulator:

   ```bash
   python simulator/simulate_esp32.py
   ```

## Cara Upload ESP32

1. Buka folder `esp32/` di Visual Studio Code menggunakan PlatformIO.
2. Isi konfigurasi Wi-Fi dan MQTT pada `include/config.h`, jika file tersebut digunakan oleh program.
3. Hubungkan ESP32 ke komputer.
4. Klik **Upload** di PlatformIO.

## Dokumentasi Hardware

### 1. Heartbeat Raspberry Pi

- **PIC:** Ilvina Muvarika (IoT & Device Integration).
- **Lokasi kode sebenarnya:** `hardware-iot/raspi/heartbeat.py`.
- **Catatan:** Folder `hardware-iot/rpi_heartbeat/` kosong karena script heartbeat Raspberry Pi sudah tersedia di folder `raspi/`.
- **Solusi:** Folder `rpi_heartbeat/` dapat dihapus setelah dipastikan kosong dan tidak digunakan.

### 2. Simulator ESP32

- **PIC:** Ilvina Muvarika (IoT & Device Integration).
- **Lokasi kode sebenarnya:** `hardware-iot/simulator/simulate_esp32.py`.
- **Catatan:** Folder `hardware-iot/simulators/` kosong. Script simulasi menggunakan folder `simulator/` (tanpa huruf `s` di akhir).
- **Solusi:** Folder `simulators/` dapat dihapus setelah dipastikan kosong dan tidak digunakan.

### 3. Firmware ESP32

- **PIC:** Ilvina Muvarika (IoT & Device Integration).
- **Lokasi kode sebenarnya:** `hardware-iot/esp32/src/main.cpp`.
- **Catatan:** Folder `hardware-iot/esp32_firmware/` kosong karena kode sumber firmware ESP32 sudah tersedia di folder `esp32/src/`.
- **Solusi:** Folder `esp32_firmware/` dapat dihapus setelah dipastikan kosong dan tidak digunakan.

## Dokumentasi Pengujian

Lihat file berikut untuk informasi lebih lanjut:

- `docs/heartbeat.md`
- `docs/test-result.md`
