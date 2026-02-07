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
