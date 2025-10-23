import serial
import threading
import time
import curses
import comunicacaoESP

PORT = '/dev/rfcomm0'
BAUD = 115200

def main_curses(stdscr):
    curses.curs_set(0)
    height, width = stdscr.getmaxyx()
    menu_win = curses.newwin(height, width//2, 0, 0)
    info_win = curses.newwin(height, width - width//2, 0, width//2)

    try:
        ser = serial.Serial(PORT, BAUD, timeout=1)
        time.sleep(2)  # espera ESP32 inicializar
        info_win.addstr(f"Conectado a {PORT} a {BAUD}bps\n")
        info_win.refresh()
    except Exception as e:
        info_win.addstr(f"Erro ao abrir {PORT}: {e}\n")
        info_win.refresh()
        time.sleep(2)
        return

    # Inicia thread de recebimento
    t = threading.Thread(target=comunicacaoESP.receber_esp32, args=(ser, info_win), daemon=True)
    t.start()


    # Loop do menu dentro da janela menu_win
    comunicacaoESP.loop_menu(ser, menu_win, curses)

    ser.close()

if __name__ == "__main__":
    curses.wrapper(main_curses)

