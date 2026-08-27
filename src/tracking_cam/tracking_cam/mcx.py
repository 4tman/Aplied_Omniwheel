#!/usr/bin/env python3

import motorcortex


URL = "ws://192.168.42.1:5558:5557"


def main():
    try:
        req, sub = motorcortex.connect(URL)
        print("Connected to:", URL)
    except Exception as exc:
        print(f"Connection failed: {type(exc).__name__}: {exc}")
        return

    try:
        reply = req.getParameterTree()
        print("getParameterTree reply:", reply)

        tree = motorcortex.ParameterTree(reply)
        paths = tree.getAllPaths()

        keywords = ("camera", "image", "jpeg", "jpg", "frame", "video",
                    "yolo", "detect", "blob", "aruco")

        for path in paths:
            if any(word in path.lower() for word in keywords):
                print(path)

    except Exception as exc:
        print(f"Tree query failed: {type(exc).__name__}: {exc}")
    finally:
        try:
            req.close()
        except Exception:
            pass
        try:
            sub.close()
        except Exception:
            pass


if __name__ == "__main__":
    main()