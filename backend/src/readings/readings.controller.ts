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
