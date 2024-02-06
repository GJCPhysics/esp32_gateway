from flask import Flask, request, jsonify

app = Flask(__name__)

system_armed = False

@app.route('/update_door_status', methods=['POST'])
def update_door_status():
    global system_armed

    data = request.get_json()

    # Print the received JSON data
    print("Received JSON Data:", data)

    door_status = data.get('door_status', 'unknown')
    system_armed = data.get('system_armed', False)

    print(f"Received door status: {door_status}")
    print(f"System armed: {system_armed}")

    return jsonify({"status": "success"})

@app.route('/get_system_state', methods=['GET'])
def get_system_state():
    global system_armed
    return jsonify({"armed": system_armed})

if __name__ == '__main__':
    app.run(host='192.168.43.172', port=5000)
