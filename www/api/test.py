#!/usr/bin/env python3

import os
import sys
import json
from datetime import datetime
import cgi

# Set content type
print("Content-Type: application/json")
print()  # Empty line required between headers and body

# Get environment variables
method = os.environ.get('REQUEST_METHOD', 'GET')
path = os.environ.get('PATH_INFO', '/')
query = os.environ.get('QUERY_STRING', '')

# Prepare response data
response_data = {
    'status': 'success',
    'message': 'Python CGI is working!',
    'method': method,
    'path': path,
    'query_string': query,
    'timestamp': datetime.now().isoformat(),
    'server': 'Webserv/1.0',
    'python_version': sys.version,
    'environment': {
        'REQUEST_METHOD': os.environ.get('REQUEST_METHOD', ''),
        'PATH_INFO': os.environ.get('PATH_INFO', ''),
        'QUERY_STRING': os.environ.get('QUERY_STRING', ''),
        'CONTENT_TYPE': os.environ.get('CONTENT_TYPE', ''),
        'CONTENT_LENGTH': os.environ.get('CONTENT_LENGTH', '')
    }
}

# Handle POST data if present
if method == 'POST':
    content_length = os.environ.get('CONTENT_LENGTH')
    if content_length:
        try:
            post_data = sys.stdin.read(int(content_length))
            response_data['post_data'] = post_data
        except:
            response_data['post_error'] = 'Could not read POST data'

# Output JSON response
print(json.dumps(response_data, indent=2))
