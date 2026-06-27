env = DefaultEnvironment()

import os
import glob as fileglob

if os.environ.get("PLATFORMIO_CALLER") == "vscode":
    print("PIO called from VS Code extension")
    import platform
    import json

    # Check if pioarduino IDE >= 1.3.6 is installed (handles port natively)
    os_name = platform.system()
    if os_name == "Windows":
        ext_base = os.path.join(os.environ.get("USERPROFILE", ""), ".vscode", "extensions")
    else:
        ext_base = os.path.expanduser("~/.vscode/extensions")

    skip = False
    for ext_dir in fileglob.glob(os.path.join(ext_base, "pioarduino.pioarduino-ide-*")):
        try:
            ver_str = os.path.basename(ext_dir).split("pioarduino.pioarduino-ide-")[1]
            ver_parts = tuple(int(x) for x in ver_str.split(".")[:3])
            if ver_parts >= (1, 3, 6):
                print("pioarduino IDE %s detected, port handled by extension" % ver_str)
                skip = True
                break
        except (IndexError, ValueError):
            pass

    if not skip:
        import sqlite3
        from platformio.project.helpers import get_project_dir

        print("OS Platform:", os_name)
        os_paths = {
            "Darwin": "~/Library/Application Support/Code/User/globalStorage/state.vscdb",
            "Linux": "~/.config/Code/User/globalStorage/state.vscdb",
            "Windows": r"%APPDATA%\Code\User\globalStorage\state.vscdb"
        }
        project_path = get_project_dir()

        try:
            db_path = os.path.expanduser(os.path.expandvars(os_paths[os_name]))
        except KeyError:
            print("Unknown OS: " + os_name)

        # If the database is not found, check if running in WSL
        # and try to find the database in the Windows file system
        if not os.path.exists(db_path) and os_name == "Linux":
            try:
                db_path = os.path.expanduser(os.path.expandvars(os_paths["Windows"]))
                print("Windows running PIO in WSL")
            except KeyError:
                pass

        # Only when the database is found we can go on
        if os.path.exists(db_path):
            conn = sqlite3.connect(db_path)
            cursor = conn.cursor()

            for key in ['pioarduino.pioarduino-ide', 'platformio.platformio-ide']:
                cursor.execute("SELECT value FROM ItemTable WHERE key = ?", (key,))
                row = cursor.fetchone()
                if row:
                    data = json.loads(row[0])
                    projects = data.get("projects", {})
                    project = projects.get(project_path)
                    if project and "customPort" in project:
                        print("USB port set in VSC:", project["customPort"])
                        env["UPLOAD_PORT"] = project["customPort"]
                        break
            conn.close()
