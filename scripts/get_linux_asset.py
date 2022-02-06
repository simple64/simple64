#!/usr/bin/python3

import requests
import sys


def main():
    resp = requests.get(
        f"https://api.github.com/repos/m64p/m64p/releases/tags/{sys.argv[1]}"
    )
    resp.raise_for_status()
    for item in resp.json()["assets"]:
        if "m64p-linux64" in item["name"]:
            print(item["browser_download_url"])
            break


if __name__ == "__main__":
    main()
