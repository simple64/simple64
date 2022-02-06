#!/usr/bin/python3

import requests


def main():
    resp = requests.get("https://api.github.com/repos/m64p/m64p/releases/latest")
    resp.raise_for_status()
    print(resp.json()["tag_name"])


if __name__ == "__main__":
    main()
