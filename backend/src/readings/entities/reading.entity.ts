export class Reading {
  id: number;
  sensor: string;
  temp: number;
  humidity: number;
  createdAt: Date;

  constructor(sensor: string, temp: number, humidity: number) {
    this.sensor = sensor;
    this.temp = temp;
    this.humidity = humidity;
    this.createdAt = new Date();
  }
}
