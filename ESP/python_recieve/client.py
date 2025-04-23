import socket

ESP_IP = '192.168.0.13'   # ← wpisz tutaj IP swojego ESP32
PORT = 1234

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        print(f"Łączenie z {ESP_IP}:{PORT}...")
        s.connect((ESP_IP, PORT))
        print("Połączono! Odbieranie danych:\n")

        while True:
            data = s.recv(1024)
            if not data:
                break
            print(data.decode(), end='')

if __name__ == "__main__":
    main()
