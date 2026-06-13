import pytest
import sys
import os
from flask import Flask

# Add the tools/ota_server directory to the path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'tools', 'ota_server'))

# Import the actual Flask app from fw-server.py
from importlib import import_module
fw_server = import_module('fw-server')


@pytest.mark.parametrize("auth_header,filename", [
    (None, "firmware.bin"),  # No authentication header
    ({"Authorization": "Bearer expired_token_xyz"}, "firmware.bin"),  # Expired/invalid token
    ({"Authorization": "malformed"}, "test.bin"),  # Malformed auth header
    ({"X-Custom-Auth": "valid_token"}, "firmware.bin"),  # Wrong header name
])
def test_protected_endpoints_reject_unauthenticated_requests(auth_header, filename):
    """Invariant: Protected endpoints must reject unauthenticated requests with 401 or 403"""
    
    # Get the Flask app instance
    app = fw_server.app
    app.config['TESTING'] = True
    client = app.test_client()
    
    # Make request with the given auth header (or no header)
    if auth_header:
        response = client.get(f'/{filename}', headers=auth_header)
    else:
        response = client.get(f'/{filename}')
    
    # Assert that unauthenticated requests are rejected
    assert response.status_code in [401, 403], (
        f"Expected 401 or 403 for unauthenticated request, got {response.status_code}. "
        f"Endpoint /{filename} should require authentication."
    )