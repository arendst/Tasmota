import pytest
import sys
import os

# Add the tools/ota_server directory to the path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'tools', 'ota_server'))

from importlib import import_module

# Import the Flask app from fw-server.py
fw_server = import_module('fw-server')
app = fw_server.app


@pytest.fixture
def client():
    app.config['TESTING'] = True
    with app.test_client() as client:
        yield client


@pytest.mark.parametrize("payload", [
    "../../../etc/passwd",
    "....//....//....//etc/passwd",
    "%2e%2e%2f%2e%2e%2f%2e%2e%2fetc/passwd",
    "valid_firmware.bin",
    "../fw-server.py",
])
def test_path_traversal_blocked(payload, client):
    """Invariant: File operations never resolve paths outside the declared root directory"""
    response = client.get(f'/{payload}')
    
    # Path traversal attempts should be rejected (400/403/404) or return safe content
    # They should NEVER return sensitive file contents
    if '..' in payload or '%2e' in payload.lower():
        # Traversal payloads must not succeed with file content
        assert response.status_code in (400, 403, 404), \
            f"Path traversal payload '{payload}' returned unexpected status {response.status_code}"
        # Ensure we didn't leak /etc/passwd content
        assert b'root:' not in response.data, \
            f"Path traversal payload '{payload}' leaked sensitive file content"