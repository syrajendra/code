import gdb
import commands
import sys
import os

class show_valid_ret_addr(gdb.Command):
    def __init__(self):
        self.addr_dumped = []
        super(show_valid_ret_addr, self).__init__("show_valid_ret_addr", gdb.COMMAND_DATA)

    def check_addr(self, addr):
        o 	= gdb.execute("info line *%s" %(addr), to_string=True)
        if o:
            if not "No line number information available for address" in o:
                if addr not in self.addr_dumped:
                    print("%s: %s" %(addr, o.strip()))
                    self.addr_dumped.append(addr)
                
    def process_stack(self, sp_addr, range):
        o = None
        try:
            o 	= gdb.execute("x/%sx %s" %(range, sp_addr), to_string=True)
        except:
            print "Address range %s is too high" %range
            return
        if o:
            if not "Cannot access memory at address" in o:
                lines = o.split("\n")
                for line in lines:
                    tks = line.split(":")
                    if len(tks) > 1:
                        addrs = tks[1].strip().split()
                        #print addrs
                        for addr in addrs:
                            #print("Addr: " + addr)
                            if addr != "0x00000000":
                                self.check_addr(addr)
            else:
                print("Failed to get stack address")
        else:
            print("Failed to run x/%sx %s" %(range, sp_addr))

    def scan_stack(self, addr_range):
        o 	= gdb.execute("p/x $sp", to_string=True)
        if o:
            if not "Value can't be converted to integer" in o:
                sp_addr = o.split("=")[1].strip()
                print("Stack address : " + sp_addr)
                if sp_addr != "0x0":
                    self.process_stack(sp_addr, addr_range)
                else:
                    print("Stack pointer empty")
                    return
            else:
                print("Failed to get stack address")
        else:
            print("Failed to run p/x $sp")

    def invoke(self, arg, from_tty):
        if not from_tty:
            return
        args = arg.strip()
        tks = args.split()
        addr_range = tks[0]
        if addr_range == "":
            print("Supply 1. address range as first argument 2. optional stack pointer as second argument if $sp is 0x0")
            return
        print("Address range : " + addr_range)
        gdb.execute('set height 0')
        if len(tks) == 1:
            self.scan_stack(addr_range)
        else:
            sp_addr = tks[1]
            print("Stack address : " + sp_addr)
            self.process_stack(sp_addr, addr_range)

if __name__ == "__main__":
    show_valid_ret_addr()