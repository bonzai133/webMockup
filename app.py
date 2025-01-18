from flask import Flask, render_template, jsonify, request
from flask_minify import Minify

app = Flask(__name__)
Minify(app=app, html=True, js=True, cssless=True)


@app.route("/api/data")
def get_data():
    return app.send_static_file("data.json")


# Replace the existing home function with the one below
@app.route("/")
def index():
    return render_template("home.html")


@app.route("/home.html")
def home():
    return render_template("home.html")


# New functions
@app.route("/about.html")
def about():
    return render_template("about.html")


@app.route("/config.html")
def config():
    return render_template("config.html")


@app.route("/configAdvanced.html")
def configAdvanced():
    return render_template("configAdvanced.html")


@app.route("/configTimeSlots.html")
def configTimeSlots():
    return render_template("configTimeSlots.html")


# API
@app.route("/api/homeStatus/")
def homeStatus():
    tempDoc = {
        'temperature': 21.15,
        'mode': "confort",
        'pump': "On"
    }

    return jsonify(tempDoc)


@app.route("/api/config", methods=['GET', 'POST'])
def api_config():
    if request.method == 'POST':
        data = request.get_json()
        print(data)  # you can remove this line, it's just for debugging
        return jsonify({'message': 'Config received'})
    else:
        return jsonify({'mode': 'eco', 'confort': 19.2, 'deltaConfort': 0.5, 'eco': 16, 'deltaEco': 0.5, 'horsGel': 6, 'deltaHorsGel': 0.5})


@app.route("/api/advancedConfig", methods=['GET', 'POST'])
def api_advancedConfig():
    if request.method == 'POST':
        data = request.get_json()
        print(data)  # you can remove this line, it's just for debugging
        return jsonify({'message': 'Advanced Config received'})
    else:
        return jsonify({'date': '2023-05-08', 'time': '17:17', 'ipAddress': '192.168.0.69',
                        'ntpServer': 'fr.poopool.org', 'lcdContrast': '128', 'tempOffset': '-1.5',
                        'heatTime': 900, 'restTime': 300})


@app.route("/api/timeSlots", methods=['GET', 'POST'])
def api_timeSlots():
    if request.method == 'POST':
        data = request.get_json()
        print(data)  # you can remove this line, it's just for debugging
        return jsonify({'message': 'Timeslots received'})
    else:
        return jsonify(
            {
                'timeSlots': [
                    {'day': 'Mon', 'times': [{"start": "06:20", "end": "07:30"}, {"start": "09:30", "end": "10:30"}]},
                    {'day': 'Tue', 'times': [{"start": "08:00", "end": "08:30"}]},
                    {'day': 'Wed', 'times': [{"start": "09:00", "end": "09:30"}]},
                    {'day': 'Thu', 'times': [{"start": "12:00", "end": "12:30"}]},
                    {'day': 'Fri', 'times': [{"start": "17:30", "end": "20:00"}]},
                    {'day': 'Sat', 'times': [{"start": "10:00", "end": "12:00"}]},
                    {'day': 'Sun', 'times': [{"start": "11:00", "end": "14:00"}]}
                ]
            }
        )


if __name__ == "__main__":
    app.run(debug=True)
