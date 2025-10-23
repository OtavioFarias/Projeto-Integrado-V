from kivy.metrics import dp
from kivy.core.window import Window
from kivy.uix.togglebutton import ToggleButton
from kivy.uix.scrollview import ScrollView
from kivy.clock import Clock

from kivymd.app import MDApp
from kivymd.uix.screenmanager import MDScreenManager
from kivymd.uix.screen import MDScreen
from kivymd.uix.boxlayout import MDBoxLayout
from kivymd.uix.label import MDLabel
from kivymd.uix.button import MDRaisedButton
from kivymd.uix.slider import MDSlider
from kivymd.uix.tab import MDTabs, MDTabsBase

# =========================================================
# Tentativa de importar Pyjnius e permissões Android
# =========================================================
try:
    from jnius import autoclass
    from android.permissions import request_permissions, check_permission, Permission
    from kivy.clock import Clock

    BluetoothAdapter = autoclass('android.bluetooth.BluetoothAdapter')
    ANDROID = True
except Exception as e:
    print("⚠️ Bluetooth desativado ou Pyjnius não disponível:", e)
    BluetoothAdapter = None
    ANDROID = False


# =========================================================
# ABA CONTROLE
# =========================================================
class AbaControle(MDBoxLayout, MDTabsBase):
    def __init__(self, **kwargs):
        super().__init__(orientation="vertical", spacing=dp(15), padding=dp(20), **kwargs)

        self.add_widget(MDLabel(text="🧭 Controle do Robô Aspirador", halign="center", font_style="H5"))

        self.vel_label = MDLabel(text="Velocidade: 120", halign="center", font_style="H6")
        self.slider = MDSlider(min=0, max=255, value=120)
        self.slider.bind(value=self.on_slider_change)
        self.add_widget(self.vel_label)
        self.add_widget(self.slider)

        self.toggle_btn = MDRaisedButton(text="Ligar", size_hint_y=None, height=dp(50))
        self.toggle_btn.bind(on_release=self.toggle_motor)
        self.add_widget(self.toggle_btn)

        suc_box = MDBoxLayout(orientation="horizontal", spacing=dp(10), size_hint_y=None, height=dp(60))
        suc_box.add_widget(MDLabel(text="Sucção:", halign="left", font_style="H6"))
        self.suc_toggle = ToggleButton(text="Desativada", size_hint=(None, None), size=(dp(120), dp(40)))
        self.suc_toggle.bind(on_press=self.toggle_suction)
        suc_box.add_widget(self.suc_toggle)
        self.add_widget(suc_box)

        self.add_widget(MDLabel(text="Distância mínima: 20 cm", halign="center"))
        self.add_widget(MDLabel(text="Ângulo de rotação: 45°", halign="center"))
        self.add_widget(MDLabel(text="🔋 Bateria: 82%", halign="center", font_style="H6"))

    def on_slider_change(self, instance, value):
        self.vel_label.text = f"Velocidade: {int(value)}"

    def toggle_motor(self, instance):
        if self.toggle_btn.text == "Ligar":
            self.toggle_btn.text = "Desligar"
        else:
            self.toggle_btn.text = "Ligar"

    def toggle_suction(self, instance):
        instance.text = "Ativada" if instance.state == "down" else "Desativada"


# =========================================================
# ABA BLUETOOTH (com permissão em tempo de execução)
# =========================================================
class AbaBluetooth(MDBoxLayout, MDTabsBase):
    def __init__(self, **kwargs):
        super().__init__(orientation="vertical", spacing=dp(10), padding=dp(20), **kwargs)

        self.status_label = MDLabel(text="🔵 Procurando dispositivos...", halign="center", font_style="H6")
        self.add_widget(self.status_label)

        self.scroll = ScrollView(size_hint=(1, 1))
        self.devices_box = MDBoxLayout(orientation="vertical", size_hint_y=None, spacing=dp(5))
        self.devices_box.bind(minimum_height=self.devices_box.setter('height'))
        self.scroll.add_widget(self.devices_box)
        self.add_widget(self.scroll)

        Clock.schedule_once(self.listar_dispositivos, 2)

    def listar_dispositivos(self, *args):
        self.devices_box.clear_widgets()

        if not ANDROID or BluetoothAdapter is None:
            self.status_label.text = "Bluetooth não disponível (modo PC)."
            self.devices_box.add_widget(MDLabel(text="Nenhum dispositivo detectado.", halign="center"))
            return

        try:
            # --- Verifica e solicita permissão se necessário ---
            if not check_permission(Permission.BLUETOOTH_CONNECT):
                self.status_label.text = "🟡 Solicitando permissão BLUETOOTH_CONNECT..."
                request_permissions([Permission.BLUETOOTH_CONNECT], self._on_permission_result)
                return

            # --- Se já tem permissão ---
            self._mostrar_dispositivos()
        except Exception as e:
            self.status_label.text = f"Erro ao acessar Bluetooth: {e}"

    def _on_permission_result(self, permissions, results):
        if all(results):
            self.status_label.text = "✅ Permissão concedida!"
            Clock.schedule_once(lambda dt: self._mostrar_dispositivos(), 1)
        else:
            self.status_label.text = "❌ Permissão Bluetooth negada."

    def _mostrar_dispositivos(self):
        try:
            adapter = BluetoothAdapter.getDefaultAdapter()
            if adapter and adapter.isEnabled():
                paired = adapter.getBondedDevices().toArray()
                if len(paired) > 0:
                    self.status_label.text = "📱 Dispositivos pareados:"
                    for dev in paired:
                        nome = dev.getName()
                        addr = dev.getAddress()
                        self.devices_box.add_widget(MDLabel(text=f"{nome} ({addr}\n\n", halign="center"))
                else:
                    self.status_label.text = "Nenhum dispositivo pareado."
            else:
                self.status_label.text = "⚠️ Bluetooth desativado."
        except Exception as e:
            self.status_label.text = f"Erro ao acessar Bluetooth: {e}"


# =========================================================
# ABA MAPA
# =========================================================
class AbaMapa(MDBoxLayout, MDTabsBase):
    def __init__(self, **kwargs):
        super().__init__(orientation="vertical", spacing=dp(20), padding=dp(20), **kwargs)
        self.add_widget(MDLabel(text="🗺️ Mapa de Navegação", halign="center", font_style="H5"))
        self.add_widget(MDLabel(text="(Mapa será exibido aqui)", halign="center"))


# =========================================================
# TELA PRINCIPAL
# =========================================================
class TelaPrincipal(MDScreen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        tabs = MDTabs()
        tabs.add_widget(AbaControle(title="Controle"))
        tabs.add_widget(AbaBluetooth(title="Bluetooth"))
        tabs.add_widget(AbaMapa(title="Mapa"))
        self.add_widget(tabs)


# =========================================================
# APP PRINCIPAL
# =========================================================
class AppRoboAspirador(MDApp):
    def build(self):
        Window.clearcolor = (0.85, 0.85, 0.85, 1)
        sm = MDScreenManager()
        sm.add_widget(TelaPrincipal(name="principal"))
        return sm


if __name__ == "__main__":
    AppRoboAspirador().run()
