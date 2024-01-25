import socket
import json

host = '192.168.43.181'  # our IP address assigned to your ESP32
port = 80

def send_json_data(data):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(json.dumps(data).encode('utf-8'))
        response = s.recv(1024)
        print("Received from ESP32:", response.decode('utf-8'))

if __name__ == "__main__":
    data_to_send = {"message": "Hello from Python!"}
    send_json_data(data_to_send)


