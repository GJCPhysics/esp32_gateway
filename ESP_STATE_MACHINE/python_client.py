import requests
import threading

class Alarm(threading.Thread):
    def __init__(self):
        super().__init__()
        self._stop_event = threading.Event()

    def stop(self):
        self._stop_event.set()

    def stopped(self):
        return self._stop_event.is_set()

    def run(self):
        while not self.stopped():
            get_intrusion_alarm_status()
            get_smoke_alarm_status()

def arm_system():
    try:
        response = requests.post('http://192.168.43.172:5000/receive_command', json={"command": "arm"})
        if response.status_code == 200:
            print("Command sent to arm system successfully!")
        else:
            print("Failed to send command to arm system.")
    except requests.exceptions.RequestException as e:
        print("Error:", e)

def disarm_system():
    try:
        response = requests.post('http://192.168.43.172:5000/receive_command', json={"command": "disarm"})
        if response.status_code == 200:
            print("Command sent to disarm system successfully!")
        else:
            print("Failed to send command to disarm system.")
    except requests.exceptions.RequestException as e:
        print("Error:", e)

def get_sensor_status():
    try:
        response = requests.get('http://192.168.43.172:5000/send_status')
        if response.status_code == 200:
            data = response.json()
            print("Door Sensor Status:", data['door_sensor_status'])
            print("Smoke Sensor Status:", data['smoke_sensor_status'])
        else:
            print("Failed to get sensor status.")
    except requests.exceptions.RequestException as e:
        print("Error:", e)

def get_intrusion_alarm_status():
    try:
        response = requests.get('http://192.168.43.172:5000/send_intrusion_alarm')
        if response.status_code == 200:
            data = response.json()
            if data.get('intrusion_alarm_triggered'):
                print("Intrusion alarm triggered!")
        else:
            print("Failed to get intrusion alarm status.")
    except requests.exceptions.RequestException as e:
        print("Error:", e)

def get_smoke_alarm_status():
    try:
        response = requests.get('http://192.168.43.172:5000/send_smoke_alarm')
        if response.status_code == 200:
            data = response.json()
            if data.get('smoke_alarm_triggered'):
                print("Smoke sensor alarm triggered!")
        else:
            print("Failed to get smoke alarm status.")
    except requests.exceptions.RequestException as e:
        print("Error:", e)

def main():
    alarm_listener = Alarm()
    alarm_listener.start()

    while True:
        print("1. Arm system")
        print("2. Disarm system")
        print("3. Get sensor status")
        print("4. Exit")

        choice = input("Enter your choice: ")

        if choice == '1':
            arm_system()
        elif choice == '2':
            disarm_system()
        elif choice == '3':
            get_sensor_status()
        elif choice == '4':
            alarm_listener.stop()
            break
        else:
            print("Invalid choice. Please try again.")

    alarm_listener.join()

if __name__ == "__main__":
    main()

