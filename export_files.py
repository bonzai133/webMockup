import requests

# Export the html / css / js files from running server
# 1. Run Flask app on http://localhost:5000
# 2. Execute this script

def main(hostname, output):
    files = [
        ("", "home.html"),
        ("config", "config.html"),
        ("configAdvanced", "configAdvanced.html"),
        ("about", "about.html"),
        ("static/index.js", "index.js"),
        ("static/site.css", "site.css"),
    ]

    for (url, file_path) in files:
        r = requests.get(f"{hostname}/{url}")
        my_page = r.text
        f2 = open(f"{output}/{file_path}", "w")
        f2.write(my_page)
        f2.close()

if __name__ == "__main__":
    hostname = "http://localhost:5000"
    output = "output"

    main(hostname, output)
