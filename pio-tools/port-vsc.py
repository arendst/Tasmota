env = DefaultEnvironment()

import os

if os.environ.get("PLATFORMIO_CALLER") == "vscode":
    print("PIO called from VS Code extension")
    import platform
    import sqlite3
    import json
    from platformio.project.helpers import get_project_dir

    os_name = platform.system()
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
