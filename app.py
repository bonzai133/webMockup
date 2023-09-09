from flask import Flask, render_template, jsonify, request
from flask_minify import Minify
from datetime import datetime
import re

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

@app.route("/schedule.html")
def schedule():
    return render_template("schedule.html")

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
        return jsonify({'date': '2023-05-08', 'time': '17:17', 'ipAddress': '192.168.0.69', 'ntpServer': 'fr.poopool.org', 'lcdContrast': '128'})


@app.route("/api/timeSlots", methods=['GET', 'POST'])
def api_timeSlots():
    if request.method == 'POST':
        data = request.get_json()
        print(data)  # you can remove this line, it's just for debugging
        return jsonify({'message': 'Timeslots received'})
    else:
        return jsonify({})


if __name__ == "__main__":
    app.run(debug=True)
