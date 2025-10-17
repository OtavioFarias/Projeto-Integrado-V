import serial
import time

# abre porta criada pelo rfcomm
ser = serial.Serial('/dev/rfcomm0', 115200, timeout=1)

print("Conectado ao ESP32 via Bluetooth!")

try:
    # envia mensagem
    ser.write(b"Hello ESP32!\n")

    while True:
        # lê respostas do ESP32
        line = ser.readline()
        if line:
            print("ESP32 disse:", line.decode(errors='ignore').strip())

        # manda algo a cada 10s
        time.sleep(10)
        ser.write(b"O PC diz oi!\n")

except KeyboardInterrupt:
    print("Encerrando...")
finally:
    ser.close()

