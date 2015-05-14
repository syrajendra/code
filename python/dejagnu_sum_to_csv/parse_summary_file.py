#!/usr/bin/env python

import sys
import re
import os
import commands
import logging
import datetime

test_result = ["FAIL:", "PASS:", "XFAIL:", "KFAIL:", "XPASS:", "KPASS:", "UNTESTED:", "UNRESOLVED:", "UNSUPPORTED:"]

def is_result(line):
	for res in test_result:
		if line.startswith(res):
			return True
	return False

def open_read_log_file(filename):
	fd 	= open(filename, 'r')
	log	= fd.read()
	fd.close()
	return log.split("\n")

def append_spaces(filename):
	spaces = len(filename)
	ret    = ''
	while spaces:
		ret = ret + ' '
		spaces = spaces - 1
	return ret

def write_info(fd, with_ttype, compiler, target, username, when, summary):
	fd.write("Compiler: " 	+ compiler + "\n")
	fd.write("Target: "   	+ target + "\n")
	fd.write("Run By: " 	+ username + "\n")
	fd.write("Date: " 		+ when + "\n")
	fd.write('Summary:\n' 	+ summary + "\n")
	if with_ttype:
		heading = 'Testcase, Flags, Testtype, Compilation, Execution'
	else:
		heading = 'Testcase, Flags, Compilation, Execution'
	fd.write(heading + "\n")

def write_csv(with_ttype, no_redudant_filename, logname, csv_data, compiler, target, username, when, summary):
	output_lines	= {}
	fd_sort 	= open(os.path.basename(logname) + ".sorted.csv", "w")
	write_info(fd_sort, with_ttype, compiler, target, username, when, summary)
	for filename, info_dict in csv_data.iteritems():
		output = ''
		non_sorted = ''
		#print "Filename: " + filename
		if no_redudant_filename:
			output 	= filename
		cnt    	= csv_data[filename]['count'][0]
		new_exp	= csv_data[filename]['exp'][0]
		#print "Count: " + repr(cnt)
		for flags, flags_dict in info_dict.iteritems():
			if flags == "count" or flags == 'exp':
				continue
			if no_redudant_filename:
				if output != filename:
					output = output + append_spaces(filename)
			else:
				output = filename
			compilation 	= execution = ''
			if 'compilation' in flags_dict:
				compilation = flags_dict['compilation']
			if 'execution' in flags_dict:
				execution 	= flags_dict['execution']
			ttype 	= flags_dict['testtype']

			if flags == '-':
				flags = ', '
			else:
				flags = flags.replace(',', ' ')
				flags = ', ' + flags

			if with_ttype:
				ttype = ttype.replace(',', ' ')
				ttype = ', ' + ttype
			else:
				ttype = ''
			output 	= output +  flags + ttype + ", " + compilation + ", " + execution + "\n"
			non_sorted = non_sorted + output
			#print "Output: " + output
			fd_sort.write(output)

		if cnt not in output_lines:
			output_lines[cnt] = []
		output_lines[cnt].append(non_sorted)
		output_lines[cnt].append(new_exp)

	fd_sort.close()
	fd = open(os.path.basename(logname) + ".csv", "w")
	write_info(fd, with_ttype, compiler, target, username, when, summary)
	old_exp = ''
	for cnt, info_list in output_lines.iteritems():
		if old_exp != info_list[1]:
			fd.write(info_list[1] + "\n");
			old_exp = info_list[1]
		fd.write(info_list[0])
	fd.close()

def trim_filename(filename):
	if 'testsuite' in filename:
		tks = filename.split('testsuite/')
		filename = tks[1]
	if ',' in filename:
		filename = filename.replace(',', '')
	return filename

def parse_flags_ttype(tks):
	flags = ''
	ttype = ''
	i  	  = 0
	while i < len(tks):
		if (tks[i].startswith('-') and tks[i][1:2].isalpha()) or \
		   (tks[i].startswith('--') and tks[i][2:3].isalpha()):
			flags = flags + " " + tks[i]
			if i+2 < len(tks):
				if tks[i+2].startswith('-'):
					flags = flags + " " + tks[i+1]
					i = i + 1
		else:
			ttype = ttype + " " + tks[i]
		i = i + 1
	flags	= flags.strip(' ')
	ttype	= ttype.strip(' ')
	if flags == '':
		flags = '-'
	return flags, ttype

def parse_ce(line):
	# both compile + execution
	tks = []
	if 'compilation' in line:
		tks 	= line.split('compilation,');
		phase	= 'compilation'
	else:
		tks 	= line.split('execution,');
		phase	= 'execution'
	ntks	= tks[0].split(' ')

	result	= (ntks[0].strip(' '))[:-1]
	filename= trim_filename(ntks[1].strip(' '))

	tks = tks[1].split(' ')
	flags, ttype = parse_flags_ttype(tks)
	#print "Flags: " + flags
	#print "Ttypes: " + ttype
	#print "Phase: " + phase
	return result, filename, phase, flags, ttype

def parse_c(line):
	# only compile
	tks		= line.split(' ')
	result	= (tks[0].strip(' '))[:-1]
	filename= trim_filename(tks[1].strip(' '))
	phase	= "compilation"
	tks.pop(0) # 1st element
	tks.pop(0) # 2nd element
	flags, ttype = parse_flags_ttype(tks)
	#print "Flags: " + flags
	#print "Ttypes: " + ttype
	return result, filename, phase, flags, ttype

def parse_exp(line):
	# get which exp is executed
	tks = line.split('testsuite/')
	exp = tks[1][:-4]
	return exp.strip(' ')

def print_csv_data(filename, csv_data):
	info_dict= csv_data[filename]
	cnt    	= csv_data[filename]['count'][0]
	new_exp	= csv_data[filename]['exp'][0]
	output 	= filename
	print "Filename: " + filename + " Count: " + repr(cnt) + " Exp: " + new_exp
	for flags, flags_dict in info_dict.iteritems():
		if flags == "count" or flags == 'exp':
			continue
		compilation 	= execution = ''
		if 'compilation' in flags_dict:
			compilation = flags_dict['compilation']
		if 'execution' in flags_dict:
			execution 	= flags_dict['execution']
		ttype 	= flags_dict['testtype']
		print "Flag: " + flags + " Compilation: " + compilation + " Execution: " + execution + " Ttype: " + ttype


def parse_log_file(filename):
	cnt 	= 0
	lines 	= open_read_log_file(filename)
	csv_data= {}
	exp	= target = compiler = username = when = summary = ''
	for line in lines:
		# For tetsing
		#line = 'PASS: gcc.c-torture/execute/20000112-1.c execution,  -O0 '
		if is_result(line):
			#print "Line: " + line
			result = filename = phase = flags = ''
			if "compilation," in line or 'execution,' in line:
				result, filename, phase, flags, ttype = parse_ce(line)
			else:
				result, filename, phase, flags, ttype = parse_c(line)
			if filename not in csv_data:
				csv_data[filename] = {}
				csv_data[filename]['count'] = [cnt]
				cnt = cnt + 1
				csv_data[filename]['exp'] = [exp]

			if flags not in csv_data[filename]:
				csv_data[filename][flags] = {}

			if phase == 'compilation':
				csv_data[filename][flags]['compilation'] = result
			if phase == 'execution':
				csv_data[filename][flags]['execution'] 	 = result
			csv_data[filename][flags]['testtype'] = ttype
			# For testing
			#print_csv_data(filename, csv_data)
			#sys.exit(1)

		if line.startswith("Running") and line.endswith('exp ...'):
			exp = parse_exp(line)
			#print "Exp: " + exp
		if line.startswith("Target is "):
			compiler = (line.split('Target is '))[1]
		if line.startswith("Running target "):
			target = (line.split('Running target '))[1]
		if line.startswith("Test Run By "):
			tks = line.split(' on ')
			username = (tks[0].split(' '))[3]
			when = tks[1]
		if line.endswith("Summary ==="):
			summary = summary + line + "\n"
		if line.startswith("# of "):
			tks = line.split('\t')
			num = tks[-1]
			tks.pop()
			summary = summary + ' '.join(tks) + ": " + num + "\n"
	return csv_data, compiler, target, username, when, summary

def parse_arguments(args):
	with_ttype = no_redudant_filename = False
	for arg in args:
		if arg == '--with-ttype':
			with_ttype = True
		elif arg == '--no-redundant-filename':
			no_redudant_filename = True
	return with_ttype, no_redudant_filename

def main():
	time_start 	= datetime.datetime.now().strftime("%d-%B-%Y %I:%M%p")
	num_args	= len(sys.argv)
	if num_args > 1:
		filename = sys.argv[1]
		csv_data, compiler, target, username, when, summary = parse_log_file(filename)
		with_ttype = no_redudant_filename = False
		if num_args > 2:
			with_ttype, no_redudant_filename = parse_arguments(sys.argv)
		write_csv(with_ttype, no_redudant_filename, filename, csv_data, compiler, target, username, when, summary)
	else:
		print "Usage: " + sys.argv[0] + " <summary-output-file.sum>"
        print "Optional arguments: \n" + \
				"--with-ttype : Extra column showing test type/description\n" + \
				"--no-redundant-filename : Repeated filenames are discarded\n"
		sys.exit(1)

ret = main()
