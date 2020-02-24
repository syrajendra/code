import gdb
import commands
import os
import sys
import re

class verify_rebuilt_sb(gdb.Command):
	def __init__(self):
		self.info_proc_mappings = None
		super(verify_rebuilt_sb, self).__init__("verify_rebuilt_sb", gdb.COMMAND_DATA)

	def run_readelf(self, path):
		offset  = None
		size    = None
		cmd 	= "/volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/5.0/current/bin/readelf -S %s | grep -A 1 .text" %path
		s, o = commands.getstatusoutput(cmd)
		if s == 0 and o:
			#print o
			tks1     = o.split("\n")
			line1 	 = tks1[0]
			line2 	 = tks1[1]
			tks2 	 = line1.split()
			#print tks
			sec_name = tks2[1]
			offset   = tks2[4]
			tks3 	 = line2.split()
			if not tks3[0].startswith("["):
				size = tks3[0]
			else:
				size = tks2[5]
			print("Text offset from readelf      : %s" %offset)
			#print("Text size from readelf  : %s" %size)
		return offset, size

	def parse_isld(self, o):
		tks   = o.split("\n")
		tks   = tks[1].split()
		#print tks
		start = tks[0]
		end   = tks[1]
		lib_path = tks[-1]
		print("Library path                  : " + lib_path)
		print("Core: lib .text address       : %s" %start)
		#print("Lib .text end address   : %s" %end)
		offset, size = self.run_readelf(lib_path)
		return start, end, offset, size

	def read_info_shared_library_data(self, lib_name, lib_path):
		o 	= gdb.execute("info sharedlibrary %s"%lib_name, to_string=True)
		if o:
			if not "No shared libraries matched" in o:
				return self.parse_isld(o)
			else:
				print("Error: No shared libraries matched with name '%s'" %lib_path)
				return None
		else:
			print("Error: Failed to run CMD 'info sharedlibrary %s'" %lib_path)
			return None

	def parse_ipm(self, lines, lib_name):
		for line in lines:
			tks = line.split()
			if len(tks) == 5:
				proc_name = os.path.basename(tks[4]).strip()
				if lib_name in proc_name:
					load_addr = tks[0]
					return load_addr
		return None

	def search_info_proc_mappings(self, lib_name):
		ret  	 = None
		print("Actual library name           : %s" %lib_name)
		if not self.info_proc_mappings:
			o 	 = gdb.execute("info proc mappings", to_string=True)
			if o:
				lines   = o.split("\n")
				self.info_proc_mappings = lines
				ret = self.parse_ipm(self.info_proc_mappings, lib_name)
			else:
				print("Error: Failed to run CMD 'info proc mappings'")
				ret = None
		else:
			ret = self.parse_ipm(self.info_proc_mappings, lib_name)
		return ret;

	def read_elf_magic(self, load_addr):
		o 	= gdb.execute("x/s %s"%load_addr, to_string=True)
		if o:
			tks = o.split(":")
			print("ELF magic at %16s : %s" %(tks[0], tks[1].strip()))

	def verify_lib(self, lib_name, load_addr, lib_start, lib_offset, lib_end, lib_size):
		text_start = int(load_addr, 16) + int(lib_offset, 16)
		print(".text start                   : %s" %hex(text_start))
		if int(hex(text_start), 16) == int(lib_start, 16):
			print("Library %s is OK" %lib_name)
		else:
			print("Library %s has different offsets" %lib_name)
		print("\n")

	def parse_if(self, o):
		libs  = []
		exe_path  = None
		exe_start = None
		lines = o.split("\n")
		for index, line in enumerate(lines):
			if " is .text in /" in line:
				tks = line.split()
				path = tks[-1]
				libname = os.path.basename(path)
				if libname not in libs and path.startswith("/"):
					libs.append(path)
			elif " is .text" in line and not exe_start:
				tks = line.split()
				exe_start = tks[0]
				#print tks
			elif "Local exec file:" in line:
				tks = lines[index+1].split()
				exe_path = tks[0].replace('`', '').replace('\'', '').replace(',', '')
				print("Executable         : %s" %exe_path)
		return exe_path, exe_start, libs

	def find_bin_libs(self):
		o 	 = gdb.execute("info files", to_string=True)
		if o:
			return self.parse_if(o)
		else:
			print("Error: Failed to run CMD 'info files'")
		return None, []

	def verify_exe(self, load_addr, exe_path, exe_start):
		exe_name = os.path.basename(exe_path)
		offset, size = self.run_readelf(exe_path)
		text_start = int(load_addr, 16) + int(offset, 16)
		print(".text load address            : %s" %load_addr)
		self.read_elf_magic(load_addr)
		print(".text start                   : %s" %hex(text_start))
		if int(hex(text_start), 16) == int(exe_start, 16):
			print("Executable %s is OK" %exe_name)
		else:
			print("Executable %s has different offsets" %exe_name)
		print("\n")

	def get_actual_library_name(self, libpath):
		if os.path.islink(libpath):
			link = os.readlink(libpath)
			if "/" in link:
				lib_name = os.path.basename(link)
			else:
				lib_name = link
		else:
			lib_name = os.path.basename(libpath)
		return lib_name

	def invoke(self, arg, from_tty):
		print("Verify binary & libs: ")
		gdb.execute('set height 0')
		exe_path, exe_start, libs = self.find_bin_libs()
		print("Core: exe .text address       : %s" %exe_start)
		exe_name = os.path.basename(exe_path)
		load_addr = self.search_info_proc_mappings(exe_name)
		self.verify_exe(load_addr, exe_path, exe_start)
		for libpath in libs:
			# testing
			#libpath = "/volume/evo/files/opt/poky/2.2.1-13/sysroots/core2-64-poky-linux/lib64/libnss_compat.so.2"
			lib_name_for_proc_info  = self.get_actual_library_name(libpath)
			lib_name_for_shared_lib = os.path.basename(libpath).split(".so")[0] + ".so"
			lib_start, lib_end, lib_offset, lib_size = self.read_info_shared_library_data(lib_name_for_shared_lib, libpath)
			load_addr = self.search_info_proc_mappings(lib_name_for_proc_info)
			if load_addr:
				self.read_elf_magic(load_addr)
				self.verify_lib(lib_name_for_shared_lib, load_addr, lib_start, lib_offset, lib_end, lib_size)
			else:
				print("Error: Failed to find load address for %s" %lib_name_for_shared_lib)
				return
			# testing
			#return

if __name__ == "__main__":
	verify_rebuilt_sb()


