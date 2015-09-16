import sys
import json
import cgi

def index():
    form = cgi.FieldStorage()
    item = form.getvalue("key")
    return item

