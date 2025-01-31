import requests

# Export the configuration files from running Thermostato for backup


def main(hostname, output):
    files = [
        ("config/thermostato.cfg", "thermostato.cfg"),
        ("config/timeslots.cfg", "timeslots.cfg"),
    ]

    for (url, file_path) in files:
        r = requests.get(f"{hostname}/{url}")
        my_page = r.text
        f2 = open(f"{output}/{file_path}", "w")
        f2.write(my_page)
        f2.close()


if __name__ == "__main__":
    hostname = "http://192.168.0.103"
    output = "../Thermostato/Thermostato/data/config"

    main(hostname, output)
