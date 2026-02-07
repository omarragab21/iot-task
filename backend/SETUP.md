# NestJS Backend Setup Guide

## Section 1: Terminal Commands

### Install NestJS CLI globally
```bash
npm install -g @nestjs/cli
```

### Create a new NestJS project (if starting fresh)
```bash
nest new backend
cd backend
```

### Generate readings resource/module
```bash
nest generate resource readings
# When prompted:
# - What transport layer do you use? REST API
# - Would you like to generate CRUD entry points? No (we'll create custom endpoints)
```

**Note:** Since we've already created the files manually, you can skip the generation step if following this guide.

### Install dependencies
```bash
cd backend
npm install
```

### Start the development server
```bash
npm run start:dev
```

The server will run on `http://localhost:3000`

---

## Section 2: DTO Code

**File:** `src/readings/dto/create-reading.dto.ts`

```typescript
import { IsString, IsNumber, IsNotEmpty } from 'class-validator';

export class CreateReadingDto {
  @IsString()
  @IsNotEmpty()
  sensor: string;

  @IsNumber()
  temp: number;

  @IsNumber()
  humidity: number;
}
```

**Validation Rules:**
- `sensor`: Must be a non-empty string
- `temp`: Must be a number
- `humidity`: Must be a number

---

## Section 3: Controller Code

**File:** `src/readings/readings.controller.ts`

```typescript
import {
  Controller,
  Get,
  Post,
  Body,
  HttpCode,
  HttpStatus,
} from '@nestjs/common';
import { ReadingsService } from './readings.service';
import { CreateReadingDto } from './dto/create-reading.dto';

@Controller('readings')
export class ReadingsController {
  constructor(private readonly readingsService: ReadingsService) {}

  @Post()
  @HttpCode(HttpStatus.CREATED)
  create(@Body() createReadingDto: CreateReadingDto) {
    return this.readingsService.create(createReadingDto);
  }

  @Get('latest')
  findLatest() {
    const latest = this.readingsService.findLatest();
    if (!latest) {
      return { message: 'No readings available' };
    }
    return latest;
  }
}
```

**Endpoints:**
- `POST /readings` - Creates a new reading (returns 201 Created)
- `GET /readings/latest` - Returns the latest reading or a message if none exists

---

## Section 4: Service Code

**File:** `src/readings/readings.service.ts`

```typescript
import { Injectable } from '@nestjs/common';
import { CreateReadingDto } from './dto/create-reading.dto';
import { Reading } from './entities/reading.entity';

@Injectable()
export class ReadingsService {
  private readings: Reading[] = [];
  private nextId = 1;

  create(createReadingDto: CreateReadingDto): Reading {
    const reading = new Reading(
      createReadingDto.sensor,
      createReadingDto.temp,
      createReadingDto.humidity,
    );
    reading.id = this.nextId++;
    this.readings.push(reading);
    return reading;
  }

  findLatest(): Reading | null {
    if (this.readings.length === 0) {
      return null;
    }
    return this.readings[this.readings.length - 1];
  }

  findAll(): Reading[] {
    return this.readings;
  }
}
```

**Storage:** In-memory array (data is lost on server restart)

---

## Section 5: main.ts Changes

**File:** `src/main.ts`

```typescript
import { NestFactory } from '@nestjs/core';
import { ValidationPipe } from '@nestjs/common';
import { AppModule } from './app.module';

async function bootstrap() {
  const app = await NestFactory.create(AppModule);

  // Enable CORS for Flutter/mobile apps
  app.enableCors({
    origin: true,
    methods: ['GET', 'POST', 'PUT', 'DELETE', 'PATCH', 'OPTIONS'],
    allowedHeaders: ['Content-Type', 'Authorization'],
    credentials: true,
  });

  // Enable validation pipe globally
  app.useGlobalPipes(
    new ValidationPipe({
      whitelist: true,
      forbidNonWhitelisted: true,
      transform: true,
    }),
  );

  const port = process.env.PORT || 3000;
  await app.listen(port);
  console.log(`🚀 NestJS server running on http://localhost:${port}`);
}
bootstrap();
```

**Key Features:**
- CORS enabled for all origins (suitable for development)
- Global validation pipe with class-validator
- Runs on port 3000 by default (configurable via PORT env variable)

---

## Section 6: Test Commands

### Test POST /readings

```bash
curl -X POST http://localhost:3000/readings \
  -H "Content-Type: application/json" \
  -d '{
    "sensor": "dht22",
    "temp": 25.5,
    "humidity": 60.0
  }'
```

**Expected Response (201 Created):**
```json
{
  "id": 1,
  "sensor": "dht22",
  "temp": 25.5,
  "humidity": 60.0,
  "createdAt": "2024-01-15T10:30:00.000Z"
}
```

### Test GET /readings/latest

```bash
curl http://localhost:3000/readings/latest
```

**Expected Response (200 OK):**
```json
{
  "id": 1,
  "sensor": "dht22",
  "temp": 25.5,
  "humidity": 60.0,
  "createdAt": "2024-01-15T10:30:00.000Z"
}
```

### Test Validation (Invalid Request)

```bash
curl -X POST http://localhost:3000/readings \
  -H "Content-Type: application/json" \
  -d '{
    "sensor": "",
    "temp": "not-a-number",
    "humidity": 60.0
  }'
```

**Expected Response (400 Bad Request):**
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

### Test GET /readings/latest (No Data)

```bash
# First, restart the server to clear in-memory data, then:
curl http://localhost:3000/readings/latest
```

**Expected Response (200 OK):**
```json
{
  "message": "No readings available"
}
```

---

## Section 7: Public URL Setup (Cloudflare Tunnel)

To expose your local NestJS server publicly so Wokwi can access it via HTTPS:

### Step 1: Install Cloudflare Tunnel (cloudflared)

**On macOS:**
```bash
# Using Homebrew
brew install cloudflare/cloudflare/cloudflared

# Or download directly from:
# https://developers.cloudflare.com/cloudflare-one/connections/connect-apps/install-and-setup/installation/
```

### Step 2: Start the NestJS Server

```bash
cd backend
npm run start:dev
```

Ensure the server is running on `http://localhost:3000`

### Step 3: Create Public Tunnel

In a **new terminal window**, run:

```bash
cloudflared tunnel --url http://localhost:3000
```

**Output Example:**
```
+--------------------------------------------------------------------------------------------+
|  Your quick Tunnel has been created! Visit it at (it may take some time to be reachable): |
|  https://random-words-1234.trycloudflare.com                                               |
|                                                                                            |
+--------------------------------------------------------------------------------------------+
```

### Step 4: Use the Public URL

Copy the HTTPS URL provided (e.g., `https://random-words-1234.trycloudflare.com`) and use it in:

1. **Wokwi ESP32 Code:**
   ```python
   # Update your Wokwi code to use the public URL
   server_url = "https://random-words-1234.trycloudflare.com"
   ```

2. **Flutter App:**
   ```dart
   // Update your Flutter API base URL
   final baseUrl = 'https://random-words-1234.trycloudflare.com';
   ```

### Step 5: Test the Public URL

```bash
# Test POST via public URL
curl -X POST https://random-words-1234.trycloudflare.com/readings \
  -H "Content-Type: application/json" \
  -d '{
    "sensor": "dht22",
    "temp": 25.5,
    "humidity": 60.0
  }'

# Test GET via public URL
curl https://random-words-1234.trycloudflare.com/readings/latest
```

### Important Notes:

- **Temporary URLs:** The free Cloudflare Tunnel URLs are temporary and change each time you restart the tunnel. For production, consider setting up a named tunnel with a custom domain.
- **Keep Both Running:** Keep both the NestJS server (`npm run start:dev`) and the Cloudflare tunnel (`cloudflared tunnel --url ...`) running simultaneously.
- **HTTPS:** Cloudflare Tunnel automatically provides HTTPS, which is required for Wokwi ESP32 connections.
- **Security:** The tunnel URL is publicly accessible. For production, implement authentication/authorization.

### Alternative: Named Tunnel (Persistent URL)

For a persistent URL, you can set up a named tunnel:

```bash
# Authenticate
cloudflared tunnel login

# Create a named tunnel
cloudflared tunnel create iot-backend

# Configure the tunnel
cloudflared tunnel route dns iot-backend your-subdomain.yourdomain.com

# Run the tunnel
cloudflared tunnel run iot-backend
```

---

## Quick Start Summary

1. **Install dependencies:**
   ```bash
   cd backend
   npm install
   ```

2. **Start server:**
   ```bash
   npm run start:dev
   ```

3. **Test locally:**
   ```bash
   curl -X POST http://localhost:3000/readings \
     -H "Content-Type: application/json" \
     -d '{"sensor":"dht22","temp":25.5,"humidity":60.0}'
   ```

4. **Expose publicly (optional):**
   ```bash
   cloudflared tunnel --url http://localhost:3000
   ```

---

## Project Structure

```
backend/
├── src/
│   ├── readings/
│   │   ├── dto/
│   │   │   └── create-reading.dto.ts
│   │   ├── entities/
│   │   │   └── reading.entity.ts
│   │   ├── readings.controller.ts
│   │   ├── readings.service.ts
│   │   └── readings.module.ts
│   ├── app.module.ts
│   └── main.ts
├── package.json
├── tsconfig.json
└── nest-cli.json
```
