import network
import time
from machine import Pin
import dht
import ujson
import urequests  # HTTP requests in MicroPython

# API Endpoint
API_URL = "https://hiring-describe-tab-catherine.trycloudflare.com/readings"

sensor = dht.DHT22(Pin(15))

# Connect WiFi
print("Connecting to WiFi", end="")
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect("Wokwi-GUEST", "")
while not sta_if.isconnected():
  print(".", end="")
  time.sleep(0.1)
print(" Connected!")
print("IP:", sta_if.ifconfig()[0])

prev_payload = ""

while True:
  try:
    print("Measuring DHT22... ", end="")
    sensor.measure()

    payload = {
      "sensor": "dht22",
      "temp": round(sensor.temperature(), 1),
      "humidity": round(sensor.humidity(), 1),
    }

    body = ujson.dumps(payload)

    if body != prev_payload:
      print("Updated!")
      print("POST ->", API_URL, body)

      headers = {"Content-Type": "application/json"}

      # Send POST
      resp = urequests.post(API_URL, data=body, headers=headers)
      print("HTTP", resp.status_code, resp.text)
      resp.close()

      prev_payload = body
    else:
      print("No change")

  except Exception as e:
    print("Error:", e)

  time.sleep(3)
