#!/usr/bin/env python3
import curses
import serial
import threading
import time

PORT = '/dev/rfcomm0'
BAUD = 115200

# Lista para armazenar mensagens recebidas
mensagens_esp = []

def ler_esp32(ser):
    """Thread para ler dados do ESP32 continuamente"""
    while True:
        try:
            if ser.in_waiting:
                linha = ser.readline().decode('utf-8', errors='ignore').strip()
                if linha:
                    mensagens_esp.append(linha)
        except Exception:
            break

def enviar_mensagem(ser, msg_dict):
    """Envia dicionário como chave:valor;chave:valor;\n"""
    msg = ''.join(f"{k}:{v};" for k, v in msg_dict.items()) + '\n'
    ser.write(msg.encode('utf-8'))
    mensagens_esp.append(f"[ENVIADO] {msg.strip()}")

def menu_input(stdscr, prompt):
    """Função para receber input do usuário na janela curses"""
    curses.echo()
    stdscr.addstr(curses.LINES-1, 0, prompt)
    stdscr.clrtoeol()
    entrada = stdscr.getstr(curses.LINES-1, len(prompt)).decode('utf-8')
    curses.noecho()
    return entrada

def main(stdscr):
    # Inicialização do curses
    curses.curs_set(0)
    height, width = stdscr.getmaxyx()
    menu_win = curses.newwin(height, width//2, 0, 0)
    info_win = curses.newwin(height, width - width//2, 0, width//2)

    # Conexão serial
    try:
        ser = serial.Serial(PORT, BAUD, timeout=1)
        time.sleep(2)
    except Exception as e:
        stdscr.addstr(0, 0, f"Erro ao abrir {PORT}: {e}")
        stdscr.refresh()
        stdscr.getch()
        return

    # Thread de leitura
    t = threading.Thread(target=ler_esp32, args=(ser,), daemon=True)
    t.start()

    while True:
        # --- Menu ---
        menu_win.clear()
        menu_win.box()
        menu_win.addstr(1, 2, "Menu Interativo")
        menu_win.addstr(3, 2, "1 - Definir velocidade")
        menu_win.addstr(4, 2, "2 - Definir anguloObjetivo")
        menu_win.addstr(5, 2, "3 - Definir distanciaParaVirar")
        menu_win.addstr(6, 2, "4 - Definir leiturasUltrassonico")
        menu_win.addstr(7, 2, "5 - Definir andarAutonomo")
        menu_win.addstr(8, 2, "6 - Enviar mensagem customizada")
        menu_win.addstr(9, 2, "0 - Sair")
        menu_win.refresh()

        # --- Info ESP32 ---
        info_win.clear()
        info_win.box()
        info_win.addstr(1, 2, "Dados do ESP32:")
        for i, msg in enumerate(mensagens_esp[-(height-3):]):
            info_win.addstr(i+2, 2, msg[:width - width//2 - 4])
        info_win.refresh()

        # Input do usuário
        escolha = menu_input(menu_win, "Escolha uma opção: ")

        if escolha == '0':
            break
        elif escolha in ['1','2','3','4','5']:
            chave = {
                '1':'velocidade',
                '2':'anguloObjetivo',
                '3':'distanciaParaVirar',
                '4':'leiturasUltrassonico',
                '5':'andarAutonomo'
            }[escolha]
            valor = menu_input(menu_win, f"Digite o valor para {chave}: ")
            enviar_mensagem(ser, {chave:valor})
        elif escolha == '6':
            custom = menu_input(menu_win, "Mensagem customizada (chave:valor;...;): ")
            if not custom.endswith('\n'):
                custom += '\n'
            ser.write(custom.encode('utf-8'))
            mensagens_esp.append(f"[ENVIADO] {custom.strip()}")
        else:
            mensagens_esp.append("[ERRO] Opção inválida!")

    ser.close()

if __name__ == "__main__":
    curses.wrapper(main)
