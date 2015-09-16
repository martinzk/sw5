from __future__ import print_function
import tinypacks
from binascii import unhexlify
import json		

keyStrNames = ['BL', 'REL', 'MSL', 'ML', 'AL', 'SSL', 'RUL']	
from binascii import unhexlify

def index(req):
 	postData = req.form
 	jsonList = []

	for key in keyStrNames:
		if key in postData:
			tp = postData[key].value
			bytes = unhexlify(tp)
			data = tinypacks.unpack(bytes)
			jsonList.append(json.dumps(data))

	return jsonList