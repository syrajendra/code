#!/usr/bin/env python
import os
import pwd
from SOAPpy import WSDL, Types

if __name__ == '__main__':  
	username = pwd.getpwuid(os.getuid())[0]
	wsdlfile = 'http://server_name/apt/test/blog_webservice/sample.wsdl'
	client = WSDL.Proxy(wsdlfile)
	job = dict()
	job['id'] = 1;
	job['name'] = 'job name';
	job['task'] = [];

	task = dict()
	task['name'] = 'python_task_name';
	task['progress'] = dict();
	task['progress']['status'] = 'Stopped';
	task['progress']['data'] = dict();
	task['progress']['data']['start'] = 'python start data';
	task['progress']['data']['end'] = 'python end data';

	job['task'].append(task) 
	
	print "\n"	
	try:
		ret = client.create_job(username, job)
		if(ret == 1):
			print "Successfully created job \n"
		else:
			print ret + "\n"

		ret_list = client.get_jobs(username)		
		print Types.simplify(ret_list)
						
	except Exception as e:
		print e 
	
