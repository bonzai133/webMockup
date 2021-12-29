from flask import Flask, render_template
from datetime import datetime
import re

app = Flask(__name__)

@app.route("/api/data")
def get_data():
    return app.send_static_file("data.json")

# Replace the existing home function with the one below
@app.route("/")
def home():
    return render_template("home.html")

# New functions
@app.route("/about/")
def about():
    return render_template("about.html")

@app.route("/config/")
def config():
    return render_template("config.html")
