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
        raise RuntimeError("Unknown OS: " + os_name)

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
