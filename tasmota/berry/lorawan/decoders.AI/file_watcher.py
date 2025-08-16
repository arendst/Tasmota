#!/usr/bin/env python3
"""
File Watcher for LoRaWAN Decoder AI Framework
Monitors ROOT markdown files and triggers auto-updates
"""

import os
import sys
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from pathlib import Path

# Import our auto-update system
try:
    from auto_update import auto_updater
except ImportError:
    print("❌ Cannot import auto_update module")
    sys.exit(1)

class MarkdownFileHandler(FileSystemEventHandler):
    """Handler for markdown file changes in ROOT directory"""
    
    def __init__(self, root_path):
        self.root_path = Path(root_path)
        self.last_update_time = {}
        
    def on_modified(self, event):
        """Handle file modification events"""
        if event.is_directory:
            return
            
        filepath = Path(event.src_path)
        
        # Only process ROOT markdown files (not SESSION-STATE.md)
        if (filepath.suffix == '.md' and 
            filepath.parent == self.root_path and
            filepath.name != 'SESSION-STATE.md'):
            
            # Debounce: prevent multiple rapid updates
            current_time = time.time()
            if filepath.name in self.last_update_time:
                if current_time - self.last_update_time[filepath.name] < 2:
                    return  # Skip if less than 2 seconds since last update
            
            self.last_update_time[filepath.name] = current_time
            
            print(f"📝 ROOT markdown modified: {filepath.name}")
            success = auto_updater.monitor_file_change(str(filepath))
            
            if success:
                print(f"✅ Auto-update completed for {filepath.name}")
            else:
                print(f"⚠️ Auto-update skipped for {filepath.name}")

def start_file_watcher(root_path):
    """Start the file watcher for ROOT directory"""
    event_handler = MarkdownFileHandler(root_path)
    observer = Observer()
    observer.schedule(event_handler, str(root_path), recursive=False)
    
    print(f"👁️ File watcher started for ROOT: {root_path}")
    print("📋 Monitoring ROOT .md files (excluding SESSION-STATE.md)")
    print("🔄 Auto-update system active")
    
    observer.start()
    
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n🛑 Stopping file watcher...")
        observer.stop()
    
    observer.join()
    print("✅ File watcher stopped")

if __name__ == "__main__":
    # Get ROOT path (directory containing this script)
    root_path = Path(__file__).parent
    
    # Check if watchdog is available
    try:
        from watchdog.observers import Observer
        from watchdog.events import FileSystemEventHandler
    except ImportError:
        print("❌ watchdog module not available")
        print("💡 Install with: pip install watchdog")
        print("🔄 Falling back to manual trigger mode")
        
        # Manual trigger for testing
        print("🧪 Testing auto-update system...")
        auto_updater.update_session_state("manual_test")
        sys.exit(0)
    
    # Start the watcher
    start_file_watcher(root_path)
