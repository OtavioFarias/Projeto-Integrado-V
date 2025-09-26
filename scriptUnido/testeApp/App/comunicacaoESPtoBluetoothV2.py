#!/usr/bin/env python3
import serial
import threading
import time

PORT = '/dev/rfcomm0'
BAUD = 115200

def receber_esp32(ser):
    """
    Thread para ler mensagens do ESP32 continuamente
    """
    while True:
        try:
            if ser.in_waiting:
                linha = ser.readline().decode('utf-8', errors='ignore').strip()
                if linha:
                    print(f"\n[ESP32] {linha}")
        except Exception:
            break

def enviar_mensagem(ser, msg_dict):
    """
    Envia dicionário como chave:valor;chave:valor;\n
    """
    msg = ''.join(f"{k}:{v};" for k, v in msg_dict.items()) + '\n'
    ser.write(msg.encode('utf-8'))
    print(f"[ENVIADO] {msg.strip()}")

def menu():
    print("\n--- Menu de envio ---")
    print("1 - Definir velocidade")
    print("2 - Definir anguloObjetivo")
    print("3 - Definir distanciaParaVirar")
    print("4 - Definir leiturasUltrassonico")
    print("5 - Definir andarAutonomo")
    print("6 - Enviar mensagem customizada")
    print("0 - Sair")
    escolha = input("Escolha uma opção: ")
    return escolha

def main():
    try:
        ser = serial.Serial(PORT, BAUD, timeout=1)
        time.sleep(2)  # espera ESP32 inicializar
        print(f"Conectado a {PORT} a {BAUD}bps")
    except Exception as e:
        print(f"Erro ao abrir {PORT}: {e}")
        return

    # Inicia thread de leitura
    t = threading.Thread(target=receber_esp32, args=(ser,), daemon=True)
    t.start()

    while True:
        escolha = menu()
        if escolha == '0':
            print("Saindo...")
            break
        elif escolha in ['1','2','3','4','5']:
            chave = {
                '1':'velocidade',
                '2':'anguloObjetivo',
                '3':'distanciaParaVirar',
                '4':'leiturasUltrassonico',
                '5':'andarAutonomo'
            }[escolha]
            valor = input(f"Digite o valor para {chave}: ")
            enviar_mensagem(ser, {chave:valor})
        elif escolha == '6':
            custom = input("Digite a mensagem no formato chave:valor;chave:valor;: ")
            if not custom.endswith('\n'):
                custom += '\n'
            ser.write(custom.encode('utf-8'))
            print(f"[ENVIADO] {custom.strip()}")
        else:
            print("Opção inválida!")

    ser.close()

if __name__ == "__main__":
    main()
