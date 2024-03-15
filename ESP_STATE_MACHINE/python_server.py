from flask import Flask, request, jsonify

app = Flask(__name__)

current_command = ""
door_sensor_status = "unknown"
smoke_sensor_status = "unknown"
intrusion_alarm_triggered = False
smoke_alarm_triggered = False

intrusion_alarm_sent = False
smoke_alarm_sent = False

@app.route('/update_sensor_status', methods=['POST'])
def update_sensor_status():
    global door_sensor_status, smoke_sensor_status
    data = request.get_json()
    print("Received JSON Data:", data)

    door_sensor_status = data.get('DoorSensorValue', 'unknown')
    smoke_sensor_status = data.get('SmokeSensorValue', 'unknown')

    print(f"Received door status: {door_sensor_status}")
    print(f"Received smoke status: {smoke_sensor_status}")

    return jsonify({"status": "success"})

@app.route('/notify/intrusion_alarm', methods=['POST'])
def notify_intrusion_alarm():
    global intrusion_alarm_triggered, intrusion_alarm_sent
    intrusion_alarm_triggered = True
    intrusion_alarm_sent = True
    print("Intrusion alarm triggered!")
    return jsonify({"status": "success"})

@app.route('/notify/smoke_sensor_alarm', methods=['POST'])
def notify_smoke_sensor_alarm():
    global smoke_alarm_triggered, smoke_alarm_sent
    smoke_alarm_triggered = True
    smoke_alarm_sent = True
    print("Smoke sensor alarm triggered!")
    return jsonify({"status": "success"})

@app.route('/send_commands', methods=['GET'])
def send_commands():
    global current_command
    command = current_command
    current_command = "" 
    return jsonify({"command": command})

@app.route('/receive_command', methods=['POST'])
def receive_command():
    global current_command
    data = request.get_json()
    current_command = data.get('command', '')
    print("Received command from client:", current_command)
    return jsonify({"status": "success"})

@app.route('/send_status', methods=['GET'])
def send_status():
    global door_sensor_status, smoke_sensor_status
    return jsonify({
        "door_sensor_status": door_sensor_status,
        "smoke_sensor_status": smoke_sensor_status
    })

@app.route('/send_intrusion_alarm', methods=['GET'])
def send_intrusion_alarm():
    global intrusion_alarm_triggered, intrusion_alarm_sent
    if intrusion_alarm_sent:
        intrusion_alarm_sent = False
        return jsonify({"intrusion_alarm_triggered": intrusion_alarm_triggered})
    else:
        return jsonify({"status": "No intrusion alarm triggered"})

@app.route('/send_smoke_alarm', methods=['GET'])
def send_smoke_alarm():
    global smoke_alarm_triggered, smoke_alarm_sent
    if smoke_alarm_sent:
        smoke_alarm_sent = False
        return jsonify({"smoke_alarm_triggered": smoke_alarm_triggered})
    else:
        return jsonify({"status": "No smoke alarm triggered"})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

