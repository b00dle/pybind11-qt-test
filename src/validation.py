import os
import json

SECRETS_PATH = "/home/basti/Documents/git/pybind11-qt-test/src/secret.json"


def load_access_token(path):
    if os.path.exists(SECRETS_PATH):
        with open(path, 'r') as f:
            secret = json.load(f)
            if "access_token" in secret:
                return secret["access_token"]
    return ""

ACCESS_TOKEN = load_access_token(SECRETS_PATH)