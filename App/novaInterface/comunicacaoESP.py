import curses
import serial
import threading
import time

def receber_esp32(ser, win, max_linhas=20):
    """
    Thread para ler mensagens do ESP32 continuamente e mostrar na janela info_win.
    max_linhas define quantas linhas são mantidas na tela.
    """
    win.scrollok(True)
    buffer = []

    while True:
        try:
            if ser.in_waiting:
                linha = ser.readline().decode('utf-8', errors='ignore').strip()
                if linha:
                    # Remove caracteres nulos
                    linha = linha.replace('\x00', '')

                    buffer.append(f"[ESP32] {linha}")

                    if len(buffer) > max_linhas:
                        buffer.pop(0)

                    win.clear()
                    for l in buffer:
                        win.addstr(l + "\n")
                    win.refresh()
            else:
                time.sleep(0.05)
        except Exception as e:
            win.addstr(f"Erro: {e}\n")
            win.refresh()
            break
            
def enviar_mensagem(ser, msg_dict):
    """
    Envia dicionário como chave:valor;chave:valor;\n
    """
    msg = ''.join(f"{k}:{v};" for k, v in msg_dict.items()) + '\n'
    ser.write(msg.encode('utf-8'))
    print(f"[ENVIADO] {msg.strip()}")

def loop_menu(ser, menu_win, curses):
    menu_win.keypad(True)  # Permite capturar teclas especiais
    curses.echo()  # Permite digitar texto visível
    while True:
        menu_win.clear()
        menu_win.addstr("=== MENU ===\n")
        menu_win.addstr("1 - Velocidade\n")
        menu_win.addstr("2 - Ângulo Objetivo\n")
        menu_win.addstr("3 - Distância Para Virar\n")
        menu_win.addstr("4 - Leituras Ultrassônico\n")
        menu_win.addstr("5 - Andar Autônomo\n")
        menu_win.addstr("6 - frente\n")
        menu_win.addstr("7 - tras\n")
        menu_win.addstr("8 - direita\n")
        menu_win.addstr("9 - esquerda\n")
        menu_win.addstr("10 - parar\n")
        menu_win.addstr("11 - passoFrente\n")
        menu_win.addstr("12 - passoTras\n")
        menu_win.addstr("13 - passoDireita\n")
        menu_win.addstr("14 - passoEsquerda\n")
        menu_win.addstr("15 - virarCoordenado\n")
        menu_win.addstr("16 - ativarEnvioDados\n")
        menu_win.addstr("17 - Mensagem Customizada\n")
        menu_win.addstr("0 - Sair\n")
        menu_win.addstr("\nEscolha: ")
        menu_win.refresh()

        escolha = menu_win.getstr().decode('utf-8').strip()

        if escolha == '0':
            menu_win.addstr("\nSaindo...\n")
            menu_win.refresh()
            curses.napms(1000)  # Aguarda 1 segundo
            break
        elif escolha in ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16']:
            chave = {
                '1':'velocidade',
                '2':'anguloObjetivo',
                '3':'distanciaParaVirar',
                '4':'leiturasUltrassonico',
                '5':'andarAutonomo',
                '6':'frente',
                '7':'tras',
                '8':'direita',
                '9':'esquerda',
                '10':'parar',
                '11':'passoFrente',
                '12':'passoTras',
                '13':'passoEsquerda',
                '14':'passoDireita',
                '15':"virarCoordenado",
                '16':"ativarEnvioDados"

            }[escolha]

            menu_win.addstr(f"Digite o valor para {chave}: ")
            menu_win.refresh()
            valor = menu_win.getstr().decode('utf-8').strip()
            enviar_mensagem(ser, {chave: valor})
            menu_win.addstr(f"[ENVIADO] {chave}: {valor}\n")
            menu_win.refresh()
            curses.napms(1000)
        elif escolha == '17':
            menu_win.addstr("Digite a mensagem no formato chave:valor;chave:valor;: ")
            menu_win.refresh()
            custom = menu_win.getstr().decode('utf-8').strip()
            if not custom.endswith('\n'):
                custom += '\n'
            ser.write(custom.encode('utf-8'))
            menu_win.addstr(f"[ENVIADO] {custom.strip()}\n")
            menu_win.refresh()
            curses.napms(1000)
        else:
            menu_win.addstr("Opção inválida!\n")
            menu_win.refresh()
            curses.napms(1000)

