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
