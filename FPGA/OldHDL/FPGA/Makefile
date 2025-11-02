all:
	/home/kavinho/intelFPGA/20.1/modelsim_ase/bin/vlog *.sv
	/home/kavinho/intelFPGA/20.1/modelsim_ase/bin/vsim -c tb -do "log -r /*; vcd file build/dump.svcd; vcd add -r /*; run -all; quit -f"

comp:
	/home/kavinho/intelFPGA/20.1/modelsim_ase/bin/vlog *.sv
	
sim:
	/home/kavinho/intelFPGA/20.1/modelsim_ase/bin/vsim -c tb -do "log -r /*; vcd file build/dump.svcd; vcd add -r /*; run -all; quit -f"

