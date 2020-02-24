import gdb
import commands
import sys
import os

class verify_lib(gdb.Command):
	def __init__(self):
		self.lib_path = None
		self.lib_name = None
		super(verify_lib, self).__init__("verify_lib", gdb.COMMAND_DATA)

	def parse_isld(self, o):
		tks   = o.split("\n")
		tks   = tks[1].split()
		#print tks
		start = tks[0]
		end   = tks[1]
		lib_path = tks[-1]
		print("Library path            : " + lib_path)
		print("Lib .text start address : %s" %start)
		print("Lib .text end address   : %s" %end)
		cmd = "/volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/5.0/current/bin/readelf -S %s | grep -A 1 .text" %lib_path
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
			print("Text offset from readelf: %s" %offset)
			print("Text size from readelf  : %s" %size)
			return start, end, offset, size
		return None

	def read_info_shared_library_data(self):
		o 	= gdb.execute("info sharedlibrary %s"%self.lib_name, to_string=True)
		if o:
			if not "No shared libraries matched" in o:
				return self.parse_isld(o)
			else:
				print("Error: No shared libraries matched with name '%s'" %self.lib_path)
				return None
		else:
			print("Error: Failed to run CMD 'info sharedlibrary %s'" %self.lib_path)
			return None

	def parse_ipm(self, o, tmp_file):
		lines   = o.split("\n")
		for line in lines:
			tks = line.split()
			if len(tks) == 5:
				lib_name = os.path.basename(tks[4]).strip()
				if self.lib_name in lib_name:
					elf_start_addr = tks[0]
					return elf_start_addr
		return None

	def search_info_proc_mappings(self):
		ret  	 = None
		home 	 = os.getenv("HOME")
		tmp_file = "%s/verify-lib-gdb.txt" %home
		# logging to file crashes GDB
		#o    = gdb.execute("set logging file %s" %tmp_file, to_string=True)
		#o    = gdb.execute("set logging on", to_string=True)
		o 	 = gdb.execute("info proc mappings", to_string=True)
		if o:
			ret = self.parse_ipm(o, tmp_file)
		else:
			print("Error: Failed to run CMD 'info proc mappings'")
			ret = None
		#o    = gdb.execute("set logging off", to_string=True)
		return ret;

	def read_elf_magic(self, elf_start_addr):
		o 	= gdb.execute("x/s %s"%elf_start_addr, to_string=True)
		if o:
			tks = o.split(":")
			print("ELF start %s: %s" %(tks[0], tks[1].strip()))

	def check_lib(self, elf_start_addr, lib_start, lib_offset, lib_end, lib_size):
		text_start = int(elf_start_addr, 16) + int(lib_offset, 16)
		print("ELF .text start         : %s" %hex(text_start))
		if int(hex(text_start), 16) == int(lib_start, 16):
			print("Library %s is OK" %self.lib_name)
		else:
			print("Library %s has different offsets" %self.lib_name)

	def invoke(self, arg, from_tty):
		if not from_tty:
			return

		self.lib_name = arg.strip()
		print("Verify lib : " + self.lib_name)
		gdb.execute('set height 0')
		lib_start, lib_end, lib_offset, lib_size = self.read_info_shared_library_data()
		elf_start_addr = self.search_info_proc_mappings()
		if elf_start_addr:
			self.read_elf_magic(elf_start_addr)
			self.check_lib(elf_start_addr, lib_start, lib_offset, lib_end, lib_size)

if __name__ == "__main__":
	verify_lib()
