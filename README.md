# IoT Task - End-to-End Sensor Monitoring System

A complete IoT system that demonstrates real-time sensor data collection, processing, and visualization. This project consists of three main components: an ESP32 sensor device (simulated in Wokwi), a NestJS backend API, and a Flutter mobile application.

## 📋 Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Components](#components)
- [Technologies Used](#technologies-used)
- [Project Structure](#project-structure)
- [Setup Instructions](#setup-instructions)
- [API Documentation](#api-documentation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Development](#development)

---

## 🎯 Overview

This project implements a complete IoT pipeline:

1. **ESP32 Device** (simulated in Wokwi) reads temperature and humidity from a DHT22 sensor
2. **NestJS Backend** receives and stores sensor readings via REST API
3. **Flutter Mobile App** displays real-time sensor data with automatic polling

The system demonstrates:
- Real-time sensor data collection
- RESTful API design with validation
- Mobile app development with state management
- Clean architecture patterns (Flutter)
- Dependency injection
- Error handling and user feedback

---

## 🏗 Architecture

```
┌─────────────────┐
│   ESP32 (Wokwi) │
│   DHT22 Sensor  │
└────────┬────────┘
         │
         │ HTTP POST /readings
         │ (every 3 seconds)
         ▼
┌─────────────────┐
│  NestJS Backend  │
│  Port: 3000     │
│  ┌───────────┐  │
│  │ In-Memory │  │
│  │  Storage  │  │
│  └───────────┘  │
└────────┬────────┘
         │
         │ HTTP GET /readings/latest
         │ (polled every 3 seconds)
         ▼
┌─────────────────┐
│  Flutter App    │
│  Mobile/Web     │
│  Real-time UI   │
└─────────────────┘
```

### Data Flow

1. **ESP32** reads DHT22 sensor (temperature & humidity) every 3 seconds
2. ESP32 sends POST request to NestJS backend with sensor data
3. **NestJS** validates and stores the reading in memory
4. **Flutter App** polls the backend every 3 seconds for the latest reading
5. Flutter app updates the UI with the latest sensor values

---

## 🧩 Components

### 1. ESP32 Firmware (Wokwi Simulator)

**Location:** `MicroPython MQTT Weather Logger (ESP32) Copy/main.py` and `wokwi/wokwi.ino`

- **Language:** MicroPython / Arduino C++
- **Sensor:** DHT22 (Temperature & Humidity)
- **Network:** WiFi connection to Wokwi-GUEST
- **Protocol:** HTTP POST requests
- **Frequency:** Sends data every 3 seconds

**Features:**
- Automatic WiFi connection
- DHT22 sensor reading
- HTTP POST to backend API
- Error handling and retry logic
- Serial monitor output for debugging

### 2. NestJS Backend

**Location:** `backend/`

- **Framework:** NestJS (Node.js)
- **Language:** TypeScript
- **Storage:** In-memory array (data persists until server restart)
- **Port:** 3000 (configurable via `PORT` env variable)
- **CORS:** Enabled for all origins (development mode)

**Features:**
- RESTful API endpoints
- Request validation using `class-validator`
- CORS support for mobile/web clients
- Global validation pipe
- Error handling

**Endpoints:**
- `POST /readings` - Create a new sensor reading
- `GET /readings/latest` - Get the most recent reading

### 3. Flutter Mobile Application

**Location:** `flutter_app/`

- **Framework:** Flutter
- **Language:** Dart
- **Architecture:** Clean Architecture with BLoC pattern
- **State Management:** flutter_bloc
- **Dependency Injection:** get_it
- **HTTP Client:** Dio

**Features:**
- Real-time sensor data display
- Automatic polling (every 3 seconds)
- Beautiful dark theme UI
- Error handling with retry functionality
- Loading states
- Responsive design

**UI Components:**
- Temperature card with icon
- Humidity card with icon
- Sensor type display
- Last updated timestamp
- Manual refresh button

---

## 🛠 Technologies Used

### Backend
- **NestJS** ^10.3.0 - Progressive Node.js framework
- **TypeScript** ^5.3.3 - Type-safe JavaScript
- **class-validator** ^0.14.0 - Validation decorators
- **class-transformer** ^0.5.1 - Object transformation

### Flutter App
- **Flutter** SDK ^3.10.4
- **flutter_bloc** ^8.1.6 - State management
- **dio** ^5.7.0 - HTTP client
- **get_it** ^7.7.0 - Dependency injection
- **equatable** ^2.0.7 - Value equality
- **google_fonts** ^6.2.1 - Custom fonts
- **flutter_animate** ^4.5.0 - Animations
- **intl** ^0.20.2 - Internationalization

### Hardware/Simulation
- **Wokwi** - ESP32 simulator
- **MicroPython** / **Arduino** - Embedded programming
- **DHT22** - Temperature and humidity sensor

---

## 📁 Project Structure

```
iot-task/
├── backend/                          # NestJS Backend
│   ├── src/
│   │   ├── main.ts                   # Application entry point
│   │   ├── app.module.ts             # Root module
│   │   └── readings/                 # Readings feature module
│   │       ├── dto/
│   │       │   └── create-reading.dto.ts    # Data Transfer Object
│   │       ├── entities/
│   │       │   └── reading.entity.ts        # Domain entity
│   │       ├── readings.controller.ts      # REST controller
│   │       ├── readings.service.ts         # Business logic
│   │       └── readings.module.ts          # Feature module
│   ├── package.json
│   ├── tsconfig.json
│   └── SETUP.md                      # Backend setup guide
│
├── flutter_app/                     # Flutter Mobile App
│   ├── lib/
│   │   ├── main.dart                 # App entry point
│   │   ├── injection_container.dart  # Dependency injection setup
│   │   ├── core/                     # Core utilities
│   │   │   ├── error/                # Error handling
│   │   │   ├── network/             # Network utilities
│   │   │   ├── theme/                # App theme
│   │   │   └── usecases/            # Base use case
│   │   └── features/
│   │       └── readings/             # Readings feature
│   │           ├── data/            # Data layer
│   │           │   ├── datasources/  # Remote data source
│   │           │   ├── models/     # Data models
│   │           │   └── repositories/ # Repository implementation
│   │           ├── domain/          # Domain layer
│   │           │   ├── entities/    # Domain entities
│   │           │   ├── repositories/ # Repository interfaces
│   │           │   └── usecases/    # Business logic
│   │           └── presentation/    # Presentation layer
│   │               ├── bloc/        # BLoC state management
│   │               ├── pages/       # UI pages
│   │               └── widgets/     # Reusable widgets
│   ├── pubspec.yaml
│   └── README.md
│
├── MicroPython MQTT Weather Logger (ESP32) Copy/
│   └── main.py                       # MicroPython ESP32 code
│
├── wokwi/
│   └── wokwi.ino                     # Arduino ESP32 code
│
├── DEMO_SCRIPT.md                    # Demo presentation script
└── README.md                         # This file
```

---

## 🚀 Setup Instructions

### Prerequisites

- **Node.js** (v18 or higher) and npm
- **Flutter** SDK (v3.10.4 or higher)
- **Wokwi Account** (free) - for ESP32 simulation
- **Cloudflare Tunnel** (optional) - for public URL exposure

### 1. Backend Setup

```bash
# Navigate to backend directory
cd backend

# Install dependencies
npm install

# Start development server
npm run start:dev
```

The backend will run on `http://localhost:3000`

**Available Scripts:**
- `npm run start:dev` - Start with hot reload
- `npm run build` - Build for production
- `npm run start:prod` - Run production build
- `npm run test` - Run tests
- `npm run lint` - Lint code

### 2. Expose Backend Publicly (for Wokwi)

Since Wokwi needs to access your local backend, you need to expose it publicly:

**Option A: Cloudflare Tunnel (Recommended)**

```bash
# Install cloudflared (macOS)
brew install cloudflare/cloudflare/cloudflared

# Start tunnel (in a new terminal)
cloudflared tunnel --url http://localhost:3000
```

Copy the generated HTTPS URL (e.g., `https://xyz-1234.trycloudflare.com`) and use it in:
- ESP32 code (update `API_URL`)
- Flutter app (update `_baseUrl` in `reading_remote_data_source.dart`)

**Option B: ngrok**

```bash
# Install ngrok
brew install ngrok

# Start tunnel
ngrok http 3000
```

Use the generated HTTPS URL.

### 3. ESP32 Setup (Wokwi)

1. Go to [Wokwi ESP32 Simulator](https://wokwi.com/projects/new/esp32)
2. Copy the code from `MicroPython MQTT Weather Logger (ESP32) Copy/main.py` or `wokwi/wokwi.ino`
3. Update the `API_URL` in the code with your public URL:
   ```python
   API_URL = "https://your-tunnel-url.trycloudflare.com/readings"
   ```
4. Start the simulation
5. Monitor the Serial Monitor for connection status and data transmission

### 4. Flutter App Setup

```bash
# Navigate to Flutter app directory
cd flutter_app

# Install dependencies
flutter pub get

# Update API URL (if needed)
# Edit: lib/features/readings/data/datasources/reading_remote_data_source.dart
# Update the _baseUrl constant

# Run the app
flutter run
```

**Platform-specific API URLs:**
- **Android Emulator:** `http://10.0.2.2:3000/readings/latest`
- **iOS Simulator / Web / macOS:** `http://localhost:3000/readings/latest`
- **Physical Device / Public URL:** `https://your-tunnel-url.trycloudflare.com/readings/latest`

---

## 📡 API Documentation

### Base URL

- **Local:** `http://localhost:3000`
- **Public:** `https://your-tunnel-url.trycloudflare.com`

### Endpoints

#### `POST /readings`

Create a new sensor reading.

**Request Body:**
```json
{
  "sensor": "dht22",
  "temp": 25.5,
  "humidity": 60.0
}
```

**Validation Rules:**
- `sensor`: Required, non-empty string
- `temp`: Required, number
- `humidity`: Required, number

**Response:** `201 Created`
```json
{
  "id": 1,
  "sensor": "dht22",
  "temp": 25.5,
  "humidity": 60.0,
  "createdAt": "2024-01-15T10:30:00.000Z"
}
```

**Example (cURL):**
```bash
curl -X POST http://localhost:3000/readings \
  -H "Content-Type: application/json" \
  -d '{
    "sensor": "dht22",
    "temp": 25.5,
    "humidity": 60.0
  }'
```

#### `GET /readings/latest`

Get the most recent sensor reading.

**Response:** `200 OK`
```json
{
  "id": 1,
  "sensor": "dht22",
  "temp": 25.5,
  "humidity": 60.0,
  "createdAt": "2024-01-15T10:30:00.000Z"
}
```

**If no readings exist:**
```json
{
  "message": "No readings available"
}
```

**Example (cURL):**
```bash
curl http://localhost:3000/readings/latest
```

### Error Responses

**400 Bad Request** - Validation errors:
```json
{
  "statusCode": 400,
  "message": [
    "sensor should not be empty",
    "temp must be a number"
  ],
  "error": "Bad Request"
}
```

---

## ⚙️ Configuration

### Backend Configuration

**Environment Variables:**
- `PORT` - Server port (default: 3000)

**CORS Settings:**
Currently configured for development (allows all origins). For production, update `main.ts`:
```typescript
app.enableCors({
  origin: ['https://yourdomain.com'], // Specific origins
  methods: ['GET', 'POST'],
  allowedHeaders: ['Content-Type'],
});
```

### Flutter App Configuration

**API Base URL:**
Edit `lib/features/readings/data/datasources/reading_remote_data_source.dart`:
```dart
static const String _baseUrl = 'YOUR_API_URL_HERE/readings/latest';
```

**Polling Interval:**
Edit `lib/features/readings/presentation/pages/dashboard_page.dart`:
```dart
Timer.periodic(const Duration(seconds: 3), (timer) { // Change 3 to desired seconds
```

### ESP32 Configuration

**WiFi Credentials:**
- SSID: `Wokwi-GUEST` (for Wokwi simulator)
- Password: (empty)

**API Endpoint:**
Update `API_URL` in the ESP32 code with your public backend URL.

---

## 💻 Usage

### Running the Complete System

1. **Start Backend:**
   ```bash
   cd backend
   npm run start:dev
   ```

2. **Expose Backend (if using Wokwi):**
   ```bash
   cloudflared tunnel --url http://localhost:3000
   ```

3. **Update ESP32 Code:**
   - Copy the public URL from step 2
   - Update `API_URL` in ESP32 code
   - Start Wokwi simulation

4. **Update Flutter App:**
   - Update `_baseUrl` in `reading_remote_data_source.dart`
   - Run Flutter app: `flutter run`

5. **Monitor:**
   - Backend logs: Check terminal running NestJS
   - ESP32 logs: Check Wokwi Serial Monitor
   - Flutter app: Watch the UI update every 3 seconds

### Testing the API

**Test POST endpoint:**
```bash
curl -X POST http://localhost:3000/readings \
  -H "Content-Type: application/json" \
  -d '{"sensor":"dht22","temp":25.5,"humidity":60.0}'
```

**Test GET endpoint:**
```bash
curl http://localhost:3000/readings/latest
```

---

## 🔧 Development

### Backend Development

**Project Structure:**
- Follows NestJS module-based architecture
- Uses DTOs for request validation
- In-memory storage (can be replaced with database)

**Adding a Database:**
To persist data, you can add TypeORM with SQLite:
```bash
npm install @nestjs/typeorm typeorm sqlite3
```

**Code Style:**
- Uses ESLint and Prettier
- Run `npm run lint` to check code style

### Flutter Development

**Architecture:**
- Clean Architecture with three layers:
  - **Data Layer:** Data sources, models, repository implementations
  - **Domain Layer:** Entities, use cases, repository interfaces
  - **Presentation Layer:** BLoC, pages, widgets

**State Management:**
- Uses BLoC pattern for state management
- Events trigger state changes
- States: `Initial`, `Loading`, `Loaded`, `Error`

**Dependency Injection:**
- Uses `get_it` for service locator pattern
- All dependencies registered in `injection_container.dart`

---

## 📝 Notes

### Current Limitations

1. **In-Memory Storage:** Data is lost on server restart
2. **No Authentication:** API is publicly accessible
3. **No Database:** Using in-memory array storage
4. **Single Sensor:** Currently supports one sensor type (DHT22)

### Future Enhancements

- [ ] Add database persistence (PostgreSQL, MongoDB, or SQLite)
- [ ] Implement authentication and authorization
- [ ] Add multiple sensor support
- [ ] Implement WebSocket for real-time updates (instead of polling)
- [ ] Add data visualization (charts, graphs)
- [ ] Implement data filtering and history
- [ ] Add push notifications for threshold alerts
- [ ] Support multiple devices/sensors
- [ ] Add unit and integration tests
- [ ] Docker containerization
- [ ] CI/CD pipeline

---

## 🐛 Troubleshooting

### Backend Issues

**Port already in use:**
```bash
# Change port in main.ts or use environment variable
PORT=3001 npm run start:dev
```

**CORS errors:**
- Ensure CORS is enabled in `main.ts`
- Check that the Flutter app is using the correct base URL

### Flutter Issues

**API connection errors:**
- Verify backend is running
- Check API URL in `reading_remote_data_source.dart`
- For Android emulator, use `10.0.2.2:3000` instead of `localhost`
- For physical devices, use the public tunnel URL

**Build errors:**
```bash
flutter clean
flutter pub get
flutter run
```

### ESP32/Wokwi Issues

**Connection timeout:**
- Verify backend is accessible via public URL
- Check that Cloudflare tunnel is running
- Ensure API URL in ESP32 code is correct

**WiFi connection:**
- Wokwi-GUEST network is automatically available in Wokwi simulator
- No password required

---

## 📄 License

MIT License - See LICENSE file for details

---

## 👤 Author

IoT Task Project

---

## 🙏 Acknowledgments

- NestJS team for the excellent framework
- Flutter team for the cross-platform framework
- Wokwi for the ESP32 simulator
- Cloudflare for the free tunnel service

---

## 📞 Support

For issues and questions:
1. Check the troubleshooting section
2. Review the code comments
3. Check the `SETUP.md` and `DEMO_SCRIPT.md` files

---

**Happy Coding! 🚀**
