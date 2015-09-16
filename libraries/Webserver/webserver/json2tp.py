#! /usr/lib/python2.7
from __future__ import print_function
import tinypacks
from binascii import hexlify
import json
#List for popping
popList = ["isCondition", "eventString", "isAction", "waitSeconds", "waitMinutes", "waitHours", "durationSeconds", "durationMinutes", "durationHours", "wait", "duration", "as", "cs"]
#Maybe get this list from the arduino.
keyStrNames = ['BL', 'REL', 'MSL', 'ML', 'AL', 'SSL', 'RUL']		
from binascii import hexlify

MAX_BYTE_SIZE = 1000

def index(req):
 	postData = req.form

	serializeList = []
	#Create list of sendable json objects
	for key in keyStrNames:
		if key in postData:
			dataDict = {}
			data = json.loads(postData[key].value)
			dataDict['type'] = key
			dataDict['data'] = data
			serializeList.append(dataDict)	

	returnList = []

	#Pop unused properties and pack json into tinypacks
	for obj in serializeList:
		if obj['type'] == 'RUL':
			obj['data'] = popItemsFromRules(obj['data'])			
		else:
			obj['data'] = popItems(obj['data'])

	#one more time, can't be one foreach loop
	for obj in serializeList:
		#Split obj data into sublists of maximum MAX_BYTE_SIZE and serialize it
		newData = SplitData(obj['data'])
		#If there are sublists(more than one element), make it returnable for the js code.
		if len(newData) > 1:
			#Create new dict objs for every sub item
			counter = 1;
			for d in newData:
				dataDict = {}
				dataDict['type'] = obj['type'] + str(counter)
				dataDict['data'] = d
				returnList.append(dataDict)
				counter += 1
				if(counter > 99):
					return "ERROR"
		else:
			if(newData):
				obj['data'] = newData[0]
				obj['type'] = obj['type'] + str(1);
				returnList.append(obj)

	return json.dumps(returnList)


def SplitData(data):
	sublist = []
	templist = []
	totalsize = 0
	count = 0;
	i = 0; 

	#only one element?
	if(len(data) == 1):
		d =  hexlify(tinypacks.pack(data))
		sublist.append(d)
		return sublist
	while(i < len(data)):
		#Try to serialize data
		size = len(hexlify(tinypacks.pack(data[i])))/2;
		totalsize += size;
		if totalsize <= MAX_BYTE_SIZE:
			templist.append(data[i]);
			i += 1;
		elif totalsize > MAX_BYTE_SIZE:
			totalsize = 0;
			size = 0
			l = hexlify(tinypacks.pack(templist));
			sublist.append(l);
			templist = [];
			#subtract one from i, since the element didn't get appended.
			i -= 1;

		if(i == len(data)):
			l = hexlify(tinypacks.pack(templist));
			sublist.append(l);
			break
	return sublist;


def popItems(list):
	for item in list:
		for popItem in popList:
			item.pop(popItem, None)
	return list

def popItemsFromRules(rules):
	for rule in rules:
		event = rule['e']
		event.pop('eventString', None)
		event.pop('en', None)

		actions = rule['a']
		for action in actions:
			action.pop('an', None)

		conditions = rule['c']
		for condition in conditions:
			condition.pop('cn', None)
	return rules

