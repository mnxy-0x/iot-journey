import sys
import requests

URL = "https://api.open-meteo.com/v1/forecast?latitude=21.5&longitude=38.2&current=temperature_2m"

try:
    r = requests.get(URL, timeout=5)
    r.raise_for_status()                       # raises HTTPError for 4xx and 5xx
    value = r.json()["current"]["temperature_2m"]
except requests.exceptions.Timeout:
    sys.exit("timeout: server did not answer in 5 s")
except (requests.exceptions.JSONDecodeError, KeyError) as e:
    sys.exit("unexpected response shape: %r" % e)
except requests.exceptions.RequestException as e:
    sys.exit("request failed: %s" % e)

print("temperature_2m =", value)
