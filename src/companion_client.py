import requests
import urllib3
import validation

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
BASE_URL = "https://0.0.0.0:5000/"


def get(api_path, params={}, provide_access_token=True):
    if provide_access_token:
        params.update({"access_token": validation.ACCESS_TOKEN})

    r = requests.get(
        BASE_URL+api_path,
        params=params,
        verify=False
    )

    data = None
    if r.status_code == 200:
        if r.headers['content-type'] == "application/json":
            data = r.json()

    return r.status_code, r.content, data

def get_sounds():
    import companion as c
    status, message, data = get("api/sound")
    res = None
    if data is not None:
        res = []
        for d in data:
            sound = c.PySound(**d)
            if "resource" in d:
                sound.resource = c.PyResource(**d["resource"])
            res.append(sound)
    return c.PyHttpResponse(
        status_code=status,
        message=message,
        data=res
    )