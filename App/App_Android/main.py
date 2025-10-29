from kivy.metrics import dp
from kivy.core.window import Window
from kivy.uix.togglebutton import ToggleButton
from kivy.clock import Clock
from kivy.uix.scrollview import ScrollView
from kivy.uix.textinput import TextInput

from kivymd.app import MDApp
from kivymd.uix.screenmanager import MDScreenManager
from kivymd.uix.screen import MDScreen
from kivymd.uix.boxlayout import MDBoxLayout
from kivymd.uix.label import MDLabel
from kivymd.uix.button import MDRaisedButton
from kivymd.uix.slider import MDSlider
from kivymd.uix.tab import MDTabs, MDTabsBase
from kivymd.uix.textfield import MDTextField

# =========================================================
# Importação segura Pyjnius + permissões Android
# =========================================================
try:
    from jnius import autoclass
    from android.permissions import request_permissions, Permission

    BluetoothAdapter = autoclass('android.bluetooth.BluetoothAdapter')
    UUID = autoclass('java.util.UUID')

    ANDROID = True
except Exception as e:
    print("⚠ Bluetooth desativado ou Pyjnius não disponível:", e)
    BluetoothAdapter = None
    ANDROID = False


# =========================================================
# GERENCIADOR BLUETOOTH
# =========================================================
class BluetoothManager:
    def __init__(self):
        self.adapter = BluetoothAdapter.getDefaultAdapter() if ANDROID else None
        self.socket = None
        self.device = None
        self.uuid = None
        if ANDROID and self.adapter:
            self.uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb")

    def conectar(self, endereco):
        """Conecta ao dispositivo Bluetooth pelo MAC address."""
        try:
            device = self.adapter.getRemoteDevice(endereco)
            socket = device.createRfcommSocketToServiceRecord(self.uuid)
            self.adapter.cancelDiscovery()
            print(f"Tentando conectar a {device.getName()} ({endereco})...")
            socket.connect()
            self.socket = socket
            self.device = device
            print(f"Conectado a {device.getName()} ({device.getAddress()})")
            return True
        except Exception as e:
            print("Erro ao conectar:", e)
            self.socket = None
            return False

    def enviar(self, texto):
        """Envia uma string terminada com ';' ao ESP."""
        if self.socket:
            try:
                output = self.socket.getOutputStream()
                output.write((texto + ";").encode())  # 🔹 agora compatível com o ESP
                output.flush()
                print(f"-> Enviado: {texto}")
            except Exception as e:
                print("Erro ao enviar:", e)
        else:
            print("Nenhum dispositivo conectado.")

    def receber(self):
        """Lê dados recebidos do ESP."""
        if not self.socket:
            return None
        try:
            input_stream = self.socket.getInputStream()
            available = input_stream.available()
            if available > 0:
                data = bytearray()
                for _ in range(available):
                    data.append(input_stream.read())
                msg = data.decode(errors="ignore").strip()
                if msg:
                    print(f"<- Recebido: {msg}")
                    return msg
        except Exception as e:
            print("Erro ao receber:", e)
        return None

    def desconectar(self):
        """Desconecta e fecha o socket Bluetooth."""
        if self.socket:
            try:
                self.socket.close()
                print("🔌 Desconectado.")
            except Exception as e:
                print("Erro ao desconectar:", e)
        self.socket = None
        self.device = None


# =========================================================
# ABA CONTROLE
# =========================================================
class AbaControle(MDBoxLayout, MDTabsBase):
    def __init__(self, bt_manager, **kwargs):
        super().__init__(orientation="vertical", spacing=dp(15), padding=dp(20), **kwargs)
        self.bt_manager = bt_manager

        self.add_widget(MDLabel(text="Controle do Robô", halign="center", font_style="H5"))

        # Slider de velocidade
        self.vel_label = MDLabel(text="Velocidade: 120", halign="center")
        self.slider = MDSlider(min=0, max=255, value=120)
        self.slider.bind(value=self.on_slider_change)
        self.add_widget(self.vel_label)
        self.add_widget(self.slider)

        # Botão motor
        self.toggle_btn = MDRaisedButton(text="Ligar", size_hint_y=None, height=dp(50),_no_ripple_effect=False)
        self.toggle_btn.bind(on_release=self.toggle_motor)
        self.add_widget(self.toggle_btn)

    def on_slider_change(self, instance, value):
        v = int(value)
        self.vel_label.text = f"Velocidade: {v}"
        self.bt_manager.enviar(f"velocidade:{v}")

    def toggle_motor(self, instance):
        if instance.text == "Ligar":
            instance.text = "Desligar"
            self.bt_manager.enviar("motor:on")
        else:
            instance.text = "Ligar"
            self.bt_manager.enviar("motor:off")


# =========================================================
# ABA TERMINAL BLUETOOTH (modificada)
# =========================================================
class AbaBluetooth(MDBoxLayout, MDTabsBase):
    def __init__(self, bt_manager, **kwargs):
        super().__init__(orientation="vertical", spacing=dp(10), padding=dp(10), **kwargs)
        self.bt_manager = bt_manager

        # 🔹 Status + botão conectar/desconectar em uma linha
        status_box = MDBoxLayout(orientation="horizontal", spacing=dp(10), size_hint_y=None, height=dp(50))
        self.status_label = MDLabel(text="Status: Desconectado", halign="left", font_style="H6")
        self.connect_btn = MDRaisedButton(text="Conectar ao ESP", size_hint_x=None, width=dp(160),_no_ripple_effect=False)
        self.connect_btn.bind(on_release=self.tentar_conectar)
        status_box.add_widget(self.status_label)
        status_box.add_widget(self.connect_btn)
        self.add_widget(status_box)

        # 🔹 Campo de envio fica logo abaixo do status
        self.text_field = MDTextField(hint_text="Digite um comando...", size_hint_y=None, height=dp(50))
        self.add_widget(self.text_field)

        # 🔹 Terminal (ScrollView) abaixo do campo de envio
        self.scroll = ScrollView(size_hint=(1, 1))
        self.terminal = TextInput(
            readonly=True,
            font_size=30 ,
            size_hint_y=None,
            height=dp(300),
            background_color=(0, 0, 0, 1),
            foreground_color=(0, 1, 0, 1),
        )
        self.scroll.add_widget(self.terminal)
        self.add_widget(self.scroll)

        # 🔹 Botão enviar abaixo do terminal
        self.send_btn = MDRaisedButton(text="Enviar", size_hint_y=None, height=dp(50),_no_ripple_effect=False)
        self.send_btn.bind(on_release=self.enviar_texto)
        self.add_widget(self.send_btn)

        self.update_event = None

    # 🔹 Conexão
    def tentar_conectar(self, instance):
        endereco_esp = "7C:9E:BD:F2:E1:C6"
        if self.bt_manager.conectar(endereco_esp):
            self.status_label.text = "Conectado ao ESP"
            self.connect_btn.text = "Desconectar"
            self.connect_btn.unbind(on_release=self.tentar_conectar)
            self.connect_btn.bind(on_release=self.desconectar)
            self.update_event = Clock.schedule_interval(self.ler_dados, 0.5)
            self.terminal.text += "\nConectado com sucesso!"
        else:
            self.status_label.text = "Falha ao conectar"
            self.terminal.text += "\nErro ao tentar conectar."

    def desconectar(self, instance):
        if self.update_event:
            self.update_event.cancel()
        self.bt_manager.desconectar()
        self.status_label.text = "Status: Desconectado"
        self.connect_btn.text = "Conectar ao ESP"
        self.connect_btn.unbind(on_release=self.desconectar)
        self.connect_btn.bind(on_release=self.tentar_conectar)
        self.terminal.text += "\nDesconectado."

    # 🔹 Envia o texto digitado para o ESP
    def enviar_texto(self, instance):
        texto = self.text_field.text.strip()
        if texto:
            self.bt_manager.enviar(texto)
            self.terminal.text += f"\n-> {texto}"
            self.text_field.text = ""
            self.scroll.scroll_y = 0

    # 🔹 Lê dados do ESP
    def ler_dados(self, dt):
        msg = self.bt_manager.receber()
        if msg:
            self.terminal.text += f"\n<- {msg}"
            self.scroll.scroll_y = 0

# =========================================================
# ABA MAPA
# =========================================================
class AbaMapa(MDBoxLayout, MDTabsBase):
    def __init__(self, **kwargs):
        super().__init__(orientation="vertical", spacing=dp(20), padding=dp(20), **kwargs)
        self.add_widget(MDLabel(text="Mapa de Navegação (futuro recurso)", halign="center", font_style="H5"))


# =========================================================
# TELA PRINCIPAL
# =========================================================
class TelaPrincipal(MDScreen):
    def __init__(self, bt_manager, **kwargs):
        super().__init__(**kwargs)
        tabs = MDTabs()
        tabs.add_widget(AbaControle(bt_manager, title="Controle"))
        tabs.add_widget(AbaBluetooth(bt_manager, title="Terminal"))
        tabs.add_widget(AbaMapa(title="Mapa"))
        self.add_widget(tabs)


# =========================================================
# APP PRINCIPAL
# =========================================================
class AppRoboAspirador(MDApp):
    def build(self):
        Window.clearcolor = (0.9, 0.9, 0.9, 1)

        # 🔹 Solicita permissões antes de tudo
        if ANDROID:
            from android.permissions import request_permissions, Permission
            request_permissions([
                Permission.BLUETOOTH,
                Permission.BLUETOOTH_ADMIN,
                Permission.BLUETOOTH_CONNECT,
                Permission.BLUETOOTH_SCAN,
                Permission.ACCESS_FINE_LOCATION,
                Permission.ACCESS_COARSE_LOCATION
            ])

        self.bt_manager = BluetoothManager()
        sm = MDScreenManager()
        sm.add_widget(TelaPrincipal(name="principal", bt_manager=self.bt_manager))
        return sm


if __name__ == "__main__":
    AppRoboAspirador().run()
