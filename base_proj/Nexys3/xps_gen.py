import os
import sys
import platform
from optparse import OptionParser


parser = OptionParser()
parser.add_option("-s", "--synthesis",
                  action="store_true", dest="synthesis", default=False,
                  help="Synthesize - XST")
parser.add_option("-i", "--implement",
                  action="store_true", dest="implement", default=False,
                  help="Implement Design")
parser.add_option("-g", "--generate",
                  action="store_true", dest="generate", default=False,
                  help="Generate Programming File")

(options, args) = parser.parse_args()

# #os.system("xps_gen")

PLATFORM = platform.system()

if (PLATFORM == 'Linux') :
	print 'Linux Detected'
	os.environ['XILINX'] = '/opt/Xilinx/13.3/ISE_DS/ISE'
	os.environ['PATH'] += ':/opt/Xilinx/13.3/ISE_DS/EDK/bin/lin64'
	os.environ['PATH'] += ':/opt/Xilinx/13.3/ISE_DS/ISE/bin/lin64'

	CMD_PLATGEN = "platgen -p xc6slx16csg324-3 -lang vhdl -intstyle default system.mhs"
	CMD_SYNTHESIS = "cd synthesis && ./synthesis.sh && cd .."
	CMD_COPY_OPT = "cp etc/fast_runtime.opt implementation/fast_runtime.opt"
	CMD_COPY_UCF = "cp data/system.ucf implementation/system.ucf"
	CMD_XFLOW = "xflow -wd implementation -p xc6slx16csg324-3 -implement fast_runtime.opt system.ngc"
	CMD_XILPERL = "xilperl /opt/Xilinx/13.3/ISE_DS/EDK/data/fpga_impl/observe_par.pl -error yes implementation/system.par"
	CMD_COPY_UT = "cp etc/bitgen.ut implementation/bitgen.ut"
	CMD_BITGEN = "cd implementation && bitgen -w -f bitgen.ut system && cd .."
elif (PLATFORM == 'Windows') :
	print 'Windows Detected'
	os.environ['XILINX'] = 'C:/Xilinx/13.4/ISE_DS/ISE'
	os.environ['PATH'] += ';C:/Xilinx/13.4/ISE_DS/EDK/bin/nt64'
	os.environ['PATH'] += ';C:/Xilinx/13.3/ISE_DS/ISE/bin/nt64'

	CMD_PLATGEN = "platgen -p xc6slx16csg324-3 -lang vhdl -intstyle default system.mhs"
	CMD_SYNTHESIS = "cd synthesis & synthesis.cmd & cd .."
	CMD_COPY_OPT = "echo f|xcopy etc\\fast_runtime.opt implementation\\fast_runtime.opt /Y"
	CMD_COPY_UCF = "echo f|xcopy data\\system.ucf implementation\\system.ucf /Y /I"
	CMD_XFLOW = "xflow.exe -wd implementation -p xc6slx16csg324-3 -implement fast_runtime.opt system.ngc"
	CMD_XILPERL = "xilperl C:/Xilinx/13.4/ISE_DS/EDK/data/fpga_impl/observe_par.pl -error yes implementation/system.par"
	CMD_COPY_UT = "echo f|xcopy etc\\bitgen.ut implementation\\bitgen.ut /Y"
	CMD_BITGEN = "cd implementation & bitgen -w -f bitgen.ut system & cd .."
else :
	print 'Platform Not Supported'
	exit()

if (options.synthesis):
	#print('######################')
	print '## Synthesize - XST ##'
	#print('######################')
	sys.stdout.flush()
	os.system(CMD_PLATGEN)
	os.system(CMD_SYNTHESIS)
	
if (options.implement):
	#print('######################')
	print('## Implement Design ##')
	# print('######################')
	sys.stdout.flush()
	os.system(CMD_COPY_OPT)
	os.system(CMD_COPY_UCF)
	os.system(CMD_XFLOW)
	os.system(CMD_XILPERL)

if (options.generate):
	#print('#################################')
	print('## Generate Programming File ##')
	#print('#################################')
	sys.stdout.flush()
	os.system(CMD_COPY_UT)
	os.system(CMD_BITGEN)
