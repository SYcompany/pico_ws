import tkinter as tk
from tkinter import ttk
import serial
import serial.tools.list_ports


class SerialPortFrame(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid()

        # 첫 번째 행: 시리얼 포트 선택
        self.port_label = tk.Label(self, text="Select Serial Port:")
        self.port_label.grid(row=0, column=0, padx=10, pady=10)

        self.port_combobox = ttk.Combobox(self, values=self.get_serial_ports())
        self.port_combobox.grid(row=0, column=1, padx=10, pady=10)

        # 포트 목록이 비어 있지 않을 경우 첫 번째 포트 선택
        if self.port_combobox['values']:
            self.port_combobox.current(0)

        self.connect_button = tk.Button(self, text="Connect", command=self.connect_serial)
        self.connect_button.grid(row=0, column=2, padx=10, pady=10)

        # 두 번째 행: 수신된 데이터를 표시하는 시리얼 모니터
        self.monitor_label = tk.Label(self, text="Received Data:")
        self.monitor_label.grid(row=1, column=0, padx=10, pady=10)

        self.receive_text = tk.Text(self, height=5, width=20)
        self.receive_text.grid(row=1, column=1, columnspan=2, padx=10, pady=10)

        # 세 번째 행: 전송된 데이터를 표시하는 시리얼 모니터
        self.sent_label = tk.Label(self, text="Sent Data:")
        self.sent_label.grid(row=2, column=0, padx=10, pady=10)

        self.send_text = tk.Text(self, height=5, width=20)
        self.send_text.grid(row=2, column=1, columnspan=2, padx=10, pady=10)

        # 네 번째 행: 명령 전송
        self.command_label = tk.Label(self, text="Send Command:")
        self.command_label.grid(row=3, column=0, padx=10, pady=10)

        self.command_entry = tk.Entry(self, width=20)
        self.command_entry.grid(row=3, column=1, padx=10, pady=10)

        self.send_button = tk.Button(self, text="Send", command=self.send_command)
        self.send_button.grid(row=3, column=2, padx=10, pady=10)

        # 시리얼 포트 연결 객체
        self.serial_port = None

        self.after(100, self.read_serial)

    def get_serial_ports(self):
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]

    def connect_serial(self):
        selected_port = self.port_combobox.get()
        try:
            self.serial_port = serial.Serial(selected_port, baudrate=9600, timeout=1)
            self.receive_text.insert(tk.END, f"Connected to {selected_port}\n")
        except Exception as e:
            self.receive_text.insert(tk.END, f"Failed to connect: {e}\n")

    def send_command(self):
        command = self.command_entry.get() +"\n"
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.write(command.encode())
            self.send_text.insert(tk.END, f"Sent: {command}\n")
        else:
            self.send_text.insert(tk.END, "Not connected to any serial port.\n")

    def read_serial(self):
        if self.serial_port and self.serial_port.is_open:
            data = self.serial_port.readline().decode('utf-8').strip()
            if data:
                self.receive_text.delete(1.0, tk.END)  # 기존 수신된 텍스트를 지우고
                self.receive_text.insert(tk.END, f"Received: {data}\n")  # 새 데이터를 삽입
        self.after(100, self.read_serial)


class SerialGUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Serial Communication")
        self.geometry("1500x400")

        # 3개의 SerialPortFrame을 생성하여 배치
        for i in range(3):
            frame = SerialPortFrame(self)
            frame.grid(row=0, column=i, padx=5, pady=10)


if __name__ == "__main__":
    app = SerialGUI()
    app.mainloop()
