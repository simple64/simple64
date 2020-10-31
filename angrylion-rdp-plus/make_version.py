#!/usr/bin/env python3

import sys, os, subprocess

def system(cmd, default):
    args = cmd.split()
    try:
        return subprocess.check_output(args).decode("ascii").strip()
    except:
        return default

if __name__ == "__main__":
    branch = system("git rev-parse --abbrev-ref HEAD", "master")
    hash = system("git show -s --format=%h", "0" * 40)
    date = system("git show -s --format=%ci", "")[:10]
    tag = system("git describe --dirty --always --tags", "")

    # remove hash from git describe output
    tag = tag.split("-")
    if len(tag) > 2 and tag[2][1:] in hash:
        del tag[2]
    tag = "-".join(tag)

    mappings = {
        "GIT_BRANCH": branch,
        "GIT_TAG": tag,
        "GIT_COMMIT_HASH": hash,
        "GIT_COMMIT_DATE": date,
    }

    base_path = os.path.dirname(sys.argv[0])
    core_path = os.path.join(base_path, "src", "core")
    in_path = os.path.join(core_path, "version.h.in")
    out_path = os.path.join(core_path, "version.h")

    with open(in_path, "r") as f:
        version_str = f.read()

    for mapping, value in mappings.items():
        version_str = version_str.replace("@" + mapping + "@", value)

    with open(out_path, "w") as f:
        f.write(version_str)
