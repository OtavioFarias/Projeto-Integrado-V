#!/usr/bin/env python3
# console_esp32_curses.py
import curses
import serial
import threading
import time
from collections import deque

PORT = '/dev/rfcomm0'
BAUD = 115200

# Buffer de mensagens (thread-safe com lock)
mensagens_esp = deque(maxlen=1000)
mensagens_lock = threading.Lock()

# Lock para escrita serial
serial_lock = threading.Lock()

# Flags de controle
stop_threads = False

def clean_msg(s: str) -> str:
    # Remove caracteres nulos e carriage returns, mantendo texto legível
    return s.replace('\0', '').replace('\r', '')

def ler_esp32(ser):
    """Thread que lê do ESP32 continuamente e guarda em mensagens_esp."""
    global stop_threads
    while not stop_threads:
        try:
            if ser.in_waiting:
                linha = ser.readline().decode('utf-8', errors='ignore')
                linha = clean_msg(linha).strip()
                if linha:
                    with mensagens_lock:
                        mensagens_esp.append(f"[ESP32] {linha}")
            else:
                time.sleep(0.05)
        except Exception as e:
            with mensagens_lock:
                mensagens_esp.append(f"[ERRO LEITURA] {e}")
            time.sleep(1)

def keepalive_thread(ser, interval=2.0):
    """Envia caractere nulo periodicamente para manter SPP vivo."""
    global stop_threads
    while not stop_threads:
        try:
            with serial_lock:
                ser.write(b'\0')  # enviamos nulo; será limpo na exibição
        except Exception:
            pass
        time.sleep(interval)

def enviar_mensagem(ser, msg_dict):
    """Formata e envia dicionário como chave:valor;...;\n"""
    msg = ''.join(f"{k}:{v};" for k, v in msg_dict.items()) + '\n'
    try:
        with serial_lock:
            ser.write(msg.encode('utf-8'))
        with mensagens_lock:
            mensagens_esp.append(f"[ENVIADO] {msg.strip()}")
    except Exception as e:
        with mensagens_lock:
            mensagens_esp.append(f"[ERRO ENVIO] {e}")

def enviar_custom(ser, custom):
    if not custom.endswith('\n'):
        custom += '\n'
    try:
        with serial_lock:
            ser.write(custom.encode('utf-8'))
        with mensagens_lock:
            mensagens_esp.append(f"[ENVIADO] {custom.strip()}")
    except Exception as e:
        with mensagens_lock:
            mensagens_esp.append(f"[ERRO ENVIO] {e}")

def draw_menu(win):
    win.box()
    win.addstr(1, 2, "Menu Interativo")
    win.addstr(3, 2, "1 - Definir velocidade")
    win.addstr(4, 2, "2 - Definir anguloObjetivo")
    win.addstr(5, 2, "3 - Definir distanciaParaVirar")
    win.addstr(6, 2, "4 - Definir leiturasUltrassonico")
    win.addstr(7, 2, "5 - Definir andarAutonomo")
    win.addstr(8, 2, "6 - Enviar mensagem customizada")
    win.addstr(9, 2, "0 - Sair")
    win.addstr(11, 2, "Pressione a tecla correspondente")
    win.addstr(13, 2, "Depois digite valor e ENTER")
    win.refresh()

def main(stdscr):
    global stop_threads

    curses.curs_set(0)
    stdscr.nodelay(True)  # getch não bloqueia
    stdscr.timeout(100)   # refresh a cada 100ms

    height, width = stdscr.getmaxyx()
    left_w = max(30, width // 3)
    right_w = width - left_w - 1

    menu_win = curses.newwin(height - 2, left_w, 0, 0)
    info_win = curses.newwin(height - 2, right_w, 0, left_w + 1)
    input_win = curses.newwin(2, width, height - 2, 0)

    # desenha separador vertical
    stdscr.vline(0, left_w, curses.ACS_VLINE, height - 2)
    stdscr.refresh()

    # tenta abrir serial
    try:
        ser = serial.Serial(PORT, BAUD, timeout=0.1)
        time.sleep(1)  # espera ESP boot/handshake
    except Exception as e:
        stdscr.addstr(0, 0, f"Erro ao abrir {PORT}: {e}")
        stdscr.addstr(1, 0, "Feche e verifique /dev/rfcomm0. Pressione qualquer tecla para sair.")
        stdscr.refresh()
        stdscr.getch()
        return

    # inicia threads
    t_read = threading.Thread(target=ler_esp32, args=(ser,), daemon=True)
    t_read.start()
    t_keep = threading.Thread(target=keepalive_thread, args=(ser,2.0), daemon=True)
    t_keep.start()

    # estado do input
    mode = 'menu'   # 'menu', 'value', 'custom'
    input_buffer = ''
    current_key = None  # '1'..'5' quando em mode 'value'

    # mapa de chaves
    chave_map = {
        '1':'velocidade',
        '2':'anguloObjetivo',
        '3':'distanciaParaVirar',
        '4':'leiturasUltrassonico',
        '5':'andarAutonomo'
    }

    # loop principal
    while True:
        # desenha menu
        menu_win.clear()
        draw_menu(menu_win)

        # desenha info_win (últimas mensagens)
        info_win.clear()
        info_win.box()
        info_win.addstr(1, 2, "Dados do ESP32 (recebidos):")
        with mensagens_lock:
            msgs = list(mensagens_esp)
        # mostra do fim para o começo (scroll automático)
        max_lines = height - 6
        last_msgs = msgs[-max_lines:]
        for i, m in enumerate(last_msgs):
            # Trunca se maior que janela
            line = m
            if len(line) > (right_w - 4):
                line = line[:right_w - 7] + "..."
            try:
                info_win.addstr(2 + i, 2, line)
            except curses.error:
                pass
        info_win.refresh()

        # desenha input_win com prompt conforme o modo
        input_win.clear()
        input_win.box()
        if mode == 'menu':
            prompt = "Tecla (0-sair, 1-5 opções, 6-custom): "
        elif mode == 'value':
            chave = chave_map.get(current_key, '?')
            prompt = f"Digite valor para {chave}: {input_buffer}"
        elif mode == 'custom':
            prompt = f"Mensagem customizada: {input_buffer}"
        else:
            prompt = input_buffer

        try:
            input_win.addstr(1, 2, prompt[:width-4])
        except curses.error:
            pass
        input_win.refresh()

        # captura tecla (não-bloqueante)
        try:
            ch = stdscr.getch()
        except KeyboardInterrupt:
            ch = ord('0')

        if ch == -1:
            # sem tecla, apenas continua loop para atualizar janelas
            continue

        # ENTER
        if ch in (10, 13):
            if mode == 'menu':
                # ignorar ENTER no menu
                pass
            elif mode == 'value' and current_key:
                # envia valor recebido
                chave = chave_map[current_key]
                valor = input_buffer.strip()
                if valor != '':
                    enviar_mensagem(ser, {chave: valor})
                else:
                    with mensagens_lock:
                        mensagens_esp.append("[ERRO] valor vazio, não enviado")
                # reset estado
                input_buffer = ''
                mode = 'menu'
                current_key = None
            elif mode == 'custom':
                text = input_buffer.strip()
                if text:
                    enviar_custom(ser, text)
                input_buffer = ''
                mode = 'menu'
            continue

        # Backspace
        if ch in (8, 127, curses.KEY_BACKSPACE):
            if len(input_buffer) > 0:
                input_buffer = input_buffer[:-1]
            continue

        # ESC -> volta ao menu
        if ch == 27:
            input_buffer = ''
            mode = 'menu'
            current_key = None
            continue

        # leitura de tecla em modo menu (espera tecla numérica)
        if mode == 'menu':
            try:
                key = chr(ch)
            except Exception:
                key = ''
            if key == '0':
                # sair
                stop_threads = True
                break
            elif key in ('1','2','3','4','5'):
                # entra modo "value"
                mode = 'value'
                current_key = key
                input_buffer = ''
            elif key == '6':
                mode = 'custom'
                input_buffer = ''
            else:
                # tecla desconhecida -> mostra aviso rápido
                with mensagens_lock:
                    mensagens_esp.append("[AVISO] tecla inválida. Use 0..6.")
            continue

        # se estiver em modo value/custom, adiciona caractere digitado
        if mode in ('value','custom'):
            # apenas caracteres imprimíveis
            if 32 <= ch <= 126:
                input_buffer += chr(ch)
            # ignore outros
            continue

    # fim do loop: cleanup
    stop_threads = True
    try:
        ser.close()
    except Exception:
        pass
